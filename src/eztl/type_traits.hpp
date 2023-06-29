/*
 *  (1) type traits
 *      has trivial default constructor
 *      has trivial copy constructor
 *      has trivial assignment operator
 *      has trivial destructor
 *      is_POD_type
 * 
 *  1.  intergral_constants
 *      bool_constants
 *      true_type & false_type
 *      void_type
 *  2.  conditional
 *      enable_if_t
 *  3.  conjunction, disjunction, negation
 *  4.  is_same
 *  5.  is_one_of
 *  6.  remove_...
 *  7.  is_integral, is_floating_point,
 *      is_signed_integer, is_unsigned_integer
 */
#pragma once

namespace eztl
{
    //* intergral_type

    template <typename T, T V>
    struct integral_constant
    {
        static constexpr inline T value = V;
        typedef T value_type;
        typedef integral_constant<value_type, value> type;
    };

    template <bool V>
    using bool_constant = integral_constant<bool, V>;

    typedef bool_constant<true> true_type;
    typedef bool_constant<false> false_type;

    template <typename... T>
    using void_t = void;

    //* type traits

#define __define_type_traits(x, y, v)                \
    template <x>                                   \
    struct type_traits y {                         \
        typedef v has_trivial_default_constructor; \
        typedef v has_trivial_copy_constructor;    \
        typedef v has_trivial_assignment_operator; \
        typedef v has_trivial_destructor;          \
        typedef v is_POD_type;                     \
    }
    
    __define_type_traits(typename T, , false_type);
    __define_type_traits(typename T, <T *>, true_type);
    __define_type_traits(, <bool>, true_type);
    __define_type_traits(, <char>, true_type);
    __define_type_traits(, <signed char>, true_type);
    __define_type_traits(, <unsigned char>, true_type);
    __define_type_traits(, <wchar_t>, true_type);
    __define_type_traits(, <signed short>, true_type);
    __define_type_traits(, <unsigned short>, true_type);
    __define_type_traits(, <int>, true_type);
    __define_type_traits(, <unsigned int>, true_type);
    __define_type_traits(, <long>, true_type);
    __define_type_traits(, <unsigned long>, true_type);
    __define_type_traits(, <long long>, true_type);
    __define_type_traits(, <unsigned long long>, true_type);
    __define_type_traits(, <float>, true_type);
    __define_type_traits(, <double>, true_type);
    __define_type_traits(, <long double>, true_type);
#undef __define_type_traits

    //* conditional & enable_if_t

    template <bool If, typename Then, typename Else>
    struct conditional {
        typedef Then type;
    };

    template <typename Then, typename Else>
    struct conditional<false, Then, Else> {
        typedef Else type;
    };

    template <bool If, typename Then, typename Else>
    using conditional_t = typename conditional<If, Then, Else>::type;

    template <bool Cond, typename T = void>
    struct enable_if {};

    template <typename T>
    struct enable_if<true, T> {
        typedef T type;
    };

    template <bool Cond, typename T = void>
    using enable_if_t = typename enable_if<Cond, T>::type;

    //* conjunction disjunction negation

    template <typename... Ps>
    struct __and {};

    template <>
    struct __and<> : public true_type {};

    template <typename P>
    struct __and<P> : public P {};

    template <typename P, typename Q>
    struct __and<P, Q> : public conditional<P::value, Q, P>::type {};

    template <typename P, typename Q, typename R, typename... S>
    struct __and<P, Q, R, S...> : public conditional<P::value, __and<Q, R, S...>, P>::type {};

    template <typename... Ps>
    struct __or;

    template <>
    struct __or<> : public false_type {};

    template <typename P>
    struct __or<P> : public P {};

    template <typename P, typename Q>
    struct __or<P, Q> : public conditional<P::value, P, Q>::type {};

    template <typename P, typename Q, typename R, typename... S>
    struct __or<P, Q, R, S...> : public conditional<P::value, P, __or<Q, R, S...>>::type {};

    template <typename P>
    struct __not : public bool_constant<!bool(P::value)> {};

    template <typename... Ps>
    using conjunction = typename __and<Ps...>::type;

    template <typename... Ps>
    using disjunction = typename __or<Ps...>::type;

    template <typename P>
    using negation = typename __not<P>::type;

    template <typename... Ps>
    constexpr inline bool conjunction_v = __and<Ps...>::value;

    template <typename... Ps>
    constexpr inline bool disjunction_v = __or<Ps...>::value;

    template <typename P>
    constexpr inline bool negation_v = __not<P>::value;

    //* is_same
    template <typename T, typename U>
    struct is_same : public false_type {};

