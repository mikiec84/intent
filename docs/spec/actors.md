# Actors

Modeled on actors in Scala and Erlang.

http://savanne.be/articles/concurrency-in-erlang-scala/

independent objects with their own thread of execution plus ability to send/receive messages. Messages can contain primitives but not objects; no shared state. No guarantee on order of delivery.

support for request/reply and synchronous
support for zmq patterns incl pub/sub and push/pull

thread-based (heavier, some pre-emption and fairness) and event-based actors (very light-weight but no fairness)

relocatable?

http://en.wikipedia.org/wiki/Actor_model

unlike in scala, you can't invoke methods on actors directly; you can only pass messages