#ifndef _INTENT_CORE_EVENT_CODES_H_
#define _INTENT_CORE_EVENT_CODES_H_

#include "foundation/events.h"

using namespace intent::foundation::events;

namespace intent { namespace core { namespace event_codes {

#define EVENT(name, severity, escalation, number, topic, msg, comments) \
	const ecode_t name = static_cast<ecode_t>( number \
		| static_cast<int>(sev_##severity) << 28 \
		| static_cast<int>(1) << 16 \
		| static_cast<int>(esc_##escalation) << 14);
#include "core/event_tuples.h"

// Force these events to be registered and linked into any binary that
// #includes this header.
bool publish();
static bool published = publish();

}}} // end namespace

#endif // sentry
