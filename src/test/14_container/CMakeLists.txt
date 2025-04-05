ADD_TARGET(
        TEST
        MODE EXE
        RET_TARGET_NAME tname
        LIB
        My::MyDRefl_core
        C_OPTION
        /bigobj
)

TARGET_PRECOMPILE_HEADERS(${tname} REUSE_FROM MyDRefl_core)