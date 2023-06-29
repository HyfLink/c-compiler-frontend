/* 
 *  1. functor
 *    1. plus, minus, multiplies, divides, modulus, negate
 *    2. equal_to, not_equal_to, less, less_equal, greater, greater_equal
 *    3. logical_and, logical_or, logical_not
 *    4. bit_and, bit_or, bit_xor, bit_not
 *    5. identity, project1st, project2nd, select1st, select2nd
 *  2. functions
 *    1. not1 & not2
 *    2. fun_ptr
 *    3. mem_fun & mem_fun_ref
 * 
 */
#pragma once

namespace eztl
{
    template <typename Arg, typename Ret>
    struct unary_function {
        typedef Arg arg_type;
        typedef Ret ret_type;
    };

    template <typename Arg1, typename Arg2, typename Ret>
    struct binary_function {
        typedef Arg1 arg1_type;
        typedef Arg2 arg2_type;
        typedef Ret ret_type;
    };

#define __define_unary_function(a, b, c)                         \
    template <typename T> struct a : public unary_function<T, b> \
    { constexpr inline T operator()(const T &x) { return (c x); } }
#define __define_binary_function(a, b, c)                            \
    template <typename T> struct a : public binary_function<T, T, b> \
    { constexpr inline operator()(const T &x, const T &y) { return (x c y); } }

    __define_unary_function(identity,    T,     );
    __define_unary_function(negate,      T,    -);
    __define_unary_function(logical_not, bool, !);
    __define_unary_function(bit_not,     T,    ~);

    __define_binary_function(plus,          T,    +);
    __define_binary_function(minus,         T,    -);
    __define_binary_function(multiplies,    T,    *);
    __define_binary_function(divides,       T,    /);
    __define_binary_function(modulus,       T,    %);
    __define_binary_function(equal_to,      bool, ==);
    __define_binary_function(not_equal_to,  bool, !=);
    __define_binary_function(less,          bool, <);
    __define_binary_function(less_equal,    bool, <=);
    __define_binary_function(greater,       bool, >);
    __define_binary_function(greater_equal, bool, >=);
    __define_binary_function(logical_and,   bool, &&);
    __define_binary_function(logical_or,    bool, ||);
    __define_binary_function(bit_and,       T,    &);
    __define_binary_function(bit_or,        T,    |);
    __define_binary_function(bit_xor,       T,    ^);

#undef __define_unary_function
#undef __define_binary_function

    template <typename Arg1, typename Arg2>
    struct project1st : public binary_function<Arg1, Arg2, Arg1>
    { constexpr inline Arg1 operator()(const Arg1 &x, const Arg2 &) const { return x; }};

    template <typename Arg1, typename Arg2>
    struct project2nd : public binary_function<Arg1, Arg2, Arg2>
    { constexpr inline Arg2 operator()(const Arg1 &, const Arg2 &y) const { return y; }};

    template <typename Pair>
    struct select1st : public unary_function<Pair, typename Pair::first_type>
    { constexpr inline typename Pair::first_type operator()(const Pair& x) { return x.first; }};

    template <typename Pair>
    struct select2nd : public unary_function<Pair, typename Pair::second_type>
    { constexpr inline typename Pair::second_type operator()(const Pair& x) { return x.second; }};

    template <typename Predicate>
    class __unary_negate : public unary_function<typename Predicate::arg_type, bool>
    {
    public:
        constexpr explicit __unary_negate(const Predicate &pred) : m_pred(pred) {}
        constexpr bool operator()(const typename Predicate::arg_type &x) const { return !m_pred(x); }

    private:
        Predicate m_pred;
    };

    template <typename Predicate>
    class __binary_negate : public binary_function<typename Predicate::arg1_type, typename Predicate::arg2_type, bool>
    {
    public:
        constexpr explicit __binary_negate(const Predicate &pred) : m_pred(pred) {}
        constexpr bool operator()(const typename Predicate::arg1_type &x, const typename Predicate::arg2_type &y) const { return !m_pred(x, y); }

    private:
        Predicate m_pred;
    };

    template <typename Predicate>
    constexpr inline auto not1(const Predicate& pred) { return __unary_negate<Predicate>(pred); }

    template <typename Predicate>
    constexpr inline auto not2(const Predicate& pred) { return __binary_negate<Predicate>(pred); }

    template <typename Arg, typename Ret>
    class __unary_function_pointer : public unary_function<Arg, Ret>
    {
    public:
        constexpr explicit __unary_function_pointer(Ret (*fnptr)(Arg)) : m_fnptr(fnptr) {}
        constexpr Ret operator()(const Arg &x) const { return m_fnptr(x); }

    private:
        Ret (*m_fnptr)(Arg);
    };

    template <typename Arg1, typename Arg2, typename Ret>
    class __binary_function_pointer : public binary_function<Arg1, Arg2, Ret>
    {
    public:
        constexpr explicit __binary_function_pointer(Ret (*fnptr)(Arg1, Arg2)) : m_fnptr(fnptr) {}
        constexpr Ret operator()(const Arg1 &x, const Arg2 &y) const { return m_fnptr(x, y); }

    private:
        Ret (*m_fnptr)(Arg1, Arg2);
    };

