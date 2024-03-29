include(FetchContent_MakeAvailableExcludeFromAll)

if(NOT DISABLE_LTO)
  # Force CMake to raise an error if INTERPROCEDURAL_OPTIMIZATION
  # is enabled and compiler does not support IPO
  set(CMAKE_POLICY_DEFAULT_CMP0069 NEW)
endif()

if(DEVILUTIONX_STATIC_LIBPNG)
  set(PNG_LIBRARY png_static)
  set(PNG_STATIC ON CACHE BOOL "Build static lib" FORCE)
  set(PNG_SHARED OFF CACHE BOOL "Build shared lib" FORCE)
else()
  set(PNG_LIBRARY png)
  set(PNG_STATIC OFF CACHE BOOL "Build static lib" FORCE)
  set(PNG_SHARED ON CACHE BOOL "Build shared lib" FORCE)
endif()
set(PNG_TESTS OFF CACHE BOOL "Build libpng tests")

include(FetchContent)
FetchContent_Declare(png
  URL https://github.com/StephenCWills/libpng/archive/5cfc16530ae7f2c1f1c88ecd08b0fde2e2a4cd93.tar.gz
  URL_HASH MD5=e98756c6316f51ca9c045bb2f1bdf307
)
FetchContent_MakeAvailableExcludeFromAll(png)

target_include_directories(${PNG_LIBRARY} INTERFACE
  $<BUILD_INTERFACE:${png_SOURCE_DIR}>
  $<BUILD_INTERFACE:${png_BINARY_DIR}>)

add_library(PNG::PNG ALIAS ${PNG_LIBRARY})
