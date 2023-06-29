#pragma once

#include <string>

#include "astfwd.hpp"
#include "operator.hpp"
#include "scope.hpp"
#include "type.hpp"

namespace mcc {

using AstPointer     = std::unique_ptr<IAst>;
using AstDeclPointer = std::unique_ptr<IAstDecl>;
using AstStmtPointer = std::unique_ptr<IAstStmt>;
using AstExprPointer = std::unique_ptr<IAstExpr>;

/// IAst
/// ----------------------------------------------------------------------------
class IAst {
public:
    constexpr IAst() = default;
    virtual ~IAst()  = default;

    virtual auto accept(IAstVisitor &) -> void = 0;
};

/// AstProgram
/// ----------------------------------------------------------------------------
class AstProgram final : public IAst {
public:
    ~AstProgram() override = default;
    auto accept(IAstVisitor &) -> void override;

    AstProgram(std::vector<AstDeclPointer> &&decls)
        : m_decls(std::move(decls)),
          m_scope(ScopeKind::File, nullptr) {}
    AstProgram(AstProgram &&)      = default;
    AstProgram(const AstProgram &) = delete;
    auto operator=(AstProgram &&) -> AstProgram & = default;
    auto operator=(const AstProgram &) -> AstProgram & = delete;

    inline auto &decls() { return m_decls; }
    inline auto &decls() const { return m_decls; }
    inline auto &scope() { return m_scope; }
    inline auto &scope() const { return m_scope; }
    inline auto begin() -> decltype(auto) { return m_decls.begin(); }
    inline auto end() -> decltype(auto) { return m_decls.end(); }
    inline auto begin() const -> decltype(auto) { return m_decls.begin(); }
    inline auto end() const -> decltype(auto) { return m_decls.end(); }

private:
    std::vector<AstDeclPointer> m_decls;
    Scope m_scope;
};

/// IAstDecl
/// ----------------------------------------------------------------------------
class IAstDecl : public IAst {
public:
    static constexpr std::string_view kAnonymous = "";

    ~IAstDecl() override = default;

    auto accept(IAstVisitor &) -> void override = 0;

    IAstDecl(QualType type, std::string_view name = kAnonymous)
        : m_type(type), m_name(name) {}
    IAstDecl(QualType type, std::string name) : m_type(type), m_name(name) {}

    inline auto &type() { return m_type; }
    inline auto &name() { return m_name; }
    inline auto &type() const { return m_type; }
    inline auto &name() const { return m_name; }

private:
    QualType m_type;
    std::string m_name;
};

/// AstDeclVar
/// ----------------------------------------------------------------------------
class AstDeclVar final : public IAstDecl {
public:
    ~AstDeclVar() override = default;
    auto accept(IAstVisitor &) -> void override;
    AstDeclVar(StorageClass storage, QualType type, AstExprPointer &&init, std::string_view name = kAnonymous)
        : IAstDecl(type, name), m_storage(storage), m_initial(std::move(init)) {}

    inline auto &initial() { return m_initial; }
    inline auto &initial() const { return m_initial; }
    inline auto storage() const { return m_storage; }

private:
    StorageClass m_storage;
    AstExprPointer m_initial;
};

/// AstDeclFunc
/// ----------------------------------------------------------------------------
class AstDeclFunc final : public IAstDecl {
public:
    ~AstDeclFunc() override = default;
    auto accept(IAstVisitor &) -> void override;
    AstDeclFunc(StorageClass storage, QualType type,
                std::unique_ptr<AstStmtCompound> &&body,
                std::string_view name = kAnonymous)
        : IAstDecl(type, name),
          m_storage(storage),
          m_body(std::move(body)),
          m_scope(ScopeKind::Proto, nullptr) {}

