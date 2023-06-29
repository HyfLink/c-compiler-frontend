/*
 *  (1) pair structure
 *  (2) pair comparison
 *  (3) pair functions
 *      i.  make_pair(pair)
 *      ii. get<i>(pair)
 */
#pragma once
#include <cstddef>

namespace eztl
{
    //* pair structure

    template <typename T1, typename T2>
    struct pair {
        typedef T1 first_type;
        typedef T2 second_type;

        template <typename U1, typename U2>
        pair(const pair<U1, U2> &p) : first(p.first), second(p.second) {}
        pair(const T1 &a, const T2 &b) : first(a), second(b) {}
        pair() : first(T1()), second(T2()) {}

        T1 first;
        T2 second;
    };

    //* pair comparison

    template <typename T1, typename T2>
    inline bool operator==(const pair<T1, T2> &x, const pair<T1, T2> &y) {
        return x.first == y.first && x.second == y.second;
    }

    template <typename T1, typename T2>
    inline bool operator<(const pair<T1, T2> &x, const pair<T1, T2> &y) {
        return x.first < y.first || (!(y.first < x.first) && x.second < y.second);
    }

    template <typename T1, typename T2>
    inline bool operator>(const pair<T1, T2> &x, const pair<T1, T2> &y) { return y < x; }

    template <typename T1, typename T2>
    inline bool operator!=(const pair<T1, T2> &x, const pair<T1, T2> &y) { return !(y == x); }

    template <typename T1, typename T2>
    inline bool operator<=(const pair<T1, T2> &x, const pair<T1, T2> &y) { return !(y < x); }

    template <typename T1, typename T2>
    inline bool operator>=(const pair<T1, T2> &x, const pair<T1, T2> &y) { return !(x < y); }

    //* pair functions : make_pair

    template <typename T1, typename T2>
    inline pair<T1, T2> make_pair(const T1& a, const T2 &b) {
        return pair<T1, T2>(a, b);
    }

    //* pair functions : get

    template <size_t Idx, typename T> struct __get;

    template <typename T1, typename T2>
    struct __get<0, pair<T1, T2>> {
        const T1& operator()(const pair<T1, T2> &p) { return p.first; }
    };

    template <typename T1, typename T2>
    struct __get<1, pair<T1, T2>> {
        inline const T2& operator()(const pair<T1, T2> &p) { return p.second; }
    };

    template <size_t Idx, typename T1, typename T2>
    inline auto get(const pair<T1, T2>& p) {
        return __get<Idx, pair<T1, T2>>()(p);
    }

} // namespace eztl
