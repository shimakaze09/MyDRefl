SET(c_options "")
IF (MSVC)
    LIST(APPEND c_options "/wd5030")
ELSEIF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    LIST(APPEND c_options "-Wno-attributes")
ELSEIF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    LIST(APPEND c_options "-Wno-attributes")
ENDIF ()

SET(c_options_i "")
IF(MSVC)
    SET(c_options_i "/bigobj")
ENDIF()

ADD_TARGET(
        MODE STATIC
        RET_TARGET_NAME tname
        SOURCE
        "${PROJECT_SOURCE_DIR}/include/MyDRefl"
        INC
        "${PROJECT_SOURCE_DIR}/include"
        C_OPTION
        ${c_options}
        C_OPTION_INTERFACE
        ${c_options_i}
        LIB
        My::MyContainer_core
        My::MyTemplate_core
)

TARGET_PRECOMPILE_HEADERS(${tname} PRIVATE "${PROJECT_SOURCE_DIR}/include/MyDRefl/MyDRefl.h")