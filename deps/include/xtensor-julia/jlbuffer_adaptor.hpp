/***************************************************************************
* Copyright (c) 2016, Johan Mabille and Sylvain Corlay                     *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#ifndef JLBUFFER_ADAPTOR_HPP
#define JLBUFFER_ADAPTOR_HPP

#include <cstddef>
#include <iterator>
#include <algorithm>

namespace xt
{

    template <class T>
    class jlbuffer_adaptor
    {
    public:

        using value_type = T;
        using reference = T&;
        using const_reference = const T&;
        using pointer = T*;
        using const_pointer = const T*;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;

        using iterator = pointer;
        using const_iterator = const_pointer;

        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        jlbuffer_adaptor() = default;
        jlbuffer_adaptor(pointer data, size_type size);
        
        bool empty() const noexcept;
        size_type size() const noexcept;
        
        reference operator[](size_type i);
        const_reference operator[](size_type i) const;

        reference front();
        const_reference front() const;

        reference back();
        const_reference back() const;

        iterator begin();
        iterator end();

        const_iterator begin() const;
        const_iterator end() const;
        const_iterator cbegin() const;
        const_iterator cend() const;

        reverse_iterator rbegin();
        reverse_iterator rend();

        const_reverse_iterator rbegin() const;
        const_reverse_iterator rend() const;
        const_reverse_iterator crbegin() const;
        const_reverse_iterator crend() const;

    private:

        pointer p_data;
        size_type m_size;
    };

    template<class T>
    bool operator==(const jlbuffer_adaptor<T>& x, const jlbuffer_adaptor<T>& y);

    template<class T>
    bool operator<(const jlbuffer_adaptor<T>& x, const jlbuffer_adaptor<T>& y);

    template<class T>
    bool operator!=(const jlbuffer_adaptor<T>& x, const jlbuffer_adaptor<T>& y);

    template<class T>
    bool operator>(const jlbuffer_adaptor<T>& x, const jlbuffer_adaptor<T>& y);

    template<class T>
    bool operator<=(const jlbuffer_adaptor<T>& x, const jlbuffer_adaptor<T>& y);

    template<class T>
    bool operator>=(const jlbuffer_adaptor<T>& x, const jlbuffer_adaptor<T>& y);

    template <std::size_t N>
    class jlstrides_iterator;

    template <std::size_t N>
    class jlstrides_adaptor
    {

    public:

        using value_type = std::size_t;
        using const_reference = value_type;
        using const_pointer = const value_type*;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;

        using const_iterator = jlstrides_iterator<N>;

        jlstrides_adaptor() = default;
        jlstrides_adaptor(const_pointer data, size_type size);

        bool empty() const noexcept;
        size_type size() const noexcept;

        const_reference operator[](size_type i) const;

        const_reference front() const;
        const_reference back() const;

        const_iterator begin() const;
        const_iterator end() const;
        const_iterator cbegin() const;
        const_iterator cend() const;

    private:

        const_pointer p_data;
        size_type m_size;
    };

    /***********************************
     * jlbuffer_adaptor implementation *
     ***********************************/

    template <class T>
    inline jlbuffer_adaptor<T>::jlbuffer_adaptor(pointer data, size_type size)
        : p_data(data), m_size(size)
    {
    }

    template <class T>
    inline bool jlbuffer_adaptor<T>::empty() const noexcept
    {
        return m_size == 0;
    }

    template <class T>
    inline auto jlbuffer_adaptor<T>::size() const noexcept -> size_type
    {
        return m_size;
    }

    template <class T>
    inline auto jlbuffer_adaptor<T>::operator[](size_type i) -> reference
    {
        return p_data[i];
    }

    template <class T>
    inline auto jlbuffer_adaptor<T>::operator[](size_type i) const -> const_reference
    {
        return p_data[i];
    }

    template <class T>
    inline auto jlbuffer_adaptor<T>::front() -> reference
    {
        return p_data[0];
    }

    template <class T>
    inline auto jlbuffer_adaptor<T>::front() const -> const_reference
    {
        return p_data[0];
    }

    template <class T>
    inline auto jlbuffer_adaptor<T>::back() -> reference
    {
        return p_data[m_size - 1];
    }

    template <class T>
    inline auto jlbuffer_adaptor<T>::back() const -> const_reference
    {
        return p_data[m_size - 1];
    }

    template <class T>
    inline auto jlbuffer_adaptor<T>::begin() -> iterator
    {
        return p_data;
    }

    template <class T>
    inline auto jlbuffer_adaptor<T>::end() -> iterator
    {
        return p_data + m_size;
    }

    template <class T>
    inline auto jlbuffer_adaptor<T>::begin() const -> const_iterator
    {
        return const_iterator(p_data);
    }

    template <class T>
    inline auto jlbuffer_adaptor<T>::end() const -> const_iterator
    {
        return const_iterator(p_data + m_size);
    }

    template <class T>
    inline auto jlbuffer_adaptor<T>::cbegin() const -> const_iterator
    {
        return begin();
    }

    template <class T>
    inline auto jlbuffer_adaptor<T>::cend() const -> const_iterator
    {
        return end();
    }

    template <class T>
    inline auto jlbuffer_adaptor<T>::rbegin() -> reverse_iterator
    {
        return reverse_iterator(end());
    }

    template <class T>
    inline auto jlbuffer_adaptor<T>::rend() -> reverse_iterator
    {
        return reverse_iterator(begin());
    }

    template <class T>
    inline auto jlbuffer_adaptor<T>::rbegin() const -> const_reverse_iterator
    {
        return const_reverse_iterator(end());
    }

    template <class T>
    inline auto jlbuffer_adaptor<T>::rend() const -> const_reverse_iterator
    {
        return const_reverse_iterator(begin());
    }

    template <class T>
    inline auto jlbuffer_adaptor<T>::crbegin() const -> const_reverse_iterator
    {
        return rbegin();
    }

    template <class T>
    inline auto jlbuffer_adaptor<T>::crend() const -> const_reverse_iterator
    {
        return rend();
    }
 
    template<class T>
    inline bool operator==(const jlbuffer_adaptor<T>& x, const jlbuffer_adaptor<T>& y)
    {
        return (x.size() == y.size() && std::equal(x.begin(), x.end(), y.begin()));
    }

    template<class T>
    inline bool operator<(const jlbuffer_adaptor<T>& x, const jlbuffer_adaptor<T>& y)
    {
        return std::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
    }

    template<class T>
    inline bool operator!=(const jlbuffer_adaptor<T>& x, const jlbuffer_adaptor<T>& y)
    {
        return !(x == y);
    }

    template<class T>
    inline bool operator>(const jlbuffer_adaptor<T>& x, const jlbuffer_adaptor<T>& y)
    {
        return y < x;
    }

    template<class T>
    inline bool operator<=(const jlbuffer_adaptor<T>& x, const jlbuffer_adaptor<T>& y)
    {
        return !(y < x);
    }

    template<class T>
    inline bool operator>=(const jlbuffer_adaptor<T>& x, const jlbuffer_adaptor<T>& y)
    {
        return !(x < y);
    }
}

#endif
