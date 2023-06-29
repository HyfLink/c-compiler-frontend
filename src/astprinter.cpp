#include "astprinter.hpp"

#include <iostream>

namespace mcc {

inline auto AstPrinter::or_accept(IAst *ast) -> void {
    if (ast) {
        ast->accept(*this);
    } else {
        os() << "none\n";
    }
}

inline auto AstPrinter::os() -> std::ostream & {
    for (size_t i = 0; i < m_indent; ++i) {
        m_os.put('\t');
    }
    return m_os;
}

template <typename F>
inline auto AstPrinter::indent(F f) -> void {
    ++m_indent;
    f();
    --m_indent;
}

auto AstPrinter::visitAstProgram(AstProgram &ast) -> void {
    for (auto &decl : ast) decl->accept(*this);
}
auto AstPrinter::visitAstDeclVar(AstDeclVar &ast) -> void {
    os() << "variable decl:\n";
    indent([&] {
        os() << "storage class: " << ast.storage() << '\n';
        os() << "name: " << ast.name() << '\n';
        os() << "type: " << ast.type() << '\n';
        os() << "initializer:\n";
        indent([&] { or_accept(ast.initial().get()); });
    });
}
auto AstPrinter::visitAstDeclFunc(AstDeclFunc &ast) -> void {
    os() << "function decl:\n";
    indent([&] {
        os() << "storage class: " << ast.storage() << '\n';
        os() << "name: " << ast.name() << '\n';
        os() << "type: " << ast.type() << '\n';
        os() << "body:\n";
        or_accept(ast.body().get());
    });
}
auto AstPrinter::visitAstDeclMember(AstDeclMember &) -> void {
    /// TODO: visitAstDeclMember
}
auto AstPrinter::visitAstStmtJumpGoto(AstStmtJumpGoto &ast) -> void {
    os() << "goto stmt: " << ast.lable() << '\n';
}
auto AstPrinter::visitAstStmtJumpBreak(AstStmtJumpBreak &) -> void {
    os() << "break stmt:\n";
}
auto AstPrinter::visitAstStmtJumpReturn(AstStmtJumpReturn &ast) -> void {
    os() << "return stmt:\n";
    indent([&] { or_accept(ast.expr().get()); });
}
auto AstPrinter::visitAstStmtJumpContinue(AstStmtJumpContinue &) -> void {
    os() << "continue stmt:\n";
}
auto AstPrinter::visitAstStmtLable(AstStmtLable &ast) -> void {
    os() << "lable: " << ast.lable() << '\n';
}
auto AstPrinter::visitAstStmtLableCase(AstStmtLableCase &ast) -> void {
    os() << "case lable:\n";
    indent([&] { or_accept(ast.expr().get()); });
}
auto AstPrinter::visitAstStmtLableDefault(AstStmtLableDefault &) -> void {
    os() << "default lable:\n";
}
auto AstPrinter::visitAstStmtSelectionIfElse(AstStmtSelectionIfElse &ast) -> void {
    os() << "if stmt:\n";
    indent([&] {
        ast.cond()->accept(*this);
        ast.then()->accept(*this);
        or_accept(ast.elze().get());
    });
}
auto AstPrinter::visitAstStmtSelectionSwitch(AstStmtSelectionSwitch &ast) -> void {
    os() << "switch stmt:\n";
    indent([&] {
        ast.cond()->accept(*this);
        ast.body()->accept(*this);
    });
}
auto AstPrinter::visitAstStmtIterationFor(AstStmtIterationFor &ast) -> void {
    os() << "for stmt:\n";
    indent([&] {
        or_accept(ast.init().get());
        or_accept(ast.cond().get());
        or_accept(ast.iter().get());
        ast.body()->accept(*this);
    });
}
auto AstPrinter::visitAstStmtIterationWhile(AstStmtIterationWhile &ast) -> void {
    os() << "while stmt:\n";
    indent([&] {
        ast.cond()->accept(*this);
        ast.body()->accept(*this);
    });
}
auto AstPrinter::visitAstStmtIterationDoWhile(AstStmtIterationDoWhile &ast) -> void {
    os() << "do-while stmt:\n";
    indent([&] {
        ast.cond()->accept(*this);
        ast.body()->accept(*this);
    });
}
auto AstPrinter::visitAstStmtEmpty(AstStmtEmpty &) -> void {}
auto AstPrinter::visitAstStmtExpr(AstStmtExpr &ast) -> void {
    os() << "expr stmt:\n";
    indent([&] { ast.expr()->accept(*this); });
}
auto AstPrinter::visitAstStmtCompound(AstStmtCompound &ast) -> void {
    os() << "compound stmt:\n";
    indent([&] {
        for (auto &stmt_decl : ast) {
            stmt_decl->accept(*this);
        }
    });
}
auto AstPrinter::visitAstExprUnary(AstExprUnary &ast) -> void {
    os() << to_string(ast.optr()) << " expr:\n";
    indent([&] { ast.opnd()->accept(*this); });
}
auto AstPrinter::visitAstExprBinary(AstExprBinary &ast) -> void {
    os() << to_string(ast.optr()) << " expr:\n";
    indent([&] {
        ast.rhs()->accept(*this);
        ast.lhs()->accept(*this);
    });
}
auto AstPrinter::visitAstExprTernary(AstExprTernary &ast) -> void {
    os() << "conditional expr:\n";
    indent([&] {
        ast.cond()->accept(*this);
        ast.then()->accept(*this);
        ast.elze()->accept(*this);
    });
}
auto AstPrinter::visitAstExprFuncCall(AstExprFuncCall &ast) -> void {
    os() << "function call expr:\n";
    indent([&] {
        os() << "function:\n";
        ast.func()->accept(*this);
        os() << "arguments:\n";
        for (auto &arg : ast.args()) {
            arg->accept(*this);
        }
    });
}
auto AstPrinter::visitAstExprString(AstExprString &ast) -> void {
    os() << "string: \"" << ast.value() << "\"\n";
}
auto AstPrinter::visitAstExprConstant(AstExprConstant &ast) -> void {
    os() << "constant: " << ast.value() << "\n";
}
auto AstPrinter::visitAstExprCharacter(AstExprCharacter &ast) -> void {
    os() << "character: \'" << ast.value() << "\'\n";
}
auto AstPrinter::visitAstExprIdentifier(AstExprIdentifier &ast) -> void {
    os() << "identifier: " << ast.name() << "\n";
}

}  // namespace mcc