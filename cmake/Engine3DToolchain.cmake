set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD
    "d0edc3af-4c50-42ea-a356-e2862fe7a444"
    CACHE STRING "Enable experimental `import std` support")
set(CMAKE_CXX_MODULE_STD ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_CXX_STDLIB_MODULES_JSON AND CMAKE_CXX_COMPILER MATCHES "clang")
    get_filename_component(_engine3d_llvm_bin_dir "${CMAKE_CXX_COMPILER}" DIRECTORY)
    get_filename_component(_engine3d_llvm_prefix "${_engine3d_llvm_bin_dir}" DIRECTORY)
    set(_engine3d_libcxx_modules_json "${_engine3d_llvm_prefix}/lib/c++/libc++.modules.json")
    if(EXISTS "${_engine3d_libcxx_modules_json}")
        set(CMAKE_CXX_STDLIB_MODULES_JSON "${_engine3d_libcxx_modules_json}")
    endif()
endif()
