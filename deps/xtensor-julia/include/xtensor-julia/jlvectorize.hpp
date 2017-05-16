/***************************************************************************
* Copyright (c) 2016, Johan Mabille and Sylvain Corlay                     *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#ifndef JL_VECTORIZE_HPP
#define JL_VECTORIZE_HPP

#include <type_traits>

#include "xtensor/xvectorize.hpp"

#include "jlarray.hpp"

namespace xt
{

    template <class Func, class R, class... Args>
    struct jlvectorizer
    {
        xvectorizer<Func, R> m_vectorizer;

        template <class F, class = std::enable_if_t<!std::is_same<std::decay_t<F>, jlvectorizer>::value>>
        jlvectorizer(F&& func)
            : m_vectorizer(std::forward<F>(func))
        {
        }

        inline jlarray<R> operator()(jlarray<Args>... args) const
        {
            jlarray<R> res = m_vectorizer(args...);
            return res;
        }
    };

    /**
     * @brief Create numpy-style universal function from scalar function.
     */
    template <class R, class... Args>
    inline jlvectorizer<R(*)(Args...), R, Args...> jlvectorize(R(*f) (Args...))
    {
        return jlvectorizer<R(*) (Args...), R, Args...>(f);
    }

    /// @cond DOXYGEN_INCLUDE_OVERLOADS
    template <class F, class R, class... Args>
    inline jlvectorizer<F, R, Args...> jlvectorize(F&& f, R(*) (Args...))
    {
        return jlvectorizer<F, R, Args...>(std::forward<F>(f));
    }

    template <class F>
    inline auto jlvectorize(F&& f) -> decltype(jlvectorize(std::forward<F>(f), (detail::get_function_type<F>*)nullptr))
    {
        return jlvectorize(std::forward<F>(f), (detail::get_function_type<F>*)nullptr);
    }
    /// @endcond
}

#endif
