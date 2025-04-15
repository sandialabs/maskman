# Copyright (c) 2022, Sandia National Laboratories
# All rights reserved.

cmake_minimum_required(VERSION 3.25)

# Suppress Shim warnings about unused functions
add_compile_options(-Wno-error=unused-function)
add_link_options(-Wno-error=unused-function)

# C appMaskman
set(M_TARGET maskman)

# add the executable (doesn't need world-wide installation)
add_executable(${M_TARGET} appMaskman.c)
set_property(TARGET ${M_TARGET} PROPERTY C_STANDARD 11)
install(TARGETS ${M_TARGET} EXPORT MaskmanTargets)
target_link_libraries(${M_TARGET} LINK_PUBLIC Killist)
target_link_libraries(${M_TARGET} LINK_PUBLIC Maskman)
target_link_libraries(${M_TARGET} LINK_PUBLIC LUIS)
target_link_libraries(${M_TARGET} LINK_PUBLIC SimpleSTDOUT)

if(LLVM_CLANG_TIDY)
  find_program(CLANG_TIDY_EXE NAMES "clang-tidy" "clang-tidy17" "clang-tidy-mp-17")
  set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}" "-config-file=${CMAKE_SOURCE_DIR}/.clang-tidy" "-warnings-as-errors=*")
  set_target_properties(${M_TARGET} PROPERTIES C_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
endif()

set(TESTNAME "check42")
set(APP_EXE ${CMAKE_CURRENT_BINARY_DIR}/${M_TARGET})
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${TESTNAME})
add_test(NAME ${TESTNAME}
         COMMAND ${CMAKE_COMMAND}
           -DTST=${TESTNAME}
           -DCMD=${APP_EXE}
           -DCCSD=${CMAKE_CURRENT_SOURCE_DIR}
           -DCCBD=${CMAKE_CURRENT_BINARY_DIR}
           -P ${CMAKE_SOURCE_DIR}/test/ctest/runtest.cmake
         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${TESTNAME}
         )
set_tests_properties(${TESTNAME} PROPERTIES PROCESSORS 1 )

set(TESTNAME "checkspr")
set(APP_EXE ${CMAKE_CURRENT_BINARY_DIR}/${M_TARGET})
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${TESTNAME})
add_test(NAME ${TESTNAME}
         COMMAND ${CMAKE_COMMAND}
           -DTST=${TESTNAME}
           -DCMD=${APP_EXE}
           -DCCSD=${CMAKE_CURRENT_SOURCE_DIR}
           -DCCBD=${CMAKE_CURRENT_BINARY_DIR}
           -P ${CMAKE_SOURCE_DIR}/test/ctest/runtest.cmake
         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${TESTNAME}
         )
set_tests_properties(${TESTNAME} PROPERTIES PROCESSORS 1 )
