#ifndef intent_core_xsv_file_h
#define intent_core_xsv_file_h

namespace intent {
namespace core {

/**
 * Implement reading and writing of comma-, tab-, and *-separated-values
 * files.
 */
class xsv_file {
    struct data_t;
    data_t * data;

    xsv_file();
    char next_char();

public:
    // Use static factory functions to create.
    static xsv_file open_for_read(char const * fname, char delim);
    static xsv_file open_for_write(char const * fname, char delim);

    // Is not copyable.
    xsv_file(xsv_file const &) = delete;
    xsv_file & operator =(xsv_file const &) = delete;

    // .. but IS movable.
    xsv_file(xsv_file &&);
    xsv_file & operator =(xsv_file &&);

    ~xsv_file();

    bool is_writable() const;
    size_t get_field_count() const;
    char const * get_field_by_index(size_t i) const;
    bool next_record();
};


}} // end namespace

#endif // sentry

