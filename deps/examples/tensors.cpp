#include <cxx_wrap.hpp>

#include "xtensor-julia/jltensor.hpp"

namespace tensors
{
    double test_access(const xt::jltensor<double, 2> m)
    {
        return m(1, 1);
    }

    void init_tensor_module(cxx_wrap::Module& mod)
    {
        // Test jltensor
        mod.method("test_access", test_access);
    }
}

JULIA_CPP_MODULE_BEGIN(registry)
  tensors::init_tensor_module(registry.create_module("TensorFunctions"));
JULIA_CPP_MODULE_END
