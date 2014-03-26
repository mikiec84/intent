// No sentry. Don't add one!

//TUPLE( number, name, msg )

// codes 1-1999 are for definite errors
TUPLE( 1, unterminated_string_literal, "String literal ends without a closing quote." )
TUPLE( 2, unexpected_continuation, "It makes no sense for a line continuation to appear here." )

// codes 2000-2999 are for warnings (might be a problem, might not)

// codes 3000-3999 are for suggestions


#undef TUPLE

