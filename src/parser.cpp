#include "asttypes.hpp"
#include "error.hpp"
#include "tkstream.hpp"
#include "type.hpp"

namespace mcc {

///
/// storage_class_spec  : 'auto' | 'register' | 'stmtic' | 'extern' | 'typedef'
/// type_qual           : 'const' | 'volatile'
/// type_spec           : 'void' | 'char' | 'short' | 'int' | 'long'
///                     | 'float' | 'double' | 'signed' | 'unsigned'
///                     | struct_spec | union_spec | enum_spec | id
///
static auto parse_stmt(TkStream &ts) -> AstStmtPointer;
static auto parse_declarator(TkStream &ts, QualType type) -> std::pair<QualType, std::string>;
static auto parse_compound_stmt(TkStream &ts) -> std::unique_ptr<AstStmtCompound>;
static auto parse_conditional_expr(TkStream &) -> AstExprPointer;
static auto parse_type_cast_expr(TkStream &) -> AstExprPointer;

namespace detail {

static auto modify_type(QualType &dst, const QualType &src) -> bool {
    if (dst.type() == nullptr) {
        dst = src;
        return true;
    } else if (auto ptr = dynamic_cast<PointerType *>(dst.type().get()); ptr) {
        return modify_type(ptr->base(), src);
    } else if (auto ptr = dynamic_cast<ArrayType *>(dst.type().get()); ptr) {
        return modify_type(ptr->base(), src);
    }
    return false;
}

template <typename ToOptr, typename Descent>
static auto recursive_descent(Descent descent) {
    return [descent](TkStream &ts) -> AstExprPointer {
        auto result = descent(ts);
        auto optr   = ToOptr::to_optr(ts.peek().kind);

        while (optr != OptrKind::None) {
            ts.next();
            result = std::make_unique<AstExprBinary>(optr, std::move(result), descent(ts));
            optr   = ToOptr::to_optr(ts.peek().kind);
        }

        return result;
    };
};

}  // namespace detail

static const auto parse_multiplicative_expr = detail::recursive_descent<token_to_multiplicative_optr_t>(parse_type_cast_expr);
static const auto parse_additive_expr       = detail::recursive_descent<token_to_additive_optr_t>(parse_multiplicative_expr);
static const auto parse_bitwise_shift_expr  = detail::recursive_descent<token_to_bitwise_shift_optr_t>(parse_additive_expr);
static const auto parse_relational_expr     = detail::recursive_descent<token_to_relational_optr_t>(parse_bitwise_shift_expr);
static const auto parse_equality_expr       = detail::recursive_descent<token_to_equality_optr_t>(parse_relational_expr);
static const auto parse_bitwise_and_expr    = detail::recursive_descent<token_to_bitwise_and_optr_t>(parse_equality_expr);
static const auto parse_bitwise_xor_expr    = detail::recursive_descent<token_to_bitwise_xor_optr_t>(parse_bitwise_and_expr);
static const auto parse_bitwise_or_expr     = detail::recursive_descent<token_to_bitwise_or_optr_t>(parse_bitwise_xor_expr);
static const auto parse_logical_and_expr    = detail::recursive_descent<token_to_logical_and_optr_t>(parse_bitwise_or_expr);
static const auto parse_logical_or_expr     = detail::recursive_descent<token_to_logical_or_optr_t>(parse_logical_and_expr);
static const auto parse_assign_expr         = detail::recursive_descent<token_to_assign_optr_t>(parse_conditional_expr);
static const auto parse_expr                = detail::recursive_descent<token_to_comma_optr_t>(parse_assign_expr);

static auto parse_qual_spec(TkStream &ts) -> std::pair<QualSpec, QualType> {
    ///
    /// qual_spec           : {type_spec | type_qual}
    ///
    auto decl_spec = QualSpec{Qualifier::None, Specifier::None};

    for (; ts; ts.next()) {
        const auto token = ts.peek();
        if (const auto specifier = token_to_specifier(token.kind); specifier != Specifier::None) {
            if ((specifier & std::get<Specifier>(decl_spec)) != Specifier::None) {
                panic("type specifier redefined.", token.loc);
            }
            std::get<Specifier>(decl_spec) |= specifier;
        } else if (const auto qualifier = token_to_qualifier(token.kind); qualifier != Qualifier::None) {
            if ((qualifier & std::get<Qualifier>(decl_spec)) != Qualifier::None) {
                panic("type qualifier redefined.", token.loc);
            }
            std::get<Qualifier>(decl_spec) |= qualifier;
        } else {
            break;
        }
    }

    auto base_type = BasicType::make(std::get<Specifier>(decl_spec));
    auto qual_type = QualType(std::move(base_type), std::get<Qualifier>(decl_spec));
    return {decl_spec, qual_type};
}
static auto parse_decl_spec(TkStream &ts) -> std::pair<DeclSpec, QualType> {
    ///
    /// decl_spec           : {type_qual | type_spec | storage_class_spec }
    ///
    auto decl_spec = DeclSpec{StorageClass::None, Qualifier::None, Specifier::None};

    for (; ts; ts.next()) {
        const auto token = ts.peek();
        if (const auto storage = token_to_storage_class(token.kind); storage != StorageClass::None) {
            if (std::get<StorageClass>(decl_spec) != StorageClass::None) {
                panic("storage class specifier redefined.", token.loc);
            }
            std::get<StorageClass>(decl_spec) = storage;
        } else if (const auto specifier = token_to_specifier(token.kind); specifier != Specifier::None) {
            if ((specifier & std::get<Specifier>(decl_spec)) != Specifier::None) {
                panic("type specifier redefined.", token.loc);
            }
            std::get<Specifier>(decl_spec) |= specifier;
        } else if (const auto qualifier = token_to_qualifier(token.kind); qualifier != Qualifier::None) {
            if ((qualifier & std::get<Qualifier>(decl_spec)) != Qualifier::None) {
                panic("type qualifier redefined.", token.loc);
            }
            std::get<Qualifier>(decl_spec) |= qualifier;
        } else {
            break;
        }
    }

    auto base_type = BasicType::make(std::get<Specifier>(decl_spec));
    auto qual_type = QualType(std::move(base_type), std::get<Qualifier>(decl_spec));
    return {decl_spec, qual_type};
}
static auto parse_param_decl(TkStream &ts) -> std::pair<QualType, std::string> {
    ///
    /// param_decl          : qual_spec declarator
    ///                     | qual_spec abstract_declarator
    ///                     | qual_spec
    ///
    auto [qual_spec, type] = parse_qual_spec(ts);
    return parse_declarator(ts, type);
}
static auto parse_array_declarator(TkStream &ts, QualType type) -> QualType {
    ///
    /// array_declarator : {'[' [const_exp] ']'}
    ///
    while (ts.match(TokenKind::LBracket /* [ */)) {
        type = QualType(ArrayType::make(type, parse_expr(ts)));
        ts.expect(TokenKind::RBracket, "expect array terminator `]`.");
    }
    return type;
}
static auto parse_func_declarator(TkStream &ts, const QualType &type) -> QualType {
    ///
    /// func_declarator : '(' {param_decl ','}['...'] ')';
    ///
    auto func = FunctionType::make(type);
    ts.expect(TokenKind::LParen, "expect token `(`");

    /// no parameter
    if (ts.match(TokenKind::RParen /* ) */)) {
        return QualType(func);
    }

    /// first parameter
    {
        auto [param_type, param_name] = parse_param_decl(ts);
        auto basic_param_type         = std::dynamic_pointer_cast<BasicType>(param_type.type());
        if (basic_param_type && basic_param_type->is_void()) {
            ts.expect(TokenKind::RParen, "expect function param list terminator `)`.");
            return QualType(func);
        } else {
            func->param_types().emplace_back(std::move(param_type));
            func->param_names().emplace_back(std::move(param_name));
        }
    }

    /// rest parameters
    while (ts.match(TokenKind::Comma /* , */)) {
        if (ts.match(TokenKind::Ellipsis /* ... */)) {
            func->variadic() = true;
            break;
        }

        auto [param_type, param_name] = parse_param_decl(ts);
        func->param_types().emplace_back(std::move(param_type));
        func->param_names().emplace_back(std::move(param_name));
    }

    ts.expect(TokenKind::RParen, "expect function param list terminator `)`.");
    return QualType(func);
}
static auto parse_declarator(TkStream &ts, QualType type) -> std::pair<QualType, std::string> {
    ///
    /// declarator          : [pointer] direct_declarator
    /// pointer             : '*' {type_qual}+ [pointer]
    /// direct_declarator   : <id | '(' declarator ')'> <array_declarator | func_declaractor>
    /// func_declarator     : '(' {param_decl ','}['...'] ')';
    /// array_declarator    : {'[' [const_exp] ']'}
    ///
    auto ident = std::string(IAstDecl::kAnonymous);

    while (ts.match(TokenKind::Mul /* * */)) {
        auto qual  = Qualifier::None;
        auto value = token_to_qualifier(ts.peek().kind);
        while (value != Qualifier::None) {
            if ((value & qual) != Qualifier::None) {
                panic("type qualifier redefined.", ts.peek().loc);
            }
            qual |= value;
            ts.next();
            value = token_to_qualifier(ts.peek().kind);
        }
        type = QualType(std::make_unique<PointerType>(std::move(type)), qual);
    }

    const auto parse_array_func_declarator = [](TkStream &ts, const QualType &type) {
        if (ts.detect(TokenKind::LParen /* ( */)) {
            return parse_func_declarator(ts, type);
        } else if (ts.detect(TokenKind::LBracket /* [ */)) {
            return parse_array_declarator(ts, type);
        }
        return type;
    };

    if (ts.match(TokenKind::LParen /* ( */)) {
        auto base_type        = type;
        std::tie(type, ident) = parse_declarator(ts, QualType(nullptr));
        ts.expect(TokenKind::RParen /* ) */, "expect right parentheses `)`.");
        detail::modify_type(type, parse_array_func_declarator(ts, base_type));
    } else {
        ts.match(TokenKind::Ident, ident);
        type = parse_array_func_declarator(ts, type);
    }

    return {type, ident};
}
static auto parse_decl(TkStream &ts) -> AstDeclPointer {
    ///
    /// decl : decl_spec declarator compound_stmt
    ///      | decl_spec declarator [= initializer] {',' declarator [= initializer]}  ';'
    ///
    auto [decl_spec, raw_type] = parse_decl_spec(ts);
    auto [type, identifier]    = parse_declarator(ts, raw_type);
    auto storage               = std::get<StorageClass>(decl_spec);

    if (auto func = std::dynamic_pointer_cast<FunctionType>(type.type()); func) {
        if (ts.match(TokenKind::Semicolon /* ; */)) {
            return std::make_unique<AstDeclFunc>(storage, type, nullptr, identifier);
        } else if (ts.detect(TokenKind::LBrace /* } */)) {
            return std::make_unique<AstDeclFunc>(storage, type, parse_compound_stmt(ts), identifier);
        }
        panic("expect `;` or `{` in function declaration.", ts.peek().loc);
    } else {
        if (ts.match(TokenKind::Semicolon /* ; */)) {
            return std::make_unique<AstDeclVar>(storage, type, nullptr, identifier);
        } else if (ts.match(TokenKind::Assign /* = */)) {
            auto initial = parse_expr(ts);
            ts.expect(TokenKind::Semicolon, "expect `;` after initializer list.");
            return std::make_unique<AstDeclVar>(storage, type, std::move(initial), identifier);
        }
        panic("expect `;` or `=` in variable declaration.", ts.peek().loc);
    }
}
static auto parse_compound_stmt(TkStream &ts) -> std::unique_ptr<AstStmtCompound> {
    std::vector<AstPointer> stmts;
    ts.expect(TokenKind::LBrace, "expect `{`.");
    while (!ts.match(TokenKind::RBrace)) {
        auto kind = ts.peek().kind;

        if (token_to_qualifier(kind) != Qualifier::None ||
            token_to_specifier(kind) != Specifier::None ||
            token_to_storage_class(kind) != StorageClass::None) {
            stmts.emplace_back(parse_decl(ts));
        } else {
            stmts.emplace_back(parse_stmt(ts));
        }
    }
    return std::make_unique<AstStmtCompound>(std::move(stmts));
}
static auto parse_stmt(TkStream &ts) -> AstStmtPointer {
    if (ts.detect(TokenKind::LBrace)) {
        return parse_compound_stmt(ts);
    } else if (ts.match(TokenKind::Semicolon)) {
        return std::make_unique<AstStmtEmpty>();
    } else if (ts.match(TokenKind::KwIf)) {
        ts.expect(TokenKind::LParen, "expect `(` after `if`.");
        auto cond = parse_expr(ts);
        ts.expect(TokenKind::RParen, "expect `)` after `if`.");
        auto then = parse_stmt(ts);
        auto elze = ts.match(TokenKind::KwElse) ? parse_stmt(ts) : nullptr;
        return std::make_unique<AstStmtSelectionIfElse>(std::move(cond), std::move(then), std::move(elze));
    } else if (ts.match(TokenKind::KwSwitch)) {
        ts.expect(TokenKind::LParen, "expect `(` after `switch`");
        auto cond = parse_expr(ts);
        ts.expect(TokenKind::RParen, "expect `)` after `switch`");
        auto body = parse_stmt(ts);
        return std::make_unique<AstStmtSelectionSwitch>(std::move(cond), std::move(body));
    } else if (ts.match(TokenKind::KwFor)) {
        ts.expect(TokenKind::LParen, "expect `(` after `for`.");
        auto init = ts.detect(TokenKind::Semicolon) ? nullptr : parse_expr(ts);
        ts.expect(TokenKind::Semicolon, "expect `;` after `for`.");
        auto cond = ts.detect(TokenKind::Semicolon) ? nullptr : parse_expr(ts);
        ts.expect(TokenKind::Semicolon, "expect `;` after `for`.");
        auto iter = ts.detect(TokenKind::RParen) ? nullptr : parse_expr(ts);
        ts.expect(TokenKind::RParen, "expect `)` after `for`.");
        auto body = parse_stmt(ts);
        return std::make_unique<AstStmtIterationFor>(std::move(init), std::move(cond), std::move(iter), std::move(body));
    } else if (ts.match(TokenKind::KwWhile)) {
        ts.expect(TokenKind::LParen, "expect `(` after `while`.");
        auto cond = parse_expr(ts);
        ts.expect(TokenKind::RParen, "expect `)` after `while`.");
        auto body = parse_stmt(ts);
        return std::make_unique<AstStmtIterationWhile>(std::move(cond), std::move(body));
    } else if (ts.match(TokenKind::KwDo)) {
        auto body = parse_stmt(ts);
        ts.expect(TokenKind::KwWhile, "expect keyword `while`.");
        ts.expect(TokenKind::LParen, "expect `(` after `while`.");
        auto cond = parse_expr(ts);
        ts.expect(TokenKind::RParen, "expect `)` after `while`.");
        ts.expect(TokenKind::Semicolon, "expect `;` after `while`.");
        return std::make_unique<AstStmtIterationDoWhile>(std::move(cond), std::move(body));
    } else if (ts.match(TokenKind::KwGoto)) {
        auto lable = ts.peek().string;
        ts.expect(TokenKind::Ident, "expect goto lable.");
        ts.expect(TokenKind::Semicolon, "expect `;` after `goto`.");
        return std::make_unique<AstStmtJumpGoto>(lable);
    } else if (ts.match(TokenKind::KwBreak)) {
        ts.expect(TokenKind::Semicolon, "expect `;` after `break`.");
        return std::make_unique<AstStmtJumpBreak>();
    } else if (ts.match(TokenKind::KwContinue)) {
        ts.expect(TokenKind::Semicolon, "expect `;` after `continue`.");
        return std::make_unique<AstStmtJumpContinue>();
    } else if (ts.match(TokenKind::KwReturn)) {
        auto expr = ts.detect(TokenKind::Semicolon) ? nullptr : parse_expr(ts);
        ts.expect(TokenKind::Semicolon, "expect `;` after `return`.");
        return std::make_unique<AstStmtJumpReturn>(std::move(expr));
    } else if (ts.match(TokenKind::KwDefault)) {
        ts.expect(TokenKind::Colon, "expect `:` after `default`.");
        return std::make_unique<AstStmtLableDefault>();
    } else if (ts.match(TokenKind::KwCase)) {
        auto expr = parse_expr(ts);
        ts.expect(TokenKind::Colon, "expect `:` after `case`.");
        return std::make_unique<AstStmtLableCase>(std::move(expr));
    } else {
        auto loc   = ts.location();
        auto token = ts.next();

        if (token.kind == TokenKind::Ident && ts.match(TokenKind::Colon)) {
            return std::make_unique<AstStmtLable>(token.string);
        } else {
            ts.reset(loc);
            auto expr = parse_expr(ts);
            ts.expect(TokenKind::Semicolon, "expect `;` in expression statement.");
            return std::make_unique<AstStmtExpr>(std::move(expr));
        }
    }
}
static auto parse_conditional_expr(TkStream &ts) -> AstExprPointer {
    AstExprPointer result = parse_logical_or_expr(ts);
    while (ts.match(TokenKind::Ques)) {
        auto then = parse_expr(ts);
        ts.expect(TokenKind::Colon, "expect `:` in conditional expression.");
        auto elze = parse_logical_or_expr(ts);
        result    = std::make_unique<AstExprTernary>(std::move(result), std::move(then), std::move(elze));
    }
    return result;
}
static auto parse_primary_expr(TkStream &ts) -> AstExprPointer {
    std::string string;

    if (ts.match(TokenKind::LParen)) {
        auto result = parse_expr(ts);
        ts.expect(TokenKind::RParen, "expect `)`.");
        return result;
    } else if (ts.match(TokenKind::Ident, string)) {
        return std::make_unique<AstExprIdentifier>(string);
    } else if (ts.match(TokenKind::Str, string)) {
        return std::make_unique<AstExprString>(string);
    } else if (ts.match(TokenKind::Char, string)) {
        return std::make_unique<AstExprCharacter>(string);
    } else if (ts.match(TokenKind::Const, string)) {
        return std::make_unique<AstExprConstant>(string);
    }

    panic("invalid expression.", ts.peek().loc);
}
static auto parse_postfix_expr(TkStream &ts) -> AstExprPointer {
    auto result = parse_primary_expr(ts);

    if (ts.match(TokenKind::LParen)) {
        std::vector<AstExprPointer> args;

        while (!ts.match(TokenKind::RParen)) {
            args.emplace_back(parse_assign_expr(ts));
            if (!ts.detect(TokenKind::RParen)) {
                ts.expect(TokenKind::Comma, "expect `,` in function call expression");
            }
        }

        result = std::make_unique<AstExprFuncCall>(std::move(result), std::move(args));
    } else if (ts.match(TokenKind::LBracket)) {
        result = std::make_unique<AstExprBinary>(OptrKind::Bracket, std::move(result), parse_expr(ts));
        ts.expect(TokenKind::RBracket, "expect `]` in array member select expression.");
    } else if (ts.match(TokenKind::Increase)) {
        result = std::make_unique<AstExprUnary>(OptrKind::PostInc, std::move(result));
    } else if (ts.match(TokenKind::Decrease)) {
        result = std::make_unique<AstExprUnary>(OptrKind::PostDec, std::move(result));
    }

    /// TODO: .  member
    /// TODO: -> member

    return result;
}
static auto parse_unary_expr(TkStream &ts) -> AstExprPointer {
    /// TODO: parse sizeof operator

    if (auto optr = token_to_unary_optr_t::to_optr(ts.peek().kind); optr != OptrKind::None) {
        ts.next();
        return std::make_unique<AstExprUnary>(optr, parse_unary_expr(ts));
    } else if (auto optr = token_to_inc_dec_optr_t::to_optr(ts.peek().kind); optr != OptrKind::None) {
        ts.next();
        return std::make_unique<AstExprUnary>(optr, parse_type_cast_expr(ts));
    }

    return parse_postfix_expr(ts);
}
static auto parse_type_cast_expr(TkStream &ts) -> AstExprPointer {
    /// TODO: parse_type_cast_expr
    return parse_unary_expr(ts);
}
extern auto parse(TkStream &&_ts) -> AstProgram {
    TkStream ts = _ts;
    std::vector<AstDeclPointer> decls;
    while (ts) decls.emplace_back(parse_decl(ts));
    return AstProgram(std::move(decls));
}

}  // namespace mcc