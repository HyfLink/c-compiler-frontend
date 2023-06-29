#include "token.hpp"

#include <iostream>

namespace mcc {

extern auto to_string(TokenKind kind) -> std::string_view {
    if (kind == TokenKind::Eof) return "eof";
    if (kind == TokenKind::Str) return "string";
    if (kind == TokenKind::Char) return "character";
    if (kind == TokenKind::Const) return "constant";
    if (kind == TokenKind::Ident) return "identifier";
    if (is_keyword(kind)) return "keyword";
    if (is_punct(kind)) return "punctuator";
    return "invalid";
}

extern auto operator<<(std::ostream &os, const Token &token) -> std::ostream & {
    os << '<' << to_string(token.kind);
    if (!token.string.empty()) {
        os << ':' << token.string;
    }
    return os << '>';
}

}  // namespace mcc