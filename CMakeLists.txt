CMAKE_MINIMUM_REQUIRED(VERSION 3.20 FATAL_ERROR)

PROJECT(MyDRefl VERSION 0.10.0)
MESSAGE(STATUS "Project: ${PROJECT_NAME} ${PROJECT_VERSION}")

INCLUDE(cmake/InitMyCMake.cmake)
INIT_MYCMAKE(VERSION 0.6.4)

INIT_PROJECT()

ADD_DEP(MyTemplate 0.7.1)
ADD_DEP(MySmallFlat 0.1.0)

INCLUDE(cmake/Platform.cmake)

OPTION(My_MyDRefl_Build_ext_Bootstrap "build ext Bootstrap" OFF)
OPTION(My_MyDRefl_include_all_StdName "switch MY_MYDREFL_INCLUDE_ALL_STD_NAME" OFF)

ADD_SUB_DIRS_REC(include)
ADD_SUB_DIRS_REC(src)

EXPORT_TARGETS(
        TARGET
        DIRECTORIES
        "include"
)