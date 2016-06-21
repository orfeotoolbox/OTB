cmake_minimum_required(VERSION 2.8.3)

project(glew)

option(BUILD_SHARED_LIBS "Building shared libs. Default is ON" ON)

#NOTE: Below is output from "make" on osx. This was used to make up this cmakelists.txt
# cc -DGLEW_NO_GLU -DGLEW_BUILD -O2 -Wall -W -Iinclude -dynamic -fno-common -ansi -pedantic -fPIC  -o tmp/darwin/default/shared/glew.o -c src/glew.c
# cc -dynamiclib -install_name /usr/lib/libGLEW.1.13.0.dylib -current_version 1.13.0 -compatibility_version 1.13 -o lib/libGLEW.1.13.0.dylib tmp/darwin/default/shared/glew.o  -framework OpenGL
# ln -sf libGLEW.1.13.0.dylib lib/libGLEW.1.13.dylib
# ln -sf libGLEW.1.13.0.dylib lib/libGLEW.dylib
# strip -x lib/libGLEW.1.13.0.dylib
# cc -DGLEW_NO_GLU -DGLEW_MX -DGLEW_BUILD -O2 -Wall -W -Iinclude -dynamic -fno-common -ansi -pedantic -fPIC  -o tmp/darwin/mx/shared/glew.o -c src/glew.c
# cc -dynamiclib -install_name /usr/lib/libGLEWmx.1.13.0.dylib -current_version 1.13.0 -compatibility_version 1.13 -o lib/libGLEWmx.1.13.0.dylib tmp/darwin/mx/shared/glew.o  -framework OpenGL
# ln -sf libGLEWmx.1.13.0.dylib lib/libGLEWmx.1.13.dylib
# ln -sf libGLEWmx.1.13.0.dylib lib/libGLEWmx.dylib
# strip -x lib/libGLEWmx.1.13.0.dylib
# cc -DGLEW_NO_GLU -O2 -Wall -W -Iinclude -dynamic -fno-common -ansi -pedantic -fPIC  -o tmp/darwin/default/shared/glewinfo.o -c src/glewinfo.c
# cc -O2 -Wall -W -Iinclude -dynamic -fno-common -ansi -pedantic -fPIC -o bin/glewinfo tmp/darwin/default/shared/glewinfo.o -Llib  -lGLEW  -framework OpenGL
# clang: warning: argument unused during compilation: '-ansi'
# strip -x bin/glewinfo
# cc -DGLEW_NO_GLU -O2 -Wall -W -Iinclude -dynamic -fno-common -ansi -pedantic -fPIC  -o tmp/darwin/default/shared/visualinfo.o -c src/visualinfo.c
# cc -O2 -Wall -W -Iinclude -dynamic -fno-common -ansi -pedantic -fPIC -o bin/visualinfo tmp/darwin/default/shared/visualinfo.o -Llib  -lGLEW  -framework
# OpenGL
# clang: warning: argument unused during compilation: '-ansi'


if(BUILD_SHARED_LIBS)
  add_definitions("-DGLEW_BUILD")
else()
  add_definitions("-DGLEW_STATIC")
endif()

if(UNIX AND NOT APPLE)
  find_package(X11 REQUIRED)
endif()

if(UNIX)
  if(CMAKE_C_FLAGS)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -fPIC -pedantic")
  else()
    set(CMAKE_C_FLAGS "-Wall")
  endif()
else()
  #TODO: MSVC

endif()

#-ansi
if(APPLE)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -dynamic -fno-common")
endif()

option(GLEW_NO_GLU "define GLEW_NO_GLU" TRUE)

if(NOT GLEW_NO_GLU)
  #need for pkgconfig file
  set(LIBGLU glu)
else()
  add_definitions("-DGLEW_NO_GLU")
endif()

find_package(OpenGL REQUIRED)

mark_as_advanced(OPENGL_INCLUDE_DIR)
mark_as_advanced(OPENGL_LIBRARY)

include_directories(${OPENGL_INCLUDE_DIR})

include_directories(include)

if(NOT OPENGL_FOUND)
  message(FATAL_ERROR "Cannot find OpenGL. Set OPENGL_INCLUDE_DIR and OPENGL_LIBRARY")
endif()

set(VERSION_MAJOR "1")
set(VERSION_MINOR "13")
set(VERSION_PATCH "0")
set(VERSION_STRING "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

set(GLEW_SRCS
  src/glew.c )

set(GLEW_HEADERS
  include/GL/glew.h
  include/GL/wglew.h
  include/GL/glxew.h)

add_library(libGLEW ${GLEW_SRCS})

target_link_libraries(libGLEW ${OPENGL_LIBRARY})

set_target_properties(libGLEW
  PROPERTIES
  PREFIX ""
  VERSION   "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}"
  SOVERSION "${VERSION_MAJOR}")

install(TARGETS libGLEW
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)


add_library(libGLEWmx ${GLEW_SRCS})

target_link_libraries(libGLEWmx ${OPENGL_LIBRARY})

set_target_properties(libGLEWmx
  PROPERTIES
  PREFIX ""
  VERSION   "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}"
  SOVERSION "${VERSION_MAJOR}")

set_target_properties(libGLEWmx
  PROPERTIES
  COMPILE_DEFINITIONS GLEW_MX
  )

install(TARGETS libGLEWmx
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)

#make executables glewinfo and visualinfo

foreach(executable glew visual)
  add_executable(${executable}info src/${executable}info.c)
  add_dependencies(${executable}info libGLEW)
  target_link_libraries(${executable}info libGLEW ${X11_LIBRARIES})
  #install
  install(TARGETS ${executable}info
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)

endforeach()

#install include headers
install(FILES ${GLEW_HEADERS} DESTINATION include/GL/)
