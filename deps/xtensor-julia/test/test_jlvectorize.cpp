/***************************************************************************
* Copyright (c) 2016, Johan Mabille and Sylvain Corlay                     *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#include "gtest/gtest.h"

#include "test_common.hpp"

#include "xtensor-julia/jltensor.hpp"
#include "xtensor-julia/jlvectorize.hpp"

namespace xt
{

    double f1(double a, double b)
    {
        return a + b;
    }

    using shape_type = std::vector<std::size_t>;

    TEST(jlvectorize, function)
    {
        auto vecf1 = jlvectorize(f1);
        shape_type shape = { 3, 2 };
        jlarray<double> a(shape, 1.5);
        jlarray<double> b(shape, 2.3);
        jlarray<double> c = vecf1(a, b);
        EXPECT_EQ(a(0, 0) + b(0, 0), c(0, 0));
    }

    TEST(jlvectorize, lambda)
    {
        auto vecf1 = jlvectorize([](double a, double b) { return a + b; });
        shape_type shape = { 3, 2 };
        jlarray<double> a(shape, 1.5);
        jlarray<double> b(shape, 2.3);
        jlarray<double> c = vecf1(a, b);
        EXPECT_EQ(a(0, 0) + b(0, 0), c(0, 0));
    }

/*
    TEST(jlvectorize, complex)
    {
        using complex_t = std::complex<double>;
        shape_type shape = { 3, 2 };
        jlarray<complex_t> a(shape, complex_t(1.2, 2.5));
        auto f = jlvectorize([](complex_t x) { return std::abs(x); });
        auto res = f(a);
        double exp = std::abs(a(1, 1));
        EXPECT_EQ(exp, res(1, 1));
    }
*/
}
