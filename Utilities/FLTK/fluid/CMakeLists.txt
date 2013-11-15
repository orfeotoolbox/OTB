set(CPPFILES
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

if(CMAKE_COMPILER_IS_GNUCXX)
  foreach(f ${CPPFILES} )
        set_source_files_properties( ${f} PROPERTIES COMPILE_FLAGS -w )
  endforeach()
else()
  if(WIN32)
    if(CMAKE_CXX_COMPILER MATCHES "^cl$")
        foreach(f ${CPPFILES} )
                set_source_files_properties( ${f} PROPERTIES COMPILE_FLAGS "/W0" )
        endforeach()
    endif()
  endif()
endif()


add_executable(fluid ${CPPFILES})
install_targets(/bin fluid)

target_link_libraries(fluid fltk fltk_images fltk_forms ${FLTK_PLATFORM_DEPENDENT_LIBS})
if(OPENGL_FOUND)
	target_link_libraries(fluid fltk_gl)
#    if(APPLE)
#	    target_link_libraries(fluid -Wl,-dylib_file,/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/libGL.dylib:/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/libGL.dylib)
#    endif()
endif()

