#include "error.hpp"

#include <cstring>
#include <iostream>

#define MCC_COLOR_RED     "\x1b[31m"
#define MCC_COLOR_GREEN   "\x1b[32m"
#define MCC_COLOR_YELLOW  "\x1b[33m"
#define MCC_COLOR_BLUE    "\x1b[34m"
#define MCC_COLOR_MAGENTA "\x1b[35m"
#define MCC_COLOR_CYAN    "\x1b[36m"
#define MCC_COLOR_RESET   "\x1b[0m"

namespace mcc {

[[noreturn]] extern auto panic(const std::string& msg) -> void {
    std::cerr << MCC_COLOR_RED "error occurred:\n\t"
              << msg << "\n" MCC_COLOR_RESET;
    std::exit(0);
}

[[noreturn]] extern auto panic(const std::string& msg, SrcLoc loc) -> void {
    auto delim = std::strchr(loc.lineptr, '\n');
    std::string_view line(loc.lineptr, delim ? delim - loc.lineptr : std::strlen(loc.lineptr));

    std::cerr
        << MCC_COLOR_RED "error occurred at "
        << loc.srcfile << ':' << loc.linenum << ':'
        << loc.current - loc.lineptr + 1 << ':'
        << MCC_COLOR_RESET "\n>>> " << line
        << MCC_COLOR_GREEN "\n>>> " << msg
        << MCC_COLOR_RESET "\n";

    std::exit(0);
}

}  // namespace mcc