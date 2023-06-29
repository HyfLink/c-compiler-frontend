#include "asthighlighter.hpp"

#include <iostream>

#include "asttypes.hpp"

#define MCC_COLOR_RED "\x1b[31m"
#define MCC_COLOR_GREEN "\x1b[32m"
#define MCC_COLOR_YELLOW "\x1b[33m"
#define MCC_COLOR_BLUE "\x1b[34m"
#define MCC_COLOR_MAGENTA "\x1b[35m"
#define MCC_COLOR_CYAN "\x1b[36m"
#define MCC_COLOR_RESET "\x1b[0m"

namespace mcc {

inline auto AstHighlighter::or_accept(IAst *ast) -> void {
    if (ast) ast->accept(*this);
}

inline auto AstHighlighter::os() -> std::ostream & {
    for (size_t i = 0; i < m_indent; ++i) {
        m_os.put('\t');
    }
    return m_os;
}

template <typename F>
inline auto AstHighlighter::indent(F f) -> void {
    ++m_indent;
    f();
    --m_indent;
}

auto AstHighlighter::visitAstProgram(AstProgram &ast) -> void {
    for (auto &decl : ast) {
        decl->accept(*this);
        std::cout << '\n';
    }
}
auto AstHighlighter::visitAstDeclVar(AstDeclVar &ast) -> void {
    os() << MCC_COLOR_GREEN + ast.type().format(MCC_COLOR_CYAN + ast.name() + MCC_COLOR_GREEN) + MCC_COLOR_RESET;

    if (ast.initial()) {
        m_os << " = ";
        ast.initial()->accept(*this);
    }

    m_os << ";\n";
}
auto AstHighlighter::visitAstDeclFunc(AstDeclFunc &ast) -> void {
    os() << MCC_COLOR_GREEN + ast.type().format(MCC_COLOR_CYAN + ast.name() + MCC_COLOR_GREEN) + MCC_COLOR_RESET;
    if (ast.body()) {
        m_os << '\n';
        indent([&] { ast.body()->accept(*this); });
    } else {
        m_os << ";\n";
    }
}
auto AstHighlighter::visitAstDeclMember(AstDeclMember &) -> void {
    /// TODO: visitAstDeclMember
}
auto AstHighlighter::visitAstStmtJumpGoto(AstStmtJumpGoto &ast) -> void {
    os() << MCC_COLOR_MAGENTA "goto " MCC_COLOR_RESET << ast.lable() << ";\n";
}
auto AstHighlighter::visitAstStmtJumpBreak(AstStmtJumpBreak &) -> void {
    os() << MCC_COLOR_MAGENTA "break" MCC_COLOR_RESET ";\n";
}
auto AstHighlighter::visitAstStmtJumpReturn(AstStmtJumpReturn &ast) -> void {
    os() << MCC_COLOR_MAGENTA "return " MCC_COLOR_RESET;
    or_accept(ast.expr().get());
    m_os << ";\n";
}
auto AstHighlighter::visitAstStmtJumpContinue(AstStmtJumpContinue &) -> void {
    os() << MCC_COLOR_MAGENTA "continue" MCC_COLOR_RESET ";\n";
}
auto AstHighlighter::visitAstStmtLable(AstStmtLable &ast) -> void {
    os() << ast.lable() << ":\n";
}
auto AstHighlighter::visitAstStmtLableCase(AstStmtLableCase &ast) -> void {
    os() << MCC_COLOR_MAGENTA "case " MCC_COLOR_RESET;
    ast.expr()->accept(*this);
    m_os << ":\n";
}
auto AstHighlighter::visitAstStmtLableDefault(AstStmtLableDefault &) -> void {
    os() << MCC_COLOR_MAGENTA "default" MCC_COLOR_RESET ":\n";
}
auto AstHighlighter::visitAstStmtSelectionIfElse(AstStmtSelectionIfElse &ast) -> void {
    os() << MCC_COLOR_MAGENTA "if" MCC_COLOR_RESET " (";
    ast.cond()->accept(*this);
    m_os << ")\n";
    indent([&] { ast.then()->accept(*this); });

    if (ast.elze()) {
        os() << MCC_COLOR_MAGENTA "else\n" MCC_COLOR_RESET;
        indent([&] { ast.elze()->accept(*this); });
    }
}
auto AstHighlighter::visitAstStmtSelectionSwitch(AstStmtSelectionSwitch &ast) -> void {
    os() << MCC_COLOR_MAGENTA "switch" MCC_COLOR_RESET " (";
    ast.cond()->accept(*this);
    m_os << ")\n";
    indent([&] { ast.body()->accept(*this); });
}
auto AstHighlighter::visitAstStmtIterationFor(AstStmtIterationFor &ast) -> void {
    os() << MCC_COLOR_MAGENTA "for" MCC_COLOR_RESET "(";
    if (ast.init()) ast.init()->accept(*this);
    m_os << "; ";
    if (ast.cond()) ast.cond()->accept(*this);
    m_os << "; ";
    if (ast.iter()) ast.iter()->accept(*this);
    m_os << ")\n";
    indent([&] { ast.body()->accept(*this); });
}
auto AstHighlighter::visitAstStmtIterationWhile(AstStmtIterationWhile &ast) -> void {
    os() << MCC_COLOR_MAGENTA "while" MCC_COLOR_RESET " (";
    ast.cond()->accept(*this);
    m_os << ")\n";
    indent([&] { ast.body()->accept(*this); });
}
auto AstHighlighter::visitAstStmtIterationDoWhile(AstStmtIterationDoWhile &ast) -> void {
    os() << MCC_COLOR_MAGENTA "do" MCC_COLOR_RESET " \n";
    indent([&] { ast.body()->accept(*this); });
    os() << MCC_COLOR_MAGENTA "while" MCC_COLOR_RESET " (";
    ast.cond()->accept(*this);
    m_os << ");\n";
}
auto AstHighlighter::visitAstStmtEmpty(AstStmtEmpty &) -> void {}
auto AstHighlighter::visitAstStmtExpr(AstStmtExpr &ast) -> void {
    os();
    ast.expr()->accept(*this);
    m_os << ";\n";
}
auto AstHighlighter::visitAstStmtCompound(AstStmtCompound &ast) -> void {
    --m_indent;
    os() << "{\n";
    indent([&] {
        for (auto &stmt_decl : ast) {
            stmt_decl->accept(*this);
        }
    });
    os() << "}\n";
    ++m_indent;
}
auto AstHighlighter::visitAstExprUnary(AstExprUnary &ast) -> void {
    if (ast.optr() == OptrKind::PostDec || ast.optr() == OptrKind::PostInc) {
        // m_os << '(';
        ast.opnd()->accept(*this);
        m_os << to_symbol(ast.optr());
        // m_os << ')';
    } else {
        // m_os << '(';
        m_os << to_symbol(ast.optr());
        ast.opnd()->accept(*this);
        // m_os << ')';
    }
}
auto AstHighlighter::visitAstExprBinary(AstExprBinary &ast) -> void {
    if (ast.optr() == OptrKind::Bracket) {
        ast.lhs()->accept(*this);
        m_os << '[';
        ast.rhs()->accept(*this);
        m_os << ']';
    } else {
        // m_os << '(';
        ast.lhs()->accept(*this);
        m_os << to_symbol(ast.optr());
        ast.rhs()->accept(*this);
        // m_os << ')';
    }
}
auto AstHighlighter::visitAstExprTernary(AstExprTernary &ast) -> void {
    // m_os << '(';
    ast.cond()->accept(*this);
    m_os << '?';
    ast.then()->accept(*this);
    m_os << ':';
    ast.elze()->accept(*this);
    // m_os << ')';
}
auto AstHighlighter::visitAstExprFuncCall(AstExprFuncCall &ast) -> void {
    ast.func()->accept(*this);

    m_os << '(';
    size_t count = 0;
    for (auto &arg : ast.args()) {
        if (count++) m_os << ',';
        arg->accept(*this);
    }
    m_os << ')';
}
auto AstHighlighter::visitAstExprString(AstExprString &ast) -> void {
    m_os << MCC_COLOR_YELLOW "\"" << ast.value() << "\"" MCC_COLOR_RESET;
}
auto AstHighlighter::visitAstExprConstant(AstExprConstant &ast) -> void {
    m_os << MCC_COLOR_YELLOW << ast.value() << MCC_COLOR_RESET;
}
auto AstHighlighter::visitAstExprCharacter(AstExprCharacter &ast) -> void {
    m_os << MCC_COLOR_YELLOW "\'" << ast.value() << "\'" MCC_COLOR_RESET;
}
auto AstHighlighter::visitAstExprIdentifier(AstExprIdentifier &ast) -> void {
    m_os << MCC_COLOR_CYAN << ast.name() << MCC_COLOR_RESET;
}

}  // namespace mcc