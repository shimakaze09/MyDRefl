IF (NOT MYDREFL_BUILD_BIG_OBJ)
    MESSAGE(WARNING "Skip a target because the current environment doesn't support building a big obj")
    RETURN()
ENDIF ()

ADD_TARGET(
        TEST
        MODE EXE
        RET_TARGET_NAME tname
        LIB
        My::MyDRefl_core
        C_OPTION
        ${c_options}
)

TARGET_PRECOMPILE_HEADERS(${tname} REUSE_FROM ${MYDREFL_PCH_TARGET_NAME})