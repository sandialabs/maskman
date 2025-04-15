# Copyright (c) 2022, Sandia National Laboratories
# All rights reserved.

cmake_minimum_required(VERSION 3.21)

set(ListLUIS libLUIS.c)

# Create a static library called "libLUIS".
add_library(LUIS STATIC ${ListLUIS})
set_target_properties(LUIS PROPERTIES
  PUBLIC_HEADER "libLUIS.h"
  OUTPUT_NAME LUIS
  ARCHIVE_OUTPUT_DIRECTORY static
  VERSION ${maskman_VERSION}
  )
# install(TARGETS LUIS EXPORT MaskmanTargets)
target_include_directories(
  LUIS
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  )
set_property(TARGET LUIS PROPERTY C_STANDARD 11)
target_compile_definitions(LUIS PUBLIC LUIS_BUFFER_SIZE=${Maskman_Buffer_Size_Bits})
target_link_libraries(LUIS PUBLIC Maskman)
target_link_libraries(LUIS PUBLIC Killist)

if(LLVM_CLANG_TIDY)
  find_program(CLANG_TIDY_EXE NAMES "clang-tidy" "clang-tidy17" "clang-tidy-mp-17")
  set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}" "-config-file=${CMAKE_SOURCE_DIR}/.clang-tidy" "-warnings-as-errors=*")
  set_target_properties(LUIS PROPERTIES C_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
endif()

