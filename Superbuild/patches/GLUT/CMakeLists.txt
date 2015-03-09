cmake_minimum_required(VERSION 2.8.3)

project(freeglut)

set(FREEGLUT_INC_DIR ${CMAKE_SOURCE_DIR}/include)
set(FREEGLUT_SRC_DIR ${CMAKE_SOURCE_DIR}/src)

option(BUILD_SHARED_LIBS "Building shared libs. Default is ON" ON)

if(BUILD_SHARED_LIBS)
    add_definitions(-DFREEGLUT_EXPORTS)
else()
    add_definitions(-DFREEGLUT_STATIC)	
endif()

 add_definitions(-D_CRT_SECURE_NO_WARNINGS)

set(FREEGLUT_SRCS ${FREEGLUT_SRC_DIR}/freeglut_callbacks.c 
${FREEGLUT_SRC_DIR}/freeglut_cursor.c 
${FREEGLUT_SRC_DIR}/freeglut_display.c 
${FREEGLUT_SRC_DIR}/freeglut_ext.c 
${FREEGLUT_SRC_DIR}/freeglut_font.c 
${FREEGLUT_SRC_DIR}/freeglut_font_data.c 
${FREEGLUT_SRC_DIR}/freeglut_gamemode.c 
${FREEGLUT_SRC_DIR}/freeglut_geometry.c 
${FREEGLUT_SRC_DIR}/freeglut_glutfont_definitions.c 
${FREEGLUT_SRC_DIR}/freeglut_init.c 
${FREEGLUT_SRC_DIR}/freeglut_input_devices.c 
${FREEGLUT_SRC_DIR}/freeglut_joystick.c 
${FREEGLUT_SRC_DIR}/freeglut_main.c 
${FREEGLUT_SRC_DIR}/freeglut_menu.c 
${FREEGLUT_SRC_DIR}/freeglut_misc.c 
${FREEGLUT_SRC_DIR}/freeglut_overlay.c 
${FREEGLUT_SRC_DIR}/freeglut_spaceball.c 
${FREEGLUT_SRC_DIR}/freeglut_state.c 
${FREEGLUT_SRC_DIR}/freeglut_stroke_mono_roman.c 
${FREEGLUT_SRC_DIR}/freeglut_stroke_roman.c 
${FREEGLUT_SRC_DIR}/freeglut_structure.c 
${FREEGLUT_SRC_DIR}/freeglut_teapot.c 
${FREEGLUT_SRC_DIR}/freeglut_videoresize.c 
${FREEGLUT_SRC_DIR}/freeglut_window.c)

set(FREEGLUT_INCLUDES 
${FREEGLUT_INC_DIR}/GL/glut.h
${FREEGLUT_INC_DIR}/GL/freeglut.h
${FREEGLUT_INC_DIR}/GL/freeglut_ext.h
${FREEGLUT_INC_DIR}/GL/freeglut_std.h
)

include_directories(${FREEGLUT_INC_DIR})

add_library(freeglut ${FREEGLUT_SRCS})

install(TARGETS freeglut
  RUNTIME DESTINATION bin COMPONENT RuntimeLibraries
  LIBRARY DESTINATION lib COMPONENT RuntimeLibraries
  ARCHIVE DESTINATION lib COMPONENT Development)


install(FILES ${FREEGLUT_INCLUDES}  
    DESTINATION include/GL/
    COMPONENT Development)


