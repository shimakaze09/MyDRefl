SET(c_options "")
IF (MSVC)
    LIST(APPEND c_options "/wd5030")
ENDIF ()

ADD_TARGET(
        MODE INTERFACE
        INC
        "${PROJECT_SOURCE_DIR}/include"
        C_OPTION
        ${c_options}
)