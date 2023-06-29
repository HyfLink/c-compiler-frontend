/*
 *  1.  median          (a, b, c[, comp])           -> typeof a
 *  2.  for_each        (first, last, func)         -> func
 *  3.  find            (first, last, val)          -> iterator
 *      find_if         (first, last, pred)         -> iterator
 *      adjacent_find   (first, last[, pred])       -> iterator
 *  4.  count           (first, last, val)          -> size
 *                      (first, last, val, &size)
 *      count_if        (first, last, pred)         -> size
 *                      (first, last, pred, &size)
 TODO algorithm
 *  5.  search
 *      search_n
 *  6.  swap_ranges
 *  7.  transform
 *  8.  replace
 *      replace_if
 *      replace_copy
 *      replace_copy_if
 *  9.  generate
 *      generate_n
 *  10. remove          (first, last, val)
 *      remove_if       (first, last, pred)
 *      remove_copy     (first, last, result, val)
 *      remove_copy_if  (first, last, result, pred)
 *  11. unique
 *      unique_copy
 *  12. reverse
 *      reverse_copy
 *  13. rotate
 *      rotate_copy
 *  14. random_shuffle
 *  15. random_sample
 *      random_sample_n
 *  16. partition       (first, last, pred)
 *      stable_partition
 *  17. sort
 *      stable_sort
 *  18. partial_sort
 *      partial_sort_copy
 *  19. n_th_element
 *  20. lower_bound
 *      upper_bound
 *      equal_range
 *      binary_search
 *  21. merge           (first1, last1, first2, last2, result[, comp])
 *      inplace_merge
 *  22. includes        (first1, last1, first2, last2[, comp])
 *  23. set_union
 *      set_intersection
 *      set_difference
 *      set_symmetric_difference
 *  24. max_element     (first, last[, comp])
 *      min_element     (first, last[, comp])
 *  25. next_permutation
 *      prev_permutation
 *  26. find_first_of   (first1, last1, first2, last2[, comp])
 *      find_end        (first1, last1, first2, last2)
 *  27. is_heap
 *      is_sorted
 */
#pragma once

#include "algobase.hpp"
#include "iterator.hpp"
#include "function.hpp"

namespace eztl
{
    //* median

    template <typename T, typename Compare = less<T>>
    inline const T &median(const T &a, const T &b, const T &c, Compare comp = Compare())
    {
        return comp(a, b) ? (comp(b, c) ? b : (comp(a, c) ? c : a)) : (comp(a, c) ? a : (comp(b, c) ? c : a));
    }

    //* for_each

    template <typename InIter, typename UnFunction>
    UnFunction for_each(InIter first, InIter last, UnFunction func)
    {
        for (; first != last; ++first)
            func(*first);
        return func;
    }

    //* find

    template <typename InIter, typename T>
    InIter __find(InIter first, InIter last, const T &val, input_iterator_tag)
    {
        while (first != last && !(*first == val))
            ++first;
        return first;
    }

    template <typename RaIter, typename T>
    RaIter __find(RaIter first, RaIter last, const T &val, random_access_iterator_tag)
    {
        iterator_distance_t<RaIter> count = (last - first) >> 2;
        for (; count > 0; --count) {
            if (*first == val) return first; ++first;
            if (*first == val) return first; ++first;
            if (*first == val) return first; ++first;
            if (*first == val) return first; ++first;
        }
        switch (last - first) {
        case 3: if (*first == val) return first; ++first;
        case 2: if (*first == val) return first; ++first;
        case 1: if (*first == val) return first; ++first;
        case 0: return last;
        }
    }

    template <typename InIter, typename T>
    inline InIter find(InIter first, InIter last, const T &val)
    {
        return __find(first, last, val, iterator_category_t<InIter>());
    }

    //* find_if

    template <typename InIter, typename Predicate>
    InIter __find_if(InIter first, InIter last, Predicate pred, input_iterator_tag)
    {
        while (first != last && !pred(*first))
            ++first;
        return first;
    }

