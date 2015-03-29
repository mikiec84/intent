#ifndef _59da4c677b704d259672cc9c267cfe02
#define _59da4c677b704d259672cc9c267cfe02

#include "core/marks/marks.h"


define_mark(pass_ownership, (), param, none,
	"The responsibility of releasing resources is being passed across a function"
	" call -- either a function is accepting ownership of a raw pointer, or it is"
	" returning a raw pointer that the caller must manage. This is the ownership"
	" pattern implemented by std::unique_ptr, so there is no need to document it"
	" when std::unique_ptr is being used.")


define_mark(share_ownership, (), param | instance, none,
	"A resource is ref-counted and will be mutually owned -- with release"
	" occurring when ref count reaches zero. This is the ownership pattern"
	" associated with std::shared_ptr.")


define_mark(no_ownership_change, (), param, none
	"A reference or pointer is being passed, but ownership is not transferring."
	" This implies that the called function/class must use the resource during a"
	" lifetime that ends before that of the owner.")


define_mark(owned_elsewhere, member_instance, none
	"Used on declarations of member variables to make it clear that a resource"
	" isn't \"owned\"--just referred to. Owned members are ones that must be"
	" released. In C++, member variables that are references can never be owned,"
	" and member variables that are simple values (whether POD or objects) are"
	" owned by definition--but members variables that are pointers might be"
	" owned or not...")

#endif // sentry
