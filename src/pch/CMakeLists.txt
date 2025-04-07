SET(c_options "")
IF (MSVC)
    SET(c_options "/bigobj")
ENDIF ()

ADD_TARGET(
        MODE STATIC
        RET_TARGET_NAME tname
        SOURCE
        "${PROJECT_SOURCE_DIR}/include/MyDRefl_pch.h"
        LIB
        My::MyDRefl_core
        C_OPTION
        ${c_options}
)

TARGET_PRECOMPILE_HEADERS(${tname} PRIVATE "${PROJECT_SOURCE_DIR}/include/MyDRefl_pch.h")