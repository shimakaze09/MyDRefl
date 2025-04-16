IF (NOT My_MyDRefl_Build_ext_Bootstrap)
   RETURN()
ENDIF ()
 
SET(c_options_private "")
IF (MSVC)
   LIST(APPEND c_options_private "/MP")
ELSEIF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
   #
ELSEIF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
   #
ENDIF ()
 
ADD_TARGET(
        MODE SHARED
        SOURCE
        "${PROJECT_SOURCE_DIR}/include/MyDRefl_ext/Bootstrap.h"
        INC
        "${PROJECT_SOURCE_DIR}/include"
        LIB
        My::MyDRefl_core
        C_OPTION_PRIVATE
        ${c_options_private}
        PCH_REUSE_FROM MyDRefl_core
 )