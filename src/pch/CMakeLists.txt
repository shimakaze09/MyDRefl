IF (NOT MYDREFL_BUILD_BIG_OBJ)
    MESSAGE(WARNING "Skip a target because the current environment doesn't support building a big obj")
    RETURN()
ENDIF ()

ADD_TARGET(
        MODE STATIC
        RET_TARGET_NAME tname
        SOURCE
        "${PROJECT_SOURCE_DIR}/include/MyDRefl_pch.h"
        LIB
        My::MyDRefl_core
)

TARGET_PRECOMPILE_HEADERS(${tname} PRIVATE "${PROJECT_SOURCE_DIR}/include/MyDRefl_pch.h")