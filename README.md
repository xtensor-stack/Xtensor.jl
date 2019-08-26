# ![Xtensor.jl](xtensor-julia.svg)

[![Travis](https://travis-ci.org/QuantStack/Xtensor.jl.svg?branch=master)](https://travis-ci.org/QuantStack/Xtensor.jl)
[![Appveyor](https://ci.appveyor.com/api/projects/status/ddwxiu61ee6p9tp5?svg=true)](https://ci.appveyor.com/project/QuantStack/xtensor-jl)
[![Documentation Status](http://readthedocs.org/projects/xtensor-julia/badge/?version=latest)](https://xtensor-julia.readthedocs.io/en/latest/?badge=latest)
[![Join the Gitter Chat](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/QuantStack/Lobby?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

Julia package for the `xtensor-julia` library, the Julia bindings for [xtensor](https://github.com/QuantStack/xtensor).

 - `xtensor` is a C++ library for multi-dimensional arrays enabling numpy-style broadcasting and lazy computing.
 - `xtensor-julia` enables inplace use of julia arrays in C++ with all the benefits from `xtensor`

     - C++ universal function and broadcasting
     - STL - compliant APIs.
     - A broad coverage of numpy APIs (see [the numpy to xtensor cheat sheet](http://xtensor.readthedocs.io/en/latest/numpy.html)).

The Julia bindings for `xtensor` are based on the [CxxWrap.jl](https://github.com/JuliaInterop/CxxWrap.jl/) C++ library.

## Installation

```julia
using Pkg; Pkg.add("Xtensor");
```

## Documentation

To get started with using `Xtensor.jl` and `xtensor-julia`, check out the full documentation

http://xtensor-julia.readthedocs.io/

## Usage

xtensor-julia offers two container types wrapping julia arrays inplace to provide an xtensor semantics

 - `jltensor`
 - `jlarray`.

Both containers enable the numpy-style APIs of xtensor (see [the numpy to xtensor cheat sheet](http://xtensor.readthedocs.io/en/latest/numpy.html)).

 - On the one hand, `jlarray` has a dynamic number of dimensions. It can be reshaped dynamically and the new shape is reflected on the Julia side.

 - On the other hand `jltensor` has a compile time number of dimensions, specified with a template parameter. Shapes of `jltensor` instances are stack allocated, making `jltensor` a significantly faster expression than `jlarray`.

### Example 1: Use an algorithm of the C++ standard library with Julia array.

**C++ code**

```cpp
#include <numeric>                        // Standard library import for std::accumulate
#include <cxx_wrap.hpp>                   // libcxxwrap import to define Julia bindings
#include "xtensor-julia/jltensor.hpp"     // Import the jltensor container definition
#include "xtensor/xmath.hpp"              // xtensor import for the C++ universal functions

double sum_of_sines(xt::jltensor<double, 2> m)
{
    auto sines = xt::sin(m);  // sines does not actually hold values.
    return std::accumulate(sines.cbegin(), sines.cend(), 0.0);
}

JLCXX_MODULE define_julia_module(jlcxx::Module& mod)
{
    mod.method("sum_of_sines", sum_of_sines);
}
```

**Julia Code**

```julia
using xtensor_julia_test

arr = [[1.0 2.0]
       [3.0 4.0]]

s = sum_of_sines(arr)
s
```

**Outputs**

```
1.1350859243855171
```

### Example 2: Create a numpy-style universal function from a C++ scalar function

**C++ code**

```cpp
#include <cxx_wrap.hpp>
#include "xtensor-julia/jlvectorize.hpp"

double scalar_func(double i, double j)
{
    return std::sin(i) - std::cos(j);
}

JLCXX_MODULE define_julia_module(jlcxx::Module& mod)
{
    mod.method("vectorized_func", xt::jlvectorize(scalar_func));
}
```

**Julia Code**

```julia
using xtensor_julia_test

x = [[ 0.0  1.0  2.0  3.0  4.0]
     [ 5.0  6.0  7.0  8.0  9.0]
     [10.0 11.0 12.0 13.0 14.0]]
y = [1.0, 2.0, 3.0, 4.0, 5.0]
z = xt.vectorized_func(x, y)
z
```

**Outputs**

```
[[-0.540302  1.257618  1.89929   0.794764 -1.040465],
 [-1.499227  0.136731  1.646979  1.643002  0.128456],
 [-1.084323 -0.583843  0.45342   1.073811  0.706945]]
```

## Building the HTML Documentation

`xtensor-julia`'s documentation is built with three tools

 - [doxygen](http://www.doxygen.org)
 - [sphinx](http://www.sphinx-doc.org)
 - [breathe](https://breathe.readthedocs.io)

While doxygen must be installed separately, you can install breathe by typing

```bash
pip install breathe
```

Breathe can also be installed with `conda`

```bash
conda install -c conda-forge breathe
```

Finally, build the documentation with

```bash
make html
```

from the `docs` subdirectory.

## Running the C++ tests

From `deps/build`

```
cmake -D JlCxx_DIR=/path/to/.julia/v1.1/CxxWrap/deps/usr/lib/cmake/JlCxx -D BUILD_TESTS=ON ..
```

## Dependencies on `xtensor`, `xtensor-julia`, and `CxxWrap`

`Xtensor.jl` depends on `xtensor-julia`, `xtensor` and `CxxWrap` libraries

| `Xtensor.jl` | `xtensor`      | `xtensor-julia` | `CxxWrap`    |
|--------------|----------------|-----------------|--------------|
| master       | >=0.20.8,<0.21 | 0.8.4           | >=0.8.1,<0.9 |
| 0.8.2        | >=0.20.8,<0.21 | 0.8.4           | >=0.8.1,<0.9 |
| 0.8.1        | >=0.20.4,<0.21 | 0.8.2           | >=0.8.1,<0.9 |
| 0.8.0        | >=0.20.4,<0.21 | 0.8.2           | >=0.8.1,<0.9 |
| 0.7.0        | >=0.19.0,<0.20 | 0.7.0           | >=0.8.1,<0.9 |
| 0.6.2        | >=0.18.3,<0.19 | 0.6.2           | >=0.8.1,<0.9 |
| 0.6.1        | >=0.18.1,<0.19 | 0.6.0           | >=0.8.1,<0.9 |
| 0.6.0        | >=0.18.1,<0.19 | 0.6.0           | >=0.8.1,<0.9 |

These dependencies are automatically resolved when using the Julia package manager.

## License

We use a shared copyright model that enables all contributors to maintain the copyright on their contributions.

This software is licensed under the BSD-3-Clause license. See the [LICENSE](LICENSE) file for details.
