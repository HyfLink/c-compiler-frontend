#include "operator.hpp"

namespace mcc {

extern auto to_string(OptrKind optr) -> std::string_view {
    switch (optr) {
#define MCC_DEFINE_OPERATOR(ENUM, STRING, VALUE, TOKEN, SYMBOL) \
    case OptrKind::ENUM:                                        \
        return STRING;
#include "inl/operator.inl"
#undef MCC_DEFINE_OPERATOR
        default: return "[none]";
    }
}

extern auto to_symbol(OptrKind optr) -> std::string_view {
    switch (optr) {
#define MCC_DEFINE_OPERATOR(ENUM, STRING, VALUE, TOKEN, SYMBOL) \
    case OptrKind::ENUM:                                        \
        return SYMBOL;
#include "inl/operator.inl"
#undef MCC_DEFINE_OPERATOR
        default: return "[none]";
    }
}

}  // namespace mcc