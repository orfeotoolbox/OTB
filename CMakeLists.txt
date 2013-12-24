PROJECT(Ice)

cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_MODULE_PATH})
#----------------------------------------------------------------------------
#Prevents conflicts with visual min/max function
#Add the OSSIMSTATIC flag to prevent from including dclspec(dllimport) from ossim
if(WIN32)
  add_definitions(-DNOMINMAX -DOSSIM_STATIC)
endif(WIN32)

#----------------------------------------------------------------------------
# Orfeo ToolBox
find_package(OTB REQUIRED)
if(OTB_FOUND)
  include(${OTB_USE_FILE})
else(OTB_FOUND)
  message(FATAL_ERROR "OTB not found. Please set OTB_DIR")
endif(OTB_FOUND)

#----------------------------------------------------------------------------
# OpenGL (needed by Qt4)
find_package( OpenGL REQUIRED )

if (OPENGL_FOUND)
  MESSAGE("OpenGL found")
  include_directories(${OPENGL_INCLUDE_DIR})
 else (OPENGL_FOUND)
    MESSAGE("OpenGL missing")
 endif (OPENGL_FOUND)

find_package(GLFW REQUIRED)

if(GLFW_FOUND)
  MESSAGE("GLFW found")
  include_directories(${GLFW_INCLUDE_DIR})
else(GLFW_FOUND)
  MESSAGE("GLFW missing")
endif(GLFW_FOUND)

find_package(GLEW REQUIRED)

if(GLEW_FOUND)
MESSAGE("Glew found")
include_directories(${GLEW_INCLUDE_DIR})
else(GLEW_FOUND)
MESSAGE("Glew missing")
endif(GLEW_FOUND)

ADD_LIBRARY(OTBIce otbViewSettings.cxx otbGlView.cxx otbGlActor.cxx otbGlImageActor.cxx otbFragmentShaderRegistry.cxx otbFragmentShader.cxx otbStandardShader.cxx)
TARGET_LINK_LIBRARIES(OTBIce OTBIO ${OPENGL_LIBRARY} ${GLEW_LIBRARY})

ADD_EXECUTABLE(otbIce otbIce.cxx)
TARGET_LINK_LIBRARIES(otbIce OTBIce ${GLFW_LIBRARY})
