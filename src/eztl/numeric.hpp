/*
 *  1.  accumulate(first, last, init[, op])
 *  2.  adjacent_difference(first, last, result[, op])
 *  3.  inner_product(first1, last1, first2, init[, op])
 *  4.  partial_sum(first, last, result[, op])
 TODO   power & iota
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 */
#pragma once

#include "iterator.hpp"
#include "function.hpp"

namespace eztl
{
    //* accumulate

    template <typename InIter, typename T, typename Operation = plus<T>>
    T accumulate(InIter first, InIter last, T init, Operation op = Operation())
    {
        for (; first != last; ++first)
            init = op(init, *first);
        return init;
    }

    //* adjacent_difference

    template <typename InIter, typename OuIter, typename Operation = minus<iterator_value_t<InIter>>>
    OuIter adjacent_difference(InIter first, InIter last, OuIter result, Operation op = Operation())
    {
        typedef iterator_value_t<InIter> T;

        if (first == last) return result;

        T value = *first;
        *result = value;

        while (++first != last) {
            T tmp = *first;
            *++result = op(tmp, value);
            value = tmp;
        }
     
        return ++result;
    }

    //* inner_product

    template <typename InIter1, typename InIter2, typename T, typename Operation = multiplies<T>>
    T inner_product(InIter1 first1, InIter1 last1, InIter2 first2, T init, Operation op = Operation())
    {
        for (; first1 != last1; ++first1, ++first2)
            init = init + op(*first1, *first2);
        return init;
    }

    //* partial_sum

    template <typename InIter, typename OuIter, typename Operation = plus<iterator_value_t<InIter>>>
    OuIter parital_sum(InIter first, InIter last, OuIter result, Operation op = Operation())
    {
        typedef iterator_value_t<InIter> T;
        if (first == last) return result;

        T value = *first;
        *result = value;

        while (++first != last) {
            value = op(value, *first);
            *++result = value;
        }

        return ++result;
    }

    //* power
    //* iota

} // namespace eztl
