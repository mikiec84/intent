/* DO NOT ADD A SENTRY TO THIS FILE. OMITTED DELIBERATELY. */


// EVENT(name, severity, escalation, number, topic, msg, comments)
//
// Field descriptions/examples/valid values:
//
// name: [unique identifier]
// severity: info, WARNING, error, fatal
// escalation: user, POWERUSER, ADMIN, internal
// number: [unique number for MTM--13 bits max]
// msg: [associated message that is shown, with interp-style substitution]
// comments: [description used for documentation/explanation purposes]


EVENT(E_BAD_CMDLINE, error, user, 1,
		"intent.input",
		"BAD CMDLINE; REPLACE",
		"")


#undef EVENT
