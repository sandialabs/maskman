# Copyright (c) 2022, Sandia National Laboratories
# All rights reserved.

cmake_minimum_required(VERSION 3.21)

set(ListMaskman libMaskman.c)

# Create a static library called "libMaskman".
add_library(Maskman STATIC ${ListMaskman})
set_target_properties(Maskman PROPERTIES
  PUBLIC_HEADER "libMaskman.h"
  OUTPUT_NAME Maskman
  ARCHIVE_OUTPUT_DIRECTORY static
  VERSION ${maskman_VERSION}
  )
# install(TARGETS Maskman EXPORT MaskmanTargets)
target_include_directories(
  Maskman
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  )
set_property(TARGET Maskman PROPERTY C_STANDARD 99)

if(LLVM_CLANG_TIDY)
  find_program(CLANG_TIDY_EXE NAMES "clang-tidy" "clang-tidy17" "clang-tidy-mp-17")
  set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}" "-config-file=${CMAKE_SOURCE_DIR}/.clang-tidy" "-warnings-as-errors=*")
  set_target_properties(Maskman PROPERTIES C_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
endif()

