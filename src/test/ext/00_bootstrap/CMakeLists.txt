IF (NOT My_MyRefl_Build_BootStrap)
    RETURN()
ENDIF ()

ADD_TARGET(
        TEST
        MODE EXE
        LIB
        My::MyDRefl_ext_Bootstrap
)