    template <typename RaIter, typename Predicate>
    RaIter __find_if(RaIter first, RaIter last, Predicate pred, random_access_iterator_tag)
    {
        iterator_distance_t<RaIter> count = (last - first) >> 2;
        for (; count > 0; --count) {
            if (pred(*first)) return first; ++first;
            if (pred(*first)) return first; ++first;
            if (pred(*first)) return first; ++first;
            if (pred(*first)) return first; ++first;
        }
        switch (last - first) {
        case 3: if (pred(*first)) return first; ++first;
        case 2: if (pred(*first)) return first; ++first;
        case 1: if (pred(*first)) return first; ++first;
        case 0: return last;
        }
    }

    template <typename InIter, typename Predicate>
    inline InIter find_if(InIter first, InIter last, Predicate pred)
    {
        return __find_if(first, last, pred, iterator_category_t<InIter>());
    }

    //* adjacent_find

    template <typename FrIter, typename Compare = equal_to<iterator_value_t<FrIter>>>
    FrIter adjacent_find(FrIter first, FrIter last, Compare pred = Compare())
    {
        if (first == last) return last;
        for (FrIter next = first; ++next != last; first = next)
            if (pred(*first, *next)) return first;
        return last;
    }

    //* count[_if]

    template <typename InIter, typename T>
    iterator_distance_t<InIter> count(InIter first, InIter last, const T &value)
    {
        iterator_distance_t<InIter> result = 0;
        for (; first != last; ++first)
            if (*first == value) ++result;
        return result;
    }

    template <typename InIter, typename Predicate>
    iterator_distance_t<InIter> count_if(InIter first, InIter last, Predicate pred)
    {
        iterator_distance_t<InIter> result = 0;
        for (; first != last; ++first)
            if (pred(*first)) ++result;
        return result;
    }

    //* search
    //* search_n
    //* swap_ranges
    //* transform
    //* replace
    //* replace_if
    //* replace_copy
    //* replace_copy_if

    //* generate[_n]

    template <typename FrIter, typename Generator>
    void generate(FrIter first, FrIter last, Generator gen)
    {
        for (; first != last; ++first)
            *first = gen();
    }

    template <typename FrIter, typename Size, typename Generator>
    FrIter generate(FrIter first, Size n, Generator gen)
    {
        for (; n > 0; --n, ++first)
            *first = gen();
        return first;
    }

    //* remove_copy[_if]
    
    template <typename InIter, typename OuIter, typename T>
    OuIter remove_copy(InIter first, InIter last, OuIter result, const T& val)
    {
        for (; first != last; ++first)
            if (*first != val) {
                *result = *first;
                ++result;
            }
        return result;
    }

    template <typename InIter, typename OuIter, typename Predicate>
    OuIter remove_copy_if(InIter first, InIter last, OuIter result, Predicate pred)
    {
        for (; first != last; ++first)
            if (pred(*first)) {
                *result = *first;
                ++result;
            }
        return result;
    }

    //* remove[_if]

    template <typename FrIter, typename T>
    inline FrIter remove(FrIter first, FrIter last, const T& val)
    {
        first = find(first, last, val);
        FrIter next = first;
        return first == last ? last : remove_copy(++next, last, first, val);
    }

    template <typename FrIter, typename Predicate>
    inline FrIter remove_if(FrIter first, FrIter last, Predicate pred)
    {
        first = find_if(first, last, pred);
        FrIter next = first;
        return first == last ? last : remove_copy_if(++next, last, first, pred);
    }

    //* unique
    //* unique_copy
    //* reverse
    //* reverse_copy
    //* rotate
    //* rotate_copy
    //* random_shuffle
    //* random_sample
    //* random_sample_n
    //* partition

    template <typename BiIter, typename Predicate>
    BiIter partition(BiIter first, BiIter last, Predicate pred)
    {
        for (;;) {
            for (;;)
                if  (first == last) return first;
                else if (pred(*first)) ++first;
                else break;

            --last;
            for (;;)
                if (first == last) return first;
                else if (!pred(*last)) --last;
                else break;
            iter_swap(first, last);
            ++first;
        }
    }

