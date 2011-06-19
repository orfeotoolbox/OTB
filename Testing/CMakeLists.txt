set(TEMP ${CMAKE_BINARY_DIR}/Testing/Temporary)
file(MAKE_DIRECTORY ${TEMP})

add_subdirectory(Core)

if ( WRAP_PYTHON )
  add_subdirectory(Python)
endif ()

if ( WRAP_JAVA )
  add_subdirectory(Java)
endif ()

if ( WRAP_RUBY )
  add_subdirectory(Ruby)
endif ()

if ( WRAP_LUA )
  add_subdirectory(Lua)
endif ()

if ( WRAP_QT )
  add_subdirectory(QtWidget)
endif ()

if ( WRAP_PYQT )
  add_subdirectory(PyQt)
endif ()
