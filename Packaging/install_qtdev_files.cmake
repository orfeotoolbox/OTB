function(install_qtdev_files)
  if(NOT HAVE_QT4)
    return()
  endif()
set(QT_REQ_DIRS)
  if(MSVC)
    file(GLOB ALL_LIB_FILES "${SUPERBUILD_INSTALL_DIR}/lib/*.lib")
  endif()
  foreach(LIB_FILE ${LIB_FILES})
    install_rule(${LIB_FILE})
  endforeach()
  list(APPEND QT_REQ_DIRS imports)  
  list(APPEND QT_REQ_DIRS mkspecs)
  list(APPEND QT_REQ_DIRS plugins)
  list(APPEND QT_REQ_DIRS translations)
  foreach(QT_REQ_DIR ${QT_REQ_DIRS} )
    if(EXISTS "${SUPERBUILD_INSTALL_DIR}/${QT_REQ_DIR}")
      install_without_message("${SUPERBUILD_INSTALL_DIR}/${QT_REQ_DIR}" "")
    endif()
  endforeach()

  install(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Files/qt.conf
    DESTINATION ${PKG_STAGE_DIR}/bin
    )
 
endfunction()
