#pragma once

#include <string>

namespace mcc {

struct SrcLoc {
    const char *srcfile;
    const char *current;
    const char *lineptr;
    size_t linenum;
};

class SrcStream {
public:
    SrcStream(const char *srcfile);
    ~SrcStream() = default;

    auto operator*() const -> char;
    auto operator++() -> SrcStream &;
    auto operator++(int) -> SrcStream;
    operator bool() const;

    auto reset(SrcLoc) -> void;
    auto match(char) -> bool;
    auto match(char, char) -> bool;
    auto match(char, char, char) -> bool;
    [[nodiscard]] auto location() const -> SrcLoc;

    template <typename Pred>
    auto skip(Pred pred) -> void {
        while (pred(*m_current)) ++*this;
    }

private:
    std::string m_source;
    const char *m_srcfile;
    const char *m_current;
    const char *m_lineptr;
    size_t m_linenum;
};

}  // namespace mcc