SET(c_options "")
IF (MSVC)
    LIST(APPEND c_options "/wd5030;/wd4251")
ENDIF ()

ADD_TARGET(
        MODE SHARED
        SOURCE
        "${PROJECT_SOURCE_DIR}/include/MyDRefl"
        INC
        "${PROJECT_SOURCE_DIR}/include"
        C_OPTION
        ${c_options}
        DEFINE
        MY_MYDREFL_BUILD_AS_SHARED
        DEFINE_PRIVATE
        MY_MYDREFL_EXPORT
)