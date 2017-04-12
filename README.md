# ![xtensor-julia](http://quantstack.net/assets/images/xtensor-julia.svg)

[![Join the Gitter Chat](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/QuantStack/Lobby?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

Julia bindings for the [xtensor](https://github.com/QuantStack/xtensor) C++ multi-dimensional array library.

 - `xtensor` is a C++ library for multi-dimensional arrays enabling numpy-style broadcasting and lazy computing.
 - `xtensor-julia` enables inplace use of julia arrays in C++ with all the benefits from `xtensor`

     - C++ universal function and broadcasting 
     - STL - compliant APIs.
     - A broad coverage of numpy APIs (see [the numpy to xtensor cheat sheet](http://xtensor.readthedocs.io/en/latest/numpy.html)).

The Julia bindings for `xtensor` are based on the [CxxWrap.jl](https://github.com/JuliaInterop/CxxWrap.jl/) C++ library.

Installation and testing
------------------------

This package is still at a very experimental stage.

```julia
Pkg.clone("https://github.com/QuantStack/xtensor-julia", "xtensor_julia");
Pkg.build("xtensor_julia")
Pkg.test("xtensor_julia")
```
