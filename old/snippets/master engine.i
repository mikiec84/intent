server engine: class

    properties:
        - assigned helpers: +assignonce list of +name
        - direct queues: map of +name to mq.connection
        - assignments: map of +name to assignment
        - file batches: list of batch
        - dynamic batch: batch
        - active batch: +calculated batch
        
    Make(*engine.takes, assigned helpers):

    Get active batch() -> ab: batch = dynamic batch
        If file batches:
            ab = file batches[0]
        
    Receive message(msg: mq.message): engine.Receive message
        If engine.receive message(msg): Return

        # If all the functions below had the same signature, and if all of
        # them had a reliable mapping between constant and function name,
        # then we could do the following instead of using "Handle":
        #
        # function namer: lambda code: *(name of(code)).lower().replace('_', '')
        # dispatch(namer, arg1, arg2)
        Handle msg.code:
            # The "When xyz" is a short form of "When it == "
            When ADD_BATCH: add batch(msg.batch file path)
            When PAUSE_BATCH: pause batch(msg.batch id)
            When ABANDON_BATCH: abandon batch(msg.batch id)
            When FLUSH: abandon batch(null)
            When NEED_ASSIGNMENT: assign(msg.from, msg.proposed item count)
            When it == ACCEPT_ASSIGNMENT || it == REJECT_ASSIGNMENT:
                finish enrolling(msg)
                    
    Finish enrolling(msg: mq.message):
        lock assignments
        If msg.from in assignments: Return
            
            
            
    Assign(requester host name: +name, proposed item count: int):
        assignment := Make assignment--requester host name, proposed item count.
        response := Make message({
            topic := "domain.nitro.assignment"
            to := requester host name
        })
        If assignment:
            response.Extend({
                code := NITRO_NEW_ASSIGNMENT
                payload := assignment
            })
        Else:
            response.Extend({
                code := NITRO_ASSIGNMENT_NOT_AVAILABLE
            })
        response queue := Get response queue for host--requester host name.
        response queue.Send(msg)
            
    Make assignment(*assign.params): assignment
        commands := list of str
        batch := active batch
        lock batch:
            while len(commands) < proposed item count:
                cmd := batch.next()
                If cmd:
                    commands.append(cmd)
                Else:
                    break
        If commands:
            new := Make assignment(batch.id, commands)
    
    Add batch(batch file path: str): batch
        batch := make batch(batch file path)
        If batch:
            lock file batches:
                file batches.append(batch)
                    
    Enroll helpers():
        # It's only legal to call this function if we haven't already
        # enrolled a bunch of helpers.
        precondition: len(heartbeats) == 0
        msg = make message({
            topic := "domain.nitro.enroll",
            to := "*",
            code := NITRO_HELP_REQUESTED,
            nodes requested := assigned helpers
        })
        mq.multicast(msg)

        # Sleep for a second to let responses come back -- but interrupt
        # if we get word that all helpers responded.
        If Sleep(1000\millisecs, finished signal) == 0:
            for node name in helpers:
                If node not in heartbeats:
                    send direct(node name, msg)

            If Sleep(1000\millisecs, finished signal) == 0:
                log("Couldn't enroll everybody.")
                
            
    Flush():
        Lock file batches:
            for batch in file batches:
                Abandon assignments for batch(batch)
            file batches.Clear()

    Run(batches: list of batch, helpers: list of str)
        Start publishing messages()
        Start listening()
        Enroll helpers()
        Start maintenance thread()