    //* stable_partition
    //* sort
    //* stable_sort
    //* partial_sort
    //* partial_sort_copy
    //* n_th_element
    //* lower_bound
    //* upper_bound
    //* equal_range
    //* binary_search
    //* merge

    template <typename InIter1, typename InIter2, typename OuIter, typename Compare = less<iterator_value_t<InIter1>>>
    OuIter merge(InIter1 first1, InIter1 last1, InIter2 first2, InIter2 last2, OuIter result, Compare comp = Compare())
    {
        for (; first1 != last1 && first2 != last2; ++result)
            *result = comp(*first2, *first1) ? (*first2++) : (*first1++);
        return copy(first2, last2, copy(first1, last1, result));
    }

    //* inplace_merge
    
    //* includes

    template <typename InIter1, typename InIter2, typename Compare = less<iterator_value_t<InIter1>>>
    bool includes(InIter1 first1, InIter1 last1, InIter2 first2, InIter2 last2, Compare comp = Compare())
    {
        while (first1 != last1 && first2 != last2)
            if (comp(*first2, *first1))
                return false;
            else if (comp(*first1, *first2))
                ++first1;
            else
                ++first1, ++first2;
        return first2 == last2;
    }

    //* set_union
    //* set_intersection
    //* set_difference
    //* set_symmetric_difference
    
    //* max/min_element

    template <typename FrIter, typename Compare = less<iterator_value_t<FrIter>>>
    FrIter max_element(FrIter first, FrIter last, Compare comp = Compare())
    {
        if (first == last) return last;
        FrIter result = first;
        while (++first != last)
            if (comp(*result, *first))
                result = first;
        return result;
    }

    template <typename FrIter, typename Compare = less<iterator_value_t<FrIter>>>
    FrIter min_element(FrIter first, FrIter last, Compare comp = Compare())
    {
        if (first == last) return last;
        FrIter result = first;
        while (++first != last)
            if (comp(*first, *result))
                result = first;
        return result;
    }

    //* next_permutation
    //* prev_permutation
    
    //* find_first_of

    template <typename InIter, typename FrIter, typename Compare = equal_to<iterator_value_t<InIter>>>
    InIter find_first_of(InIter first1, InIter last1, FrIter first2, FrIter last2, Compare comp = Compare())
    {
        for (; first1 != last1; ++first1)
            for (FrIter it = first2; it != last2; ++it)
                if (comp(*first1, *it)) return first1;
        return last1;
    }

    //* find_end

    template <typename FrIter1, typename FrIter2>
    FrIter1 __find_end(FrIter1 first1, FrIter1 last1, FrIter2 first2, FrIter2 last2, forward_iterator_tag, forward_iterator_tag)
    {
        FrIter1 result = last1;
        for (FrIter1 next = search(first1, last1, first2, last2); next != last1; ++first1) {
            result = next;
            next = search(first1, last1, first2, last2);
        }

        return result;
    }

    template <typename BiIter1, typename BiIter2>
    BiIter1 __find_end(BiIter1 first1, BiIter1 last1, BiIter2 first2, BiIter2 last2)
    {
        typedef reverse_iterator<BiIter1> RevIter1;
        typedef reverse_iterator<BiIter2> RevIter2;

        RevIter1 rlast1(first1);
        RevIter1 rresult = search(RevIter1(last1), rlast1, RevIter1(last2), RevIter2(first2));

        if (rresult == rlast1) return last1;

        BiIter1 result = rresult.base();
        advance(result, -distance(first2, last2));
        return result;
    }

    template <typename FrIter1, typename FrIter2>
    inline FrIter1 find_end(FrIter1 first1, FrIter1 last1, FrIter2 first2, FrIter2 last2)
    {
        if (first1 == last1 || first2 == last2) return last1;
        return __find_end(first1, last1, first2, last2, iterator_category_t<FrIter1>(), iterator_category_t<FrIter2>());
    }

    //* is_heap
    //* is_sorted
} // namespace eztl
