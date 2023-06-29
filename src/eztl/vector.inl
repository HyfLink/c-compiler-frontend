namespace eztl
{
    template <typename T, typename Alloc>
    constexpr inline vector<T, Alloc>::vector()
        : m_first(nullptr), m_last(nullptr), m_finish(nullptr)
    {
    }
    template <typename T, typename Alloc>
    inline vector<T, Alloc>::vector(const vector &other)
        : m_first(nullptr), m_last(nullptr), m_finish(nullptr)
    {
        reserve(other.size());
        copy_n(other.begin(), other.size(), m_first);
        m_last = m_finish;
    }
    template <typename T, typename Alloc>
    inline vector<T, Alloc>::vector(vector &&other)
        : m_first(other.m_first), m_last(other.m_last), m_finish(other.m_finish)
    {
        other.m_last = nullptr;
        other.m_first = nullptr;
        other.m_finish = nullptr;
    }
    template <typename T, typename Alloc>
    template <typename InIter, typename>
    inline vector<T, Alloc>::vector(InIter first, InIter last)
        : m_first(nullptr), m_last(nullptr), m_finish(nullptr)
    {
        assign(first, last);
    }
    template <typename T, typename Alloc>
    inline vector<T, Alloc>::vector(size_type nitems, const_reference val)
        : m_first(nullptr), m_last(nullptr), m_finish(nullptr)
    {
        assign(nitems, val);
    }

    template <typename T, typename Alloc>
    inline vector<T, Alloc>::~vector()
    {
        if (m_first)
        {
            destroy(m_first, m_last);
            data_allocator::deallocate(m_first, capacity());
        }
    }

    template <typename T, typename Alloc>
    inline typename vector<T, Alloc>::iterator vector<T, Alloc>::begin()
    {
        return iterator(m_first);
    }
    template <typename T, typename Alloc>
    inline typename vector<T, Alloc>::iterator vector<T, Alloc>::end()
    {
        return iterator(m_last);
    }
    template <typename T, typename Alloc>
    inline typename vector<T, Alloc>::reverse_iterator vector<T, Alloc>::rbegin()
    {
        return reverse_iterator(begin());
    }
    template <typename T, typename Alloc>
    inline typename vector<T, Alloc>::reverse_iterator vector<T, Alloc>::rend()
    {
        return reverse_iterator(end());
    }
    template <typename T, typename Alloc>
    inline typename vector<T, Alloc>::const_iterator vector<T, Alloc>::begin() const
    {
        return const_iterator(m_first);
    }
    template <typename T, typename Alloc>
    inline typename vector<T, Alloc>::const_iterator vector<T, Alloc>::end() const
    {
        return const_iterator(m_last);
    }
    template <typename T, typename Alloc>
    inline typename vector<T, Alloc>::const_iterator vector<T, Alloc>::cbegin() const
    {
        return const_iterator(m_first);
    }
    template <typename T, typename Alloc>
    inline typename vector<T, Alloc>::const_iterator vector<T, Alloc>::cend() const
    {
        return const_iterator(m_last);
    }
    template <typename T, typename Alloc>
    inline typename vector<T, Alloc>::const_reverse_iterator vector<T, Alloc>::rbegin() const
    {
        return const_reverse_iterator(begin());
    }
    template <typename T, typename Alloc>
    inline typename vector<T, Alloc>::const_reverse_iterator vector<T, Alloc>::rend() const
    {
        return const_reverse_iterator(end());
    }
    template <typename T, typename Alloc>
    inline typename vector<T, Alloc>::const_reverse_iterator vector<T, Alloc>::crbegin() const
    {
        return const_reverse_iterator(cbegin());
    }
    template <typename T, typename Alloc>
    inline typename vector<T, Alloc>::const_reverse_iterator vector<T, Alloc>::crend() const
    {
        return const_reverse_iterator(cend());
    }

    template <typename T, typename Alloc>
    inline typename vector<T, Alloc>::pointer vector<T, Alloc>::data()
    {
        return pointer(m_first);
    }
    template <typename T, typename Alloc>
    inline typename vector<T, Alloc>::const_pointer vector<T, Alloc>::data() const
    {
        return const_pointer(m_first);
    }
    template <typename T, typename Alloc>
    inline typename vector<T, Alloc>::size_type vector<T, Alloc>::size() const
    {
        return size_type(m_last - m_first);
    }
    template <typename T, typename Alloc>
    inline typename vector<T, Alloc>::size_type vector<T, Alloc>::capacity() const
    {
        return size_type(m_finish - m_first);
    }
    template <typename T, typename Alloc>
    inline typename vector<T, Alloc>::reference vector<T, Alloc>::front()
    {
        return *(m_first);
    }
    template <typename T, typename Alloc>
    inline typename vector<T, Alloc>::const_reference vector<T, Alloc>::front() const
    {
        return *(m_first);
    }
    template <typename T, typename Alloc>
    inline typename vector<T, Alloc>::reference vector<T, Alloc>::back()
    {
        return *(m_last - 1);
    }
    template <typename T, typename Alloc>
    inline typename vector<T, Alloc>::const_reference vector<T, Alloc>::back() const
    {
        return *(m_last - 1);
    }

    template <typename T, typename Alloc>
    inline bool vector<T, Alloc>::empty() const
    {
        return bool(m_last == m_first);
    }
    template <typename T, typename Alloc>
    inline void vector<T, Alloc>::clear()
    {
        __earse_to_end(m_first);
    }
    template <typename T, typename Alloc>
    inline void vector<T, Alloc>::swap(vector &other)
    {
        swap(m_last, other.m_last);
        swap(m_first, other.m_first);
        swap(m_finish, other.m_finish);
    }

    template <typename T, typename Alloc>
    typename vector<T, Alloc>::reference vector<T, Alloc>::at(size_type idx)
    {
        assert(idx < size());
        return *(m_first + idx);
    }
    template <typename T, typename Alloc>
    typename vector<T, Alloc>::const_reference vector<T, Alloc>::at(size_type idx) const
    {
        assert(idx < size());
        return *(m_first + idx);
    }

    template <typename T, typename Alloc>
    void vector<T, Alloc>::resize(size_type new_size, const_reference val)
    {
        const size_type old_size = size();
        if (new_size < old_size)
        {
            __earse_to_end(m_first + new_size);
        }
        else if (new_size > old_size)
        {
            reserve(max(2 * old_size, new_size));
            fill_n(m_last, new_size - old_size, val);
        }
    }
    template <typename T, typename Alloc>
    void vector<T, Alloc>::reserve(size_type new_size)
    {
        if (new_size > capacity())
        {
            size_type old_size = size();
            pointer tmp = data_allocator::allocate(new_size);

            if (m_first != m_last)
            {
                move(m_first, m_last, tmp);
                destroy(m_first, m_last);
            }
            if (m_first != nullptr)
            {
                data_allocator::deallocate(m_first, capacity());
            }

            m_first = tmp;
            m_last = tmp + old_size;
            m_finish = tmp + new_size;
        }
    }

    template <typename T, typename Alloc>
    template <typename InIter, typename>
    void vector<T, Alloc>::assign(InIter first, InIter last)
    {
        size_type nitems = distance(first, last);
        reserve(nitems);
        if (nitems < size())
        {
            destroy(m_first, m_first + nitems);
        }
        else
        {
            destroy(m_first, m_last);
            m_last = m_first + nitems;
        }
        copy_n(first, nitems, m_first);
    }
    template <typename T, typename Alloc>
    void vector<T, Alloc>::assign(size_type nitems, const_reference val)
    {
        reserve(nitems);
        if (nitems < size())
        {
            destroy(m_first, m_first + nitems);
        }
        else
        {
            destroy(m_first, m_last);
            m_last = m_first + nitems;
        }
        fill_n(m_first, nitems, val);
    }

    template <typename T, typename Alloc>
    void vector<T, Alloc>::push_back(const_reference val)
    {
        if (size() == capacity())
            reserve(max(size_type(1), 2 * size()));

        *m_last = val;
        ++m_last;
    }
    template <typename T, typename Alloc>
    void vector<T, Alloc>::pop_back()
    {
        assert(size() > 0);
        --m_last;
        data_allocator::destroy(m_last);
    }

    template <typename T, typename Alloc>
    template <typename... Args>
    typename vector<T, Alloc>::reference vector<T, Alloc>::emplace_back(Args &&...args)
    {
        if (size() == capacity())
            reserve(max(size_type(1), 2 * size()));

        construct(m_last, forward<Args>(args)...);
        ++m_last;
        return *(m_last - 1);
    }

    template <typename T, typename Alloc>
    template <typename... Args>
    typename vector<T, Alloc>::iterator vector<T, Alloc>::emplace(iterator pos, Args &&...args)
    {
        size_type index = pos - begin();

        if (size() == capacity())
            reserve(max(size_type(1), 2 * size()));

        pos = begin() + index;
        copy_backward(pos, end(), end() + 1);
        ++m_last;
        construct(pos, forward<Args>(args)...);
        return pos;
    }

    template <typename T, typename Alloc>
    typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(iterator pos, const_reference val)
    {
        size_type index = pos - begin();

        if (size() == capacity())
            reserve(max(size_type(1), 2 * size()));

        pos = begin() + index;
        copy_backward(pos, end(), end() + 1);
        ++m_last;
        *pos = val;
        return pos;
    }
    template <typename T, typename Alloc>
    typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(iterator pos, size_type nitems, const_reference val)
    {
        size_type index = pos - begin();

        if (size() + nitems > capacity())
            reserve(max(nitems, 2 * size()));

        pos = begin() + index;
        copy_backward(pos, end(), end() + nitems);
        m_last += nitems;
        fill_n(pos, nitems, val);

        return pos;
    }
    template <typename T, typename Alloc>
    template <typename InIter, typename>
    typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(iterator pos, InIter first, InIter last)
    {
        size_type nitems = distance(first, last);
        size_type index = pos - begin();

        if (size() + nitems > capacity())
            reserve(max(nitems, 2 * size()));

        pos = begin() + index;
        copy_backward(pos, end(), end() + nitems);
        m_last += nitems;
        copy_n(first, nitems, pos);

        return pos;
    }

    template <typename T, typename Alloc>
    typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator pos)
    {
        if (pos + 1 != end())
        {
            move(pos + 1, m_last, pos);
            --m_last;
            data_allocator::destroy(m_last);
        }
        return pos;
    }
    template <typename T, typename Alloc>
    typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator first, iterator last)
    {
        if (last != end())
        {
            move(last, m_last, first);
            __earse_to_end(first + (m_last - last));
        }
        return first;
    }

    template <typename T, typename Alloc>
    inline typename vector<T, Alloc>::reference vector<T, Alloc>::operator[](size_type idx)
    {
        return *(m_first + idx);
    }
    template <typename T, typename Alloc>
    inline typename vector<T, Alloc>::const_reference vector<T, Alloc>::operator[](size_type idx) const
    {
        return *(m_first + idx);
    }

    template <typename T, typename Alloc>
    vector<T, Alloc> &vector<T, Alloc>::operator=(const vector &rhs)
    {
        reserve(rhs.size());
        copy_n(rhs.begin(), rhs.size(), m_first);
        m_last = m_finish;
        return *this;
    }

    template <typename T, typename Alloc>
    vector<T, Alloc> &vector<T, Alloc>::operator=(vector &&rhs)
    {
        m_last = rhs.m_last;
        m_first = rhs.m_first;
        m_finish = rhs.m_finish;

        rhs.m_last = nullptr;
        rhs.m_first = nullptr;
        rhs.m_finish = nullptr;
        return *this;
    }

    template <typename T, typename Alloc>
    inline void vector<T, Alloc>::__earse_to_end(pointer ptr)
    {
        if (ptr < m_last)
        {
            destroy(ptr, m_last);
            m_last = ptr;
        }
    }


} // namespace eztl
