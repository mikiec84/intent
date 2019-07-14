#ifndef _b5e16d9c007043fca863ec0855726d27
#define _b5e16d9c007043fca863ec0855726d27

#include <cstdio>
#include <cstdint>
#include <string>
#include <vector>

#include "core/filesystem.h"
#include "core/util/value_semantics.h"

namespace intent { namespace core { namespace io { class c_file; }}}

/**
 * Safe overload so c_file isn't cast to FILE * in ordinary call to fclose().
 */
int fclose(intent::core::io::c_file &);

namespace intent {
namespace core {
namespace io {

/**
 * Return all the bytes in a file.
 *
 * Note: Move semantics allow a large buffer to be returned without unnecessary
 * memcpy'ing; this is as cheap as malloc'ing a raw buf and returning it.
 */
std::vector<uint8_t> read_binary_file(filesystem::path const & fpath, size_t max_size=1024*1024);

/**
 * Return all the bytes in a text file as a null-terminated string.
 *
 * Note: Move semantics allow a large buffer to be returned without unnecessary
 * memcpy'ing; this is as cheap as malloc'ing a raw buf and returning it.
 */
std::string read_text_file(filesystem::path const & fpath, size_t max_size=1024*1024);

/**
 * Wrap a c-style FILE *
 */
class c_file {
    friend int ::fclose(c_file &);
protected: // normally, should be private--but needed to fix order-of-construction in easy_temp_c_file ctor
    FILE * f;
public:
    /** Wrap an already-open FILE or the results of a call to fopen() *. */
    c_file(FILE *);

    /** Call fopen() and wrap the results. */
    c_file(filesystem::path const & fpath, char const * mode);

    /**
     * Call fclose() if internal FILE * is open.
     */
    virtual ~c_file();

    /**
     * Allow this object to be implicitly cast to a FILE *.
     */
    operator FILE *();

    /**
     * operator bool: Test whether file is open.
     */
    operator bool() const;

    NOT_COPYABLE(c_file);
    MOVEABLE(c_file);
};

/**
 * Delete a file name when exiting the current scope.
 */
class file_delete_on_exit {
    filesystem::path fpath;
public:
    file_delete_on_exit(filesystem::path const & path);
    ~file_delete_on_exit();
};

/**
 * Return a path for a file in the operating system's temp folder.
 */
filesystem::path easy_temp_file_path();

/**
 * Open a file for writing in the temp folder
 */
struct easy_temp_c_file : public c_file {
    filesystem::path path;
    easy_temp_c_file();

    NOT_COPYABLE(easy_temp_c_file);
};

}}} // end namespace

#endif // sentry
