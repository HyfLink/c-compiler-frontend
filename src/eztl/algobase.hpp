/*
 * stl_algobase.h
 *      (1) swap  & iter_swap
 *      (2) min   & max
 *      (3) copy  & move & copy_backward & copy_n
 *      (4) fill  & fill_n 
 *      (5) equal & mismatch
 *      (6) lexicographical_compare
 */
#pragma once

#include <cstring>
#include <cstdlib>
#include <climits>
#include "iterator.hpp"
#include "utilities.hpp"

namespace eztl
{
    //* swap & iter_swap

    template <typename T>
    inline void swap(T &a, T &b)
    {
        T c = move(a);
        a = move(b);
        b = move(c);
    }

    template <typename Iter1, typename Iter2>
    inline void iter_swap(Iter1 a, Iter2 b)
    {
        iterator_value_t<Iter1> c = move(*a);
        *a = move(*b);
        *b = move(c);
    }

    //* min & max
    template <typename T>
    constexpr inline const T &min(const T &a, const T &b) { return a < b ? a : b; }
    template <typename T, typename CompareLess>
    constexpr inline const T &min(const T &a, const T &b, CompareLess comp) { return comp(a, b) ? a : b; }
    
    template <typename T>
    constexpr inline const T &max(const T &a, const T &b) { return b < a ? a : b; }
    template <typename T, typename CompareLess>
    constexpr inline const T &max(const T &a, const T &b, CompareLess comp) { return comp(b, a) ? a : b; }

    //* copy

    template <typename InIter, typename OuIter, typename BoolType>
    inline OuIter __copy(InIter first, InIter last, OuIter result, BoolType)
    {
        for (; first != last; ++result, ++first)
            *result = *first;
        return result;
    }
    template <typename T>
    inline T *__copy(T *first, T *last, T *result, true_type)
    {
        memmove(result, first, sizeof(T) * (last - first));
        return result + (last - first);
    }
    template <typename InIter, typename OuIter>
    inline OuIter copy(InIter first, InIter last, OuIter result)
    {
        typedef typename type_traits<iterator_value_t<InIter>>::has_trivial_assignment_operator is_trivial_t;
        return __copy(first, last, result, is_trivial_t());
    }

    //* move

    template <typename InIter, typename OuIter, typename BoolType>
    inline OuIter __move(InIter first, InIter last, OuIter result, BoolType)
    {
        for (; first != last; ++result, ++first)
            *result = move(*first);
        return result;
    }
    template <typename T>
    inline T *__move(T *first, T *last, T *result, true_type)
    {
        memmove(result, first, sizeof(T) * (last - first));
        return result + (last - first);
    }
    template <typename InIter, typename OuIter>
    OuIter move(InIter first, InIter last, OuIter result)
    {
        typedef typename type_traits<iterator_value_t<InIter>>::has_trivial_assignment_operator is_trivial_t;
        return __move(first, last, result, is_trivial_t());
    }

    //* copy_backward
    
    template <typename InIter, typename OuIter, typename BoolType>
    OuIter __copy_backward(InIter first, InIter last, OuIter result, BoolType)
    {
        while (first != last)
            *--result = *--last;
        return result;
    }
    template <typename T>
    inline T *__copy_backward(T *first, T *last, T *result, true_type)
    {
        const ptrdiff_t nitems = last - first;
        memmove(result - nitems, first, sizeof(T) * nitems);
        return result - nitems;
    }
    template <typename InIter, typename OuIter>
    OuIter copy_backward(InIter first, InIter last, OuIter result)
    {
        typedef typename type_traits<iterator_value_t<InIter>>::has_trivial_assignment_operator is_trivial_t;
        return __copy_backward(first, last, result, is_trivial_t());
    }

    //* copy_n

    template <typename InIter, typename OuIter, typename Size, typename BoolType>
    pair<InIter, OuIter> __copy_n(InIter first, Size nitems, OuIter result, BoolType)
    {
        for (; nitems > 0; --nitems, ++first, ++result)
            *result = *first;
        return pair<InIter, OuIter>(first, result);
    }
    template <typename T, typename Size>
    inline pair<T *, T*> __copy_n(T *first, Size nitems, T *result, true_type)
    {
        memmove(result, first, sizeof(T) * nitems);
        return pair<T *, T *>(first + nitems, result + nitems);
    }
    template <typename InIter, typename OuIter, typename Size>
    pair<InIter, OuIter> copy_n(InIter first, Size nitems, OuIter result)
    {
        typedef typename type_traits<iterator_value_t<InIter>>::has_trivial_assignment_operator is_trivial_t;
        return __copy_n(first, nitems, result, is_trivial_t());
    }

