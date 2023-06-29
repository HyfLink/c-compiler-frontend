#include <map>

#include "error.hpp"
#include "mcc.hpp"
#include "tkstream.hpp"

namespace mcc {

static auto try_preprocessor(TkStream &ts, std::map<std::string, std::vector<Token>> &macros, std::vector<Token> &result) -> bool {
    while (ts.match(TokenKind::Line)) {
        if (ts.match(TokenKind::Sharp)) {
            auto pp  = ts.peek().string;
            auto loc = ts.peek().loc;
            ts.expect(TokenKind::Ident, "expect identifier after `#`.");
            if (pp == "define") {
                ///
                /// #define MACRO {TOKENS}
                ///
                auto macro   = ts.peek().string;
                auto &tokens = macros[macro];
                ts.expect(TokenKind::Ident, "expect macro name in `#define`.");
                while (ts && !ts.detect(TokenKind::Line)) {
                    tokens.push_back(ts.next());
                }
            } else if (pp == "undef") {
                ///
                /// #undef MACRO
                ///
                auto macro = ts.peek().string;
                ts.expect(TokenKind::Ident, "expect macro name in `#undef`.");
                macros.erase(macro);
            } else if (pp == "include") {
                ///
                /// #include "path/to/header"
                ///
                auto path = std::string(ts.peek().string);
                ts.expect(TokenKind::Str, "expect path in `#include`.");

                TkStream inc = preprocess(lex(SrcStream(path.c_str())));
                result.insert(result.end(), inc.begin(), inc.end());
            } else {
                panic("invalid preprocessor", loc);
            }
            return true;
        }
    }
    return false;
}
static auto try_expand_macro(TkStream &ts, std::map<std::string, std::vector<Token>> &macros, std::vector<Token> &result) -> bool {
    if (ts.detect(TokenKind::Ident)) {
        auto macro = ts.peek().string;
        auto iter  = macros.find(macro);
        if (iter != macros.end()) {
            ts.next();
            result.insert(result.end(), iter->second.begin(), iter->second.end());
            return true;
        }
    }
    return false;
}

extern auto preprocess(TkStream &&_ts) -> TkStream {
    std::map<std::string, std::vector<Token>> macros;
    std::vector<Token> result;
    auto ts = _ts;

    result.reserve(ts.end() - ts.begin());

    while (ts) {
        if (try_preprocessor(ts, macros, result)) continue;
        if (try_expand_macro(ts, macros, result)) continue;
        if (ts) result.push_back(ts.next());
    }

    return TkStream(std::move(result));
}

}  // namespace mcc