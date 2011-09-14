if(BUILD_APPLICATIONS)

add_subdirectory(CommandLine)

if ( WRAP_PYTHON )
  add_subdirectory(Python)
endif ()

if ( WRAP_JAVA )
  add_subdirectory(Java)
endif ()

if ( WRAP_QT )
  add_subdirectory(Qt)
endif ()

if ( WRAP_PYQT )
  add_subdirectory(PyQt)
endif ()

endif()