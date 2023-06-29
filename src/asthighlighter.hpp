#pragma once

#include <iosfwd>

#include "astvisitor.hpp"

namespace mcc {

class AstHighlighter : public IAstVisitor {
public:
    AstHighlighter(std::ostream &os) : m_os(os), m_indent(0) {}
    virtual ~AstHighlighter() = default;

    virtual auto visitAstProgram(AstProgram &) -> void override;
    virtual auto visitAstDeclVar(AstDeclVar &) -> void override;
    virtual auto visitAstDeclFunc(AstDeclFunc &) -> void override;
    virtual auto visitAstDeclMember(AstDeclMember &) -> void override;

    virtual auto visitAstStmtJumpGoto(AstStmtJumpGoto &) -> void override;
    virtual auto visitAstStmtJumpBreak(AstStmtJumpBreak &) -> void override;
    virtual auto visitAstStmtJumpReturn(AstStmtJumpReturn &) -> void override;
    virtual auto visitAstStmtJumpContinue(AstStmtJumpContinue &) -> void override;
    virtual auto visitAstStmtLable(AstStmtLable &) -> void override;
    virtual auto visitAstStmtLableCase(AstStmtLableCase &) -> void override;
    virtual auto visitAstStmtLableDefault(AstStmtLableDefault &) -> void override;
    virtual auto visitAstStmtSelectionIfElse(AstStmtSelectionIfElse &) -> void override;
    virtual auto visitAstStmtSelectionSwitch(AstStmtSelectionSwitch &) -> void override;
    virtual auto visitAstStmtIterationFor(AstStmtIterationFor &) -> void override;
    virtual auto visitAstStmtIterationWhile(AstStmtIterationWhile &) -> void override;
    virtual auto visitAstStmtIterationDoWhile(AstStmtIterationDoWhile &) -> void override;
    virtual auto visitAstStmtEmpty(AstStmtEmpty &) -> void override;
    virtual auto visitAstStmtExpr(AstStmtExpr &) -> void override;
    virtual auto visitAstStmtCompound(AstStmtCompound &) -> void override;

    virtual auto visitAstExprUnary(AstExprUnary &) -> void override;
    virtual auto visitAstExprBinary(AstExprBinary &) -> void override;
    virtual auto visitAstExprTernary(AstExprTernary &) -> void override;
    virtual auto visitAstExprFuncCall(AstExprFuncCall &) -> void override;
    virtual auto visitAstExprString(AstExprString &) -> void override;
    virtual auto visitAstExprConstant(AstExprConstant &) -> void override;
    virtual auto visitAstExprCharacter(AstExprCharacter &) -> void override;
    virtual auto visitAstExprIdentifier(AstExprIdentifier &) -> void override;

private:
    inline auto or_accept(IAst *ast) -> void;

    inline auto os() -> std::ostream &;

    template <typename F>
    inline auto indent(F f) -> void;

private:
    std::ostream &m_os;
    size_t m_indent;
};

}  // namespace mcc