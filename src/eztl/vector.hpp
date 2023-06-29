#pragma once

#include <cassert>

#include "allocator.hpp"
#include "construct.hpp"
#include "uninitialized.hpp"
#include "iterator.hpp"
#include "algobase.hpp"

namespace eztl
{
    template <typename T, typename Alloc = allocator<T>>
    class vector
    {
    public: /* typedefs */
        typedef T value_type;
        typedef T *pointer;
        typedef T &reference;
        typedef const T *const_pointer;
        typedef const T &const_reference;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef Alloc data_allocator;

        typedef pointer iterator;
        typedef const_pointer const_iterator;
        typedef eztl::reverse_iterator<iterator> reverse_iterator;
        typedef eztl::reverse_iterator<const_iterator> const_reverse_iterator;

    public: /* ctor & dtor */
        explicit constexpr inline vector();

        explicit inline vector(const vector &other);
        explicit inline vector(vector &&other);

        template <typename InIter, typename = enable_if_t<is_iterator<InIter>>>
        explicit inline vector(InIter first, InIter last);
        explicit inline vector(size_type nitems, const_reference val = T());

        inline ~vector();

    public: /* methods */
        // data
        inline pointer data();
        inline const_pointer data() const;

        // front & back
        inline reference front();
        inline const_reference front() const;
        inline reference back();
        inline const_reference back() const;

        // iterator
        inline iterator begin();
        inline const_iterator begin() const;
        inline iterator end();
        inline const_iterator end() const;

        inline reverse_iterator rbegin();
        inline const_reverse_iterator rbegin() const;
        inline reverse_iterator rend();
        inline const_reverse_iterator rend() const;

        inline const_iterator cbegin() const;
        inline const_iterator cend() const;
        inline const_reverse_iterator crbegin() const;
        inline const_reverse_iterator crend() const;

        inline size_type size() const;
        inline size_type capacity() const;
        inline bool empty() const;
        inline void clear();
        inline void swap(vector &other);

        reference at(size_type idx);
        const_reference at(size_type idx) const;

        void resize(size_type nitems, const_reference val = T());
        void reserve(size_type nitems);

        template <typename InIter, typename = enable_if_t<is_iterator<InIter>>>
        void assign(InIter first, InIter last);
        void assign(size_type nitems, const_reference val = T());

        void push_back(const_reference val);
        void pop_back();

        template <typename... Args>
        reference emplace_back(Args &&...args);

        template <typename... Args>
        iterator emplace(iterator pos, Args &&...args);

        template <typename InIter, typename = enable_if_t<is_iterator<InIter>>>
        iterator insert(iterator pos, InIter first, InIter last);
        iterator insert(iterator pos, size_type nitems, const_reference val);
        iterator insert(iterator pos, const_reference val = T());

        iterator erase(iterator pos);
        iterator erase(iterator first, iterator last);

        vector &operator=(const vector &rhs);
        vector &operator=(vector &&rhs);

        inline reference operator[](size_type idx);
        inline const_reference operator[](size_type idx) const;

    private: /* implementations */
        inline void __earse_to_end(pointer ptr);

    private: /* members */
        pointer m_first;
        pointer m_last;
        pointer m_finish;
    };

} // namespace eztl

#include "vector.inl"
