SET(c_options "")
IF (MSVC)
    SET(c_options "/bigobj")
ENDIF ()

ADD_TARGET(
        TEST
        MODE EXE
        LIB
        My::MyDRefl_core
        C_OPTION
        ${c_options}
)