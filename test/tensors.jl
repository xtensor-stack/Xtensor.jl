############################################################################
# Copyright (c) 2016, Johan Mabille and Sylvain Corlay                     #
#                                                                          #
# Distributed under the terms of the BSD 3-Clause License.                 #
#                                                                          #
# The full license is in the file LICENSE, distributed with this software. #
############################################################################

using CxxWrap
using Base.Test
using xtensor_julia

wrap_modules(xtensor_julia._l_tensors)

arr = [[1.0 2.0]
       [3.0 4.0]]

# Test functions from the TensorFunctions module
@test TensorFunctions.test_access(arr) == 4.0

