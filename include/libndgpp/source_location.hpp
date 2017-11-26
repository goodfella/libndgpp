#ifndef LIBNDGPP_SOURCE_LOCATION_HPP
#define LIBNDGPP_SOURCE_LOCATION_HPP

#include <ostream>

// This header file can be found in the binary directory
#include <libndgpp/parent_directory_name_length.hpp>

namespace ndgpp
{
    /** Returns a C string containing the first character passed the parent directory
     *
     *  @tparam N The length of the input C string
     *
     *  @param file The file name
     */
    template <std::size_t N>
    char const * relative_file(const char (&file) [N])
    {
        // one is added to account for the trailing directory separator
        static_assert(N > (ndgpp::parent_directory_name_length_t::value + 1),
                      "file length must be greater than parent directory length");
        return file + ndgpp::parent_directory_name_length_t::value;
    }

    /// Represents a source location i.e. file and line
    class source_location
    {
        public:

        template <std::size_t N>
        source_location(const char (&file) [N],
                        const int line):
            file_(ndgpp::relative_file(file)),
            line_(line)
            {}

        int line() const noexcept;
        char const * file() const noexcept;

        private:

        char const * file_ = "none";
        int line_ = {-1};
    };

    inline int source_location::line() const noexcept
    {
        return line_;
    }

    inline char const * source_location::file() const noexcept
    {
        return file_;
    }

    inline std::ostream& operator<<(std::ostream& stream, const source_location& location)
    {
        stream << location.file()
               << ':'
               << location.line();
        return stream;
    }
}

#define NDGPP_SOURCE_LOCATION ndgpp::source_location{__FILE__, __LINE__}

#endif
