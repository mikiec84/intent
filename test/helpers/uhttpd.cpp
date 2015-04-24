#include <atomic>
#include <chrono>
#include <csignal>
#include <random>
#include <string>
#include <thread>

#include <sys/types.h>
#include <unistd.h>

#include "core/process.h"
#include "core/net/curl/response.h"
#include "core/net/curl/request.h"

#include "helpers/uhttpd.h"
#include "helpers/testutil.h"

using std::string;

namespace curl = intent::core::net::curl;
namespace process = intent::core::process;

using intent::core::filesystem::path;


uint16_t get_first_port() {
    static std::mt19937 the_generator;
    static std::uniform_int_distribution<uint16_t> the_dist(10000, 50000);
    static uint16_t start_port = the_dist(the_generator);
    return start_port;
}


struct uhttpd::impl_t {
    uint16_t port;
    process::context ctx;
    process::child * child;
    string base_url;
    bool cant_connect;

    impl_t();
    ~impl_t();
};


std::atomic<uint16_t> next_port(get_first_port());


#define THIS_FNAME "uhttpd.cpp"
static const path & this_folder() {
    static const path the_path = find_test_folder("test/helpers/" THIS_FNAME);
    return the_path;
}


uhttpd::impl_t::impl_t(): port(next_port++), ctx(), child(nullptr), base_url(),
        cant_connect(false) {

    static const std::string python = "/usr/bin/python";

    auto path = this_folder() / "uhttpd.py";

    char port_buf[16];
    sprintf(port_buf, "%d", port);
    printf("%s %s %s\n...", python.c_str(), path.c_str(), port_buf);

    ctx.stdout_behavior = process::silence_stream();
    std::vector<string> args;
    args.push_back(port_buf);
    child = new process::child(process::launch(python, args, ctx));

    char url[128];
    sprintf(url, "http://localhost:%d/", port);
    base_url = url;
    // wait a few ms for server to be fully up
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
}


uhttpd::impl_t::~impl_t() {
    delete child;
}


uhttpd::uhttpd() : impl(new impl_t()) {
}


char const * uhttpd::get_base_url() const {
    return impl->base_url.c_str();
}


bool uhttpd::stop() {
    auto quit_url = impl->base_url + "quit";
    auto response = curl::request::get(quit_url.c_str());
    if (response.get_status_code() >= 400) {
        auto status = impl->child->wait();
        if (status.exit_status() == -1) {
            return false;
        }
    }
    return true;
}


uhttpd::~uhttpd() {
    if (impl->cant_connect || !stop()) {
        fprintf(stderr, "Unable to stop web server on port %d cleanly. Killing...\n", impl->port);
        kill(impl->child->get_id(), SIGTERM);
    }
    delete impl;
}
