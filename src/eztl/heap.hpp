/*
 *  1.  push_heap   (first, last[, comp])
 *  2.  pop_heap    (first, last[, comp])
 *  3.  make_heap   (first, last[, comp])
 *  4.  sort_heap   (first, last[, comp])
 */
#pragma once

#include "iterator.hpp"
#include "function.hpp"

namespace eztl
{
    //* heap_shift_up
    
    template <typename RaIter, typename Distance, typename T, typename Compare = less<T>>
    void heap_shift_up(RaIter first, Distance holeIndex, Distance topIndex, T value, Compare comp = Compare())
    {
        Distance parent = (holeIndex - 1) / 2;
        while (holeIndex > topIndex && comp(*(first + parent), value))
        {
            *(first + holeIndex) = *(first + parent);
            holeIndex = parent;
            parent = (holeIndex - 1) / 2;
        }
        *(first + holeIndex) = value;
    }

    //* heap_shift_down

    template <typename RaIter, typename Distance, typename T, typename Compare = less<T>>
    void heap_shift_down(RaIter first, Distance holeIndex, Distance len, T value, Compare comp = Compare())
    {
        Distance topIndex = holeIndex;
        Distance child = 2 * holeIndex + 2;
        while (child < len)
        {
            if (comp(*(first + child), *(first + (child - 1))))
                child--;
            *(first + holeIndex) = *(first + child);
            holeIndex = child;
            child = 2 * (child + 1);
        }
        if (child == len)
        {
            *(first + holeIndex) = *(first + (child - 1));
            holeIndex = child - 1;
        }
        heap_shift_up(first, holeIndex, topIndex, value, comp);
    }

    //* push_heap

    template <typename RaIter, typename Compare = less<iterator_value_t<RaIter>>>
    inline void push_heap(RaIter first, RaIter last, Compare comp = Compare())
    {
        typedef iterator_distance_t<RaIter> distance_t;
        heap_shift_up(first, distance_t((last - first) - 1), distance_t(0), *(last - 1), comp);
    }

    //* pop_heap

    template <typename RaIter, typename Compare = less<iterator_value_t<RaIter>>>
    inline void pop_heap(RaIter first, RaIter last, Compare comp = Compare())
    {
        typedef iterator_distance_t<RaIter> distance_t;
        auto value = *--last;
        *last = *first;
        heap_shift_down(first, distance_t(0), distance_t(last - first), value, comp);
    }

    //* make_heap

    template <typename RaIter, typename Compare = less<iterator_value_t<RaIter>>>
    void make_heap(RaIter first, RaIter last, Compare comp = Compare())
    {
        typedef iterator_distance_t<RaIter> distance_t;
        if (last - first < 2) return;
        distance_t len = last - first;
        distance_t parent = (len - 2) / 2;
        do {
            heap_shift_down(first, parent, len, *(first + parent), comp);
        } while (parent-- != 0);
    }

    //* sort_heap

    template <typename RaIter, typename Compare = less<iterator_value_t<RaIter>>>
    void sort_heap(RaIter first, RaIter last, Compare comp = Compare())
    {
        while (last - first > 1)
            pop_heap(first, last--, comp);
    }


} // namespace eztl
