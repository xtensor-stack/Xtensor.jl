
if(Julia_FOUND)
    return()
endif()

#################### 
# Julia Executable #
####################

find_program(Julia_EXECUTABLE julia DOC "Julia executable")
if(NOT Julia_EXECUTABLE)
    return()
endif()

################# 
# Julia Version #
################# 

execute_process(
    COMMAND ${Julia_EXECUTABLE} --version
    OUTPUT_VARIABLE Julia_VERSION_STRING
    RESULT_VARIABLE RETURN_CODE
)
if(RETURN_CODE EQUAL 0)
    string(
      REGEX REPLACE ".*([0-9]+\\.[0-9]+\\.[0-9]+).*" "\\1"
        Julia_VERSION_STRING ${Julia_VERSION_STRING}
    )
else()
    return()
endif()

################## 
# Julia Includes #
################## 

execute_process(
    COMMAND ${Julia_EXECUTABLE} -E "julia_include_dir = joinpath(match(r\"(.*)(bin)\",JULIA_HOME).captures[1],\"include\",\"julia\")\n
        if !isdir(julia_include_dir)  # then we're running directly from build\n
          julia_base_dir_aux = splitdir(splitdir(JULIA_HOME)[1])[1]  # useful for running-from-build\n
          julia_include_dir = joinpath(julia_base_dir_aux, \"usr\", \"include\" )\n
          julia_include_dir *= \";\" * joinpath(julia_base_dir_aux, \"src\", \"support\" )\n
          julia_include_dir *= \";\" * joinpath(julia_base_dir_aux, \"src\" )\n
        end\n
        julia_include_dir"
    OUTPUT_VARIABLE Julia_INCLUDE_DIRS
    RESULT_VARIABLE RETURN_CODE
)
if(RETURN_CODE EQUAL 0)
    string(REGEX REPLACE "\"" "" Julia_INCLUDE_DIRS ${Julia_INCLUDE_DIRS})
    string(REGEX REPLACE "\n" "" Julia_INCLUDE_DIRS ${Julia_INCLUDE_DIRS})
    set(Julia_INCLUDE_DIRS ${Julia_INCLUDE_DIRS}
        CACHE PATH "Location of Julia include files")
else()
    return()
endif()

################### 
# Julia Libraries #
################### 

execute_process(
    COMMAND ${Julia_EXECUTABLE} -E "abspath(dirname(Libdl.dlpath(\"libjulia\")))"
    OUTPUT_VARIABLE Julia_LIBRARY_DIR
    RESULT_VARIABLE RETURN_CODE
)
if(RETURN_CODE EQUAL 0)
    string(REGEX REPLACE "\"" "" Julia_LIBRARY_DIR ${Julia_LIBRARY_DIR})
    string(REGEX REPLACE "\n" "" Julia_LIBRARY_DIR ${Julia_LIBRARY_DIR})
    string(STRIP ${Julia_LIBRARY_DIR} Julia_LIBRARY_DIR)
    set(Julia_LIBRARY_DIR ${Julia_LIBRARY_DIR}
        CACHE PATH "Julia library directory")
else()
    return()
endif()

find_library(Julia_LIBRARY
    NAMES julia
    PATHS ${Julia_LIBRARY_DIR}
)

############## 
# JULIA_HOME #
############## 

execute_process(
    COMMAND ${Julia_EXECUTABLE} -E "JULIA_HOME"
    OUTPUT_VARIABLE JULIA_HOME
    RESULT_VARIABLE RETURN_CODE
)

if(RETURN_CODE EQUAL 0)
    string(REGEX REPLACE "\"" "" JULIA_HOME ${JULIA_HOME})
    string(REGEX REPLACE "\n" "" JULIA_HOME ${JULIA_HOME})
else()
    return()
endif()

###################
# libLLVM version #
###################

execute_process(
    COMMAND ${Julia_EXECUTABLE} -E "Base.libllvm_version"
    OUTPUT_VARIABLE Julia_LLVM_VERSION
    RESULT_VARIABLE RETURN_CODE
)

if(RETURN_CODE EQUAL 0)
    string(REGEX REPLACE "\"" "" Julia_LLVM_VERSION ${Julia_LLVM_VERSION})
    string(REGEX REPLACE "\n" "" Julia_LLVM_VERSION ${Julia_LLVM_VERSION})
else()
    return()
endif()

##################################
# Check for Existence of Headers #
##################################

find_path(Julia_MAIN_HEADER julia.h HINTS ${Julia_INCLUDE_DIRS})
find_library(Julia_LIBRARY julia HINTS ${Julia_LIBRARY_DIR})

###########################
# FindPackage Boilerplate #
###########################

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Julia
    REQUIRED_VARS   Julia_LIBRARY Julia_LIBRARY_DIR Julia_INCLUDE_DIRS
    VERSION_VAR     Julia_VERSION_STRING
    FAIL_MESSAGE    "Julia not found"
)
