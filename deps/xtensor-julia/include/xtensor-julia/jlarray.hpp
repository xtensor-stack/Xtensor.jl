/***************************************************************************
* Copyright (c) 2016, Johan Mabille and Sylvain Corlay                     *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#ifndef XARRAY_JULIA_HPP
#define XARRAY_JULIA_HPP

#include <algorithm>
#include <array>
#include <complex>
#include <cstddef>
#include <exception>

#include "xtensor/xbuffer_adaptor.hpp"
#include "xtensor/xsemantic.hpp"

#include "jlcxx/type_conversion.hpp"

#include "jlcontainer.hpp"

namespace xt
{

    /***********************
     * jlarray declaration *
     ***********************/

    template <class T>
    class jlarray;

    template <class T>
    struct xiterable_inner_types<jlarray<T>>
        : xcontainer_iterable_types<jlarray<T>>
    {
    };

    template <class T>
    struct xcontainer_inner_types<jlarray<T>>
    {
        using container_type = xbuffer_adaptor<jlcxx::mapped_julia_type<T>*>;
        using shape_type = std::vector<std::size_t>;
        using strides_type = shape_type;
        using backstrides_type = shape_type;
        using inner_shape_type = xbuffer_adaptor<std::size_t*>;
        using inner_strides_type = strides_type;
        using inner_backstrides_type = backstrides_type;
        using temporary_type = jlarray<T>;
        static constexpr layout_type layout = layout_type::column_major;
    };

    template <class T>
    struct xcontainer_inner_types<jlarray<std::complex<T>>>
    {
        using container_type = xbuffer_adaptor<std::complex<T>*>;
        using shape_type = std::vector<std::size_t>;
        using strides_type = shape_type;
        using backstrides_type = shape_type;
        using inner_shape_type = xbuffer_adaptor<std::size_t*>;
        using inner_strides_type = strides_type;
        using inner_backstrides_type = backstrides_type;
        using temporary_type = jlarray<T>;
        static constexpr layout_type layout = layout_type::column_major;
    };

    template <class T>
    class jlarray : public jlcontainer<jlarray<T>>,
                    public xcontainer_semantic<jlarray<T>>
    {
    public:

        using self_type = jlarray<T>;
        using semantic_base = xcontainer_semantic<self_type>;
        using base_type = jlcontainer<self_type>;
        using container_type = typename base_type::container_type;
        using value_type = typename base_type::value_type;
        using reference = typename base_type::reference;
        using const_reference = typename base_type::const_reference;
        using pointer = typename base_type::pointer;
        using size_type = typename base_type::size_type;
        using shape_type = typename base_type::shape_type;
        using strides_type = typename base_type::strides_type;
        using backstrides_type = typename base_type::backstrides_type;
        using inner_shape_type = typename base_type::inner_shape_type;
        using inner_strides_type = typename base_type::inner_strides_type;
        using inner_backstrides_type = typename base_type::inner_backstrides_type;

        jlarray();
        jlarray(const value_type& t);
        jlarray(nested_initializer_list_t<T, 1> t);
        jlarray(nested_initializer_list_t<T, 2> t);
        jlarray(nested_initializer_list_t<T, 3> t);
        jlarray(nested_initializer_list_t<T, 4> t);
        jlarray(nested_initializer_list_t<T, 5> t);

        explicit jlarray(const shape_type& shape);
        explicit jlarray(const shape_type& shape, const_reference value);
        jlarray(jl_array_t* jl);

        jlarray(const self_type&);
        self_type& operator=(const self_type&);

        jlarray(self_type&&) = default;
        self_type& operator=(self_type&&) = default;

        template <class E>
        jlarray(const xexpression<E>& e);

        template <class E>
        self_type& operator=(const xexpression<E>& e);

        using base_type::begin;
        using base_type::end;

    private:

        inner_shape_type m_shape;
        inner_strides_type m_strides;
        inner_backstrides_type m_backstrides;
        container_type m_data;

        void init_array(const shape_type& shape);
        void init_from_julia();

        inner_shape_type& shape_impl() noexcept;
        const inner_shape_type& shape_impl() const noexcept;
        inner_strides_type& strides_impl() noexcept;
        const inner_strides_type& strides_impl() const noexcept;
        inner_backstrides_type& backstrides_impl() noexcept;
        const inner_backstrides_type& backstrides_impl() const noexcept;

        container_type& data_impl() noexcept;
        const container_type& data_impl() const noexcept;

        friend class xcontainer<jlarray<T>>;
        friend class jlcontainer<jlarray<T>>;
    };

    /**************************
     * jlarray implementation *
     **************************/

    /**
     * @name Constructors
     */
    //@{
    /**
     * Allocates a jlarray that holds 1 element.
     */
    template <class T>
    inline jlarray<T>::jlarray()
        : base_type()
    {
        shape_type shape = xtl::make_sequence<shape_type>(0, size_type(1));
        m_strides = xtl::make_sequence<inner_strides_type>(0, size_type(0));
        m_backstrides = xtl::make_sequence<inner_backstrides_type>(0, size_type(0));
        xt::compute_strides(shape, layout_type::column_major, m_strides, m_backstrides);
        init_array(shape);
        m_data[0] = T();
    }

    /**
     * Allocates a jlarray with a nested initializer list.
     */
    template <class T>
    inline jlarray<T>::jlarray(const value_type& t)
        : base_type()
    {
        base_type::resize(xt::shape<shape_type>(t), true);
        nested_copy(this->begin(), t);
    }

    template <class T>
    inline jlarray<T>::jlarray(nested_initializer_list_t<T, 1> t)
        : base_type()
    {
        base_type::resize(xt::shape<shape_type>(t), true);
        nested_copy(this->begin(), t);
    }

    template <class T>
    inline jlarray<T>::jlarray(nested_initializer_list_t<T, 2> t)
        : base_type()
    {
        base_type::resize(xt::shape<shape_type>(t), true);
        nested_copy(this->begin(), t);
    }

    template <class T>
    inline jlarray<T>::jlarray(nested_initializer_list_t<T, 3> t)
        : base_type()
    {
        base_type::resize(xt::shape<shape_type>(t), true);
        nested_copy(this->begin(), t);
    }

    template <class T>
    inline jlarray<T>::jlarray(nested_initializer_list_t<T, 4> t)
        : base_type()
    {
        base_type::resize(xt::shape<shape_type>(t), true);
        nested_copy(this->begin(), t);
    }

    template <class T>
    inline jlarray<T>::jlarray(nested_initializer_list_t<T, 5> t)
        : base_type()
    {
        base_type::resize(xt::shape<shape_type>(t), true);
        nested_copy(this->begin(), t);
    }

    /**
     * Allocates an uninitialized jlarray with the specified shape and
     * layout.
     * @param shape the shape of the jlarray
     */
    template <class T>
    inline jlarray<T>::jlarray(const shape_type& shape)
        : base_type()
    {
        m_strides = xtl::make_sequence<inner_strides_type>(shape.size(), 0);
        m_backstrides = xtl::make_sequence<inner_backstrides_type>(shape.size(), 0);
        xt::compute_strides(shape, layout_type::column_major, m_strides, m_backstrides);
        init_array(shape);
    }

    /**
     * Allocates a jlarray with the specified shape and layout. Elements
     * are initialized to the specified value.
     * @param shape the shape of the jlarray
     * @param value the value of the elements
     */
    template <class T>
    inline jlarray<T>::jlarray(const shape_type& shape,
                               const_reference value)
        : jlarray(shape)
    {
        std::fill(m_data.begin(), m_data.end(), value);
    }

    /**
     * Allocates a jlarray that holds 1 element.
     */
    template <class T>
    inline jlarray<T>::jlarray(jl_array_t* jl)
        : base_type(jl)
    {
        init_from_julia();
    }
    //@}

    /**
     * @name Copy semantic
     */
    //@{
    /**
     * The copy constructor.
     */
    template <class T>
    inline jlarray<T>::jlarray(const self_type& rhs)
        : base_type(rhs), semantic_base(rhs)
    {
        m_strides = rhs.m_strides;
        m_backstrides = rhs.m_backstrides;

        // TODO: prevent intermediary shape_type
        shape_type shape = xtl::forward_sequence<shape_type>(rhs.shape());
        init_array(shape);
        std::copy(rhs.data().cbegin(), rhs.data().cend(), this->data().begin());
    }

    /**
     * The assignment operator.
     */
    template <class T>
    inline auto jlarray<T>::operator=(const self_type& rhs) -> self_type&
    {
        self_type tmp(rhs);
        *this = std::move(tmp);
        return *this;
    }
    //@}

    /**
     * @name Extended copy semantic
     */
    //@{
    /**
     * The extended copy constructor.
     */
    template <class T>
    template <class E>
    inline jlarray<T>::jlarray(const xexpression<E>& e)
        : base_type()
    {
        // TODO: prevent intermediary shape type
        shape_type shape = xtl::forward_sequence<shape_type>(e.derived_cast().shape());
        m_strides = xtl::make_sequence<inner_strides_type>(shape.size(), 0);
        m_backstrides = xtl::make_sequence<inner_backstrides_type>(shape.size(), 0);
        xt::compute_strides(shape, layout_type::column_major, m_strides, m_backstrides);
        init_array(shape);
        semantic_base::assign(e);
    }

    /**
     * The extended assignment operator.
     */
    template <class T>
    template <class E>
    inline auto jlarray<T>::operator=(const xexpression<E>& e) -> self_type&
    {
        return semantic_base::operator=(e);
    }
    //@}

    template <class T>
    inline void jlarray<T>::init_array(const shape_type& shape)
    {
        jl_value_t* array_type;
        jl_datatype_t* tuple_type;
        jl_value_t* dims;

        JL_GC_PUSH3(&array_type, &tuple_type, &dims);

        array_type = make_julia_array_type<value_type>(shape.size());
        tuple_type = make_julia_shape_type(shape.size());

        // allocate array
        dims = jl_new_bits((jl_value_t*)tuple_type, const_cast<void*>(reinterpret_cast<const void*>(shape.data())));
        this->p_array = jl_new_array((jl_value_t*)array_type, dims);

        // setup buffer adaptor
        m_data = container_type(reinterpret_cast<pointer>(this->p_array->data),
                                static_cast<size_type>(jl_array_len(this->p_array)));

        m_shape = inner_shape_type(&(this->p_array->nrows), this->p_array->flags.ndims);

        JL_GC_POP();
    }

    template <class T>
    inline void jlarray<T>::init_from_julia()
    {
        m_strides = xtl::make_sequence<inner_strides_type>(this->p_array->flags.ndims, 0);
        m_backstrides = xtl::make_sequence<inner_backstrides_type>(this->p_array->flags.ndims, 0);
        m_data = container_type(reinterpret_cast<pointer>(this->p_array->data),
                                static_cast<size_type>(jl_array_len(this->p_array)));
        m_shape = inner_shape_type(&(this->p_array->nrows), this->p_array->flags.ndims);
        xt::compute_strides(m_shape, layout_type::column_major, m_strides, m_backstrides);
    }

    template <class T>
    inline auto jlarray<T>::shape_impl() noexcept -> inner_shape_type&
    {
        return m_shape;
    }

    template <class T>
    inline auto jlarray<T>::shape_impl() const noexcept -> const inner_shape_type&
    {
        return m_shape;
    }

    template <class T>
    inline auto jlarray<T>::strides_impl() noexcept -> inner_strides_type&
    {
        return m_strides;
    }

    template <class T>
    inline auto jlarray<T>::strides_impl() const noexcept -> const inner_strides_type&
    {
        return m_strides;
    }

    template <class T>
    inline auto jlarray<T>::backstrides_impl() noexcept -> inner_backstrides_type&
    {
        return m_backstrides;
    }

    template <class T>
    inline auto jlarray<T>::backstrides_impl() const noexcept -> const inner_backstrides_type&
    {
        return m_backstrides;
    }

    template <class T>
    inline auto jlarray<T>::data_impl() noexcept -> container_type&
    {
        return m_data;
    }

    template <class T>
    inline auto jlarray<T>::data_impl() const noexcept -> const container_type&
    {
        return m_data;
    }
}

namespace jlcxx
{
    /****************************************************************
     * Template specializations for ConvertToJulia and ConvertToCpp *
     ****************************************************************/

    template <class T>
    struct ConvertToJulia<xt::jlarray<T>, false, false, false>
    {
        template <class U>
        jl_array_t* operator()(U&& arr) const
        {
            return arr.wrapped();
        }
    };

    template <class T>
    struct ConvertToCpp<xt::jlarray<T>, false, false, false>
    {
        xt::jlarray<T> operator()(jl_array_t* arr) const
        {
            return xt::jlarray<T>(arr);
        }
    };

    template <class T>
    struct static_type_mapping<xt::jlarray<T>>
    {
        using type = jl_array_t*;
        static constexpr bool is_dynamic = false;

        static jl_datatype_t* julia_type()
        {
            // Array{T}
            return (jl_datatype_t*)apply_type(
                jl_get_global(jl_current_module, jl_symbol("Array")),
                jl_svec1(jlcxx::julia_type<T>()));
        }
    };
}

#endif
