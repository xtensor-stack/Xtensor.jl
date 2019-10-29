using BinDeps
using CxxWrap

@BinDeps.setup

# Build on windows: PUSH BuildProcess into BinDeps defaults
# Note: this is undone in the end of the script
@static if Sys.iswindows()
  if haskey(ENV, "BUILD_ON_WINDOWS") && ENV["BUILD_ON_WINDOWS"] == "1"
    saved_defaults = deepcopy(BinDeps.defaults)
    empty!(BinDeps.defaults)
    append!(BinDeps.defaults, [BuildProcess])
  end
end

# Set generator if on windows
@static if Sys.iswindows()
    genopt = "MinGW Makefiles"
else
    genopt = "Unix Makefiles"
end

# Functions library for testing
example_labels = [:tensors]
xtensor_examples = BinDeps.LibraryDependency[]
for l in example_labels
  @eval $l = $(library_dependency(string(l), aliases=["lib" * string(l)]))
  push!(xtensor_examples, eval(:($l)))
end

# If the XTENSOR_PREFIX environment variable is not set
# then we vendor xtl, xtensor, and xtensor-julia.
# If it is set, then it is correpsonds to the prefix path
# for all these dependencies.

const XTENSOR_PREFIX = get(ENV, "XTENSOR_PREFIX", "")
const BUILD_TYPE = "Release"
const VENDOR_PREFIX = joinpath(dirname(@__FILE__), "usr")

if XTENSOR_PREFIX == ""
  JLCXX_CMAKE_DIR   = joinpath(dirname(pathof(CxxWrap)), "..", "deps",   "usr", "lib", "cmake", "JlCxx")

  xtl_cmake_dir             = joinpath(dirname(@__FILE__), "usr", "lib", "cmake", "xtl")
  xtensor_cmake_dir         = joinpath(dirname(@__FILE__), "usr", "lib", "cmake", "xtensor")
  julia_bindir              = Sys.BINDIR

  xtl_srcdir                = joinpath(dirname(@__FILE__), "xtl")
  xtensor_srcdir            = joinpath(dirname(@__FILE__), "xtensor")
  xtensor_julia_srcdir      = joinpath(dirname(@__FILE__), "xtensor-julia")
  xtensor_examples_srcdir   = joinpath(dirname(@__FILE__), "xtensor-julia-examples")

  xtl_builddir              = joinpath(dirname(@__FILE__), "..", "builds", "xtl")
  xtensor_builddir          = joinpath(dirname(@__FILE__), "..", "builds", "xtensor")
  xtensor_julia_builddir    = joinpath(dirname(@__FILE__), "..", "builds", "xtensor-julia")
  xtensor_examples_builddir = joinpath(dirname(@__FILE__), "..", "builds", "xtensor-julia-examples")

  # Versions of xtl, xtensor and xtensor-julia to vendor
  xtl_version = "0.6.5"
  xtensor_version = "0.20.8"
  xtensor_julia_version = "0.8.4"

  xtl_steps = @build_steps begin
    `git clone -b $xtl_version --single-branch https://github.com/xtensor-stack/xtl $xtl_srcdir`
    `cmake -G "$genopt" -DCMAKE_INSTALL_PREFIX="$VENDOR_PREFIX" -DBUILD_TESTS=OFF -DCMAKE_INSTALL_LIBDIR=lib $xtl_srcdir`
    `cmake --build . --config $BUILD_TYPE --target install`
  end

  xtensor_steps = @build_steps begin
    `git clone -b $xtensor_version --single-branch https://github.com/xtensor-stack/xtensor $xtensor_srcdir`
    `cmake -G "$genopt" -DCMAKE_INSTALL_PREFIX="$VENDOR_PREFIX" -DBUILD_TESTS=OFF -DCMAKE_INSTALL_LIBDIR=lib $xtensor_srcdir`
    `cmake --build . --config $BUILD_TYPE --target install`
  end

  xtensor_julia_steps = @build_steps begin
    `git clone -b $xtensor_julia_version --single-branch https://github.com/xtensor-stack/xtensor-julia $xtensor_julia_srcdir`
    `cmake -G "$genopt" -DCMAKE_INSTALL_PREFIX="$VENDOR_PREFIX" -DJlCxx_DIR=$JLCXX_CMAKE_DIR -Dxtensor_DIR=$xtensor_cmake_dir -DCMAKE_PROGRAM_PATH=$julia_bindir -DCMAKE_INSTALL_LIBDIR=lib $xtensor_julia_srcdir`
    `cmake --build . --config $BUILD_TYPE --target install`
  end

  xtensor_examples_steps = @build_steps begin
    `cmake -G "$genopt" -DCMAKE_INSTALL_PREFIX="$VENDOR_PREFIX" -D CMAKE_PREFIX_PATH="$VENDOR_PREFIX" -DCMAKE_BUILD_TYPE="$BUILD_TYPE" -DJlCxx_DIR=$JLCXX_CMAKE_DIR -Dxtensor_DIR=$xtensor_cmake_dir -DCMAKE_PROGRAM_PATH=$julia_bindir -DCMAKE_INSTALL_LIBDIR=lib $xtensor_examples_srcdir`
    `cmake --build . --config $BUILD_TYPE --target install`
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
else
  julia_bindir              = Sys.BINDIR
  xtensor_examples_srcdir   = joinpath(dirname(@__FILE__), "xtensor-julia-examples")
  xtensor_examples_builddir = joinpath(dirname(@__FILE__), "..", "builds", "xtensor-julia-examples")
  xtensor_examples_steps = @build_steps begin
    `cmake -G "$genopt" -DCMAKE_INSTALL_PREFIX="$VENDOR_PREFIX" -DCMAKE_PREFIX_PATH="$XTENSOR_PREFIX" -DCMAKE_BUILD_TYPE="$BUILD_TYPE" -DCMAKE_PROGRAM_PATH=$julia_bindir -DCMAKE_INSTALL_LIBDIR=lib $xtensor_examples_srcdir`
    `cmake --build . --config $BUILD_TYPE --target install`
  end

  provides(BuildProcess,
    (@build_steps begin
      println("Building xtensor-julia-examples")
      CreateDirectory(xtensor_examples_builddir)
      @build_steps begin
        ChangeDirectory(xtensor_examples_builddir)
        xtensor_examples_steps
      end
    end), xtensor_examples)
end

@BinDeps.install Dict([
    (:tensors, :_l_tensors)
])

# Build on windows: POP BuildProcess from BinDeps defaults
@static if Sys.iswindows()
  if haskey(ENV, "BUILD_ON_WINDOWS") && ENV["BUILD_ON_WINDOWS"] == "1"
    empty!(BinDeps.defaults)
    append!(BinDeps.defaults, saved_defaults)
  end
end
