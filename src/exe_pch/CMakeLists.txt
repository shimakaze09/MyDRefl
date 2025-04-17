IF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    # work around
    # Clang: __PIC__ level differs in PCH file vs. current file
    # ref: https://izziswift.com/static-analyser-issues-with-command-line-tools/
    ADD_TARGET(
            TEST
            MODE EXE
            PCH
            "${PROJECT_SOURCE_DIR}/include/UDRefl/UDRefl.hpp"
            LIB
            My::MyDRefl_core
    )
ELSE()
    ADD_TARGET(
            TEST
            MODE EXE
            PCH_REUSE_FROM UDRefl_core
            LIB
            My::MyDRefl_core
    )
ENDIF()