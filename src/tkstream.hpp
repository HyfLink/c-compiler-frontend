#pragma once
#include <vector>

#include "srcstream.hpp"
#include "token.hpp"

namespace mcc {

/// TkStream
/// ----------------------------------------------------------------------------
class TkStream {
public:
    TkStream(std::vector<Token> &&tokens);
    ~TkStream() = default;

    inline operator bool() const { return m_current < m_tokens.size(); }
    inline auto peek() -> Token { return m_tokens[m_current]; }
    inline auto next() -> Token { return m_tokens[m_current++]; }
    inline auto reset(size_t loc) -> void { m_current = loc; }
    inline auto location() -> size_t { return m_current; }
    inline auto detect(TokenKind kind) -> bool { return m_tokens[m_current].kind == kind; }

    auto match(TokenKind) -> bool;
    auto match(TokenKind, std::string &) -> bool;
    auto expect(TokenKind, const std::string &) -> void;

    auto begin() -> decltype(auto) { return m_tokens.begin(); }
    auto end() -> decltype(auto) { return m_tokens.end(); }

    template <typename Pred>
    auto match(Pred pred) -> bool {
        bool result = m_current < m_tokens.size() && pred(peek());
        if (result) ++m_current;
        return result;
    }

private:
    std::vector<Token> m_tokens;
    size_t m_current;
};

}  // namespace mcc