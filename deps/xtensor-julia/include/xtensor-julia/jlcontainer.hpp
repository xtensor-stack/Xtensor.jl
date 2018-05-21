/***************************************************************************
* Copyright (c) 2016, Johan Mabille and Sylvain Corlay                     *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#ifndef XCONTAINER_JULIA_HPP
#define XCONTAINER_JULIA_HPP

#include <algorithm>
#include <array>
#include <cstddef>
#include <exception>

#include "xtensor/xcontainer.hpp"

#include "xtl/xsequence.hpp"

namespace xt
{

    /***************************
     * jlcontainer declaration *
     ***************************/

    template <class D>
    class jlcontainer : public xcontainer<D>
    {
    public:

        using derived_type = D;

        using base_type = xcontainer<D>;
        using inner_types = xcontainer_inner_types<D>;
        using storage_type = typename inner_types::storage_type;
        using value_type = typename storage_type::value_type;
        using reference = typename storage_type::reference;
        using const_reference = typename storage_type::const_reference;
        using pointer = typename storage_type::pointer;
        using const_pointer = typename storage_type::const_pointer;
        using size_type = typename storage_type::size_type;
        using difference_type = typename storage_type::difference_type;

        using shape_type = typename inner_types::shape_type;
        using strides_type = typename inner_types::strides_type;
        using backstrides_type = typename inner_types::backstrides_type;
        using inner_shape_type = typename inner_types::inner_shape_type;
        using inner_strides_type = typename inner_types::inner_strides_type;

        using iterable_base = xiterable<D>;

        using iterator = typename iterable_base::iterator;
        using const_iterator = typename iterable_base::const_iterator;

        using stepper = typename iterable_base::stepper;
        using const_stepper = typename iterable_base::const_stepper;

        static constexpr layout_type static_layout = layout_type::column_major;
        static constexpr bool contiguous_layout = false;

        template <class S = shape_type>
        void resize(S&& shape, bool force = false);

        template <class S = shape_type>
        void reshape(S&& shape);

        layout_type layout() const;

        using base_type::operator();
        using base_type::operator[];
        using base_type::begin;
        using base_type::end;

        jl_array_t* wrapped() noexcept;

    protected:

        jlcontainer() = default;
        ~jlcontainer() = default;

        jlcontainer(const jlcontainer&) = default;
        jlcontainer& operator=(const jlcontainer&) = default;

        jlcontainer(jlcontainer&&) = default;
        jlcontainer& operator=(jlcontainer&&) = default;
        jlcontainer(jl_array_t*) noexcept;

        derived_type& derived_cast();
        const derived_type& derived_cast() const;

        jl_array_t* p_array;
    };

    /**********************************************
     * make_julia_[shape/array]_type declarations *
     **********************************************/

    jl_datatype_t* make_julia_shape_type(std::size_t dimension);

    template <class T>
    jl_value_t* make_julia_array_type(std::size_t dimension);

    /******************************
     * jlcontainer implementation *
     ******************************/

    /**
     * resizes the container.
     * @param shape the new shape
     */
    template <class D>
    template <class S>
    inline void jlcontainer<D>::resize(S&& shape, bool force)
    {
        if (force || shape.size() != this->dimension() || !std::equal(std::begin(shape), std::end(shape), this->shape().cbegin()))
        {
            derived_type tmp(std::forward<S>(shape));
            *static_cast<derived_type*>(this) = std::move(tmp);
        }
    }

    template <class D>
    template <class S>
    inline void jlcontainer<D>::reshape(S&& shape)
    {
        if (xt::compute_size(shape) != this->size())
        {
            throw std::runtime_error("resize sizes do not match up.");
        }

        jl_value_t* array_type;
        jl_value_t* dims;
        jl_datatype_t* tuple_type;

        JL_GC_PUSH3(&array_type, &tuple_type, &dims);

        array_type = make_julia_array_type<value_type>(shape.size());
        tuple_type = make_julia_shape_type(shape.size());
        dims = jl_new_bits((jl_value_t*)tuple_type, const_cast<void*>(reinterpret_cast<const void*>(shape.data())));

        this->p_array = jl_reshape_array((jl_value_t*) array_type, wrapped(), dims);
        this->derived_cast().init_from_julia();

        JL_GC_POP();
    }

    /**
     * Return the wrapped julia array.
     */
    template <class D>
    inline auto jlcontainer<D>::wrapped() noexcept -> jl_array_t*
    {
        return p_array;
    }

    /**
     * Constructor from julia
     * @param ptr the julia array
     */
    template <class D>
    inline jlcontainer<D>::jlcontainer(jl_array_t* ptr) noexcept
        : p_array(ptr)
    {
    }

    /**
     * Return the layout_type of the container
     * @return layout_type of the container
     */
    template <class D>
    inline auto jlcontainer<D>::layout() const -> layout_type
    {
        return layout_type::column_major;
    }

    template <class D>
    inline auto jlcontainer<D>::derived_cast() -> derived_type&
    {
        return *static_cast<derived_type*>(this);
    }

    template <class D>
    inline auto jlcontainer<D>::derived_cast() const -> const derived_type&
    {
        return *static_cast<const derived_type*>(this);
    }

    /**
     * Return the julia shape type for a prescribed dimension.
     * @param dimension the dimension of the array
     */
    inline jl_datatype_t* make_julia_shape_type(std::size_t dimension)
    {
        jl_svec_t* jtypes = jl_alloc_svec(dimension);
        for (std::size_t i = 0; i < dimension; ++i)
        {
            jl_svecset(jtypes, i, jlcxx::julia_type<std::size_t>());
        }
        return jl_apply_tuple_type(jtypes);
    }

    /**
     * Return the julia array type for the prescribed dimension.
     * @tparam T the C++ value type of the array.
     * @param dimension the dimension of the array.
     */
    template <class T>
    inline jl_value_t* make_julia_array_type(std::size_t dimension)
    {
        return jlcxx::apply_array_type(jlcxx::static_type_mapping<T>::julia_type(), dimension);
    }
}

#endif
