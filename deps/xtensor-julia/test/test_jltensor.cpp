/***************************************************************************
* Copyright (c) 2016, Johan Mabille and Sylvain Corlay                     *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#include "gtest/gtest.h"

#include <cstddef>

#include "test_common.hpp"

#include "xtensor-julia/jltensor.hpp"

#include "xtensor/xtensor.hpp"

namespace xt
{
    using container_type = std::array<std::size_t, 3>;

    TEST(jltensor, initializer_constructor)
    {
        jltensor<int, 3> t
          {{{ 0,  1,  2},
            { 3,  4,  5},
            { 6,  7,  8}},
           {{ 9, 10, 11},
            {12, 13, 14},
            {15, 16, 17}}};
        EXPECT_EQ(t.dimension(), 3);
        EXPECT_EQ(t(0, 0, 1), 1);
        EXPECT_EQ(t.shape()[0], 2);
    }

    TEST(jltensor, shaped_constructor)
    {
        column_major_result<container_type> cm;
        jltensor<int, 3> ca(cm.m_shape);
        compare_shape(ca, cm);
    }

    TEST(jltensor, valued_constructor)
    {
        column_major_result<container_type> cm;
        int value = 2;
        jltensor<int, 3> ca(cm.m_shape, value);
        compare_shape(ca, cm);
        std::vector<int> vec(ca.size(), value);
        EXPECT_TRUE(std::equal(vec.cbegin(), vec.cend(), ca.data().cbegin()));
    }

    TEST(jltensor, copy_semantic)
    {
        central_major_result<container_type> res;
        int value = 2;
        jltensor<int, 3> a(res.m_shape, value);

        {
            SCOPED_TRACE("copy constructor");
            jltensor<int, 3> b(a);
            compare_shape(a, b);
            EXPECT_EQ(a.data(), b.data());
            a.data()[0] += 1;
            EXPECT_NE(a.data()[0], b.data()[0]);
        }

        {
            SCOPED_TRACE("assignment operator");
            column_major_result<container_type> r;
            jltensor<int, 3> c(r.m_shape, 0);
            EXPECT_NE(a.data(), c.data());
            c = a;
            compare_shape(a, c);
            EXPECT_EQ(a.data(), c.data());
            a.data()[0] += 1;
            EXPECT_NE(a.data()[0], c.data()[0]);
        }
    }
    TEST(jltensor, move_semantic)
    {
        central_major_result<container_type> res;
        int value = 2;
        jltensor<int, 3> a(res.m_shape, value);

        {
            SCOPED_TRACE("move constructor");
            jltensor<int, 3> tmp(a);
            jltensor<int, 3> b(std::move(tmp));
            compare_shape(a, b);
            EXPECT_EQ(a.data(), b.data());
        }

        {
            SCOPED_TRACE("move assignment");
            column_major_result<container_type> r;
            jltensor<int, 3> c(r.m_shape, 0);
            EXPECT_NE(a.data(), c.data());
            jltensor<int, 3> tmp(a);
            c = std::move(tmp);
            compare_shape(a, c);
            EXPECT_EQ(a.data(), c.data());
        }
    }

    TEST(jltensor, extended_constructor)
    {
        xt::xtensor<int, 2> a1 = { {1, 2}, {3, 4} };
        xt::xtensor<int, 2> a2 = { {1, 2}, {3, 4} };
        jltensor<int, 2> c = a1 + a2;
        EXPECT_EQ(c(0, 0), a1(0, 0) + a2(0, 0));
        EXPECT_EQ(c(0, 1), a1(0, 1) + a2(0, 1));
        EXPECT_EQ(c(1, 0), a1(1, 0) + a2(1, 0));
        EXPECT_EQ(c(1, 1), a1(1, 1) + a2(1, 1));
    }

    TEST(jltensor, reshape)
    {
        jltensor<int, 3> a;
        test_reshape<jltensor<int, 3>, container_type>(a);
    }

    TEST(jltensor, access)
    {
        jltensor<int, 3> a;
        test_access<jltensor<int, 3>, container_type>(a);
    }

    TEST(jltensor, indexed_access)
    {
        jltensor<int, 3> a;
        test_indexed_access<jltensor<int, 3>, container_type>(a);
    }

    TEST(jltensor, broadcast_shape)
    {
        jltensor<int, 4> a;
        test_broadcast(a);
    }

    TEST(jltensor, iterator)
    {
        jltensor<int, 3> a;
        test_iterator<jltensor<int, 3>, container_type>(a);
    }

    TEST(jltensor, zerod)
    {
        jltensor<int, 3> a;
        EXPECT_EQ(0, a());
    }

}
