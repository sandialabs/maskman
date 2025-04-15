# Copyright (c) 2022, Sandia National Laboratories
# All rights reserved.

cmake_minimum_required(VERSION 3.19)

set(INSTALL_NAME "default" CACHE STRING "This is the installation name to organize multiple builds side-by-side.")

# set the project name
project(maskman VERSION 1.0.3 LANGUAGES C)

# turn on CTest
include(CTest)
enable_testing()

# so packages can find artifacts
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# create compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# add compile-time application types

# build app
option(Maskman_Application "Build Maskman Application" ON)

# build documentation
option(Maskman_Documentation "Build Maskman Documentation" OFF)

# maximum buffer size for methods that may need it
set(Maskman_Buffer_Size_Bits 2048 CACHE STRING "Maskman Buffer Size (bits)")

# automatically set GNU-based code coverage to be off
option(GNU_COVERAGE "GNU Coverage" OFF)

# automatically set LLVM-based code coverage to be off
option(LLVM_COVERAGE "LLVM Coverage" OFF)

# automatically set flags for GNU Address Sanitizer
option(GNU_ASAN "GNU Address Sanitizer" OFF)

# automatically set flags for LLVM Address Sanitizer
option(LLVM_ASAN "LLVM Address Sanitizer" OFF)

# automatically set flags for LLVM Clang Tidy Static Analyzer
option(LLVM_CLANG_TIDY "LLVM Clang Tidy Static Analyzer" OFF)

# automatically set flags to turn on pedantic warnings
option(WARNING "Increase warnings and treat them as errors" OFF)

if(WARNING)
  add_compile_options(-Wall -Wextra -Wpedantic)
  set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
endif(WARNING)

if(LLVM_COVERAGE)
  add_compile_options(-fprofile-instr-generate -fcoverage-mapping)
  add_link_options(-fprofile-instr-generate -fcoverage-mapping)
endif(LLVM_COVERAGE)

if(GNU_COVERAGE)
  add_compile_options(--coverage -fprofile-arcs -ftest-coverage)
  add_link_options(--coverage -lgcov)
endif(GNU_COVERAGE)

if(GNU_ASAN)
  add_compile_options(-fsanitize=address -fno-omit-frame-pointer )
  add_link_options(-fsanitize=address)
  if(UNIX AND NOT APPLE)
    add_link_options(-static-libasan)
  endif()
endif(GNU_ASAN)

if(LLVM_ASAN)
  # add_compile_options(-fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls -fsanitize-ignorelist=${CMAKE_CURRENT_SOURCE_DIR}/test/asan/llvm_asan_ignorelist.txt)
  # add_compile_options(-fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls)
  add_compile_options(-fsanitize=address -fno-omit-frame-pointer )
  add_link_options(-fsanitize=address)
endif(LLVM_ASAN)

# add projects
if(Maskman_Application)
  add_subdirectory(libKillist)
  add_subdirectory(libMaskman)
  add_subdirectory(methodLUIS)
  add_subdirectory(methodSimpleSTDOUT)
  add_subdirectory(appMaskman)
endif(Maskman_Application)
if(Maskman_Documentation)
  add_subdirectory(doc)
endif(Maskman_Documentation)

# manage CMake export
if(Maskman_Application)
  set(cmakeModulesDir cmake)
  set(Maskman_Application_Architecture_Display ${CMAKE_HOST_SYSTEM_PROCESSOR})
  if(COSMOPOLITAN)
    set(Maskman_Application_Architecture_Display ape)
  endif(COSMOPOLITAN)
  install(
    EXPORT MaskmanTargets
    FILE MaskmanTargets.cmake
    NAMESPACE Maskman::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Maskman
    )
  configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/MaskmanConfig.cmake.in" 
    "${CMAKE_CURRENT_BINARY_DIR}/MaskmanConfig.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/Maskman"
    PATH_VARS cmakeModulesDir
    NO_SET_AND_CHECK_MACRO
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
    )
  write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/MaskmanConfigVersion.cmake"
    VERSION "${maskman_VERSION_MAJOR}.${maskman_VERSION_MINOR}.${maskman_VERSION_PATCH}"
    COMPATIBILITY AnyNewerVersion
    )
  install(
    FILES
      "${CMAKE_CURRENT_BINARY_DIR}/MaskmanConfig.cmake"
      "${CMAKE_CURRENT_BINARY_DIR}/MaskmanConfigVersion.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/Maskman"
    )
  export(
    EXPORT MaskmanTargets
    FILE "${CMAKE_CURRENT_BINARY_DIR}/MaskmanTargets.cmake"
    )

  # use CPack for creating packages
  get_filename_component(PACK_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}"
                         REALPATH BASE_DIR "${CMAKE_BINARY_DIR}")
  include(InstallRequiredSystemLibraries)
  set(CPACK_PACKAGE_NAME maskman)
  set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
  set(CPACK_PACKAGE_VERSION_MAJOR "${maskman_VERSION_MAJOR}")
  set(CPACK_PACKAGE_VERSION_MINOR "${maskman_VERSION_MINOR}")
  set(CPACK_PACKAGE_VERSION_PATCH "${maskman_VERSION_PATCH}")
  set(CPACK_GENERATOR "TGZ")
  set(CPACK_SOURCE_GENERATOR "TGZ")
  set(CPACK_PACKAGE_DIRECTORY ${PACK_INSTALL_DIR}/package)
  set(CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME}-v${maskman_VERSION}-${CMAKE_SYSTEM_NAME}-${Maskman_Application_Architecture_Display})
  include(CPack)
endif(Maskman_Application)
