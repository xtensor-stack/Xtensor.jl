__precompile__()

module xtensor_julia

using CxxWrap

const depsfile = joinpath(dirname(dirname(@__FILE__)), "deps", "deps.jl")
if !isfile(depsfile)
  error("$depsfile not found, package xtensor-julia did not build properly")
end
include(depsfile)

end
