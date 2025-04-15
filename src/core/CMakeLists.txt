SET(c_options "")
IF (MSVC)
    LIST(APPEND c_options "/wd5030;/bigobj")
ELSEIF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    LIST(APPEND c_options "-Wno-attributes")
ELSEIF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    LIST(APPEND c_options "-Wno-attributes")
ENDIF ()

SET(c_options_private "")
IF (MSVC)
    LIST(APPEND c_options_private "/MP")
ELSEIF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    #
ELSEIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    #
ENDIF ()

SET(defines "")
IF (MSVC)
    LIST(APPEND defines "_SILENCE_STDEXT_HASH_LOWER_BOUND_DEPRECATION_WARNING")
    LIST(APPEND defines "_SILENCE_STDEXT_HASH_UPPER_BOUND_DEPRECATION_WARNING")
ELSEIF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    LIST(APPEND defines "")
ELSEIF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    LIST(APPEND defines "")
ENDIF ()

IF (My_MyDRefl_include_all_StdName)
    LIST(APPEND defines "MY_MYDREFL_INCLUDE_ALL_STD_NAME")
ENDIF ()

ADD_TARGET(
        MODE STATIC
        SOURCE
        "${PROJECT_SOURCE_DIR}/include/MyDRefl"
        INC
        "${PROJECT_SOURCE_DIR}/include"
        C_OPTION
        ${c_options}
        C_OPTION_PRIVATE
        ${c_options_private}
        LIB
        My::MyTemplate_core
        My::MySmallFlat_core
        DEFINE
        ${defines}
        PCH
        "${PROJECT_SOURCE_DIR}/include/MyDRefl/MyDRefl.h"
)