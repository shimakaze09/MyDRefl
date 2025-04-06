SET(c_options "")
IF (MSVC)
    SET(c_options "/bigobj")
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

TARGET_PRECOMPILE_HEADERS(${tname} REUSE_FROM MyDRefl_core)