#ifndef ioutil_h_cf9b63decce44726bcea23c4d5038f65
#define ioutil_h_cf9b63decce44726bcea23c4d5038f65

#include <boost/filesystem.hpp>
#include <cstdio>
#include <cstdint>
#include <string>
#include <vector>

/**
 * @brief read_file Return all the bytes in a file.
 *
 * Note: Move semantics allow a large buffer to be returned without unnecessary
 * memcpy'ing; this is as cheap as malloc'ing a raw buf and returning it.
 */
std::vector<uint8_t> read_binary_file(boost::filesystem::path const & fpath, size_t max_size=1024*1024);

/**
 * @brief read_file Return all the bytes in a text file as a string.
 *
 * Note: Move semantics allow a large buffer to be returned without unnecessary
 * memcpy'ing; this is as cheap as malloc'ing a raw buf and returning it.
 */
std::string read_text_file(boost::filesystem::path const & fpath, size_t max_size=1024*1024);

/**
 * @brief Wrap a c-style FILE *
 */
class c_file {
    friend int fclose(c_file &);
protected: // normally, should be private--but needed to fix order-of-construction in easy_temp_c_file ctor
    FILE * f;
public:
    /** @brief Wrap an already-open FILE or the results of a call to fopen() *. */
    c_file(FILE *);
    /** @brief Call fopen() and wrap the results. */
    c_file(boost::filesystem::path const & fpath, char const * mode);
    /** @brief Call fclose() if internal FILE * is open. */
    virtual ~c_file();

    /**
     * @brief Allow this object to be implicitly cast to a FILE *.
     */
    operator FILE *();

    /**
     * @brief operator bool: Test whether file is open.
     */
    operator bool() const;

    // Does not support value semantics.
    c_file(c_file &) = delete;
    c_file & operator =(c_file &) = delete;
};

/** Safe overload so c_file isn't cast to FILE * in ordinary call to fclose(). */
int fclose(c_file &);

/**
 * @brief Delete a file name when exiting the current scope.
 */
class file_delete_on_exit {
    boost::filesystem::path fpath;
public:
    file_delete_on_exit(boost::filesystem::path const & path);
    ~file_delete_on_exit();
};

/**
 * @brief easy_temp_file_path Return a path for a file in the
 *     operating system's temp folder.
 */
boost::filesystem::path easy_temp_file_path();

/**
 * @brief Open a file for writing in the temp folder. Optionally,
 *     delete file on exit.
 */
struct easy_temp_c_file : public c_file {
    boost::filesystem::path path;
    easy_temp_c_file();

    // Does not support value semantics.
    easy_temp_c_file(easy_temp_c_file &) = delete;
    easy_temp_c_file & operator =(easy_temp_c_file &) = delete;
};

#endif // IOUTIL_H
