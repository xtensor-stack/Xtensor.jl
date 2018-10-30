############################################################################
# Copyright (c) 2016, Johan Mabille and Sylvain Corlay                     #
#                                                                          #
# Distributed under the terms of the BSD 3-Clause License.                 #
#                                                                          #
# The full license is in the file LICENSE, distributed with this software. #
############################################################################

using Test

module TensorFunctions
using CxxWrap
using Xtensor
@wrapmodule Xtensor._l_tensors
end

# Test example functions from the TensorFunctions module

# example1
arr = [[1.0 2.0]
       [3.0 4.0]]
@test TensorFunctions.test_access(arr) == 4.0
@test TensorFunctions.example1([4., 5., 6.]) == 4.0

# example2
x = [[0. 1.]
     [2. 3.]]
res = [[2. 3.]
       [4. 5.]]
y = TensorFunctions.example2(x)
@test y ≈ res atol=1e-12

# vectorize_example1
x1 = [[0 1]
      [2 3]]
x2 = [0, 1]
res = [[0 2]
       [2 4]]
y = TensorFunctions.vectorize_example1(x1, x2)
@test y == res

# readme_example1
v = [[0.0  3.0  6.0   9.0  12.0]
     [1.0  4.0  7.0  10.0  13.0]
     [2.0  5.0  8.0  11.0  14.0]]

y = TensorFunctions.readme_example1(v)
@test y ≈ 1.2853996391883833 atol=1e-12

# readme_example2
x = [[ 0.0  1.0  2.0  3.0  4.0]
     [ 5.0  6.0  7.0  8.0  9.0]
     [10.0 11.0 12.0 13.0 14.0]]
y =  [1.0, 2.0, 3.0, 4.0, 5.0]
z = TensorFunctions.readme_example2(x, y)
expected = [[-0.540302  1.257618  1.89929   0.794764 -1.040465]
            [-1.499227  0.136731  1.646979  1.643002  0.128456]
            [-1.084323 -0.583843  0.45342   1.073811  0.706945]]
@test z ≈ expected atol=1e-5

# compare_shapes
x = ones(4, 4)
y = ones(5, 5)
z = zeros(4, 4)
@assert !TensorFunctions.compare_shapes(x, y)
@assert TensorFunctions.compare_shapes(x, z)
