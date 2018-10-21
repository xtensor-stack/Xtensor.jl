/***************************************************************************
* Copyright (c) 2016, Johan Mabille and Sylvain Corlay                     *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#include <numeric>

#include "jlcxx/jlcxx.hpp"

#include "xtensor/xmath.hpp"
#include "xtensor/xarray.hpp"

#include "xtensor-julia/jlarray.hpp"
#include "xtensor-julia/jltensor.hpp"
#include "xtensor-julia/jlvectorize.hpp"

// Examples

double test_access(const xt::jltensor<double, 2> m)
{
    return m(1, 1);
}

double example1(xt::jlarray<double> m)
{
    return m(0, 0);
}

xt::jlarray<double> example2(xt::jlarray<double> m)
{
    return m + 2;
}

// Readme Examples

double readme_example1(xt::jlarray<double> m)
{
    auto sines = xt::sin(m);
    return std::accumulate(sines.cbegin(), sines.cend(), 0.0);
}

double readme_example2(double i, double j)
{
    return std::sin(i) - std::cos(j);
}

// Vectorize Examples
int_t add(int_t i, int_t j)
{
    return i + j;
}

namespace tensors
{
    JLCXX_MODULE define_julia_module(jlcxx::Module& mod)
    {
        // Test jltensor
        mod.method("test_access", test_access);
        mod.method("example1", example1);

        mod.method("example2", example2);

        mod.method("readme_example1", readme_example1);
        mod.method("readme_example2", xt::jlvectorize(readme_example2));

        mod.method("vectorize_example1", xt::jlvectorize(add));

        mod.method("rect_to_polar", xt::jlvectorize([](const std::complex<double> x) {
            return std::abs(x);
        }));

        mod.method("compare_shapes", [](const xt::jlarray<double> a, const xt::jlarray<double> b) {
            return a.shape() == b.shape();
        });
    }
}
