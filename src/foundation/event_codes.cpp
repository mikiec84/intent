#include "foundation/event_codes.h"

namespace intent { namespace foundation { namespace event_codes {

event_data const ITEMS[] = {
	#define EVENT(name, severity, escal, num, topic, msg, comments) \
	{ name, #name, sev_##severity, esc_##escal, num, topic, msg, comments },
	#include "foundation/event_tuples.h"
};

static const size_t ITEM_COUNT = sizeof(ITEMS) / sizeof(event_data);

bool publish() {
	static bool published = false;
	if (!published) {
		published = publish_event_data(ITEMS, ITEM_COUNT);
	}
	return published;
}

}}} // end namespace
