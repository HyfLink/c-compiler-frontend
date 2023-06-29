#include "astjsonwriter.hpp"

#include <iostream>

namespace mcc {

inline auto AstJsonWriter::or_accept(IAst *ast) -> void {
    if (ast) {
        ast->accept(*this);
    } else {
        m_os << "null";
    }
}
auto AstJsonWriter::visitAstProgram(AstProgram &ast) -> void {
    m_os << '[';

    size_t count = 0;
    for (auto &decl : ast) {
        if (count++) m_os << ',';
        decl->accept(*this);
    }

    m_os << ']';
}
auto AstJsonWriter::visitAstDeclVar(AstDeclVar &ast) -> void {
    m_os << '{';
    m_os << "\"kind\":\"variable\"";
    m_os << ",\"storage class\":\"" << ast.storage() << '\"';
    m_os << ",\"name\":\"" << ast.name() << '\"';
    m_os << ",\"type\":\"" << ast.type() << '\"';
    m_os << ",\"initial\":";
    or_accept(ast.initial().get());
    m_os << '}';
}
auto AstJsonWriter::visitAstDeclFunc(AstDeclFunc &ast) -> void {
    m_os << '{';
    m_os << "\"kind\":\"function\"";
    m_os << ",\"storage class\":\"" << ast.storage() << '\"';
    m_os << ",\"name\":\"" << ast.name() << '\"';
    m_os << ",\"type\":\"" << ast.type() << '\"';
    m_os << ",\"body\":";
    or_accept(ast.body().get());
    m_os << '}';
}
auto AstJsonWriter::visitAstDeclMember(AstDeclMember &) -> void {
    /// TODO: visitAstDeclMember
}
auto AstJsonWriter::visitAstStmtJumpGoto(AstStmtJumpGoto &ast) -> void {
    m_os << "{\"kind\":\"goto\",\"lable\":\"" << ast.lable() << "\"}";
}
auto AstJsonWriter::visitAstStmtJumpBreak(AstStmtJumpBreak &) -> void {
    m_os << "{\"kind\":\"break\"}";
}
auto AstJsonWriter::visitAstStmtJumpReturn(AstStmtJumpReturn &ast) -> void {
    m_os << "{\"kind\":\"return\",\"retval\":";
    or_accept(ast.expr().get());
    m_os << '}';
}
auto AstJsonWriter::visitAstStmtJumpContinue(AstStmtJumpContinue &) -> void {
    m_os << "{\"kind\":\"continue\"}";
}
auto AstJsonWriter::visitAstStmtLable(AstStmtLable &ast) -> void {
    m_os << "{\"kind\" : \"goto lable\", \"lable\":\"" << ast.lable() << "\"}";
}
auto AstJsonWriter::visitAstStmtLableCase(AstStmtLableCase &ast) -> void {
    m_os << "{\"kind\" : \"case lable\", \"lable\":";
    ast.expr()->accept(*this);
    m_os << '}';
}
auto AstJsonWriter::visitAstStmtLableDefault(AstStmtLableDefault &) -> void {
    m_os << "{\"kind\":\"default lable\"}";
}
auto AstJsonWriter::visitAstStmtSelectionIfElse(AstStmtSelectionIfElse &ast) -> void {
    m_os << "{\"kind\":\"if stmt\",\"cond\":";
    ast.cond()->accept(*this);
    m_os << ",\"then\":";
    ast.then()->accept(*this);
    m_os << ",\"else\":";
    or_accept(ast.elze().get());
    m_os << '}';
}
auto AstJsonWriter::visitAstStmtSelectionSwitch(AstStmtSelectionSwitch &ast) -> void {
    m_os << "{\"kind\":\"switch stmt\",\"cond\":";
    ast.cond()->accept(*this);
    m_os << ",\"body\":";
    ast.body()->accept(*this);
    m_os << '}';
}
auto AstJsonWriter::visitAstStmtIterationFor(AstStmtIterationFor &ast) -> void {
    m_os << "{\"kind\":\"for stmt\",\"init\":";
    or_accept(ast.init().get());
    m_os << ",\"cond\":";
    or_accept(ast.cond().get());
    m_os << ",\"iter\":";
    or_accept(ast.iter().get());
    m_os << ",\"body\":";
    ast.body()->accept(*this);
    m_os << '}';
}
auto AstJsonWriter::visitAstStmtIterationWhile(AstStmtIterationWhile &ast) -> void {
    m_os << "{\"kind\":\"while\",\"cond\":";
    ast.cond()->accept(*this);
    m_os << ",\"body\":";
    ast.body()->accept(*this);
    m_os << '}';
}
auto AstJsonWriter::visitAstStmtIterationDoWhile(AstStmtIterationDoWhile &ast) -> void {
    m_os << "{\"kind\":\"do-while stmt\",\"cond\":";
    ast.cond()->accept(*this);
    m_os << ",\"body\":";
    ast.body()->accept(*this);
    m_os << '}';
}
auto AstJsonWriter::visitAstStmtEmpty(AstStmtEmpty &) -> void {
    m_os << "{\"kind\":\"empty\"}";
}
auto AstJsonWriter::visitAstStmtExpr(AstStmtExpr &ast) -> void {
    m_os << "{\"kind\":\"expr stmt\",\"expr\":";
    ast.expr()->accept(*this);
    m_os << '}';
}
auto AstJsonWriter::visitAstStmtCompound(AstStmtCompound &ast) -> void {
    m_os << '[';

    size_t count = 0;
    for (auto &stmt_decl : ast) {
        if (count++) m_os << ',';
        stmt_decl->accept(*this);
    }
    m_os << ']';
}
auto AstJsonWriter::visitAstExprUnary(AstExprUnary &ast) -> void {
    m_os << "{\"unary optr expr\":\"";
    m_os << to_string(ast.optr());
    m_os << "\",\"operand\":";
    ast.opnd()->accept(*this);
    m_os << '}';
}
auto AstJsonWriter::visitAstExprBinary(AstExprBinary &ast) -> void {
    m_os << "{\"binary optr expr\":\"";
    m_os << to_string(ast.optr());
    m_os << "\",\"lhs-operand\":";
    ast.lhs()->accept(*this);
    m_os << ",\"rhs-operand\":";
    ast.rhs()->accept(*this);
    m_os << '}';
}
auto AstJsonWriter::visitAstExprTernary(AstExprTernary &ast) -> void {
    m_os << "{\"ternary optr expr\":\"?:\",\"cond-operand\":";
    ast.cond()->accept(*this);
    m_os << ",\"then-operand\":";
    ast.then()->accept(*this);
    m_os << ",\"else-operand\":";
    ast.elze()->accept(*this);
    m_os << '}';
}
auto AstJsonWriter::visitAstExprFuncCall(AstExprFuncCall &ast) -> void {
    m_os << "{\"function call expr\":\"?:\",\"function\":";
    ast.func()->accept(*this);
    m_os << ",\"args\":[ ";

    size_t count = 0;
    for (auto &arg : ast.args()) {
        if (count++) m_os << ',';
        arg->accept(*this);
    }

    m_os << "]}";
}
auto AstJsonWriter::visitAstExprString(AstExprString &ast) -> void {
    m_os << "{\"kind\":\"string literal\",\"value\":";
    m_os << '"' << ast.value() << "\"}";
}
auto AstJsonWriter::visitAstExprConstant(AstExprConstant &ast) -> void {
    m_os << "{\"kind\":\"constant\",\"value\":";
    m_os << '"' << ast.value() << "\"}";
}
auto AstJsonWriter::visitAstExprCharacter(AstExprCharacter &ast) -> void {
    m_os << "{\"kind\":\"character\",\"value\":";
    m_os << '"' << ast.value() << "\"}";
}
auto AstJsonWriter::visitAstExprIdentifier(AstExprIdentifier &ast) -> void {
    m_os << "{\"kind\":\"identifier\",\"name\":";
    m_os << '"' << ast.name() << "\"}";
}

}  // namespace mcc