#include <cstring>
#include <unordered_map>
#include <vector>

#include "error.hpp"
#include "srcstream.hpp"
#include "tkstream.hpp"
#include "token.hpp"

namespace mcc {

static constexpr auto isident(int ch) -> bool { return isalnum(ch) || ch == '_' || ch == '$'; }

static const std::unordered_map<std::string_view, TokenKind> kKeywords{
#define MCC_DEFINE_KEYWORD(ENUM, STRING, VALUE) {STRING, TokenKind::ENUM},
#include "inl/keyword.inl"
#undef MCC_DEFINE_KEYWORD
};

static auto lex_impl(SrcStream &ss) -> Token;

static auto lex_literal(SrcStream &ss, SrcLoc loc, TokenKind kind, char term) -> Token {
    const auto first = ss.location().current;
    ss.skip([&](char ch) {
        if (ch == '\\') (++ss).match(term);
        if (ch == '\0') panic("expect literal terminator.", ss.location());
        return !ss.match(term);
    });
    const auto last = ss.location().current - 1;
    return Token{kind, {first, last}, loc};
}
static auto lex_punct_impl(SrcStream &ss) -> TokenKind {
    const int ch = *ss++;
    switch (ch) {
        case '(': return TokenKind::LParen;
        case ')': return TokenKind::RParen;
        case '{': return TokenKind::LBrace;
        case '}': return TokenKind::RBrace;
        case '[': return TokenKind::LBracket;
        case ']': return TokenKind::RBracket;
        case ';': return TokenKind::Semicolon;
        case ',': return TokenKind::Comma;
        case ':': return TokenKind::Colon;
        case '?': return TokenKind::Ques;
        case '~': return TokenKind::BitNot;
        case '.': return ss.match('.', '.') ? TokenKind::Ellipsis : TokenKind::Dot;
        case '#': return ss.match('#') ? TokenKind::DSharp : TokenKind::Sharp;
        case '*': return ss.match('=') ? TokenKind::MulAssign : TokenKind::Mul;
        case '%': return ss.match('=') ? TokenKind::ModAssign : TokenKind::Mod;
        case '/': return ss.match('=') ? TokenKind::DivAssign : TokenKind::Div;
        case '=': return ss.match('=') ? TokenKind::Equal : TokenKind::Assign;
        case '!': return ss.match('=') ? TokenKind::NotEqual : TokenKind::Not;
        case '^': return ss.match('=') ? TokenKind::XorAssign : TokenKind::BitXor;
        case '&':
            if (ss.match('=')) return TokenKind::AndAssign;
            if (ss.match('&')) return TokenKind::And;
            return TokenKind::BitAnd;
        case '|':
            if (ss.match('=')) return TokenKind::OrAssign;
            if (ss.match('|')) return TokenKind::Or;
            return TokenKind::BitOr;
        case '+':
            if (ss.match('=')) return TokenKind::AddAssign;
            if (ss.match('+')) return TokenKind::Increase;
            return TokenKind::Add;
        case '-':
            if (ss.match('=')) return TokenKind::SubAssign;
            if (ss.match('-')) return TokenKind::Decrease;
            if (ss.match('>')) return TokenKind::Arrow;
            return TokenKind::Sub;
        case '<':
            if (ss.match('<', '=')) return TokenKind::ShlAssign;
            if (ss.match('<')) return TokenKind::BitShl;
            if (ss.match('=')) return TokenKind::LessEqual;
            return TokenKind::Less;
        case '>':
            if (ss.match('>', '=')) return TokenKind::ShrAssign;
            if (ss.match('>')) return TokenKind::BitShr;
            if (ss.match('=')) return TokenKind::GreaterEqual;
            return TokenKind::Greater;
        default:
            panic("unknown punctuator.", ss.location());
    }
}
static auto lex_punct(SrcStream &ss, SrcLoc loc) -> Token {
    // line comment
    if (ss.match('/', '/')) {
        ss.skip([&](char ch) { return ch && !ss.match('\n'); });
        return lex_impl(ss);
    }

    // block comment
    if (ss.match('/', '*')) {
        ss.skip([&](char ch) {
            if (!ch) panic("expect comment terminator `*/`.", ss.location());
            return !ss.match('*', '/');
        });
        return lex_impl(ss);
    }

    if (ss.match('\"')) return lex_literal(ss, loc, TokenKind::Str, '\"');
    if (ss.match('\'')) return lex_literal(ss, loc, TokenKind::Char, '\'');
    const auto first = ss.location().current;
    const auto kind  = lex_punct_impl(ss);
    const auto last  = ss.location().current;
    return Token{kind, {first, last}, loc};
}
static auto lex_const(SrcStream &ss, SrcLoc loc) -> Token {
    const auto first = ss.location().current;
    ss.skip([](char ch) { return isident(ch) || ch == '.'; });
    const auto last = ss.location().current;
    return Token{TokenKind::Const, {first, last}, loc};
}
static auto lex_ident(SrcStream &ss, SrcLoc loc) -> Token {
    const auto first = ss.location().current;
    ss.skip(isident);
    const auto last  = ss.location().current;
    const auto ident = std::string_view(first, last - first);
    const auto iter  = kKeywords.find(ident);
    return Token{iter == kKeywords.end() ? TokenKind::Ident : iter->second, {first, last}, loc};
}
static auto lex_impl(SrcStream &ss) -> Token {
    ss.skip([](int ch) { return ch == ' ' || ch == '\t'; });
    auto loc = ss.location();
    if (!ss) return {TokenKind::Eof, "", loc};
    if (ss.match('\n')) return {TokenKind::Line, "", loc};
    if (isdigit(*ss))
        return lex_const(ss, loc);
    if (isident(*ss)) return lex_ident(ss, loc);
    if (ispunct(*ss)) return lex_punct(ss, loc);
    panic("invalid token.", ss.location());
}
extern auto lex(SrcStream &&_ss) -> TkStream {
    auto ss = _ss;
    std::vector<Token> result;

    result.emplace_back(TokenKind::Line, "", ss.location());
    while (ss) result.push_back(lex_impl(ss));
    return TkStream(std::move(result));
}

}  // namespace mcc