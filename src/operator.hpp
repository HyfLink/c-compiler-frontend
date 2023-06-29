#pragma once

#include "token.hpp"

namespace mcc {

enum class OptrKind {
    None = 0,
#define MCC_DEFINE_OPERATOR(ENUM, STRING, VALUE, TOKEN, SYMBOL) ENUM = VALUE,
#include "inl/operator.inl"
#undef MCC_DEFINE_OPERATOR
};

extern auto to_string(OptrKind optr) -> std::string_view;
extern auto to_symbol(OptrKind optr) -> std::string_view;

namespace detail {

template <TokenKind... Ts>
struct tokens {};

template <OptrKind... Ts>
struct optrs {};

template <typename TokenList, typename OptrList>
struct token_to_optr {};

template <TokenKind X, OptrKind Y>
struct token_to_optr<tokens<X>, optrs<Y>> {
    static constexpr OptrKind to_optr(TokenKind kind) {
        return kind == X ? Y : OptrKind::None;
    }
};

template <TokenKind X, TokenKind... Xs, OptrKind Y, OptrKind... Ys>
struct token_to_optr<tokens<X, Xs...>, optrs<Y, Ys...>> {
    static constexpr OptrKind to_optr(TokenKind kind) {
        return kind == X ? Y : token_to_optr<tokens<Xs...>, optrs<Ys...>>::to_optr(kind);
    }
};

using token_to_logical_or_optr_t  = token_to_optr<tokens<TokenKind::Or>, optrs<OptrKind::Or>>;
using token_to_logical_and_optr_t = token_to_optr<tokens<TokenKind::And>, optrs<OptrKind::And>>;
using token_to_comma_optr_t       = token_to_optr<tokens<TokenKind::Comma>, optrs<OptrKind::Comma>>;
using token_to_bitwise_or_optr_t  = token_to_optr<tokens<TokenKind::BitOr>, optrs<OptrKind::BitOr>>;
using token_to_bitwise_xor_optr_t = token_to_optr<tokens<TokenKind::BitXor>, optrs<OptrKind::BitXor>>;
using token_to_bitwise_and_optr_t = token_to_optr<tokens<TokenKind::BitAnd>, optrs<OptrKind::BitAnd>>;
using token_to_additive_optr_t    = token_to_optr<
    tokens<TokenKind::Add, TokenKind::Sub>,
    optrs<OptrKind::Add, OptrKind::Sub>>;
using token_to_bitwise_shift_optr_t = token_to_optr<
    tokens<TokenKind::BitShl, TokenKind::BitShr>,
    optrs<OptrKind::BitShl, OptrKind::BitShr>>;
using token_to_equality_optr_t = token_to_optr<
    tokens<TokenKind::Equal, TokenKind::NotEqual>,
    optrs<OptrKind::Equal, OptrKind::NotEqual>>;
using token_to_inc_dec_optr_t = token_to_optr<
    tokens<TokenKind::Increase, TokenKind::Decrease>,
    optrs<OptrKind::Increase, OptrKind::Decrease>>;
using token_to_multiplicative_optr_t = token_to_optr<
    tokens<TokenKind::Mul, TokenKind::Div, TokenKind::Mod>,
    optrs<OptrKind::Mul, OptrKind::Div, OptrKind::Mod>>;
using token_to_unary_optr_t = token_to_optr<
    tokens<TokenKind::BitAnd, TokenKind::Mul, TokenKind::Add,
           TokenKind::Sub, TokenKind::BitNot, TokenKind::Not>,
    optrs<OptrKind::Ref, OptrKind::Deref, OptrKind::Positive,
          OptrKind::Negative, OptrKind::BitNot, OptrKind::Not>>;
using token_to_relational_optr_t = token_to_optr<
    tokens<TokenKind::Less, TokenKind::LessEqual, TokenKind::Greater, TokenKind::GreaterEqual>,
    optrs<OptrKind::Less, OptrKind::LessEqual, OptrKind::Greater, OptrKind::GreaterEqual>>;
using token_to_assign_optr_t = token_to_optr<
    tokens<TokenKind::Assign, TokenKind::OrAssign, TokenKind::AddAssign, TokenKind::SubAssign,
           TokenKind::MulAssign, TokenKind::DivAssign, TokenKind::ModAssign, TokenKind::AndAssign,
           TokenKind::XorAssign, TokenKind::ShlAssign, TokenKind::ShrAssign>,
    optrs<OptrKind::Assign, OptrKind::OrAssign, OptrKind::AddAssign, OptrKind::SubAssign,
          OptrKind::MulAssign, OptrKind::DivAssign, OptrKind::ModAssign, OptrKind::AndAssign,
          OptrKind::XorAssign, OptrKind::ShlAssign, OptrKind::ShrAssign>>;

}  // namespace detail

using detail::token_to_additive_optr_t;
using detail::token_to_assign_optr_t;
using detail::token_to_bitwise_and_optr_t;
using detail::token_to_bitwise_or_optr_t;
using detail::token_to_bitwise_shift_optr_t;
using detail::token_to_bitwise_xor_optr_t;
using detail::token_to_comma_optr_t;
using detail::token_to_equality_optr_t;
using detail::token_to_inc_dec_optr_t;
using detail::token_to_logical_and_optr_t;
using detail::token_to_logical_or_optr_t;
using detail::token_to_multiplicative_optr_t;
using detail::token_to_relational_optr_t;
using detail::token_to_unary_optr_t;

}  // namespace mcc