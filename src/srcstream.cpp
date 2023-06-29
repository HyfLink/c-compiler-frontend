#include "srcstream.hpp"

#include <fstream>
#include <streambuf>

#include "error.hpp"

namespace mcc {

namespace detail {

auto load(const char *filename) -> std::string {
    std::ifstream file(filename);

    if (!file) panic(std::string("failed to open file ") + filename);

    return std::string(std::istreambuf_iterator<char>(file),
                       std::istreambuf_iterator<char>());
}

}  // namespace detail

SrcStream::SrcStream(const char *srcfile)
    : m_source(detail::load(srcfile)),
      m_srcfile(srcfile),
      m_current(m_source.c_str()),
      m_lineptr(m_source.c_str()),
      m_linenum(1) {}

auto SrcStream::operator*() const -> char {
    return *m_current;
}

auto SrcStream::operator++() -> SrcStream & {
    if (*m_current++ == '\n') {
        m_linenum++;
        m_lineptr = m_current;
    }
    return *this;
}

auto SrcStream::operator++(int) -> SrcStream {
    auto result = *this;
    ++*this;
    return result;
}

SrcStream::operator bool() const {
    return *m_current != '\0';
}

auto SrcStream::reset(SrcLoc loc) -> void {
    m_current = loc.current;
    m_lineptr = loc.lineptr;
    m_linenum = loc.linenum;
}

auto SrcStream::match(char c1) -> bool {
    if (m_current[0] == c1) {
        ++*this;
        return true;
    }
    return false;
}

auto SrcStream::match(char c1, char c2) -> bool {
    if (m_current[0] == c1 && m_current[1] == c2) {
        ++++*this;
        return true;
    }
    return false;
}

auto SrcStream::match(char c1, char c2, char c3) -> bool {
    if (m_current[0] == c1 && m_current[1] == c2 && m_current[2] == c3) {
        ++++++*this;
        return true;
    }
    return false;
}

auto SrcStream::location() const -> SrcLoc {
    return {m_srcfile, m_current, m_lineptr, m_linenum};
}

}  // namespace mcc