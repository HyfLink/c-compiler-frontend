/*
 *  (1) uninitialized_copy(first, last, result)
 *      copy [first, last) -> [result, ...)
 * 
 *  (2) uninitialized_fill(first, last, x)
 *      fill x -> [first, last)
 * 
 *  (3) uninitialized_fill_n(first, cnt, x)
 *      fill x -> [first, first + cnt)
 */
#pragma once

#include <cstring>
#include "iterator.hpp"
#include "type_traits.hpp"
#include "construct.hpp"
#include "algobase.hpp"

namespace eztl
{
    //* uninitialized_copy

    template <typename InIter, typename FrIter>
    inline FrIter __uninitialized_copy(InIter first, InIter last, FrIter result, true_type)
    {
        return copy(first, last, result);
    }

    template <typename InIter, typename FrIter>
    inline FrIter __uninitialized_copy(InIter first, InIter last, FrIter result, false_type)
    {
        for (; first != last; ++first, ++result)
            construct(&*result, *first);
        return result;
    }

    template <typename InIter, typename FrIter>
    inline FrIter uninitialized_copy(InIter first, InIter last, FrIter result)
    {
        typedef typename type_traits<iterator_value_t<FrIter>>::is_POD_type is_pod_t;
        return __uninitialized_copy(first, last, result, is_pod_t());
    }

    inline char *uninitialized_copy(char *first, char *last, char *result)
    {
        memmove(result, first, sizeof(char) * (last - first));
        return result + (last - first);
    }

    inline wchar_t *uninitialized_copy(wchar_t *first, wchar_t *last, wchar_t *result)
    {
        memmove(result, first, sizeof(wchar_t) * (last - first));
        return result + (last - first);
    }

    //* uninitialized_fill

    template <typename FrIter, typename T>
    inline void __uninitialized_fill(FrIter first, FrIter last, const T &val, true_type)
    {
        fill(first, last, val);
    }

    template <typename FrIter, typename T>
    inline void __uninitialized_fill(FrIter first, FrIter last, const T &val, false_type)
    {
        for (; first != last; ++first)
            construct(&*first, val);
    }

    template <typename FrIter, typename T>
    inline void uninitialized_fill(FrIter first, FrIter last, const T &val)
    {
        typedef typename type_traits<iterator_value_t<FrIter>>::is_POD_type is_pod_t;
        __uninitialized_fill(first, last, val, is_pod_t());
    }

    //* uninitialized_fill_n

    template <typename FrIter, typename Size, typename T>
    inline FrIter __uninitialized_fill_n(FrIter first, Size cnt, const T &val, true_type)
    {
        return fill_n(first, cnt, val);
    }

    template <typename FrIter, typename Size, typename T>
    FrIter __uninitialized_fill_n(FrIter first, Size cnt, const T &val, false_type)
    {
        for (; cnt > 0; --cnt, ++first)
            construct(&*first, val);
        return first;
    }

    template <typename FrIter, typename Size, typename T>
    inline FrIter uninitialized_fill_n(FrIter first, Size cnt, const T &val)
    {
        typedef typename type_traits<iterator_value_t<FrIter>>::is_POD_type is_pod_t;
        return __uninitialized_fill_n(first, cnt, val, is_pod_t());
    }

} // namespace eztl
