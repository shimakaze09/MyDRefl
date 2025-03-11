IF(NOT MyDRefl_Test_lua)
    RETURN()
ENDIF ()

ADD_TARGET(
        TEST
        MODE SHARED
        RET_TARGET_NAME MyDRefl_lua
        LIB
        My::MyLua_core
        My::MyDRefl_core
        DEFINE_PRIVATE
        MY_EXPORT_MYDREFL_LUA
)

SET_TARGET_PROPERTIES(${MyDRefl_lua} PROPERTIES OUTPUT_NAME MyDRefl_L   ua CLEAN_DIRECT_OUTPUT 1)