IF (NOT My_MyDRefl_Build_BootStrap)
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
        MODE STATIC
        SOURCE
        "${PROJECT_SOURCE_DIR}/include/MyDRefl_bootstrap.h"
        INC
        "${PROJECT_SOURCE_DIR}/include"
        LIB
        My::MyDRefl_core
        C_OPTION_PRIVATE
        ${c_options_private}
        PCH
        "${PROJECT_SOURCE_DIR}/include/MyDRefl/MyDRefl.h"
 )