using BinDeps

@BinDeps.setup

build_type = "Release"
cxx_wrap_dir = Pkg.dir("CxxWrap", "deps", "usr", "lib", "cmake")
xtensor_dir = Pkg.dir("Xtensor", "deps", "usr", "lib", "cmake")

prefix                 = Pkg.dir("Xtensor", "deps", "usr")
xtensor_core_srcdir    = Pkg.dir("Xtensor", "builds", "xtensor_src")
xtensor_julia_srcdir   = Pkg.dir("Xtensor", "deps")
xtensor_core_builddir  = Pkg.dir("Xtensor", "builds", "xtensor")
xtensor_julia_builddir = Pkg.dir("Xtensor", "builds", "xtensor-julia")

makeopts = ["--", "-j", "$(Sys.CPU_CORES+2)"]

# Set generator if on windows
genopt = "Unix Makefiles"

# Functions library for testing
example_labels = [:tensors]
xtensorjl = BinDeps.LibraryDependency[]
for l in example_labels
   @eval $l = $(library_dependency(string(l), aliases=["lib"*string(l)]))
   push!(xtensorjl, eval(:($l)))
end
xtensorjl_srcdir = joinpath(BinDeps.depsdir(tensors), "examples")
xtensorjl_builddir = joinpath(BinDeps.depsdir(tensors), "builds", "examples")

xtensor_version = "0.9.0"

xtensor_core_steps = @build_steps begin
  `git clone -b $xtensor_version --single-branch https://github.com/QuantStack/xtensor $xtensor_core_srcdir`
  `cmake -G "$genopt" -DCMAKE_INSTALL_PREFIX="$prefix" -DBUILD_TESTS=OFF $xtensor_core_srcdir`
  `make install`
end

xtensor_julia_steps = @build_steps begin
  `cmake -G "$genopt" -DCMAKE_INSTALL_PREFIX="$prefix" $xtensor_julia_srcdir`
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

xtensorjl_steps = @build_steps begin
  `cmake -G "$genopt" -DCMAKE_INSTALL_PREFIX="$prefix" -DCMAKE_BUILD_TYPE="$build_type" -DLIBDIR_SUFFIX=$libdir_opt -DCxxWrap_DIR=$cxx_wrap_dir -Dxtensor_DIR=$xtensor_dir $xtensorjl_srcdir`
  `cmake --build . --config $build_type --target install $makeopts`
end

provides(BuildProcess,
  (@build_steps begin

    println("Building xtensor-core")
    CreateDirectory(xtensor_core_builddir)
    @build_steps begin
      ChangeDirectory(xtensor_core_builddir)
      xtensor_core_steps
    end

    println("Building xtensor-julia")
    CreateDirectory(xtensor_julia_builddir)
    @build_steps begin
      ChangeDirectory(xtensor_julia_builddir)
      xtensor_julia_steps
    end

    println("Building xtensorjl")
    CreateDirectory(xtensorjl_builddir)
    @build_steps begin
      ChangeDirectory(xtensorjl_builddir)
      xtensorjl_steps
    end
  end), xtensorjl)

@BinDeps.install Dict([
    (:tensors, :_l_tensors)
])
