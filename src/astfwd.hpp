#pragma once
namespace mcc {

class IAst;
class IAstVisitor;
class AstProgram;      // -> IAst
class AstInitializer;  // -> IAst

/// declaration
/// ----------------------------------------------------------------------------
class IAstDecl;       // -> IAst
class AstDeclVar;     // -> IAstDecl    variable declaration
class AstDeclFunc;    // -> IAstDecl    function declaration
class AstDeclMember;  // -> IAstDecl    member declaration

/// statement
/// ----------------------------------------------------------------------------
class IAstStmt;                 // -> IAst        statement base
class AstStmtJumpGoto;          // -> IAstStmt    goto statement
class AstStmtJumpBreak;         // -> IAstStmt    break statement
class AstStmtJumpReturn;        // -> IAstStmt    return statement
class AstStmtJumpContinue;      // -> IAstStmt    continue statement
class AstStmtLable;             // -> IAstStmt    lable
class AstStmtLableCase;         // -> IAstStmt    case lable
class AstStmtLableDefault;      // -> IAstStmt    default lable
class AstStmtSelectionIfElse;   // -> IAstStmt    if-else selection statement
class AstStmtSelectionSwitch;   // -> IAstStmt    switch selection statement
class AstStmtIterationFor;      // -> IAstStmt    for iteration statement
class AstStmtIterationWhile;    // -> IAstStmt    while iteration statement
class AstStmtIterationDoWhile;  // -> IAstStmt    do-while iteration statement
class AstStmtEmpty;             // -> IAstStmt    empty statement
class AstStmtExpr;              // -> IAstStmt    expression statement
class AstStmtCompound;          // -> IAstStmt    compound statement

/// expression
/// ----------------------------------------------------------------------------
class IAstExpr;           // -> IAst
class AstExprUnary;       // -> IAstExpr    unary operator expression
class AstExprBinary;      // -> IAstExpr    binary operator expression
class AstExprTernary;     // -> IAstExpr    ternary operator expression
class AstExprFuncCall;    // -> IAstExpr    function call expression
class AstExprConstant;    // -> IAstExpr    numeric constant expression
class AstExprCharacter;   // -> IAstExpr    character constant expression
class AstExprString;      // -> IAstExpr    string literal expression
class AstExprIdentifier;  // -> IAstExpr    identifier expression

}  // namespace mcc