    inline auto &body() { return m_body; }
    inline auto &body() const { return m_body; }
    inline auto &scope() { return m_scope; }
    inline auto &scope() const { return m_scope; }
    inline auto &storage() { return m_storage; }
    inline auto &storage() const { return m_storage; }

private:
    StorageClass m_storage;
    std::unique_ptr<AstStmtCompound> m_body;
    Scope m_scope;
};

/// AstDeclMember
/// ----------------------------------------------------------------------------
class AstDeclMember;

/// statement
/// ----------------------------------------------------------------------------
class IAstStmt : public IAst {
public:
    constexpr IAstStmt() = default;
    ~IAstStmt() override = default;

    auto accept(IAstVisitor &) -> void override = 0;
};
class AstStmtJumpGoto : public IAstStmt {
public:
    ~AstStmtJumpGoto() override = default;
    auto accept(IAstVisitor &) -> void override;
    AstStmtJumpGoto(std::string_view lable) : m_lable(lable) {}

    inline auto &lable() { return m_lable; }
    inline auto &lable() const { return m_lable; }

private:
    std::string m_lable;
};
class AstStmtJumpBreak : public IAstStmt {
public:
    ~AstStmtJumpBreak() override = default;
    auto accept(IAstVisitor &) -> void override;
};
class AstStmtJumpReturn : public IAstStmt {
public:
    ~AstStmtJumpReturn() override = default;
    auto accept(IAstVisitor &) -> void override;
    AstStmtJumpReturn(AstExprPointer &&expr) : m_expr(std::move(expr)) {}

    inline auto &expr() { return m_expr; }
    inline auto &expr() const { return m_expr; }

private:
    AstExprPointer m_expr;
};
class AstStmtJumpContinue : public IAstStmt {
public:
    ~AstStmtJumpContinue() override = default;
    auto accept(IAstVisitor &) -> void override;
};
class AstStmtLable : public IAstStmt {
public:
    ~AstStmtLable() override = default;
    auto accept(IAstVisitor &) -> void override;

    AstStmtLable(std::string_view lable) : m_lable(lable) {}

    inline auto &lable() { return m_lable; }
    inline auto &lable() const { return m_lable; }

private:
    std::string m_lable;
};
class AstStmtLableCase : public IAstStmt {
public:
    ~AstStmtLableCase() override = default;
    auto accept(IAstVisitor &) -> void override;
    AstStmtLableCase(AstExprPointer &&expr) : m_expr(std::move(expr)) {}

    inline auto &expr() { return m_expr; }
    inline auto &expr() const { return m_expr; }

private:
    AstExprPointer m_expr;
};
class AstStmtLableDefault : public IAstStmt {
public:
    ~AstStmtLableDefault() override = default;
    auto accept(IAstVisitor &) -> void override;
};
class AstStmtSelectionIfElse : public IAstStmt {
public:
    ~AstStmtSelectionIfElse() override = default;
    auto accept(IAstVisitor &) -> void override;
    AstStmtSelectionIfElse(AstExprPointer &&cond, AstStmtPointer &&then, AstStmtPointer &&elze)
        : m_cond(std::move(cond)), m_then(std::move(then)), m_elze(std::move(elze)) {}

    inline auto &cond() { return m_cond; }
    inline auto &then() { return m_then; }
    inline auto &elze() { return m_elze; }
    inline auto &cond() const { return m_cond; }
    inline auto &then() const { return m_then; }
    inline auto &elze() const { return m_elze; }

private:
    AstExprPointer m_cond;
    AstStmtPointer m_then;
    AstStmtPointer m_elze;
};
class AstStmtSelectionSwitch : public IAstStmt {
public:
    ~AstStmtSelectionSwitch() override = default;
    auto accept(IAstVisitor &) -> void override;
    AstStmtSelectionSwitch(AstExprPointer &&cond, AstStmtPointer &&body)
        : m_cond(std::move(cond)), m_body(std::move(body)) {}

