include_directories(../Core)
include_directories(../Common)
include_directories(../CommandLine)

# Generate Qt based GUI
option ( WRAP_QT "Enable Qt wrappers" OFF )
option ( WRAP_PYQT "Enable PyQt wrappers" OFF )

if ( WRAP_QT )
  find_package(Qt4 REQUIRED)
  set(QT_USE_QTXML 1)
  include(${QT_USE_FILE})

  add_subdirectory(ApplicationLauncherQt)
  add_subdirectory(QtWidget)
endif( WRAP_QT )

add_subdirectory(SWIG)

if ( WRAP_PYQT )
  find_package(PyQt4 REQUIRED)
  add_subdirectory(PyQt)
endif()

 add_subdirectory(CommandLine)