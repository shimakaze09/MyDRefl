IF (NOT MyDRefl_Test_lua)
    RETURN()
ENDIF ()

ADD_TARGET(
        TEST
        MODE EXE
        LIB
        My::MyLua_core
)