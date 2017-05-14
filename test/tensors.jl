############################################################################
# Copyright (c) 2016, Johan Mabille and Sylvain Corlay                     #
#                                                                          #
# Distributed under the terms of the BSD 3-Clause License.                 #
#                                                                          #
# The full license is in the file LICENSE, distributed with this software. #
############################################################################

using CxxWrap
using Base.Test
using Xtensor

wrap_modules(Xtensor._l_tensors)

arr = [[1.0 2.0]
       [3.0 4.0]]

# Test functions from the TensorFunctions module
@test TensorFunctions.test_access(arr) == 4.0

@test TensorFunctions.example1([4., 5., 6.]) == 4.0

x = [[0. 1.]
     [2. 3.]]
res = [[2. 3.]
       [4. 5.]]
y = TensorFunctions.example2(x)
@test_approx_eq_eps y res 1e-12

x1 = [[0 1]
      [2 3]]
x2 = [0, 1]
res = [[0 2]
       [2 4]]
y = TensorFunctions.vectorize_example1(x1, x2)
@test y == res

v = [[0.0  3.0  6.0   9.0  12.0]
     [1.0  4.0  7.0  10.0  13.0]
     [2.0  5.0  8.0  11.0  14.0]]

y = TensorFunctions.readme_example1(v)
@test_approx_eq_eps y 1.2853996391883833 1e-12

x = [[ 0.0  1.0  2.0  3.0  4.0]
     [ 5.0  6.0  7.0  8.0  9.0]
     [10.0 11.0 12.0 13.0 14.0]]
y =  [1.0, 2.0, 3.0, 4.0, 5.0]
z = TensorFunctions.readme_example2(x, y)
expected = [[-0.540302  1.257618  1.89929   0.794764 -1.040465]
            [-1.499227  0.136731  1.646979  1.643002  0.128456]
            [-1.084323 -0.583843  0.45342   1.073811  0.706945]]
@test_approx_eq_eps z expected 1e-5

x = ones(4, 4)
y = ones(5, 5)
z = zeros(4, 4)
@assert !TensorFunctions.compare_shapes(x, y)
@assert TensorFunctions.compare_shapes(x, z)
