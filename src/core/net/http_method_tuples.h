// No sentry. Don't add one!

// TUPLE(id, str, first_rfc, last_rfc)

TUPLE(http_get, GET, 1945, 0)
TUPLE(http_put, PUT, 1945, 0)
TUPLE(http_post, POST, 1945, 0)
TUPLE(http_options, OPTIONS, 1945, 0)
TUPLE(http_head, HEAD, 1945, 0)
TUPLE(http_delete, DELETE, 1945, 0)
TUPLE(http_trace, TRACE, 2616, 0)
TUPLE(http_connect, CONNECT, 2616, 0)
TUPLE(http_link, LINK, 1945, 0)
TUPLE(http_unlink, UNLINK, 1945, 0)
TUPLE(http_patch, PATCH, 5789, 0)

// WebDAV
TUPLE(webdav_propfind, PROPFIND, 2518, 0)
TUPLE(webdav_proppatch, PROPPATCH, 2518, 0)
TUPLE(webdav_mkcol, MKCOL, 2518, 0)
TUPLE(webdav_copy, COPY, 2518, 0)
TUPLE(webdav_move, MOVE, 2518, 0)
TUPLE(webdav_lock, LOCK, 2518, 0)
TUPLE(webdav_unlock, UNLOCK, 2518, 0)

#undef TUPLE
