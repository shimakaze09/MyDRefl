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
        PCH_REUSE_FROM MyDRefl_core
)