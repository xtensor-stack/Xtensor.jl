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

#include "xtensor-julia/jlarray.hpp"

#include "xtensor/xarray.hpp"

namespace xt
{
    using container_type = std::vector<std::size_t>;

    TEST(jarray, initializer_constructor)
    {
        jlarray<int> t
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

    TEST(jlarray, shaped_constructor)
    {
        column_major_result<container_type> cm;
        jlarray<int> ca(cm.m_shape);
        compare_shape(ca, cm);
    }

    TEST(jlarray, valued_constructor)
    {
        column_major_result<container_type> cm;
        int value = 2;
        jlarray<int> ca(cm.m_shape, value);
        compare_shape(ca, cm);
        std::vector<int> vec(ca.size(), value);
        EXPECT_TRUE(std::equal(vec.cbegin(), vec.cend(), ca.data().cbegin()));
    }

    TEST(jlarray, copy_semantic)
    {
        central_major_result<container_type> res;
        int value = 2;
        jlarray<int> a(res.m_shape, value);

        {
            SCOPED_TRACE("copy constructor");
            jlarray<int> b(a);
            compare_shape(a, b);
            EXPECT_EQ(a.data(), b.data());
            a.data()[0] += 1;
            EXPECT_NE(a.data()[0], b.data()[0]);
        }

        {
            SCOPED_TRACE("assignment operator");
            column_major_result<container_type> r;
            jlarray<int> c(r.m_shape, 0);
            EXPECT_NE(a.data(), c.data());
            c = a;
            compare_shape(a, c);
            EXPECT_EQ(a.data(), c.data());
            a.data()[0] += 1;
            EXPECT_NE(a.data()[0], c.data()[0]);
        }
    }

    TEST(jlarray, move_semantic)
    {
        central_major_result<container_type> res;
        int value = 2;
        jlarray<int> a(res.m_shape, value);

        {
            SCOPED_TRACE("move constructor");
            jlarray<int> tmp(a);
            jlarray<int> b(std::move(tmp));
            compare_shape(a, b);
            EXPECT_EQ(a.data(), b.data());
        }

        {
            SCOPED_TRACE("move assignment");
            column_major_result<container_type> r;
            jlarray<int> c(r.m_shape, 0);
            EXPECT_NE(a.data(), c.data());
            jlarray<int> tmp(a);
            c = std::move(tmp);
            compare_shape(a, c);
            EXPECT_EQ(a.data(), c.data());
        }
    }

    TEST(jlarray, extended_constructor)
    {
        xt::xarray<int> a1 = { {1, 2}, {3, 4} };
        xt::xarray<int> a2 = { {1, 2}, {3, 4} };
        jlarray<int> c = a1 + a2;
        EXPECT_EQ(c(0, 0), a1(0, 0) + a2(0, 0));
        EXPECT_EQ(c(0, 1), a1(0, 1) + a2(0, 1));
        EXPECT_EQ(c(1, 0), a1(1, 0) + a2(1, 0));
        EXPECT_EQ(c(1, 1), a1(1, 1) + a2(1, 1));
    }

    TEST(jlarray, resize)
    {
        jlarray<int> a;
        test_resize<jlarray<int>, container_type>(a);
    }

    TEST(jlarray, reshape)
    {
        jlarray<int> a = {{1,2,3}, {4,5,6}};
        auto new_shape = std::vector<std::size_t>({1, 6});
        a.reshape(new_shape);
        auto orig_ptr = a.raw_data();
        EXPECT_TRUE(std::equal(a.shape().begin(), a.shape().end(), new_shape.begin()));
        EXPECT_EQ(orig_ptr, a.raw_data());
        EXPECT_THROW(a.reshape({10, 10}), std::runtime_error);
    }

    TEST(jlarray, access)
    {
        jlarray<int> a;
        test_access<jlarray<int>, container_type>(a);
    }

    TEST(jlarray, indexed_access)
    {
        jlarray<int> a;
        test_indexed_access<jlarray<int>, container_type>(a);
    }

    TEST(jlarray, broadcast_shape)
    {
        jlarray<int> a;
        test_broadcast(a);
    }

    TEST(jlarray, iterator)
    {
        jlarray<int> a;
        test_iterator<jlarray<int>, container_type>(a);
    }

    TEST(jlarray, zerod)
    {
        jlarray<int> a;
        EXPECT_EQ(0, a());
    }

}
