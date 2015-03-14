#ifndef _67525ccac9f111e48a903c15c2ddfeb4
#define _67525ccac9f111e48a903c15c2ddfeb4

#include "core/util/value_semantics.h"

namespace boost { namespace filessytem { class path; }}

namespace intent {
namespace core {

namespace io {
    class c_file;
}

namespace data {

/**
 * Implement reading of comma-, tab-, and *-separated-values data.
 */
class xsv_reader {
    struct data_t;
    data_t * data;

public:
    /**
     * Read data from a char array.
     *
     * @param data Non-const because the data will be modified as it is read,
     *     to insert null terminators between fields, and possibly to collapse
     *     escape sequences. Modifications will always leave the data
     *     null-terminated, and will never increase the size of the data.
     * @param delim Which character delimits fields?
     */
    xsv_reader(char * txt, char delim);
    /**
     * Read data from a string.
     *
     * @param data Non-const because the content will be modified as it is read,
     *     to insert null terminators between fields, and possibly to collapse
     *     escape sequences. Modifications will not change the size of data.
     * @param delim Which character delimits fields?
     */
    xsv_reader(std::string & txt, char delim);

    /**
     * Read data from a file that was fopen()'ed in read mode. The reader takes
     * ownership of the file and closes it when done.
     *
     * @param delim Which character delimits fields?
     */
    xsv_reader(intent::core::io::c_file && file, char delim);

    /**
     * Open an existing file and read its data.
     *
     * @param delim Which character delimits fields?
     */
    xsv_reader(boost::filesystem::path const & file, char delim);

    ~xsv_reader();

    MOVEABLE_BUT_NOT_COPYABLE(xsv_reader);

    size_t get_field_count() const;
    char const * get_field_by_index(size_t i) const;
    bool next_record();
};


}}} // end namespace

#endif // sentry

