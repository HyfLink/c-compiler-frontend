#pragma once

#include <cstdint>
#include <iosfwd>
#include <string>

#include "srcstream.hpp"

namespace mcc {

/// TokenKind
/// ----------------------------------------------------------------------------
enum class TokenKind {
    None = 0,
    Eof,
    Str,
    Char,
    Const,
    Ident,
    Line,

    __MASK_PUNCT__   = 0x01000000,
    __MASK_KEYWORD__ = 0x02000000,

#define MCC_DEFINE_KEYWORD(ENUM, STRING, VALUE) ENUM = static_cast<uint32_t>(__MASK_KEYWORD__) | VALUE,
#define MCC_DEFINE_PUNCTUATOR(ENUM, STRING, VALUE) ENUM = static_cast<uint32_t>(__MASK_PUNCT__) | VALUE,
#include "inl/keyword.inl"
#include "inl/punctuator.inl"
#undef MCC_DEFINE_KEYWORD
#undef MCC_DEFINE_PUNCTUATOR
};

/// Token
/// ----------------------------------------------------------------------------
struct Token {
    Token(TokenKind k, std::string s, SrcLoc l) : kind(k), string(s), loc(l) {}
    Token(TokenKind k, const char *s, SrcLoc l) : kind(k), string(s), loc(l) {}
    Token(TokenKind k, std::string_view s, SrcLoc l) : kind(k), string(s), loc(l) {}
    ~Token() = default;
    TokenKind kind;
    std::string string;
    SrcLoc loc;
};

/// TokenKind : functions
static constexpr bool is_punct(TokenKind t) { return static_cast<uint32_t>(t) & static_cast<uint32_t>(TokenKind::__MASK_PUNCT__); }
static constexpr bool is_punct(const Token &t) { return static_cast<uint32_t>(t.kind) & static_cast<uint32_t>(TokenKind::__MASK_PUNCT__); }
static constexpr bool is_keyword(TokenKind t) { return static_cast<uint32_t>(t) & static_cast<uint32_t>(TokenKind::__MASK_KEYWORD__); }
static constexpr bool is_keyword(const Token &t) { return static_cast<uint32_t>(t.kind) & static_cast<uint32_t>(TokenKind::__MASK_KEYWORD__); }

extern auto operator<<(std::ostream &, const Token &) -> std::ostream &;

}  // namespace mcc