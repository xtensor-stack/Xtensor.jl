/***************************************************************************
* Copyright (c) 2016, Johan Mabille and Sylvain Corlay                     *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#include <julia.h>

#include "gtest/gtest.h"

#include <iostream>

int main(int argc, char* argv[])
{
    // Initialize all the things (google-test and Julia interpreter)
    jl_init(NULL);
    ::testing::InitGoogleTest(&argc, argv);

    // Run test suite
    int ret = RUN_ALL_TESTS();

    // Closure of the Python interpreter
    jl_atexit_hook(0);
    return ret;
}

