#This file set the following variable : 
# * otb_build_project_option
# * otb_use_option
# * otb_wrap_option
# * CONFIGURE_OPTIONS

set (otb_build_project_option 
"BUILD_COOKBOOK:BOOL=ON
BUILD_EXAMPLES:BOOL=ON
BUILD_SHARED_LIBS:BOOL=ON
BUILD_TESTING:BOOL=ON")

set (otb_use_option
"OTB_USE_6S:BOOL=ON
OTB_USE_CURL:BOOL=ON
OTB_USE_GLEW:BOOL=ON
OTB_USE_GLFW:BOOL=ON
OTB_USE_GLUT:BOOL=ON
OTB_USE_GSL:BOOL=ON
OTB_USE_LIBKML:BOOL=ON
OTB_USE_LIBSVM:BOOL=ON
OTB_USE_MPI:BOOL=ON
OTB_USE_MUPARSER:BOOL=ON
OTB_USE_MUPARSERX:BOOL=ON
OTB_USE_OPENCV:BOOL=ON
OTB_USE_OPENGL:BOOL=ON
OTB_USE_OPENMP:BOOL=ON
OTB_USE_QT:BOOL=ON
OTB_USE_QWT:BOOL=ON
OTB_USE_SHARK:BOOL=$ENV{OTB_USESHARK}
OTB_USE_SIFTFAST:BOOL=ON
OTB_USE_SPTW:BOOL=ON
OTB_USE_SSE_FLAGS:BOOL=ON")

set (otb_wrap_option
"OTB_WRAP_JAVA:BOOL=ON
OTB_WRAP_PYTHON:BOOL=ON
OTB_WRAP_PYTHON3:BOOL=ON")

set (otb_data_option
"OTB_DATA_ROOT:PATH=${OTB_DATA_ROOT}
OTB_DATA_USE_LARGEINPUT:BOOL=OFF
OTB_DATA_LARGEINPUT_ROOT:PATH=${OTB_LARGEINPUT_ROOT}")

set (cmake_configure_option
"CMAKE_BUILD_TYPE=${CTEST_BUILD_CONFIGURATION}
CMAKE_INSTALL_PREFIX:PATH=${CTEST_INSTALL_DIRECTORY}
CMAKE_C_FLAGS:STRING=-fPIC -Wall -Wextra
CMAKE_CXX_FLAGS:STRING=-fPIC -Wall -Wextra -Wno-cpp")

#Transform the previous string in list
string (REPLACE "\n" ";" otb_build_project_option ${otb_build_project_option})
string (REPLACE "\n" ";" otb_use_option ${otb_use_option})
string (REPLACE "\n" ";" otb_wrap_option ${otb_wrap_option})
string (REPLACE "\n" ";" otb_data_option ${otb_data_option})

set (otb_options "")
list (APPEND otb_options ${otb_build_project_option})
list (APPEND otb_options ${otb_use_option})
list (APPEND otb_options ${otb_wrap_option})
list (APPEND otb_options ${otb_data_option})

foreach(option ${otb_options})
  set( CONFIGURE_OPTIONS "${CONFIGURE_OPTIONS}-D${option};")
endforeach(option)

# This should be removed as soon as possible
set( CONFIGURE_OPTIONS "${CONFIGURE_OPTIONS}-Dopencv_INCLUDE_DIR:PATH=/usr/include;")