    //* fill

    template <typename FrIter, typename T>
    void fill(FrIter first, FrIter last, const T &val)
    {
        for (; first != last; ++first)
            *first = val;
    }
    inline void fill(char *first, char *last, char val)
    {
        memset(first, static_cast<unsigned char>(val), last - first);
    }
    inline void fill(signed char *first, signed char *last, signed char val)
    {
        memset(first, static_cast<unsigned char>(val), last - first);
    }
    inline void fill(unsigned char *first, unsigned char *last, unsigned char val)
    {
        memset(first, static_cast<unsigned char>(val), last - first);
    }

    //* fill_n

    template <typename OuIter, typename Size, typename T>
    OuIter fill_n(OuIter first, Size nitems, const T &val)
    {
        for (; nitems > 0; --nitems, ++first)
            *first = val;
        return first;
    }
    inline char *fill_n(char *first, size_t nitems, char val)
    {
        memset(first, static_cast<unsigned char>(val), nitems);
        return first + nitems;
    }
    inline signed char *fill_n(signed char *first, size_t nitems, signed char val)
    {
        memset(first, static_cast<unsigned char>(val), nitems);
        return first + nitems;
    }
    inline unsigned char *fill_n(unsigned char *first, size_t nitems, unsigned char val)
    {
        memset(first, static_cast<unsigned char>(val), nitems);
        return first + nitems;
    }

    //* equal

    template <typename InIter1, typename InIter2>
    bool equal(InIter1 first1, InIter1 last1, InIter2 first2)
    {
        for (; first1 != last1; ++first1, ++first2)
            if (*first1 != *first2) return false;
        return true;
    }
    template <typename InIter1, typename InIter2, typename Predicate>
    bool equal(InIter1 first1, InIter1 last1, InIter2 first2, Predicate pred)
    {
        for (; first1 != last1; ++first1, ++first2)
            if (pred(*first1, *first2)) return false;
        return true;
    }

    //* mismatch

    template <typename InIter1, typename InIter2>
    pair<InIter1, InIter2> mismatch(InIter1 first1, InIter2 last1, InIter2 first2)
    {
        while (first1 != last1 && *first1 == *first2)
            ++first1, ++first2;
        return pair<InIter1, InIter2>(first1, first2);
    }
    template <typename InIter1, typename InIter2, typename PredicateEqual>
    pair<InIter1, InIter2> mismatch(InIter1 first1, InIter2 last1, InIter2 first2, PredicateEqual pred)
    {
        while (first1 != last1 && pred(*first1, *first1))
            ++first1, ++first2;
        return pair<InIter1, InIter2>(first1, first2);
    }

    //* lexicographical_compare

    template <typename InIter1, typename InIter2>
    bool lexicographical_compare(InIter1 first1, InIter1 last1, InIter2 first2, InIter2 last2)
    {
        for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
            if (*first1 < *first2) return true;
            if (*first2 < *first1) return false;
        }
        return first1 == last1 && first2 != last2;
    }
    template <typename InIter1, typename InIter2, typename CompareLess>
    bool lexicographical_compare(InIter1 first1, InIter1 last1, InIter2 first2, InIter2 last2, CompareLess comp)
    {
        for (; first1 != last1 && first2 != last2; ++first1, ++first2)
        {
            if (comp(*first1, *first2)) return true;
            if (comp(*first2, *first1)) return false;
        }
        return first1 == last1 && first2 != last2;
    }

    inline bool lexicographical_compare(
        const unsigned char *first1, const unsigned char *last1,
        const unsigned char *first2, const unsigned char *last2)
    {
        const size_t len1 = last1 - first1;
        const size_t len2 = last2 - first2;
        const int result = memcmp(first1, first2, min(len1, len2));
        return result != 0 ? result < 0 : len1 < len2;
    }
    inline bool lexicographical_compare(
        const signed char *first1, const signed char *last1,
        const signed char *first2, const signed char *last2)
    {
        const size_t len1 = last1 - first1;
        const size_t len2 = last2 - first2;
        const int result = memcmp(first1, first2, min(len1, len2));
        return result != 0 ? result < 0 : len1 < len2;
    }
    inline bool lexicographical_compare(
        const char *first1, const char *last1,
        const char *first2, const char *last2)
    {
        const size_t len1 = last1 - first1;
        const size_t len2 = last2 - first2;
        const int result = memcmp(first1, first2, min(len1, len2));
        return result != 0 ? result < 0 : len1 < len2;
    }

} // namespace eztl
