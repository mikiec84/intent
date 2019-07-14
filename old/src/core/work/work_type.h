#ifndef _585c8a113d3944efb2bfe1c8e8865413
#define _585c8a113d3944efb2bfe1c8e8865413

#include <cstdint>

namespace intent {
namespace core {
namespace work {

/**
 * Define an unbounded enum (suitable for extension by any number of components)
 * that distinguishes different types of work. Here, we use the term "work"
 * very loosely; basically it represents anything that a human or automated user
 * might use as an indicator that "something useful" is happening, or that
 * progress is being made.
 *
 * We tell a progress_tracker to expect work of particular types, in particular
 * quantities. We then update the tracker as work proceeds; consumers of the
 * data inside the progress_tracker can then tell where we are, relative to where
 * we think we need to be to be "done".
 *
 * A simple file-processing utility might be able to describe its progress using
 * only values declared here. However, a complex app might ignore the work types
 * below entirely, in favor of its own set.
 *
 * All extensions to this enum should be declared using custom_work_type() (see
 * below).
 */
enum class work_type: uint64_t {
    /** Rough measure of compute intensity. */
    flops,

    /** Work that involves waiting for a specific amount of time. */
    elapsed_secs,

    /** Granular work that reads data (e.g., from a file or socket). */
    read_bytes,

    /** Granular work that writes data (e.g., to a file or socket). */
    write_bytes,

    /**
     * Input I/O at a less granular level than bytes -- receiving messages,
     * files, lines, responses, or even physical packages. Also useful
     * for the 'R' in CRUD -- read a record from a DB.
     */
    read_items,

    /**
     * Output I/O at a less granular level than bytes -- sending messages,
     * files, lines, requests, or even physical packages. Not useful as the
     * 'U' or 'C' in CRUD; see #create_items or #update_items instead.
     */
    write_items,

    /**
     * Work that can be quantified by amount of physical movement, such as
     * traffic navigation, orbits and trajectories, or motion sensors on robots.
     */
    travel_meters,

    /** SI unit of mechanical work: a newton-meter. */
    joules,

    /**
     * Work that can be quantified by how many times something goes around,
     * either physically or metaphorically.
     */
    cycles,

    /**
     * Work that can be quantified by radioactive-like decay, such as mean
     * time between failure on hard drives.
     */
    half_lives,

    /** Create/insert a new record in a database, a new object in a graph, etc. */
    create_items,

    /** Update an existing record in a database, an object in a graph or object model, etc. */
    update_items,

    /** Delete an existing record in a database, an object in a graph or object model, etc. */
    delete_items,

    /** Work that involves printing. */
    print_pages,

    /** Install an app. */
    install,

    /** Uninstall an app. */
    uninstall,

    /** Launch another process but do not wait for it to complete. */
    start_process,

    /** Execute a script, or run a child process until it completes. */
    run_command,

    /** Issue a stop command to another process, and wait until it is honored. */
    stop_process,

    /** Work that is best quantified by how many events occur. */
    events,

    /**
     * Work that is best characterized by steps or phases, such as checking out
     * in a shopping cart webapp or performing a TLS handshake.
     */
    steps,

    /** Work that is layered, as in progressive texturization, map rendering, etc. */
    layers,

    /** Work that involves a reboot. */
    reboots,

    /** Course-grained project-style work that is tracked in gantt charts. */
    milestones,

    _count
};

static_assert(static_cast<int>(work_type::_count) < 4096,
        "Don't use more than 12 bits for generic, pre-defined work types.");

/**
 * Define a work_type constant unique to a particular component.
 * @param component_id The ID assigned to a component. Intent generates this
 *     from the name of the component, unless it's manually defined as one
 *     of the component's properties.
 * @param private_id An arbitrary number chosen by the component.
 * @return A work_type constant suitable for use in progress_tracker.
 */
constexpr work_type custom_work_type(uint32_t component_id, uint16_t private_id) {
    return static_cast<work_type>(static_cast<uint64_t>(component_id) << 32 |
           static_cast<uint64_t>(private_id) << 16 |
           static_cast<uint64_t>(1) << 15);
}


}}} // end namespace

#endif // sentry
