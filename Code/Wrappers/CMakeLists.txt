INCLUDE_DIRECTORIES(../Core)

# Generate Qt based GUI
option ( WRAP_QT "Enable Qt wrappers" OFF )

if ( WRAP_QT )
  FIND_PACKAGE(Qt4 REQUIRED)
  SET(QT_USE_QTXML 1)
  INCLUDE(${QT_USE_FILE})

  add_subdirectory(ApplicationLauncherQt)
  add_subdirectory(QtWidget)
endif( WRAP_QT )

# Generate SWIG wrappers
option ( WRAP_SWIG "Enable SWIG wrappers" OFF )
if ( WRAP_SWIG )
  add_subdirectory(SWIG)
endif( WRAP_SWIG )