    template <typename T>
    struct is_same<T, T> : public true_type {};

    template <typename T, typename U>
    static constexpr bool is_same_v = is_same<T, U>::value;

    //* is_one_of
    template <typename T, typename... Ts>
    struct is_one_of;

    template <typename T, typename U>
    struct is_one_of<T, U> : public is_same<T, U>::type {};

    template <typename T, typename U, typename V, typename... W>
    struct is_one_of<T, U, V, W...> : public is_one_of<T, V, W...> {};

    template <typename T, typename V, typename... W>
    struct is_one_of<T, T, V, W...> : public true_type {};

    template <typename T, typename... Ts>
    static constexpr bool is_one_of_v = is_one_of<T, Ts...>::value;

    //* remove_reference ...

    template <typename T>
    struct remove_const { typedef T type; };

    template <typename T>
    struct remove_const<T const> { typedef T type; };

    template <typename T>
    struct remove_volatile { typedef T type; };

    template <typename T>
    struct remove_volatile<T volatile> { typedef T type; };

    template <typename T>
    struct remove_const_volatile { typedef T type; };

    template <typename T>
    struct remove_const_volatile<const T> { typedef T type; };

    template <typename T>
    struct remove_const_volatile<volatile T> { typedef T type; };

    template <typename T>
    struct remove_const_volatile<const volatile T> { typedef T type; };

    template <typename T>
    struct remove_reference { typedef T type; };

    template <typename T>
    struct remove_reference<T &> { typedef T type; };

    template <typename T>
    struct remove_reference<T &&> { typedef T type; };

    template <typename T>
    struct remove_lvalue_reference { typedef T type; };

    template <typename T>
    struct remove_lvalue_reference<T &> { typedef T type; };

    template <typename T>
    struct remove_rvalue_reference { typedef T type; };

    template <typename T>
    struct remove_rvalue_reference<T &&> { typedef T type; };

    template <typename T>
    struct __remove_pointer { typedef T type; };

    template <typename T>
    struct __remove_pointer<T *> { typedef typename __remove_pointer<T>::type type; };

    template <typename T>
    struct remove_pointer : public __remove_pointer<typename remove_const_volatile<T>::type> {};

    template <typename T>
    struct remove_extend { typedef T type; };

    template <typename T>
    struct remove_extend<T[]> { typedef T type; };

    template <typename T, size_t N>
    struct remove_extend<T[N]> { typedef T type; };
    
    template <typename T>
    struct remove_all_extend { typedef T type; };

    template <typename T>
    struct remove_all_extend<T[]> { typedef typename remove_all_extend<T>::type type; };

    template <typename T, size_t N>
    struct remove_all_extend<T[N]> { typedef typename remove_all_extend<T>::type type; };

    template <typename T>
    using remove_const_t = typename remove_const<T>::type;

    template <typename T>
    using remove_volatile_t = typename remove_volatile<T>::type;

    template <typename T>
    using remove_const_volatile_t = typename remove_const_volatile<T>::type;
    
    template <typename T>
    using remove_reference_t = typename remove_reference<T>::type;

    template <typename T>
    using remove_lvalue_reference_t = typename remove_lvalue_reference<T>::type;

    template <typename T>
    using remove_rvalue_reference_t = typename remove_rvalue_reference<T>::type;

    template <typename T>
    using remove_pointer_t = typename remove_pointer<T>::type;

    template <typename T>
    using remove_extend_t = typename remove_extend<T>::type;

    template <typename T>
    using remove_all_extend_t = typename remove_all_extend<T>::type;

    //* is_integral ...
    
    template <typename T>
    using is_signed_integer = is_one_of<T, signed char, signed short, signed int, signed long, signed long long>;

    template <typename T>
    using is_unsigned_integer = is_one_of<T, unsigned char, unsigned short, unsigned int, unsigned long, unsigned long long>;

    template <typename T>
    using is_integral = disjunction<is_signed_integer<T>, is_unsigned_integer<T>, is_one_of<T, bool, char, wchar_t>>;

    template <typename T>
    using is_floating_point = is_one_of<float, double, long double>;

    template <typename T>
    static constexpr inline bool is_signed_integer_v = is_signed_integer<T>::value;

    template <typename T>
    static constexpr inline bool is_unsigned_integer_v = is_unsigned_integer<T>::value;

    template <typename T>
    static constexpr inline bool is_integral_v = is_integral<T>::value;

    template <typename T>
    static constexpr inline bool is_floating_point_v = is_floating_point<T>::value;

} // namespace eztl
