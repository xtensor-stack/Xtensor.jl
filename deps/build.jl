using BinDeps
using CxxWrap

@BinDeps.setup

build_type  = "Release"

jlcxx_cmake_dir   = joinpath(dirname(pathof(CxxWrap)), "..", "deps",   "usr", "lib", "cmake", "JlCxx")
xtl_cmake_dir     = joinpath(dirname(@__FILE__), "usr", "lib", "cmake", "xtl")
xtensor_cmake_dir = joinpath(dirname(@__FILE__), "usr", "lib", "cmake", "xtensor")
julia_bindir      = Sys.BINDIR

prefix                    = joinpath(dirname(@__FILE__), "usr")
xtl_srcdir                = joinpath(dirname(@__FILE__), "xtl")
xtensor_srcdir            = joinpath(dirname(@__FILE__), "xtensor")
xtensor_julia_srcdir      = joinpath(dirname(@__FILE__), "xtensor-julia")
xtensor_examples_srcdir   = joinpath(dirname(@__FILE__), "xtensor-julia-examples")

xtl_builddir              = joinpath(dirname(@__FILE__), "..", "builds", "xtl")
xtensor_builddir          = joinpath(dirname(@__FILE__), "..", "builds", "xtensor")
xtensor_julia_builddir    = joinpath(dirname(@__FILE__), "..", "builds", "xtensor-julia")
xtensor_examples_builddir = joinpath(dirname(@__FILE__), "..", "builds", "xtensor-julia-examples")

# Set generator if on windows
@static if Sys.iswindows()
    genopt = "MinGW Makefiles"
else
    genopt = "Unix Makefiles"
end

# Build on windows: push BuildProcess into BinDeps defaults
@static if Sys.iswindows()
  if haskey(ENV, "BUILD_ON_WINDOWS") && ENV["BUILD_ON_WINDOWS"] == "1"
    saved_defaults = deepcopy(BinDeps.defaults)
    empty!(BinDeps.defaults)
    append!(BinDeps.defaults, [BuildProcess])
  end
end

# Functions library for testing
example_labels = [:tensors]
xtensor_examples = BinDeps.LibraryDependency[]
for l in example_labels
   @eval $l = $(library_dependency(string(l), aliases=["lib" * string(l)]))
   push!(xtensor_examples, eval(:($l)))
end

# Version of xtl to vendor
xtl_version = "0.6.5"

# Version of xtensor to vendor
xtensor_version = "0.20.8"

# Version of xtensor-julia to vendor
xtensor_julia_version = "0.8.4"

xtl_steps = @build_steps begin
  `git clone -b $xtl_version --single-branch https://github.com/QuantStack/xtl $xtl_srcdir`
  `cmake -G "$genopt" -DCMAKE_INSTALL_PREFIX="$prefix" -DBUILD_TESTS=OFF -DCMAKE_INSTALL_LIBDIR=lib $xtl_srcdir`
  `cmake --build . --config $build_type --target install`
end

xtensor_steps = @build_steps begin
  `git clone -b $xtensor_version --single-branch https://github.com/QuantStack/xtensor $xtensor_srcdir`
  `cmake -G "$genopt" -DCMAKE_INSTALL_PREFIX="$prefix" -DBUILD_TESTS=OFF -DCMAKE_INSTALL_LIBDIR=lib $xtensor_srcdir`
  `cmake --build . --config $build_type --target install`
end

xtensor_julia_steps = @build_steps begin
  `git clone -b $xtensor_julia_version --single-branch https://github.com/QuantStack/xtensor-julia $xtensor_julia_srcdir`
  `cmake -G "$genopt" -DCMAKE_INSTALL_PREFIX="$prefix" -DJlCxx_DIR=$jlcxx_cmake_dir -Dxtensor_DIR=$xtensor_cmake_dir -DCMAKE_PROGRAM_PATH=$julia_bindir -DCMAKE_INSTALL_LIBDIR=lib $xtensor_julia_srcdir`
  `cmake --build . --config $build_type --target install`
end

xtensor_examples_steps = @build_steps begin
  `cmake -G "$genopt" -DCMAKE_PREFIX_PATH=$prefix -DCMAKE_INSTALL_PREFIX=$prefix -DCMAKE_BUILD_TYPE="$build_type" -DJlCxx_DIR=$jlcxx_cmake_dir -Dxtensor_DIR=$xtensor_cmake_dir -DCMAKE_PROGRAM_PATH=$julia_bindir -DCMAKE_INSTALL_LIBDIR=lib $xtensor_examples_srcdir`
  `cmake --build . --config $build_type --target install`
end

provides(BuildProcess,
  (@build_steps begin

    println("Building xtl")
    CreateDirectory(xtl_builddir)
    @build_steps begin
      ChangeDirectory(xtl_builddir)
      xtl_steps
    end

    println("Building xtensor")
    CreateDirectory(xtensor_builddir)
    @build_steps begin
      ChangeDirectory(xtensor_builddir)
      xtensor_steps
    end

    println("Building xtensor-julia")
    CreateDirectory(xtensor_julia_builddir)
    @build_steps begin
      ChangeDirectory(xtensor_julia_builddir)
      xtensor_julia_steps
    end

    println("Building xtensor-julia-examples")
    CreateDirectory(xtensor_examples_builddir)
    @build_steps begin
      ChangeDirectory(xtensor_examples_builddir)
      xtensor_examples_steps
    end
  end), xtensor_examples)

@BinDeps.install Dict([
    (:tensors, :_l_tensors)
])

# Build on windows: pop BuildProcess from BinDeps defaults
@static if Sys.iswindows()
  if haskey(ENV, "BUILD_ON_WINDOWS") && ENV["BUILD_ON_WINDOWS"] == "1"
    empty!(BinDeps.defaults)
    append!(BinDeps.defaults, saved_defaults)
  end
end
