/*
 *  (1) iterator tags :
 *  	    output, input, forward, bidirectional, random_access
 *  
 *  (2) iterator base
 *      i.   iterator
 *  	ii.  output_iterator
 *      iii. input_iterator
 *      iv.  forward_iterator
 *      v.   bidirectional_iterator 
 *      vi.  random_access_iterator
 *  
 *  (3) iterator traits
 *  	i.   iterator_traits<Iter>::...
 *      ii.  iterator_..._t
 *  
 *  (4) functions
 *  	i.   distance(iter1, iter2, dis)
 *  		 distance(iter1, iter2) -> difference_type
 *  	ii.  advance(iter, dis)
 *  
 *  (5) special iterator
 *      i.    back_insert_iterator              : output
 *      ii.   front_insert_iterator             : output
 *      iii.  insert_iterator                   : output
 *      iv.   reverse_iterator                  : bidirectional
 TODO iostream_iterator
 *      v.    istream_iterator
 *      vi.   ostream_iterator
 *      vii.  istreambuf_iterator
 *      xiii. ostreambuf_iterator
 */
#pragma once
#include <cstddef>
#include "type_traits.hpp"

namespace eztl
{
    //* iterator tags

    struct output_iterator_tag {};
    struct input_iterator_tag {};
    struct forward_iterator_tag : public input_iterator_tag {};
    struct bidirectional_iterator_tag : public forward_iterator_tag {};
    struct random_access_iterator_tag : public bidirectional_iterator_tag {};

    //* iterator base
    
    template <typename Category, typename T, typename Distance = ptrdiff_t, typename Pointer = T *, typename Reference = T &>
    struct iterator {
        typedef Category iterator_category;
        typedef T value_type;
        typedef Distance difference_type;
        typedef Pointer pointer;
        typedef Reference reference;
    };

    template <typename Category, typename T, typename Distance, typename Pointer, typename Reference>
    struct iterator<Category, const T, Distance, Pointer, Reference> {
        typedef Category iterator_category;
        typedef T value_type;
        typedef Distance  difference_type;
        typedef const Pointer pointer;
        typedef const Reference reference;
    };

    template <typename T, typename Distance = ptrdiff_t>
    struct output_iterator : public iterator<output_iterator_tag, void, void, void, void> {};
    template <typename T, typename Distance = ptrdiff_t>
    struct input_iterator : public iterator<input_iterator_tag, T, Distance> {};
    template <typename T, typename Distance = ptrdiff_t>
    struct forward_iterator : public iterator<forward_iterator_tag, T, Distance> {};
    template <typename T, typename Distance = ptrdiff_t>
    struct bidirectional_iterator : public iterator<bidirectional_iterator_tag, T, Distance> {};
    template <typename T, typename Distance = ptrdiff_t>
    struct random_access_iterator : public iterator<random_access_iterator_tag, T, Distance> {};

    //* iterator traits

    template <typename Iter, typename = void_t<>>
    struct iterator_traits {
        static constexpr bool is_iterator = false;
    };

    template <typename Iter>
    struct iterator_traits<Iter, void_t<typename Iter::iterator_category>> {
        static constexpr bool is_iterator = true;
        typedef typename Iter::iterator_category iterator_category;
        typedef typename Iter::value_type value_type;
        typedef typename Iter::difference_type difference_type;
        typedef typename Iter::pointer pointer;
        typedef typename Iter::reference reference;
    };
    template <typename T>
    struct iterator_traits<T *> {
        static constexpr bool is_iterator = true;
        typedef random_access_iterator_tag iterator_category;
        typedef T value_type;
        typedef ptrdiff_t difference_type;
        typedef T *pointer;
        typedef T &reference;
    };
    template <typename T>
    struct iterator_traits<const T *> {
        static constexpr bool is_iterator = true;
        typedef random_access_iterator_tag iterator_category;
        typedef T value_type;
        typedef ptrdiff_t difference_type;
        typedef const T *pointer;
        typedef const T &reference;
    };

    template <typename Iter>
    using iterator_category_t = typename iterator_traits<Iter>::iterator_category;
    template <typename Iter>
    using iterator_value_t = typename iterator_traits<Iter>::value_type;
    template <typename Iter>
    using iterator_distance_t = typename iterator_traits<Iter>::difference_type;
    template <typename Iter>
    using iterator_pointer_t = typename iterator_traits<Iter>::pointer;
    template <typename Iter>
    using iterator_reference_t = typename iterator_traits<Iter>::reference;
    template <typename Iter>
    static constexpr bool is_iterator = iterator_traits<Iter>::is_iterator;

    //* iterator functions : distance
    
    template <typename InIter>
    inline iterator_distance_t<InIter> __distance(InIter first, InIter last, input_iterator_tag) {
        iterator_distance_t<InIter> n = 0;
        while (first != last) { ++first; ++n; }
        return n;
    }

    template <typename RaIter>
    inline iterator_distance_t<RaIter> __distance(RaIter first, RaIter last, random_access_iterator_tag) {
        return last - first;
    }

    template <typename InIter>
    inline iterator_distance_t<InIter> distance(InIter first, InIter last) {
        return __distance(first, last, iterator_category_t<InIter>());
    }

    //* iterator functions : advance