    inline auto &cond() { return m_cond; }
    inline auto &body() { return m_body; }
    inline auto &cond() const { return m_cond; }
    inline auto &body() const { return m_body; }

private:
    AstExprPointer m_cond;
    AstStmtPointer m_body;
};
class AstStmtIterationFor : public IAstStmt {
public:
    ~AstStmtIterationFor() override = default;
    auto accept(IAstVisitor &) -> void override;
    AstStmtIterationFor(AstExprPointer &&init, AstExprPointer &&cond, AstExprPointer &&iter, AstStmtPointer &&body)
        : m_init(std::move(init)), m_cond(std::move(cond)), m_iter(std::move(iter)), m_body(std::move(body)) {}

    inline auto &init() { return m_init; }
    inline auto &cond() { return m_cond; }
    inline auto &iter() { return m_iter; }
    inline auto &body() { return m_body; }
    inline auto &init() const { return m_init; }
    inline auto &cond() const { return m_cond; }
    inline auto &iter() const { return m_iter; }
    inline auto &body() const { return m_body; }

private:
    AstExprPointer m_init;
    AstExprPointer m_cond;
    AstExprPointer m_iter;
    AstStmtPointer m_body;
};
class AstStmtIterationWhile : public IAstStmt {
public:
    ~AstStmtIterationWhile() override = default;
    auto accept(IAstVisitor &) -> void override;
    AstStmtIterationWhile(AstExprPointer &&cond, AstStmtPointer &&body)
        : m_cond(std::move(cond)), m_body(std::move(body)) {}

    inline auto &cond() { return m_cond; }
    inline auto &body() { return m_body; }
    inline auto &cond() const { return m_cond; }
    inline auto &body() const { return m_body; }

private:
    AstExprPointer m_cond;
    AstStmtPointer m_body;
};
class AstStmtIterationDoWhile : public IAstStmt {
public:
    ~AstStmtIterationDoWhile() override = default;
    auto accept(IAstVisitor &) -> void override;
    AstStmtIterationDoWhile(AstExprPointer &&cond, AstStmtPointer &&body)
        : m_cond(std::move(cond)), m_body(std::move(body)) {}

    inline auto &cond() { return m_cond; }
    inline auto &body() { return m_body; }
    inline auto &cond() const { return m_cond; }
    inline auto &body() const { return m_body; }

private:
    AstExprPointer m_cond;
    AstStmtPointer m_body;
};
class AstStmtEmpty : public IAstStmt {
public:
    ~AstStmtEmpty() override = default;
    auto accept(IAstVisitor &) -> void override;
};
class AstStmtExpr : public IAstStmt {
public:
    ~AstStmtExpr() override = default;
    auto accept(IAstVisitor &) -> void override;
    AstStmtExpr(AstExprPointer &&expr)
        : IAstStmt(), m_expr(std::move(expr)) {}

    inline auto &expr() { return m_expr; }
    inline auto &expr() const { return m_expr; }

private:
    AstExprPointer m_expr;
};
class AstStmtCompound : public IAstStmt {
public:
    ~AstStmtCompound() override = default;
    auto accept(IAstVisitor &) -> void override;
    AstStmtCompound(std::vector<AstPointer> &&stmt_delc)
        : m_stmt_decl(std::move(stmt_delc)),
          m_scope(ScopeKind::Block, nullptr) {}

    inline decltype(auto) begin() { return m_stmt_decl.begin(); }
    inline decltype(auto) end() { return m_stmt_decl.end(); }
    inline decltype(auto) begin() const { return m_stmt_decl.begin(); }
    inline decltype(auto) end() const { return m_stmt_decl.end(); }

    inline auto &stmt_decl() { return m_stmt_decl; }
    inline auto &stmt_decl() const { return m_stmt_decl; }
    inline auto &scope() { return m_scope; }
    inline auto &scope() const { return m_scope; }

private:
    std::vector<AstPointer> m_stmt_decl;
    Scope m_scope;
};

/// expression
/// ----------------------------------------------------------------------------
class IAstExpr : public IAst {
public:
    IAstExpr()                                  = default;
    ~IAstExpr() override                        = default;
    auto accept(IAstVisitor &) -> void override = 0;

private:
    QualType m_type;
};
class AstExprUnary : public IAstExpr {
public:
    ~AstExprUnary() override = default;
    auto accept(IAstVisitor &) -> void override;
    AstExprUnary(OptrKind op, AstExprPointer &&x)
        : m_optr(op), m_opnd(std::move(x)) {}

