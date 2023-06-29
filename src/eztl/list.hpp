#pragma once

#include <cassert>

#include "allocator.hpp"
#include "construct.hpp"
#include "uninitialized.hpp"
#include "iterator.hpp"
#include "algobase.hpp"

namespace eztl
{
    struct list_node_base
    {
        // 2 pointers
        list_node_base *m_prev;
        list_node_base *m_next;

        // constructors
        explicit constexpr list_node_base()
            : m_prev(nullptr), m_next(nullptr) {}

        explicit constexpr list_node_base(list_node_base *prev, list_node_base *next)
            : m_prev(prev), m_next(next) {}
    };

    struct list_node_header : public list_node_base
    {
        typedef size_t size_type;

        // size of the list nodes
        size_type m_size;

        // constructors
        explicit list_node_header() { init(); }

        explicit list_node_header(list_node_header &&other)
            : list_node_base(other.m_prev, other.m_next), m_size(other.m_size) { other.init(); }

        // other methods
        inline list_node_base *base() { return this; }
        inline void init() { m_size = 0; m_prev = m_next = this; }
    };

    template <typename T>
    struct list_node : public list_node_base
    {
        typedef T   value_type;
        typedef T * pointer;

        // memory of T
        unsigned char m_storage[sizeof(T)];

        // get pointer
        pointer valptr() { return reinterpret_cast<pointer>(m_storage); }
    };

    template <typename T>
    struct list_iterator : public bidirectional_iterator<T>
    {
        constexpr explicit list_iterator() : m_node(nullptr) {}
        constexpr explicit list_iterator(list_node<value_type> *ptr) : m_node(ptr) {}

        bool operator==(list_iterator rhs) { return m_node == rhs.m_node; }
        bool operator!=(list_iterator rhs) { return m_node != rhs.m_node; }
        list_iterator &operator++() { m_node = m_node->m_next; return *this; }
        list_iterator &operator--() { m_node = m_node->m_prev; return *this; }
        list_iterator operator++(int) { auto tmp = *this; m_node = m_node->m_next; return tmp; }
        list_iterator operator--(int) { auto tmp = *this; m_node = m_node->m_prev; return tmp; }

        list_node<value_type> *m_node;
    };

    template <typename T, typename Alloc = allocator<T>>
    class list
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

        typedef list_iterator<T> iterator;
        typedef list_iterator<T> const_iterator;
        typedef eztl::reverse_iterator<iterator> reverse_iterator;
        typedef eztl::reverse_iterator<const_iterator> const_reverse_iterator;

    private:
    };

} // namespace eztl

#include "list.inl"