#This file set the following variable : 
# * otb_build_project_option
# * otb_use_option
# * otb_wrap_option
# * CONFIGURE_OPTIONS

set (otb_build_project_option 
"BUILD_COOKBOOK:BOOL=OFF
BUILD_EXAMPLES:BOOL=OFF
BUILD_SHARED_LIBS:BOOL=OFF
BUILD_TESTING:BOOL=OFF")


set (otb_use_option
"OTB_USE_6S:BOOL=OFF
OTB_USE_CURL:BOOL=OFF
OTB_USE_GLEW:BOOL=OFF
OTB_USE_GLFW:BOOL=OFF
OTB_USE_GLUT:BOOL=OFF
OTB_USE_GSL:BOOL=OFF
OTB_USE_LIBKML:BOOL=OFF
OTB_USE_LIBSVM:BOOL=OFF
OTB_USE_MPI:BOOL=OFF
OTB_USE_MUPARSER:BOOL=OFF
OTB_USE_MUPARSERX:BOOL=OFF
OTB_USE_OPENCV:BOOL=OFF
OTB_USE_OPENGL:BOOL=OFF
OTB_USE_OPENMP:BOOL=OFF
OTB_USE_QT:BOOL=OFF
OTB_USE_QWT:BOOL=OFF
OTB_USE_SHARK:BOOL=OFF
OTB_USE_SIFTFAST:BOOL=OFF
OTB_USE_SPTW:BOOL=OFF
OTB_USE_SSE_FLAGS:BOOL=OFF")

set (otb_wrap_option
"OTB_WRAP_JAVA:BOOL=OFF
OTB_WRAP_PYTHON:BOOL=OFF
OTB_WRAP_PYTHON3:BOOL=OFF")

set (otb_data_option
"OTB_DATA_ROOT:PATH=${OTB_DATA_ROOT}
OTB_DATA_USE_LARGEINPUT:BOOL=OFF
OTB_DATA_LARGEINPUT_ROOT:PATH=${OTB_LARGEINPUT_ROOT}")

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
