if ( WRAP_QT )
  add_subdirectory(ApplicationLauncherQt)
  add_subdirectory(QtWidget)
endif( WRAP_QT )

add_subdirectory(SWIG)

if ( WRAP_PYQT )
  find_package(PyQt4 REQUIRED)
  add_subdirectory(PyQt)
endif()

add_subdirectory(CommandLine)