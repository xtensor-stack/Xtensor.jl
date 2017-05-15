.. Copyright (c) 2017, Wolf Vollprecht, Johan Mabille and Sylvain Corlay

   Distributed under the terms of the BSD 3-Clause License.

   The full license is in the file LICENSE, distributed with this software.

.. image:: xtensor-julia.svg
   :alt: xtensor-julia

Julia bindings for the xtensor C++ multi-dimensional array library.

Introduction
------------

``xtensor`` is a C++ library for multi-dimensional arrays enabling numpy-style broadcasting and lazy computing.

``xtensor-julia`` enables inplace use of julia arrays in C++ with all the benefits from xtensor

 - C++ universal function and broadcasting
 - STL - compliant APIs.
 - A broad coverage of numpy APIs (see the numpy to xtensor cheat sheet).

The Julia bindings for xtensor are based on the CxxWrap.jl_ C++ library.

Licensing
---------

We use a shared copyright model that enables all contributors to maintain the
copyright on their contributions.

This software is licensed under the BSD-3-Clause license. See the LICENSE file for details.

.. toctree::
   :caption: USAGE
   :maxdepth: 2

   basic_usage
   array_tensor

.. toctree::
   :caption: API REFERENCE
   :maxdepth: 2

   api_reference

.. _CxxWrap.jl: https://github.com/JuliaInterop/CxxWrap.jl
