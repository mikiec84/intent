#include "core/process/process_util.h"

#if defined(__APPLE__)
#include <libproc.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

std::string get_fully_qualified_path_for_current_process() {
    char pathbuf[1024];
    pid_t pid = getpid();
    int ret = proc_pidpath(pid, pathbuf, sizeof(pathbuf));
    if ( ret <= 0 ) {
        return "";
    }
    char pathbuf2[PATH_MAX];
    if (realpath(pathbuf, pathbuf2)) {
        return pathbuf2;
    }
    return "";
}

#elif defined(_WIN32)
#else
#endif

