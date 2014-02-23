helper engine: engine

    properties:
        - helpers: list of +name
        - publish port: +range[1025..65535] = 47000
        - listen port: +range[1025..65535] = 47001
        - publish queue: mq.connection
        - listen queue: mq.connection
        - direct queues: map of +name, mq.connection
        - assignments: list of assignment
        - file batches: list of batch
        - dynamic batch: batch
        - active batch: batch +computed
        - heartbeats: map of +name, date

    Update last heartbeat:
        takes:
            - host: +name
        code:
            lock heartbeats:
                heartbeats[host] = date.now()

    Get active batch:
        returns:
            ab : batch = dynamic batch
        code:
            if we have more stuff to do (file batches):
                ab = file batches[0]

    Start publishing messages:
        code:
            publish queue.Connect(publish port, PUBLISH_MODE)

    Start listening:
        code:
            listen queue.Connect(listen port, LISTEN_MODE, "domain.*", Receive message)

    Receive message:
        takes:
            - msg: mq.message
        code:
            Update last heartbeat(msg.from)

            # If all the functions below had the same signature, and if all of
            ... them had a reliable mapping between constant and function name,
            ... then we could do the following instead of using "handle":
            ...
            ... function namer: lambda code: `name of(code)`.lower().replace('_', '')
            ... Dispatch(namer, arg1, arg2)
            handle msg.code:
                when it == ADD_BATCH:
                    Add batch(msg.batch file path)
                when it == PAUSE_BATCH:
                    Pause batch(msg.batch id)
                when it == ABANDON_BATCH:
                    Abandon batch(msg.batch id)
                when it == FLUSH:
                    Abandon batch(null)
                when it == TERMINATE:
                    Terminate()
                when it == NEED_ASSIGNMENT:
                    Assign(msg.from, msg.proposed item count)

    Assign:
        takes:
            - requester host name: +name
            - proposed item count: int
        code:
            assignment := make assignment(requester host name, proposed item count)
            response := make message({
                topic := "domain.nitro.assignment"
                to := requester host name
            })
            if assignment:
                response.Extend({
                    code := NITRO_NEW_ASSIGNMENT
                    payload := assignment
                })
            else:
                response.Extend({
                    code := NITRO_ASSIGNMENT_NOT_AVAILABLE
                })
            response queue := Get response queue for host(requester host name)
            response queue.Send(msg)

    Make assignment:
        takes:
            *assign.params
        returns:
            - new: assignment
        code:
            commands := list of str
            batch := active batch
            lock batch:
                while len(commands) < proposed item count:
                    cmd := batch.Next()
                    if cmd:
                        commands.Append(cmd)
                    else:
                        break
            if commands:
                new := make assignment(batch.id, commands)

    Add batch:
        takes:
            - batch file path: str
        returns:
            - batch
        code:
            batch := make batch(batch file path)
            if batch:
                lock file batches:
                    file batches.Append(batch)

    Enroll helpers:
        code:
            # It's only legal to call this function if we haven't already
            ... enrolled a bunch of helpers.
            precondition: len(heartbeats) == 0
            msg = make message({
                topic := "domain.nitro.enroll",
                to := "*",
                code := NITRO_HELP_REQUESTED,
                nodes requested := helpers
            })
            mq.multicast(msg)

            # Sleep for a second to let responses come back -- but interrupt
            ... if we get word that all helpers responded.
            Sleep(1000 ~millisecs, finished signal)
            if not finished signal:
                for node name in helpers:
                    if node not in heartbeats:
                        queue :=

    Flush:
        code:
            lock file batches:
                for batch in file batches:
                    Abandon assignments for batch(batch)
                file batches.Clear()
    Run:
        takes:
            - batches: list of batch
            - helpers: list of str
        code:
            Start publishing messages()
            Start listening()
            Enroll helpers()
            Start maintenance thread()
