#pragma once

#include "astfwd.hpp"

namespace mcc {

class IAstVisitor {
public:
    constexpr IAstVisitor() = default;
    virtual ~IAstVisitor()  = default;

    virtual auto visitAstProgram(AstProgram &) -> void       = 0;
    virtual auto visitAstDeclVar(AstDeclVar &) -> void       = 0;
    virtual auto visitAstDeclFunc(AstDeclFunc &) -> void     = 0;
    virtual auto visitAstDeclMember(AstDeclMember &) -> void = 0;

    virtual auto visitAstStmtJumpGoto(AstStmtJumpGoto &) -> void                 = 0;
    virtual auto visitAstStmtJumpBreak(AstStmtJumpBreak &) -> void               = 0;
    virtual auto visitAstStmtJumpReturn(AstStmtJumpReturn &) -> void             = 0;
    virtual auto visitAstStmtJumpContinue(AstStmtJumpContinue &) -> void         = 0;
    virtual auto visitAstStmtLable(AstStmtLable &) -> void                       = 0;
    virtual auto visitAstStmtLableCase(AstStmtLableCase &) -> void               = 0;
    virtual auto visitAstStmtLableDefault(AstStmtLableDefault &) -> void         = 0;
    virtual auto visitAstStmtSelectionIfElse(AstStmtSelectionIfElse &) -> void   = 0;
    virtual auto visitAstStmtSelectionSwitch(AstStmtSelectionSwitch &) -> void   = 0;
    virtual auto visitAstStmtIterationFor(AstStmtIterationFor &) -> void         = 0;
    virtual auto visitAstStmtIterationWhile(AstStmtIterationWhile &) -> void     = 0;
    virtual auto visitAstStmtIterationDoWhile(AstStmtIterationDoWhile &) -> void = 0;
    virtual auto visitAstStmtEmpty(AstStmtEmpty &) -> void                       = 0;
    virtual auto visitAstStmtExpr(AstStmtExpr &) -> void                         = 0;
    virtual auto visitAstStmtCompound(AstStmtCompound &) -> void                 = 0;

    virtual auto visitAstExprUnary(AstExprUnary &) -> void           = 0;
    virtual auto visitAstExprBinary(AstExprBinary &) -> void         = 0;
    virtual auto visitAstExprTernary(AstExprTernary &) -> void       = 0;
    virtual auto visitAstExprFuncCall(AstExprFuncCall &) -> void     = 0;
    virtual auto visitAstExprString(AstExprString &) -> void         = 0;
    virtual auto visitAstExprConstant(AstExprConstant &) -> void     = 0;
    virtual auto visitAstExprCharacter(AstExprCharacter &) -> void   = 0;
    virtual auto visitAstExprIdentifier(AstExprIdentifier &) -> void = 0;
};

}  // namespace mcc