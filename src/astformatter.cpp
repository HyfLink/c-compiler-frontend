#include "astformatter.hpp"

#include <iostream>

#include "asttypes.hpp"

namespace mcc {

inline auto AstFormatter::or_accept(IAst *ast) -> void {
    if (ast) ast->accept(*this);
}

inline auto AstFormatter::os() -> std::ostream & {
    for (size_t i = 0; i < m_indent; ++i) {
        m_os.put('\t');
    }
    return m_os;
}

template <typename F>
inline auto AstFormatter::indent(F f) -> void {
    ++m_indent;
    f();
    --m_indent;
}

auto AstFormatter::visitAstProgram(AstProgram &ast) -> void {
    for (auto &decl : ast) {
        decl->accept(*this);
        std::cout << '\n';
    }
}
auto AstFormatter::visitAstDeclVar(AstDeclVar &ast) -> void {
    os() << ast.type().format(ast.name());

    if (ast.initial()) {
        m_os << " = ";
        ast.initial()->accept(*this);
    }

    m_os << ";\n";
}
auto AstFormatter::visitAstDeclFunc(AstDeclFunc &ast) -> void {
    os() << ast.type().format(ast.name());
    if (ast.body()) {
        m_os << '\n';
        indent([&] { ast.body()->accept(*this); });
    } else {
        m_os << ";\n";
    }
}
auto AstFormatter::visitAstDeclMember(AstDeclMember &) -> void {
    /// TODO: visitAstDeclMember
}
auto AstFormatter::visitAstStmtJumpGoto(AstStmtJumpGoto &ast) -> void {
    os() << "goto " << ast.lable() << ";\n";
}
auto AstFormatter::visitAstStmtJumpBreak(AstStmtJumpBreak &) -> void {
    os() << "break;\n";
}
auto AstFormatter::visitAstStmtJumpReturn(AstStmtJumpReturn &ast) -> void {
    os() << "return ";
    or_accept(ast.expr().get());
    m_os << ";\n";
}
auto AstFormatter::visitAstStmtJumpContinue(AstStmtJumpContinue &) -> void {
    os() << "continue;\n";
}
auto AstFormatter::visitAstStmtLable(AstStmtLable &ast) -> void {
    os() << ast.lable() << ":\n";
}
auto AstFormatter::visitAstStmtLableCase(AstStmtLableCase &ast) -> void {
    os() << "case ";
    ast.expr()->accept(*this);
    m_os << ":\n";
}
auto AstFormatter::visitAstStmtLableDefault(AstStmtLableDefault &) -> void {
    os() << "default:\n";
}
auto AstFormatter::visitAstStmtSelectionIfElse(AstStmtSelectionIfElse &ast) -> void {
    os() << "if (";
    ast.cond()->accept(*this);
    m_os << ")\n";
    indent([&] { ast.then()->accept(*this); });

    if (ast.elze()) {
        os() << "else\n";
        indent([&] { ast.elze()->accept(*this); });
    }
}
auto AstFormatter::visitAstStmtSelectionSwitch(AstStmtSelectionSwitch &ast) -> void {
    os() << "switch (";
    ast.cond()->accept(*this);
    m_os << ")\n";
    indent([&] { ast.body()->accept(*this); });
}
auto AstFormatter::visitAstStmtIterationFor(AstStmtIterationFor &ast) -> void {
    os() << "for (";
    if (ast.init()) ast.init()->accept(*this);
    m_os << "; ";
    if (ast.cond()) ast.cond()->accept(*this);
    m_os << "; ";
    if (ast.iter()) ast.iter()->accept(*this);
    m_os << ")\n";
    indent([&] { ast.body()->accept(*this); });
}
auto AstFormatter::visitAstStmtIterationWhile(AstStmtIterationWhile &ast) -> void {
    os() << "while (";
    ast.cond()->accept(*this);
    m_os << ")\n";
    indent([&] { ast.body()->accept(*this); });
}
auto AstFormatter::visitAstStmtIterationDoWhile(AstStmtIterationDoWhile &ast) -> void {
    os() << "do \n";
    indent([&] { ast.body()->accept(*this); });
    os() << "while (";
    ast.cond()->accept(*this);
    m_os << ");\n";
}
auto AstFormatter::visitAstStmtEmpty(AstStmtEmpty &) -> void {}
auto AstFormatter::visitAstStmtExpr(AstStmtExpr &ast) -> void {
    os();
    ast.expr()->accept(*this);
    m_os << ";\n";
}
auto AstFormatter::visitAstStmtCompound(AstStmtCompound &ast) -> void {
    os() << "{\n";
    indent([&] {
        for (auto &stmt_decl : ast) {
            stmt_decl->accept(*this);
        }
    });
    os() << "}\n";
}
auto AstFormatter::visitAstExprUnary(AstExprUnary &ast) -> void {
    if (ast.optr() == OptrKind::PostDec || ast.optr() == OptrKind::PostInc) {
        m_os << '(';
        ast.opnd()->accept(*this);
        m_os << to_symbol(ast.optr());
        m_os << ')';
    } else {
        m_os << '(';
        m_os << to_symbol(ast.optr());
        ast.opnd()->accept(*this);
        m_os << ')';
    }
}
auto AstFormatter::visitAstExprBinary(AstExprBinary &ast) -> void {
    if (ast.optr() == OptrKind::Bracket) {
        ast.lhs()->accept(*this);
        m_os << '[';
        ast.rhs()->accept(*this);
        m_os << ']';
    } else {
        m_os << '(';
        ast.lhs()->accept(*this);
        m_os << to_symbol(ast.optr());
        ast.rhs()->accept(*this);
        m_os << ')';
    }
}
auto AstFormatter::visitAstExprTernary(AstExprTernary &ast) -> void {
    m_os << '(';
    ast.cond()->accept(*this);
    m_os << '?';
    ast.then()->accept(*this);
    m_os << ':';
    ast.elze()->accept(*this);
    m_os << ')';
}
auto AstFormatter::visitAstExprFuncCall(AstExprFuncCall &ast) -> void {
    ast.func()->accept(*this);

    m_os << '(';

    size_t count = 0;
    for (auto &arg : ast.args()) {
        if (count++) m_os << ',';
        arg->accept(*this);
    }

    m_os << ')';
}
auto AstFormatter::visitAstExprString(AstExprString &ast) -> void {
    m_os << '\"' << ast.value() << '\"';
}
auto AstFormatter::visitAstExprConstant(AstExprConstant &ast) -> void {
    m_os << ast.value();
}
auto AstFormatter::visitAstExprCharacter(AstExprCharacter &ast) -> void {
    m_os << '\'' << ast.value() << '\'';
}
auto AstFormatter::visitAstExprIdentifier(AstExprIdentifier &ast) -> void {
    m_os << ast.name();
}

}  // namespace mcc