    template <typename InIter, typename Distance>
    inline void __advance(InIter &iter, Distance n, input_iterator_tag) {
        while (n--) ++iter;
    }

    template <typename BiIter, typename Distance>
    inline void __advance(BiIter &iter, Distance n, bidirectional_iterator_tag) {
        if (n > 0) while (n--) ++iter;
        else       while (n++) --iter;
    }

    template <typename RaIter, typename Distance>
    inline void __advance(RaIter &iter, Distance n, random_access_iterator_tag) {
        iter += n;
    }

    template <typename InIter, typename Distance>
    inline void advance(InIter &iter, Distance n) {
        __advance(iter, n, iterator_category_t<InIter>());
    }

    //* special iterator : back_insert_iterator

    template <typename Container>
    struct back_insert_iterator : public output_iterator<void> {
        typedef back_insert_iterator<Container> self_type;

        explicit back_insert_iterator(Container &c) : m_container(&c) {}

        self_type &operator*() { return *this; }
        self_type &operator++() { return *this; }
        self_type &operator++(int) { return *this; }
        self_type &operator=(const typename Container::value_type &value)
        {
            m_container->push_back(value);
            return *this;
        }

    protected:
        Container *m_container;
    };

    template <typename Container>
    inline back_insert_iterator<Container> back_inserter(Container &c) {
        return back_insert_iterator<Container>(c);
    }

    //* special iterator : front_insert_iterator

    template <typename Container>
    struct front_insert_iterator : public output_iterator<void> {
        typedef front_insert_iterator<Container> self_type;

        explicit front_insert_iterator(Container &c) : m_container(&c) {}

        self_type &operator*() { return *this; }
        self_type &operator++() { return *this; }
        self_type &operator++(int) { return *this; }
        self_type &operator=(const typename Container::value_type &value)
        {
            m_container->push_front(value);
            return *this;
        }

    protected:
        Container *m_container;
    };

    template <typename Container>
    inline front_insert_iterator<Container> front_inserter(Container &c) {
        return front_insert_iterator<Container>(c);
    }

    //* special iterator : insert_iterator

    template <typename Container>
    struct insert_iterator : public output_iterator<void> {
        typedef front_insert_iterator<Container> self_type;
        typedef typename Container::iterator iterator_type;

        explicit insert_iterator(Container &c, iterator_type i) : m_container(&c), m_iter(i) {}

        self_type &operator*() { return *this; }
        self_type &operator++() { return *this; }
        self_type &operator++(int) { return *this; }
        self_type &operator=(const typename Container::value_type &value)
        {
            m_container->insert(m_iter, value);
            ++m_iter;
            return *this;
        }

    protected:
        Container *m_container;
        iterator_type m_iter;
    };

    template <class Container, class Iterator>
    inline insert_iterator<Container> inserter(Container &x, Iterator i) {
        return insert_iterator<Container>(x, typename Container::iterator(i));
    }

    //* special iterator : reverse_iterator

    template <typename RaIter>
    struct reverse_iterator {
        typedef reverse_iterator<RaIter> self_type;
        typedef iterator_category_t<RaIter> iterator_category;
        typedef iterator_value_t<RaIter> value_type;
        typedef iterator_pointer_t<RaIter> pointer;
        typedef iterator_reference_t<RaIter> reference;
        typedef iterator_distance_t<RaIter> difference_type;

        constexpr reverse_iterator() {}
        constexpr explicit reverse_iterator(RaIter i) : m_iter(i) {}
        inline RaIter base() const { return m_iter; }

        // bidirectional
        inline reference operator*() const { RaIter tmp = m_iter; return *--tmp; }
        inline pointer operator->() { return &(operator*()); }
        inline self_type &operator++() { --m_iter; return *this; }
        inline self_type &operator--() { ++m_iter; return *this; }
        inline self_type operator++(int) { self_type tmp = *this; --m_iter; return tmp; }
        inline self_type operator--(int) { self_type tmp = *this; ++m_iter; return tmp; }
        inline bool operator==(const self_type &rhs) const { return base() == rhs.base(); }
        inline bool operator!=(const self_type &rhs) const { return base() != rhs.base(); }

        // random access
        inline difference_type operator-(self_type rhs) const { return difference_type(rhs.m_iter - m_iter); }
        inline self_type operator+(difference_type n) const { return self_type(m_iter - n); }
        inline self_type operator-(difference_type n) const { return self_type(m_iter + n); }
        inline self_type &operator+=(difference_type n) { m_iter -= n; return *this; }
        inline self_type &operator-=(difference_type n) { m_iter += n; return *this; }
        inline reference operator[](difference_type n) const { return *(*this + n); }
        inline bool operator>=(const self_type &rhs) const { return base() >= rhs.base(); }
        inline bool operator<=(const self_type &rhs) const { return base() <= rhs.base(); }
        inline bool operator>(const self_type &rhs) const { return base() > rhs.base(); }
        inline bool operator<(const self_type &rhs) const { return base() < rhs.base(); }

    protected:
        RaIter m_iter;
    };

    //* special iterator : istream_iterator
    //* special iterator : ostream_iterator
    //* special iterator : istreambuf_iterator
    //* special iterator : ostreambuf_iterator

} // namespace eztl
