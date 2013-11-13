# Deploy a qt.conf file in the specified
# target build directory, as a pre-build step
macro( deploy_qt_conf TARGET_NAME )

  add_custom_command( 
      TARGET ${TARGET_NAME}
      PRE_BUILD 
      COMMAND ${CMAKE_COMMAND}
      ARGS -D QT_TRANSLATIONS_DIR:PATH=${QT_TRANSLATIONS_DIR}
           -D QT_PLUGINS_DIR:PATH=${QT_PLUGINS_DIR}
           -D Monteverdi2_SOURCE_DIR:PATH=${Monteverdi2_SOURCE_DIR}
           -D TARGET_DIR:PATH=$<TARGET_FILE_DIR:${TARGET_NAME}>
           -P ${Monteverdi2_SOURCE_DIR}/CMake/DeployQtConfScript.cmake
      COMMENT "Deploying qt.conf in ${TARGET_NAME} build dir"
      VERBATIM
  )

endmacro()
