#pragma once

#include <memory>
#include <vector>

#include "astfwd.hpp"
#include "token.hpp"

#define MCC_DEFINE_MEMBER(TYPE, NAME)              \
private:                                           \
    TYPE m_##NAME;                                 \
                                                   \
public:                                            \
    inline auto NAME()->TYPE& { return m_##NAME; } \
    inline auto NAME() const->const TYPE& { return m_##NAME; }

#define MCC_DEFINE_TYPE_CLASS(TYPE)                          \
public:                                                      \
    TYPE()          = default;                               \
    virtual ~TYPE() = default;                               \
    using Pointer   = std::shared_ptr<TYPE>;                 \
    virtual auto kind() const->Kind override;                \
    virtual auto size() const->size_t override;              \
    virtual auto format(std::ostream&) const->void override; \
    virtual auto format(const std::string&)->std::string override;

namespace mcc {

/// StorageClass
/// ----------------------------------------------------------------------------
enum class StorageClass : uint8_t {
    None = 0,
    Auto,
    Extern,
    Static,
    Typedef,
    Register,
};

/// Qualifier
/// ----------------------------------------------------------------------------
enum class Qualifier : uint8_t {
    None     = 0,
    Const    = 1 << 0,
    Volatile = 1 << 1,
};
constexpr auto operator~(Qualifier lhs) -> Qualifier { return static_cast<Qualifier>(~static_cast<uint8_t>(lhs)); }
constexpr auto operator&(Qualifier lhs, Qualifier rhs) -> Qualifier { return static_cast<Qualifier>(static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs)); }
constexpr auto operator|(Qualifier lhs, Qualifier rhs) -> Qualifier { return static_cast<Qualifier>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs)); }
constexpr auto operator&=(Qualifier& lhs, Qualifier rhs) -> Qualifier { return lhs = lhs & rhs; }
constexpr auto operator|=(Qualifier& lhs, Qualifier rhs) -> Qualifier { return lhs = lhs | rhs; }

/// Specifier
/// ----------------------------------------------------------------------------
enum class Specifier : uint32_t {
    None     = 0,
    Void     = 1 << 0,
    Signed   = 1 << 1,
    Unsigned = 1 << 2,
    Char     = 1 << 3,
    Short    = 1 << 4,
    Int      = 1 << 5,
    Long     = 1 << 6,
    Float    = 1 << 7,
    Double   = 1 << 8,
    Enum     = 1 << 9,
    Union    = 1 << 10,
    Struct   = 1 << 11,

};
constexpr auto operator~(Specifier lhs) -> Specifier { return static_cast<Specifier>(~static_cast<uint32_t>(lhs)); }
constexpr auto operator&(Specifier lhs, Specifier rhs) -> Specifier { return static_cast<Specifier>(static_cast<uint32_t>(lhs) & static_cast<uint32_t>(rhs)); }
constexpr auto operator|(Specifier lhs, Specifier rhs) -> Specifier { return static_cast<Specifier>(static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs)); }
constexpr auto operator&=(Specifier& lhs, Specifier rhs) -> Specifier { return lhs = lhs & rhs; }
constexpr auto operator|=(Specifier& lhs, Specifier rhs) -> Specifier { return lhs = lhs | rhs; }

using DeclSpec = std::tuple<StorageClass, Qualifier, Specifier>;
using QualSpec = std::tuple<Qualifier, Specifier>;

constexpr auto token_to_storage_class(TokenKind kind) -> StorageClass {
    switch (kind) {
        case TokenKind::KwAuto: return StorageClass::Auto;
        case TokenKind::KwExtern: return StorageClass::Extern;
        case TokenKind::KwStatic: return StorageClass::Static;
        case TokenKind::KwTypedef: return StorageClass::Typedef;
        case TokenKind::KwRegister: return StorageClass::Register;
        default: return StorageClass::None;
    }
}
constexpr auto token_to_qualifier(TokenKind kind) -> Qualifier {
    switch (kind) {
        case TokenKind::KwConst: return Qualifier::Const;
        case TokenKind::KwVolatile: return Qualifier::Volatile;
        default: return Qualifier::None;
    }
}
constexpr auto token_to_specifier(TokenKind kind) -> Specifier {
    switch (kind) {
        case TokenKind::KwVoid: return Specifier::Void;
        case TokenKind::KwSigned: return Specifier::Signed;
        case TokenKind::KwUnsigned: return Specifier::Unsigned;
        case TokenKind::KwChar: return Specifier::Char;
        case TokenKind::KwShort: return Specifier::Short;
        case TokenKind::KwInt: return Specifier::Int;
        case TokenKind::KwLong: return Specifier::Long;
        case TokenKind::KwFloat: return Specifier::Float;
        case TokenKind::KwDouble: return Specifier::Double;
        case TokenKind::KwEnum: return Specifier::Enum;
        case TokenKind::KwUnion: return Specifier::Union;
        case TokenKind::KwStruct: return Specifier::Struct;
        default: return Specifier::None;
    }
}

