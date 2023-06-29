/*
 *  (1) construct
 *      construct(obj)
 *      construct(obj, args...)
 *  (2) destroy
 *      destroy(obj)
 *      destroy(first, last)
 */
#pragma once

#include <new>              // for placement new
#include "iterator.hpp"
#include "move.hpp"

namespace eztl
{
    //* construct(obj)
    
    template <typename T>
    inline void construct(T * ptr) {
        new (ptr) T();
    }

    //* construct(obj, args...)

    template <typename T, typename... Args>
    inline void construct(T *ptr, Args &&...args) {
        new (ptr) T(forward<Args>(args)...);
    }

    // * destroy(obj)
    
    template <typename T>
    inline void destroy(T * ptr) {
        ptr->~T();
    }

    // * destroy(first, last)

    template <typename FrIter>
    void __destroy(FrIter first, FrIter last, false_type) {
        for (; first != last; ++first)
            destroy(&*first);
    }

    template <typename FrIter>
    inline void __destroy(FrIter first, FrIter last, true_type) {}

    template <typename FrIter>
    inline void destroy(FrIter first, FrIter last) {
        __destroy(first, last, typename type_traits<iterator_value_t<FrIter>>::has_trivial_destructor());
    }

    inline void destroy(char *, char *) {}
    inline void destroy(int *, int *) {}
    inline void destroy(long *, long *) {}
    inline void destroy(float *, float *) {}
    inline void destroy(double *, double *) {}
    inline void destroy(wchar_t *, wchar_t *) {}

} // namespace eztl
