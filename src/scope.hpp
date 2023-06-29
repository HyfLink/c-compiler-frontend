#pragma once

#include <map>
#include <memory>
#include <string_view>

#include "astfwd.hpp"

namespace mcc {

enum class ScopeKind {
    File,
    Func,
    Proto,
    Block,
};

class Scope {
public:
    Scope(ScopeKind kind, Scope *parent)
        : m_kind(kind), m_parent(parent), m_idents() {}

    inline auto kind() const { return m_kind; }
    inline auto &parent() { return m_parent; }
    inline auto &parent() const { return m_parent; }
    inline auto &idents() { return m_idents; }
    inline auto &idents() const { return m_idents; }

private:
    ScopeKind m_kind;
    Scope *m_parent;
    std::map<std::string_view, IAst *> m_idents;
};

}  // namespace mcc