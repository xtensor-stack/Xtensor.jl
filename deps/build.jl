using BinDeps
using CxxWrap

@BinDeps.setup

build_type = get(ENV, "CXXWRAP_BUILD_TYPE", "Release")
cxx_wrap_dir = Pkg.dir("CxxWrap", "deps", "usr", "lib", "cmake")

xtensor_julia = library_dependency("xtensor_julia", aliases=["libxtensor_julia", "libxtensor_julia.a"])

prefix=joinpath(BinDeps.depsdir(xtensor_julia), "usr")
xtensor_julia_srcdir = BinDeps.depsdir(xtensor_julia)
xtensor_julia_builddir = joinpath(BinDeps.depsdir(xtensor_julia), "builds", "xtensor_julia")

makeopts = ["--", "-j", "$(Sys.CPU_CORES+2)"]

# Set generator if on windows
genopt = "Unix Makefiles"

# Functions library for testing
example_labels = [:tensors]
examples = BinDeps.LibraryDependency[]
for l in example_labels
   @eval $l = $(library_dependency(string(l), aliases=["lib"*string(l)]))
   push!(examples, eval(:($l)))
end
examples_srcdir = joinpath(BinDeps.depsdir(tensors), "examples")
examples_builddir = joinpath(BinDeps.depsdir(tensors), "builds", "examples")

xtensor_steps = @build_steps begin
  `cmake -G "$genopt" -DCMAKE_INSTALL_PREFIX="$prefix" -DBUILD_DUMMY=ON $xtensor_julia_srcdir`
  `cmake --build . --config $build_type --target install $makeopts`
end

libdir_opt = ""
@static if is_windows()
  libdir_opt = Sys.WORD_SIZE==32 ? "32" : ""
end
lib_prefix = @static is_windows() ? "" : "lib"
lib_suffix = @static is_windows() ? "dll" : (@static is_apple() ? "dylib" : "so")
example_paths = AbstractString[]
for l in example_labels
  push!(example_paths, joinpath(prefix,"lib$libdir_opt", "$(lib_prefix)$(string(l)).$lib_suffix"))
end

examples_steps = @build_steps begin
  `cmake -G "$genopt" -DCMAKE_INSTALL_PREFIX="$prefix" -DCMAKE_BUILD_TYPE="$build_type" -DLIBDIR_SUFFIX=$libdir_opt -DCxxWrap_DIR=$cxx_wrap_dir $examples_srcdir`
  `cmake --build . --config $build_type --target install $makeopts`
end

if isdir(xtensor_julia_builddir)
  BinDeps.run(@build_steps begin
    println("Build steps xtensor_julia")
    ChangeDirectory(xtensor_julia_builddir)
    xtensor_steps
  end)
end

if isdir(examples_builddir)
  BinDeps.run(@build_steps begin
    println("Build steps examples")
    ChangeDirectory(examples_builddir)
    examples_steps
  end)
end

provides(BuildProcess,
  (@build_steps begin
    println("Build process xtensor_julia")
    CreateDirectory(xtensor_julia_builddir)
    @build_steps begin
      ChangeDirectory(xtensor_julia_builddir)
      xtensor_steps
    end
  end), xtensor_julia)

provides(BuildProcess,
  (@build_steps begin
    println("Build process examples")
   CreateDirectory(examples_builddir)
   @build_steps begin
     ChangeDirectory(examples_builddir)
     examples_steps
   end
  end), examples)

@BinDeps.install Dict([
    (:tensors, :_l_tensors)
])

@static if is_windows()
  if build_on_windows
    empty!(BinDeps.defaults)
    append!(BinDeps.defaults, saved_defaults)
  end
end
