#include "type.hpp"

#include <iostream>
#include <sstream>

#include "astformatter.hpp"
#include "asttypes.hpp"

namespace mcc {

/// bultin types
/// ----------------------------------------------------------------------------
static auto s_void       = std::make_shared<BasicType>(Specifier::Void);
static auto s_schar      = std::make_shared<BasicType>(Specifier::Char);
static auto s_sshort     = std::make_shared<BasicType>(Specifier::Short);
static auto s_sint       = std::make_shared<BasicType>(Specifier::Int);
static auto s_slong      = std::make_shared<BasicType>(Specifier::Long);
static auto s_uchar      = std::make_shared<BasicType>(Specifier::Unsigned | Specifier::Char);
static auto s_ushort     = std::make_shared<BasicType>(Specifier::Unsigned | Specifier::Short);
static auto s_uint       = std::make_shared<BasicType>(Specifier::Unsigned | Specifier::Int);
static auto s_ulong      = std::make_shared<BasicType>(Specifier::Unsigned | Specifier::Long);
static auto s_float      = std::make_shared<BasicType>(Specifier::Float);
static auto s_double     = std::make_shared<BasicType>(Specifier::Double);
static auto s_longdouble = std::make_shared<BasicType>(Specifier::Long | Specifier::Double);

/// ostream
/// ----------------------------------------------------------------------------
extern auto operator<<(std::ostream &os, StorageClass storage) -> std::ostream & {
    switch (storage) {
        case StorageClass::Auto: os << "auto"; break;
        case StorageClass::Extern: os << "extern"; break;
        case StorageClass::Static: os << "static"; break;
        case StorageClass::Typedef: os << "typedef"; break;
        case StorageClass::Register: os << "register"; break;
        default: os << "default"; break;
    }
    return os;
}
extern auto operator<<(std::ostream &os, const IType &type) -> std::ostream & {
    type.format(os);
    return os;
}
extern auto operator<<(std::ostream &os, const QualType &type) -> std::ostream & {
    if (type.is_const()) os << "const ";
    if (type.is_volatile()) os << "volatile ";
    if (type.type()) type.type()->format(os);
    return os;
}

QualType::QualType(Qualifier qual) : m_qualifier(qual), m_type(s_void) {}
QualType::QualType(IType::Pointer type, Qualifier qual) : m_qualifier(qual), m_type(type) {}

/// virtual method : format
/// ----------------------------------------------------------------------------
auto BasicType::format(std::ostream &os) const -> void {
    switch (static_cast<uint32_t>(specifier())) {
        case static_cast<uint32_t>(Specifier::Void): os << "void"; break;
        case static_cast<uint32_t>(Specifier::Char): os << "char"; break;
        case static_cast<uint32_t>(Specifier::Short): os << "short"; break;
        case static_cast<uint32_t>(Specifier::Int): os << "int"; break;
        case static_cast<uint32_t>(Specifier::Long): os << "long"; break;
        case static_cast<uint32_t>(Specifier::Unsigned | Specifier::Char): os << "unsigned char"; break;
        case static_cast<uint32_t>(Specifier::Unsigned | Specifier::Short): os << "unsigned short"; break;
        case static_cast<uint32_t>(Specifier::Unsigned | Specifier::Int): os << "unsigned int"; break;
        case static_cast<uint32_t>(Specifier::Unsigned | Specifier::Long): os << "unsigned long"; break;
        case static_cast<uint32_t>(Specifier::Float): os << "float"; break;
        case static_cast<uint32_t>(Specifier::Double): os << "double"; break;
        case static_cast<uint32_t>(Specifier::Long | Specifier::Double): os << "long double"; break;
        default: std::abort();
    }
}
auto ArrayType::format(std::ostream &os) const -> void {
    os << "array<" << base() << '>';
}
auto PointerType::format(std::ostream &os) const -> void {
    os << "ptr<" << base() << '>';
}
auto FunctionType::format(std::ostream &os) const -> void {
    os << m_rettype << '(';

    if (!m_param_types.empty()) {
        os << m_param_types.front() << ' ' << m_param_names.front();
        for (size_t i = 1; i < m_param_types.size(); ++i) {
            os << ", " << m_param_types[i] << ' ' << m_param_names[i];
        }
    }
    if (m_variadic) os << ", ...";
    os << ')';
}

/// virtual method : format
/// ----------------------------------------------------------------------------
auto QualType::format(const std::string &id) -> std::string {
    if (is_const() && is_volatile()) return type()->format("const volatile " + id);
    if (is_volatile()) return type()->format("volatile " + id);
    if (is_const()) return type()->format("const " + id);
    return type()->format(id);
}
auto BasicType::format(const std::string &id) -> std::string {
    static const auto to_string = [](Specifier spec) {
        switch (static_cast<uint32_t>(spec)) {
            case static_cast<uint32_t>(Specifier::Void): return "void";
            case static_cast<uint32_t>(Specifier::Char): return "char";
            case static_cast<uint32_t>(Specifier::Short): return "short";
            case static_cast<uint32_t>(Specifier::Int): return "int";
            case static_cast<uint32_t>(Specifier::Long): return "long";
            case static_cast<uint32_t>(Specifier::Unsigned | Specifier::Char): return "unsigned char";
            case static_cast<uint32_t>(Specifier::Unsigned | Specifier::Short): return "unsigned short";
            case static_cast<uint32_t>(Specifier::Unsigned | Specifier::Int): return "unsigned int";
            case static_cast<uint32_t>(Specifier::Unsigned | Specifier::Long): return "unsigned long";
            case static_cast<uint32_t>(Specifier::Float): return "float";
            case static_cast<uint32_t>(Specifier::Double): return "double";
            case static_cast<uint32_t>(Specifier::Long | Specifier::Double): return "long double";
            default: std::abort();
        }
    };
    if (id.empty()) {
        return to_string(specifier());
    } else {
        return std::string{to_string(specifier())} + ' ' + id;
    }
}
auto ArrayType::format(const std::string &id) -> std::string {
    std::stringstream ss;
    AstFormatter ssfmt(ss);
    length()->accept(ssfmt);
    return base().format(id + '[' + ss.str() + ']');
}
auto PointerType::format(const std::string &id) -> std::string {
    if (base().type()->kind() == IType::Kind::Array ||
        base().type()->kind() == IType::Kind::Function) {
        return base().format("(*" + id + ')');
    }
    return base().format('*' + id);
}
auto FunctionType::format(const std::string &id) -> std::string {
    std::string result = m_rettype.format(id) + '(';
    if (!m_param_types.empty()) {
        result += m_param_types.front().format(m_param_names.front());
        for (size_t i = 1; i < m_param_types.size(); ++i) {
            result += ", ";
            result += m_param_types[i].format(m_param_names[i]);
        }
    }
    if (m_variadic) result += ", ...";
    return result + ')';
}

/// virtual method : kind
/// ----------------------------------------------------------------------------
auto BasicType::kind() const -> Kind { return Kind::Basic; }
auto ArrayType::kind() const -> Kind { return Kind::Array; }
auto PointerType::kind() const -> Kind { return Kind::Pointer; }
auto FunctionType::kind() const -> Kind { return Kind::Function; }

/// virtual method : size
/// ----------------------------------------------------------------------------
auto BasicType::size() const -> size_t {
    switch (static_cast<uint32_t>(specifier())) {
        case static_cast<uint32_t>(Specifier::Void): return kVoidWidth;
        case static_cast<uint32_t>(Specifier::Char): return kCharWidth;
        case static_cast<uint32_t>(Specifier::Short): return kShortWidth;
        case static_cast<uint32_t>(Specifier::Int): return kIntWidth;
        case static_cast<uint32_t>(Specifier::Long): return kLongLongWidth;
        case static_cast<uint32_t>(Specifier::Unsigned | Specifier::Char): return kCharWidth;
        case static_cast<uint32_t>(Specifier::Unsigned | Specifier::Short): return kShortWidth;
        case static_cast<uint32_t>(Specifier::Unsigned | Specifier::Int): return kIntWidth;
        case static_cast<uint32_t>(Specifier::Unsigned | Specifier::Long): return kLongLongWidth;
        case static_cast<uint32_t>(Specifier::Float): return kFloatWidth;
        case static_cast<uint32_t>(Specifier::Double): return kDoubleWidth;
        case static_cast<uint32_t>(Specifier::Long | Specifier::Double): return kLongDoubleWidth;
        default: std::abort();
    }
}
auto ArrayType::size() const -> size_t { return 0; }
auto PointerType::size() const -> size_t { return kPointerWidth; }
auto FunctionType::size() const -> size_t { return kVoidWidth; }

/// static method : make
/// ----------------------------------------------------------------------------
auto BasicType::make(Specifier spec) -> Pointer {
    if (spec == Specifier::Signed) {
        spec = Specifier::Int;
    } else {
        spec &= ~Specifier::Signed;
        if ((spec & Specifier::Short) != Specifier::None) spec &= ~Specifier::Int;
        if ((spec & Specifier::Long) != Specifier::None) spec &= ~Specifier::Int;
    }

    switch (static_cast<uint32_t>(spec)) {
        case static_cast<uint32_t>(Specifier::Void): return s_void;
        case static_cast<uint32_t>(Specifier::Char): return s_schar;
        case static_cast<uint32_t>(Specifier::Short): return s_sshort;
        case static_cast<uint32_t>(Specifier::Int): return s_sint;
        case static_cast<uint32_t>(Specifier::Long): return s_slong;
        case static_cast<uint32_t>(Specifier::Unsigned | Specifier::Char): return s_uchar;
        case static_cast<uint32_t>(Specifier::Unsigned | Specifier::Short): return s_ushort;
        case static_cast<uint32_t>(Specifier::Unsigned | Specifier::Int): return s_uint;
        case static_cast<uint32_t>(Specifier::Unsigned | Specifier::Long): return s_ulong;
        case static_cast<uint32_t>(Specifier::Float): return s_float;
        case static_cast<uint32_t>(Specifier::Double): return s_double;
        case static_cast<uint32_t>(Specifier::Long | Specifier::Double): return s_longdouble;
        default: return nullptr;
    }
}
auto PointerType::make(QualType base) -> Pointer { return std::make_shared<PointerType>(base); }
auto FunctionType::make(QualType ret) -> Pointer { return std::make_shared<FunctionType>(ret); }
auto ArrayType::make(QualType base, std::unique_ptr<IAstExpr> &&len) -> Pointer { return std::make_shared<ArrayType>(base, std::move(len)); }

/// BasicType
/// ----------------------------------------------------------------------------
auto BasicType::is_void() const -> bool {
    return m_specifier == Specifier::Void;
}

auto BasicType::is_integral() const -> bool {
    switch (static_cast<uint32_t>(m_specifier)) {
        case static_cast<uint32_t>(Specifier::Char):
        case static_cast<uint32_t>(Specifier::Short):
        case static_cast<uint32_t>(Specifier::Int):
        case static_cast<uint32_t>(Specifier::Long):
        case static_cast<uint32_t>(Specifier::Unsigned | Specifier::Char):
        case static_cast<uint32_t>(Specifier::Unsigned | Specifier::Short):
        case static_cast<uint32_t>(Specifier::Unsigned | Specifier::Int):
        case static_cast<uint32_t>(Specifier::Unsigned | Specifier::Long): return true;
        default: return false;
    }
}

auto BasicType::is_floating() const -> bool {
    switch (static_cast<uint32_t>(m_specifier)) {
        case static_cast<uint32_t>(Specifier::Float):
        case static_cast<uint32_t>(Specifier::Double):
        case static_cast<uint32_t>(Specifier::Double | Specifier::Long): return true;
        default: return false;
    }
}

ArrayType::ArrayType(QualType base, std::unique_ptr<IAstExpr> &&len)
    : m_base(base), m_length(std::move(len)) {}

}  // namespace mcc