    template <typename Arg, typename Ret>
    constexpr inline auto fun_ptr(Ret (*fnptr)(Arg)) { return __unary_function_pointer<Arg, Ret>(fnptr); }

    template <typename Arg1, typename Arg2, typename Ret>
    constexpr inline auto fun_ptr(Ret (*fnptr)(Arg1, Arg2)) { return __binary_function_pointer<Arg1, Arg2, Ret>(fnptr); }

    template <typename Ret, typename T>
    struct __mem_fun_t : public unary_function<T *, Ret>
    {
        explicit __mem_fun_t(Ret (Ret::*fnptr)()) : m_fnptr(fnptr) {}
        inline Ret operator()(T *object) const { return (object->*m_fnptr)(); }

    private:
        Ret (T::*m_fnptr)();
    };

    template <typename Ret, typename T>
    struct __const_mem_fun_t : public unary_function<const T *, Ret>
    {
        explicit __const_mem_fun_t(Ret (Ret::*fnptr)() const) : m_fnptr(fnptr) {}
        inline Ret operator()(const T *object) const { return (object->*m_fnptr)(); }

    private:
        Ret (T::*m_fnptr)() const;
    };

    template <typename Ret, typename T>
    struct __mem_fun_ref_t : public unary_function<T, Ret>
    {
        explicit __mem_fun_ref_t(Ret (Ret::*fnptr)()) : m_fnptr(fnptr) {}
        inline Ret operator()(T &object) const { return (object.*m_fnptr)(); }

    private:
        Ret (T::*m_fnptr)();
    };

    template <typename Ret, typename T>
    struct __const_mem_fun_ref_t : public unary_function<T, Ret>
    {
        explicit __const_mem_fun_ref_t(Ret (Ret::*fnptr)() const) : m_fnptr(fnptr) {}
        inline Ret operator()(const T &object) const { return (object.*m_fnptr)(); }

    private:
        Ret (T::*m_fnptr)() const;
    };

    template <typename Ret, typename T, typename Arg>
    class __mem_fun1_t : public binary_function<T *, Arg, Ret> {
    public:
        explicit __mem_fun1_t(Ret(T::*fnptr)(Arg)) : m_fnptr(fnptr) {}
        Ret operator()(T *object, const Arg& x) const { return (object->*m_fnptr)(x); }

    private:
        Ret (T::*m_fnptr)(Arg);
    };

    template <typename Ret, typename T, typename Arg>
    class __const_mem_fun1_t : public binary_function<const T *, Arg, Ret> {
    public:
        explicit __const_mem_fun1_t(Ret (T::*fnptr)(Arg) const) : m_fnptr(fnptr) {}
        Ret operator()(const T *object, const Arg& x) const { return (object->*m_fnptr)(x); }

    private:
        Ret (T::*m_fnptr)(Arg) const;
    };

    template <typename Ret, typename T, typename Arg>
    class __mem_fun1_ref_t : public binary_function<T, Arg, Ret> {
    public:
        explicit __mem_fun1_ref_t(Ret (T::*fnptr)(Arg)) : m_fnptr(fnptr) {}
        Ret operator()(T &object, const Arg& x) const { return (object.*m_fnptr)(x); }

    private:
        Ret (T::*m_fnptr)(Arg);
    };

    template <typename Ret, typename T, typename Arg>
    class __const_mem_fun1_ref_t : public binary_function<T, Arg, Ret> {
    public:
        explicit __const_mem_fun1_ref_t(Ret (T::*fnptr)(Arg) const) : m_fnptr(fnptr) {}
        Ret operator()(const T &object, const Arg& x) const { return (object.*m_fnptr)(x); }

    private:
        Ret (T::*m_fnptr)(Arg) const;
    };

    template <typename Ret, typename T>
    inline auto mem_fun(Ret (T::*fnptr)()) { return __mem_fun_t<Ret, T>(fnptr); }

    template <typename Ret, typename T>
    inline auto mem_fun(Ret (T::*fnptr)() const) { return __const_mem_fun_t<Ret, T>(fnptr); }

    template <typename Ret, typename T, typename Arg>
    inline auto mem_fun(Ret (T::*fnptr)(Arg)) { return __mem_fun1_t<Ret, T, Arg>(fnptr); }

    template <typename Ret, typename T, typename Arg>
    inline auto mem_fun(Ret (T::*fnptr)(Arg) const) { return __const_mem_fun1_t<Ret, T, Arg>(fnptr); }

    template <typename Ret, typename T>
    inline auto mem_fun_ref(Ret (T::*fnptr)()) { return __mem_fun_ref_t<Ret, T>(fnptr); }

    template <typename Ret, typename T>
    inline auto mem_fun_ref(Ret (T::*fnptr)() const) { return __const_mem_fun_ref_t<Ret, T>(fnptr); }

    template <typename Ret, typename T, typename Arg>
    inline auto mem_fun_ref(Ret (T::*fnptr)(Arg)) { return __mem_fun1_ref_t<Ret, T, Arg>(fnptr); }

    template <typename Ret, typename T, typename Arg>
    inline auto mem_fun_ref(Ret (T::*fnptr)(Arg) const) { return __const_mem_fun1_ref_t<Ret, T, Arg>(fnptr); }

} // namespace eztl