extern auto operator<<(std::ostream&, StorageClass) -> std::ostream&;

/// IType
/// ----------------------------------------------------------------------------
class IType {
public:
    constexpr IType() = default;
    virtual ~IType()  = default;

    enum class Kind {
        Basic,
        Array,
        // Union,
        // Struct,
        Pointer,
        Function,
    };

    static constexpr size_t kVoidWidth       = 1;
    static constexpr size_t kCharWidth       = 1;
    static constexpr size_t kShortWidth      = 2;
    static constexpr size_t kIntWidth        = 4;
    static constexpr size_t kLongWidth       = 8;
    static constexpr size_t kLongLongWidth   = 8;
    static constexpr size_t kPointerWidth    = 8;
    static constexpr size_t kFloatWidth      = 4;
    static constexpr size_t kDoubleWidth     = 8;
    static constexpr size_t kLongDoubleWidth = 16;

    using Pointer = std::shared_ptr<IType>;

    [[nodiscard]] virtual auto kind() const -> Kind        = 0;
    [[nodiscard]] virtual auto size() const -> size_t      = 0;
    virtual auto format(std::ostream&) const -> void       = 0;
    virtual auto format(const std::string&) -> std::string = 0;
};

extern auto operator<<(std::ostream&, const IType&) -> std::ostream&;

/// QualType
/// ----------------------------------------------------------------------------
class QualType final {
public:
    explicit QualType(Qualifier = Qualifier::None);
    explicit QualType(IType::Pointer type, Qualifier = Qualifier::None);
    ~QualType() = default;

    MCC_DEFINE_MEMBER(Qualifier, qualifier);
    MCC_DEFINE_MEMBER(IType::Pointer, type);

    auto format(const std::string&) -> std::string;

    [[nodiscard]] constexpr auto is_const() const -> bool { return static_cast<uint32_t>(m_qualifier & Qualifier::Const); }
    [[nodiscard]] constexpr auto is_volatile() const -> bool { return static_cast<uint32_t>(m_qualifier & Qualifier::Volatile); }
};

extern auto operator<<(std::ostream&, const QualType&) -> std::ostream&;

/// Derived types
/// ----------------------------------------------------------------------------
class BasicType : public IType {
    MCC_DEFINE_TYPE_CLASS(BasicType)
    MCC_DEFINE_MEMBER(Specifier, specifier)

    [[nodiscard]] auto is_void() const -> bool;
    [[nodiscard]] auto is_integral() const -> bool;
    [[nodiscard]] auto is_floating() const -> bool;

    BasicType(Specifier spec) : m_specifier(spec) {}
    static auto make(Specifier spec) -> Pointer;
};

class PointerType : public IType {
    MCC_DEFINE_TYPE_CLASS(PointerType)
    MCC_DEFINE_MEMBER(QualType, base)

    PointerType(const QualType& base) : m_base(base) {}
    static auto make(QualType base) -> Pointer;
};

class ArrayType : public IType {
    MCC_DEFINE_TYPE_CLASS(ArrayType)
    MCC_DEFINE_MEMBER(QualType, base)
    MCC_DEFINE_MEMBER(std::unique_ptr<IAstExpr>, length)

    ArrayType(QualType base, std::unique_ptr<IAstExpr>&& len);
    static auto make(QualType base, std::unique_ptr<IAstExpr>&& len) -> Pointer;
};

class FunctionType : public IType {
    MCC_DEFINE_TYPE_CLASS(FunctionType)
    MCC_DEFINE_MEMBER(bool, variadic)
    MCC_DEFINE_MEMBER(QualType, rettype)
    MCC_DEFINE_MEMBER(std::vector<QualType>, param_types)
    MCC_DEFINE_MEMBER(std::vector<std::string>, param_names)

    FunctionType(const QualType& ret) : m_variadic(false), m_rettype(ret) {}
    static auto make(QualType ret) -> Pointer;
};

}  // namespace mcc

#undef MCC_DEFINE_TYPE_CLASS
#undef MCC_DEFINE_MEMBER