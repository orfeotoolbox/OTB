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


ADD_EXECUTABLE(fluid ${CPPFILES})
INSTALL_TARGETS(/bin fluid)

TARGET_LINK_LIBRARIES(fluid ${OTB_VISU_GUI_LIBRARIES} ${FLTK_PLATFORM_DEPENDENT_LIBS})
IF(OPENGL_FOUND)
	TARGET_LINK_LIBRARIES(fluid fltk_gl)
#    IF(APPLE)
#	    TARGET_LINK_LIBRARIES(fluid -Wl,-dylib_file,/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/libGL.dylib:/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/libGL.dylib)
#    ENDIF(APPLE)
ENDIF(OPENGL_FOUND)

