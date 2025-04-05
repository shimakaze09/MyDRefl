ADD_TARGET(
        MODE INTERFACE
        RET_TARGET_NAME tname
        LIB
        My::MyDRefl_core
)

TARGET_PRECOMPILE_HEADERS(${tname} INTERFACE
        $<BUILD_INTERFACE:"${PROJECT_SOURCE_DIR}/include/MyDRefl/MyDRefl.h">
        $<INSTALL_INTERFACE:${package_name}/include/MyDRefl/MyDRefl.h>
)