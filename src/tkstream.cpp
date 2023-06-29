#include "tkstream.hpp"

#include <tuple>

#include "error.hpp"

namespace mcc {

TkStream::TkStream(std::vector<Token> &&tokens)
    : m_tokens(std::move(tokens)), m_current(0) {}

auto TkStream::match(TokenKind kind) -> bool {
    bool result = m_current < m_tokens.size() && peek().kind == kind;
    if (result) ++m_current;
    return result;
}

auto TkStream::match(TokenKind kind, std::string &string) -> bool {
    bool result = m_current < m_tokens.size() && peek().kind == kind;
    if (result) {
        string = peek().string;
        ++m_current;
    }
    return result;
}

auto TkStream::expect(TokenKind kind, const std::string &msg) -> void {
    if (m_current < m_tokens.size() && peek().kind != kind) {
        panic(msg, peek().loc);
    } else {
        ++m_current;
    }
}

}  // namespace mcc