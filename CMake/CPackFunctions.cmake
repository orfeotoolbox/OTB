
function(create_cpack_config application)

  if(WIN32)
     #CPack setting specific to Windows
    SET(CPACK_GENERATOR "NSIS")
    STRING(TOLOWER ${application} application_)
    SET(BATFILE_NAME "${application_}.bat")
    SET(EXEFILE_NAME "${application_}.exe")
    SET(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS
    "Delete \\\"$SMPROGRAMS\\\\${application}-${Monteverdi2_VERSION_MAJOR}.${Monteverdi2_VERSION_MINOR}\\\\${application}.lnk\\\"  ")
    SET(CPACK_NSIS_MENU_LINKS "bin/${BATFILE_NAME}" "${application}" )
    SET(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\${EXEFILE_NAME}")
    SET(CPACK_NSIS_EXTRA_INSTALL_COMMANDS
      "CreateShortCut \\\"$SMPROGRAMS\\\\-${Monteverdi2_VERSION_MAJOR}.${Monteverdi2_VERSION_MINOR}\\\\${application}.lnk\\\" \\\"$INSTDIR\\\\bin\\\\${BATFILE_NAME}\\\" \\\" \\\" \\\"$INSTDIR\\\\bin\\\\${EXEFILE_NAME}\\\"
    ")
    set(arch_prefix win32)    
    set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES")
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES64")
      set(arch_prefix win64)
    endif()    

    
  else(APPLE)
    #CPack setting specific to MacOS
    SET(CPACK_GENERATOR "Bundle")
    SET(CPACK_BUNDLE_ICON "${Monteverdi2_SOURCE_DIR}/Packaging/MacOS/Monteverdi2.icns" )
    SET(CPACK_BUNDLE_NAME "${application}-${Monteverdi2_VERSION_MAJOR}.${Monteverdi2_VERSION_MINOR}" )
    SET(CPACK_BUNDLE_PLIST "${CMAKE_BINARY_DIR}/Packaging/MacOS/${application}-Info.plist" )
    SET(CPACK_BUNDLE_STARTUP_COMMAND "${Monteverdi2_SOURCE_DIR}/Packaging/MacOS/${application}-StartupCommand" )
    set(arch_prefix Darwin)
     if(CMAKE_SIZEOF_VOID_P EQUAL 8)
      set(arch_prefix Darwin64)
    endif()    
  endif()

  
  #Common CPack configurations
  
  #should we handle this when calling function ?
  #for now mapla has no specific version or it is same as monteverdi2
  SET(CPACK_PACKAGE_VERSION "0.9.0")
  SET(CPACK_PACKAGE_VERSION_MAJOR "0")
  SET(CPACK_PACKAGE_VERSION_MINOR "9")
  SET(CPACK_PACKAGE_VERSION_PATCH "0")
  
  ##################################
  ##################################
  #CAUTION: Be careful when changing values below.#
  ##################################
  ##################################
  
  SET(CPACK_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")
  SET(CPACK_MODULE_PATH "${Monteverdi2_SOURCE_DIR}/CMake")
  SET(CPACK_NSIS_INSTALLER_ICON_CODE "")
  SET(CPACK_NSIS_INSTALLER_MUI_ICON_CODE "")
  SET(CPACK_PACKAGE_CONTACT "contact@orfeo-toolbox.org")
  SET(CPACK_PACKAGE_DEFAULT_LOCATION "/")
  SET(CPACK_PACKAGE_DESCRIPTION_FILE "${Monteverdi2_SOURCE_DIR}/Description.txt")

  SET(CPACK_RESOURCE_FILE_LICENSE "${Monteverdi2_SOURCE_DIR}/Copyright/Copyright.txt")
  SET(CPACK_OUTPUT_CONFIG_FILE "${CMAKE_BINARY_DIR}/CPackConfig-${application}.cmake")
  SET(CPACK_NSIS_MUI_ICON    "${Monteverdi2_SOURCE_DIR}/Data/Icons/monteverdi2.ico")
  SET(CPACK_NSIS_MUI_UNIICON "${Monteverdi2_SOURCE_DIR}/Data/Icons/monteverdi2.ico")

  SET(CPACK_COMPONENTS_ALL)
  LIST(APPEND CPACK_COMPONENTS_ALL  "Resources")
  LIST(APPEND CPACK_COMPONENTS_ALL "Runtime")
  LIST(APPEND CPACK_COMPONENTS_ALL "${application}")
  
  SET(CPACK_COMPONENT_DEVELOPMENT_DEPENDS Runtime)
  SET(CPACK_COMPONENT_RUNTIME_DEPENDS Resources)

  SET(CPACK_COMPONENT_RUNTIME_REQUIRED ON)
  SET(CPACK_COMPONENT_RESOURCES_REQUIRED ON)
  SET(CPACK_COMPONENT_RESOURCES_HIDDEN ON)
  
  SET(CPACK_COMPONENT_MAPLA_REQUIRED ON)
  SET(CPACK_COMPONENT_MONTEVERDI2_REQUIRED ON)

  SET(CPACK_INSTALL_CMAKE_PROJECTS "${CMAKE_BINARY_DIR};${application};ALL;/")
  
  SET(CPACK_PACKAGE_NAME "${application}")  
  
  SET(CPACK_NSIS_DISPLAY_NAME "${application}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}")
  SET(CPACK_NSIS_PACKAGE_NAME "${application}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}")
  SET(CPACK_PACKAGE_INSTALL_DIRECTORY "${application}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}")
  SET(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "${application}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}")
  
  SET(CPACK_PACKAGE_FILE_NAME "${application}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}-${arch_prefix}")
 
  INCLUDE(InstallRequiredSystemLibraries)
  INCLUDE(CPack)

endfunction(create_cpack_config) 
