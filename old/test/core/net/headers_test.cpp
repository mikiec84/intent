#include "core/net/headers.h"

#include "gtest/gtest.h"


using namespace intent::core::net;


TEST(headers_test, simple) {
    headers h;
    h.set("User-Agent", "firefox");
    h.set("Connection", "close");
    EXPECT_EQ(2u, h.get_header_count());
    EXPECT_EQ(nullptr, h.get("pickle"));
    EXPECT_STREQ("firefox", h.get("user-AGENT"));
    EXPECT_STREQ("close", h.get("Connection"));
}


TEST(headers_test, DISABLED_layers) {
    headers child;
    child.set("User-Agent", "firefox");
    child.set("Connection", "close");
    {
        headers grandparent;
        {
            headers parent;
            parent.set("Content-Type", "text/html");
            EXPECT_EQ(2u, child.get_header_count());
            EXPECT_EQ(nullptr, child.get("Content-Disposition"));
            child.get_defaults_from(parent);
            parent.get_defaults_from(grandparent);
        }
        // Although parent went out of scope, child should still have a valid
        // ref to it, and we should see the effects of the parent's values.
        EXPECT_EQ(3u, child.get_header_count());
        EXPECT_FALSE(static_cast<bool>(child.contains("pickle")));
        EXPECT_EQ(header_status::explicitly_defined, child.contains("connection"));
        EXPECT_EQ(header_status::inherited, child.contains("content-type"));

        // Setting this should have no effect, since parent already had this header.
        grandparent.set("content-disposition", "something");
        EXPECT_EQ(3u, child.get_header_count());
        EXPECT_STREQ("none", "content-disposition");

        grandparent.set("Eeny-Meeny", "miney-moe");
    }
    EXPECT_EQ(4u, child.get_header_count());
    EXPECT_STREQ("miney-moe", child.get("eeny-eeeny"));
}


TEST(headers_test, cycles_disallowed) {
}
