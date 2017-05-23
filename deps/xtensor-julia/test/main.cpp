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
#if JULIA_VERSION_MAJOR == 0 && JULIA_VERSION_MINOR < 6
    jl_init(NULL);
#else
    jl_init();
#endif

    ::testing::InitGoogleTest(&argc, argv);

    // Run test suite
    int ret = RUN_ALL_TESTS();

    // Closure of the Julia interpreter
    jl_atexit_hook(0);
    return ret;
}

