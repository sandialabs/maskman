# Copyright (c) 2024, Sandia National Laboratories
# All rights reserved.

cmake_minimum_required(VERSION 3.24)

# LIBRARY
set(ListKillist libKillist.c)
add_library(Killist STATIC ${ListKillist})
target_link_libraries(Killist PUBLIC Maskman)
set_target_properties(Killist PROPERTIES
  PUBLIC_HEADER "libKillist.h"
  OUTPUT_NAME Killist
  ARCHIVE_OUTPUT_DIRECTORY static
  VERSION ${maskman_VERSION}
  )
# need anonymous union
set_property(TARGET Killist PROPERTY C_STANDARD 11)
target_include_directories(
  Killist
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  )
# install(TARGETS Killist EXPORT MaskmanTargets)

if(LLVM_CLANG_TIDY)
  find_program(CLANG_TIDY_EXE NAMES "clang-tidy" "clang-tidy17" "clang-tidy-mp-17")
  set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}" "-config-file=${CMAKE_SOURCE_DIR}/.clang-tidy" "-warnings-as-errors=*")
  set_target_properties(Killist PROPERTIES C_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
endif()

# UNIT TESTS
add_executable(utst-killist utst-killist.c)
target_link_libraries(utst-killist LINK_PRIVATE Killist)
set_property(TARGET utst-killist PROPERTY C_STANDARD 11)
# install(TARGETS utst-killist EXPORT MaskmanTargets)

# add some tests
add_test(buildup ./utst-killist --buildup)
add_test(buildups ./utst-killist --buildups)
add_test(selectiter ./utst-killist --selectiter)
add_test(selectrm ./utst-killist --selectrm)
add_test(selectprepost ./utst-killist --selectprepost)
add_test(selectfind ./utst-killist --selectfind)
add_test(count ./utst-killist --count)
add_test(inception ./utst-killist --inception)
