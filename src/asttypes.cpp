#include "asttypes.hpp"

#include "astvisitor.hpp"

namespace mcc {

/// virtual method : accept
/// ----------------------------------------------------------------------------
// auto AstDeclMember::accept(AstVisitor &) -> void{};
auto AstProgram::accept(IAstVisitor &visitor) -> void { visitor.visitAstProgram(*this); }
auto AstDeclVar::accept(IAstVisitor &visitor) -> void { visitor.visitAstDeclVar(*this); }
auto AstDeclFunc::accept(IAstVisitor &visitor) -> void { visitor.visitAstDeclFunc(*this); }
auto AstStmtJumpGoto::accept(IAstVisitor &visitor) -> void { visitor.visitAstStmtJumpGoto(*this); }
auto AstStmtJumpBreak::accept(IAstVisitor &visitor) -> void { visitor.visitAstStmtJumpBreak(*this); }
auto AstStmtJumpReturn::accept(IAstVisitor &visitor) -> void { visitor.visitAstStmtJumpReturn(*this); }
auto AstStmtJumpContinue::accept(IAstVisitor &visitor) -> void { visitor.visitAstStmtJumpContinue(*this); }
auto AstStmtLable::accept(IAstVisitor &visitor) -> void { visitor.visitAstStmtLable(*this); }
auto AstStmtLableCase::accept(IAstVisitor &visitor) -> void { visitor.visitAstStmtLableCase(*this); }
auto AstStmtLableDefault::accept(IAstVisitor &visitor) -> void { visitor.visitAstStmtLableDefault(*this); }
auto AstStmtSelectionIfElse::accept(IAstVisitor &visitor) -> void { visitor.visitAstStmtSelectionIfElse(*this); }
auto AstStmtSelectionSwitch::accept(IAstVisitor &visitor) -> void { visitor.visitAstStmtSelectionSwitch(*this); }
auto AstStmtIterationFor::accept(IAstVisitor &visitor) -> void { visitor.visitAstStmtIterationFor(*this); }
auto AstStmtIterationWhile::accept(IAstVisitor &visitor) -> void { visitor.visitAstStmtIterationWhile(*this); }
auto AstStmtIterationDoWhile::accept(IAstVisitor &visitor) -> void { visitor.visitAstStmtIterationDoWhile(*this); }
auto AstStmtEmpty::accept(IAstVisitor &visitor) -> void { visitor.visitAstStmtEmpty(*this); }
auto AstStmtExpr::accept(IAstVisitor &visitor) -> void { visitor.visitAstStmtExpr(*this); }
auto AstStmtCompound::accept(IAstVisitor &visitor) -> void { visitor.visitAstStmtCompound(*this); }
auto AstExprUnary::accept(IAstVisitor &visitor) -> void { visitor.visitAstExprUnary(*this); }
auto AstExprBinary::accept(IAstVisitor &visitor) -> void { visitor.visitAstExprBinary(*this); }
auto AstExprTernary::accept(IAstVisitor &visitor) -> void { visitor.visitAstExprTernary(*this); }
auto AstExprFuncCall::accept(IAstVisitor &visitor) -> void { visitor.visitAstExprFuncCall(*this); }
auto AstExprString::accept(IAstVisitor &visitor) -> void { visitor.visitAstExprString(*this); }
auto AstExprConstant::accept(IAstVisitor &visitor) -> void { visitor.visitAstExprConstant(*this); }
auto AstExprCharacter::accept(IAstVisitor &visitor) -> void { visitor.visitAstExprCharacter(*this); }
auto AstExprIdentifier::accept(IAstVisitor &visitor) -> void { visitor.visitAstExprIdentifier(*this); }

}  // namespace mcc