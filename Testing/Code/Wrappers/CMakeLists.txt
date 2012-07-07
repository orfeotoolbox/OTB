if(BUILD_APPLICATIONS)

add_subdirectory(CommandLine)

if ( OTB_WRAP_PYTHON )
  add_subdirectory(Python)
endif ()

if ( OTB_WRAP_JAVA )
  add_subdirectory(Java)
endif ()

if ( OTB_WRAP_QT AND OTB_USE_VISU_GUI )
  add_subdirectory(Qt)
endif ()

if ( OTB_WRAP_PYQT )
  add_subdirectory(PyQt)
endif ()

endif()