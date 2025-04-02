SET(c_options "")
IF (MSVC)
    LIST(APPEND c_options "/wd5030;/wd4251")
ENDIF ()

ADD_TARGET(
        MODE STATIC
        SOURCE
        "${PROJECT_SOURCE_DIR}/include/MyDRefl"
        INC
        "${PROJECT_SOURCE_DIR}/include"
        C_OPTION
        ${c_options}
        LIB
        My::MyContainer_core
        My::MyTemplate_core
)