    inline auto &optr() { return m_optr; }
    inline auto &opnd() { return m_opnd; }
    inline auto &optr() const { return m_optr; }
    inline auto &opnd() const { return m_opnd; }

private:
    OptrKind m_optr;
    AstExprPointer m_opnd;
};
class AstExprBinary : public IAstExpr {
public:
    ~AstExprBinary() override = default;
    auto accept(IAstVisitor &) -> void override;
    AstExprBinary(OptrKind op, AstExprPointer &&x, AstExprPointer &&y)
        : m_optr(op), m_lhs(std::move(x)), m_rhs(std::move(y)) {}

    inline auto &optr() { return m_optr; }
    inline auto &lhs() { return m_lhs; }
    inline auto &rhs() { return m_rhs; }
    inline auto &optr() const { return m_optr; }
    inline auto &lhs() const { return m_lhs; }
    inline auto &rhs() const { return m_rhs; }

private:
    OptrKind m_optr;
    AstExprPointer m_lhs;
    AstExprPointer m_rhs;
};
class AstExprTernary : public IAstExpr {
public:
    ~AstExprTernary() override = default;
    auto accept(IAstVisitor &) -> void override;
    AstExprTernary(AstExprPointer cond, AstExprPointer then, AstExprPointer elze)
        : m_cond(std::move(cond)), m_then(std::move(then)), m_elze(std::move(elze)) {}

    inline auto &cond() { return m_cond; }
    inline auto &then() { return m_then; }
    inline auto &elze() { return m_elze; }
    inline auto &cond() const { return m_cond; }
    inline auto &then() const { return m_then; }
    inline auto &elze() const { return m_elze; }

private:
    AstExprPointer m_cond;
    AstExprPointer m_then;
    AstExprPointer m_elze;
};
class AstExprFuncCall : public IAstExpr {
public:
    ~AstExprFuncCall() override = default;
    auto accept(IAstVisitor &) -> void override;
    AstExprFuncCall(AstExprPointer &&funct, std::vector<AstExprPointer> &&args)
        : m_func(std::move(funct)), m_args(std::move(args)) {}

    inline auto &func() { return m_func; }
    inline auto &args() { return m_args; }
    inline auto &func() const { return m_func; }
    inline auto &args() const { return m_args; }

private:
    AstExprPointer m_func;
    std::vector<AstExprPointer> m_args;
};
class AstExprConstant : public IAstExpr {
public:
    ~AstExprConstant() override = default;
    auto accept(IAstVisitor &) -> void override;
    AstExprConstant(std::string_view value) : m_value(value) {}

    inline auto &value() { return m_value; }
    inline auto &value() const { return m_value; }

private:
    std::string m_value;
};
class AstExprCharacter : public IAstExpr {
public:
    ~AstExprCharacter() override = default;
    auto accept(IAstVisitor &) -> void override;
    AstExprCharacter(std::string_view value) : m_value(value) {}

    inline auto &value() { return m_value; }
    inline auto &value() const { return m_value; }

private:
    std::string m_value;
};
class AstExprString : public IAstExpr {
public:
    ~AstExprString() override = default;
    auto accept(IAstVisitor &) -> void override;
    AstExprString(std::string_view value) : m_value(value) {}

    inline auto &value() { return m_value; }
    inline auto &value() const { return m_value; }

private:
    std::string m_value;
};
class AstExprIdentifier : public IAstExpr {
public:
    ~AstExprIdentifier() override = default;
    auto accept(IAstVisitor &) -> void override;
    AstExprIdentifier(std::string_view name) : m_name(name) {}

    inline auto &name() { return m_name; }
    inline auto &name() const { return m_name; }

private:
    std::string m_name;
};

}  // namespace mcc
