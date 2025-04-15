# Copyright (c) 2022, Sandia National Laboratories
# All rights reserved.

cmake_minimum_required(VERSION 3.21)

set(ListSimpleSTDOUT libSimpleSTDOUT.c)

# Create a static library called "libSimpleSTDOUT".
add_library(SimpleSTDOUT STATIC ${ListSimpleSTDOUT})
set_target_properties(SimpleSTDOUT PROPERTIES
  PUBLIC_HEADER "libSimpleSTDOUT.h"
  OUTPUT_NAME SimpleSTDOUT
  ARCHIVE_OUTPUT_DIRECTORY static
  VERSION ${maskman_VERSION}
  )
# install(TARGETS SimpleSTDOUT EXPORT MaskmanTargets)
target_include_directories(
  SimpleSTDOUT
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  )
set_property(TARGET SimpleSTDOUT PROPERTY C_STANDARD 11)
target_link_libraries(SimpleSTDOUT LINK_PUBLIC Maskman)
target_link_libraries(SimpleSTDOUT LINK_PUBLIC Killist)

if(LLVM_CLANG_TIDY)
  find_program(CLANG_TIDY_EXE NAMES "clang-tidy" "clang-tidy17" "clang-tidy-mp-17")
  set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}" "-config-file=${CMAKE_SOURCE_DIR}/.clang-tidy" "-warnings-as-errors=*")
  set_target_properties(SimpleSTDOUT PROPERTIES C_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
endif()

