SET(CPPFILES
	CodeEditor.cxx
	Fl_Function_Type.cxx
	Fl_Group_Type.cxx
	Fl_Menu_Type.cxx
	Fl_Type.cxx
	Fl_Widget_Type.cxx
	Fl_Window_Type.cxx
	Fluid_Image.cxx
	about_panel.cxx
	align_widget.cxx
	alignment_panel.cxx
	code.cxx
	factory.cxx
	file.cxx
	fluid.cxx
	function_panel.cxx
	template_panel.cxx
	undo.cxx
	widget_panel.cxx
)

IF(CMAKE_COMPILER_IS_GNUCXX)
  FOREACH(f ${CPPFILES} )
        SET_SOURCE_FILES_PROPERTIES( ${f} PROPERTIES COMPILE_FLAGS -w )
  ENDFOREACH(f)
ELSE(CMAKE_COMPILER_IS_GNUCXX)
  IF(WIN32)
    IF (CMAKE_CXX_COMPILER MATCHES "^cl$")
        FOREACH(f ${CPPFILES} )
                SET_SOURCE_FILES_PROPERTIES( ${f} PROPERTIES COMPILE_FLAGS "/W0" )
        ENDFOREACH(f)
    ENDIF (CMAKE_CXX_COMPILER MATCHES "^cl$")
  ENDIF(WIN32)
ENDIF(CMAKE_COMPILER_IS_GNUCXX)


ADD_EXECUTABLE(fluid ${CPPFILES})
INSTALL_TARGETS(/bin fluid)

TARGET_LINK_LIBRARIES(fluid fltk fltk_images fltk_forms ${FLTK_PLATFORM_DEPENDENT_LIBS})
IF(OPENGL_FOUND)
	TARGET_LINK_LIBRARIES(fluid fltk_gl)
#    IF(APPLE)
#	    TARGET_LINK_LIBRARIES(fluid -Wl,-dylib_file,/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/libGL.dylib:/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/libGL.dylib)
#    ENDIF(APPLE)
ENDIF(OPENGL_FOUND)

