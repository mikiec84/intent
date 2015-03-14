#ifndef _6753f333c9f111e4a5563c15c2ddfeb4
#define _6753f333c9f111e4a5563c15c2ddfeb4

/**
 * The responsibility of releasing resources is being passed across a function
 * call -- either a function is accepting ownership of a raw pointer, or it is
 * returning a raw pointer that the caller must manage. This is the ownership
 * pattern implemented by std::unique_ptr, so there is no need to document it
 * when std::unique_ptr is being used.
 */
#define PASS_OWNERSHIP

/**
 * A resource is ref-counted and will be mutually "owned" -- with release
 * occurring when ref count reaches zero. This is the ownership pattern
 * associated with std::shared_ptr.
 */
#define SHARE_OWNERSHIP

/**
 * A reference or pointer is being passed, but ownership is not transferring.
 * This implies that the called function/class must use the resource during a
 * lifetime that ends before that of the owner.
 */
#define NO_OWNERSHIP_CHANGE

/**
 * Used on declarations of member variables to make it clear whether a resource
 * is owned by a particular class. Member variables that are references can
 * never be owned, and member variables that are simple values (whether POD or
 * objects) are owned by definition--but members variables that are pointers
 * might be owned or not...
 */
#define OWNED

/** Contrast OWNED */
#define NOT_OWNED

#endif // sentry
