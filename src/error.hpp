#pragma once

#include "srcstream.hpp"

namespace mcc {

[[noreturn]] extern auto panic(const std::string& msg) -> void;
[[noreturn]] extern auto panic(const std::string& msg, SrcLoc loc) -> void;

}  // namespace mcc