# Exceptions and problems

Problems are conditions that require remediation, can be declared:
–Inside any block, including func, try, catch, loop, conditional
–Problem object can take action synchronously or asynchronously
–Problem object has built-in support for frequency rules, circuit breakers, logging, notifications
problem “flaky network” if codepath.throws.frequency > 3%:

window: 10 seconds

circuit breaker: <alt code path>