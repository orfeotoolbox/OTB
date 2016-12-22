# Get the translation files coming with Qt, and install them in the bundle
# They are loaded by Monteverdi.
function(get_qt_translation_files RESULT)
    # These files are the "qt_<localename>.qm" files
    # They are located in QT_TRANSLATIONS_DIR, which comes from FindQt4
    file(GLOB translation_files ${QT_TRANSLATIONS_DIR}/qt_*)

    # We need to remove the "qt_help_<localename>.qm" files from this list
    foreach(translation_item ${translation_files})
      if(${translation_item} MATCHES "qt_help")
        list(REMOVE_ITEM translation_files ${translation_item})
      endif()
    endforeach()

    set(${RESULT} ${translation_files} PARENT_SCOPE)
  endfunction()


function(func_install_xdk_files)

  #The list of REQ_SHARE_DIR is made up from <mxe-target-dir>/share/
  #It may vary in future. I prefer not to glob on the share dir and
  #end up distributing man, info etc.. which ar irrelvant for windows

  #TODO: cleaup this function. current
  # code is bit of picking each .lib and .dll for now
  # see opencv, itk, 
  foreach(REQ_SHARE_DIR
      aclocal
      Armadillo
      applications
      cmake
      dbus-1
      fontconfig
      libgta
      locale
      xml
      applications
      cmake
      icons
      OpenCV
      pixmaps
      pkgconfig
      )
    if(EXISTS "${DEPENDENCIES_INSTALL_DIR}/share/${REQ_SHARE_DIR}")
      func_install_without_message("${DEPENDENCIES_INSTALL_DIR}/share/${REQ_SHARE_DIR}" "share")
    endif()
  endforeach()

  file(GLOB ITK_CMAKE_DIRS "${DEPENDENCIES_INSTALL_DIR}/lib/cmake/ITK*")
  foreach(ITK_CMAKE_DIR ${ITK_CMAKE_DIRS})
      func_install_without_message("${ITK_CMAKE_DIR}" "lib/cmake")
  endforeach()

  set(QT_REQ_DIRS)
  if(WIN32)
    #only affects windows due to regex on dll
    #.lib are not taken when processing dependencies. 
    # We just get .dlls which is enough for binary package
    # For XDK, we need .lib files when building OTB using xdk
    # For Linux. we get .so.X.Y.Z by finding the 'target'
    # of any .so file. So there is no need for such a copy on
    # Linux and OSX 
    if(MSVC)
      file(GLOB LIB_FILES "${DEPENDENCIES_INSTALL_DIR}/lib/*.lib")
    else()
      file(GLOB LIB_FILES "${DEPENDENCIES_INSTALL_DIR}/lib/*dll.*")
    endif()

    foreach(LIB_FILE ${LIB_FILES})
      pkg_install_rule(${LIB_FILE})
    endforeach()
    
    #qt/bin is also a special case for mxe.
    file(GLOB QT_EXTRA_DLL_FILES "${DEPENDENCIES_INSTALL_DIR}/qt/bin/*.dll")
    install(FILES ${QT_EXTRA_DLL_FILES} DESTINATION ${PKG_STAGE_DIR}/bin)

    #list(APPEND QT_REQ_DIRS lib)
    list(APPEND QT_REQ_DIRS include)
    list(APPEND QT_REQ_DIRS imports)

    #mxe installs qt in a separate directory under install prefix. So..
    set(QT_INSTALL_DIR "${DEPENDENCIES_INSTALL_DIR}/qt")
  else()
    set(QT_INSTALL_DIR "${DEPENDENCIES_INSTALL_DIR}")

  endif(WIN32)

  list(APPEND QT_REQ_DIRS mkspecs)
  list(APPEND QT_REQ_DIRS plugins)
  list(APPEND QT_REQ_DIRS translations)
  foreach(QT_REQ_DIR ${QT_REQ_DIRS} )
    if(EXISTS "${QT_INSTALL_DIR}/${QT_REQ_DIR}")
      func_install_without_message("${QT_INSTALL_DIR}/${QT_REQ_DIR}" "")
    endif()
  endforeach()

  # #install ${DEPENDENCIES_INSTALL_DIR}/include directory. Attention to OTB includes
  file(GLOB ALL_IN_INCLUDE_DIR "${DEPENDENCIES_INSTALL_DIR}/include/*")
  foreach(INCLUDE_DIR_ITEM ${ALL_IN_INCLUDE_DIR})
    get_filename_component(INCLUDE_DIR_ITEM_name ${INCLUDE_DIR_ITEM} NAME)
    get_filename_component(INCLUDE_DIR_ITEM_name_we ${INCLUDE_DIR_ITEM} NAME_WE)
    if(NOT "${INCLUDE_DIR_ITEM_name_we}" MATCHES "OTB|otb")
      if( IS_DIRECTORY ${INCLUDE_DIR_ITEM})
        install(CODE
          "message(STATUS \"Installing: ${CMAKE_INSTALL_PREFIX}/${PKG_STAGE_DIR}/include/${INCLUDE_DIR_ITEM_name}/\")" )
        install(
          DIRECTORY   "${INCLUDE_DIR_ITEM}"
          DESTINATION "${PKG_STAGE_DIR}/include/"
          MESSAGE_NEVER
          )
      else()
        install(
          FILES   "${INCLUDE_DIR_ITEM}"
          DESTINATION "${PKG_STAGE_DIR}/include/"
          )
      endif() #if( IS_DIRECTORY
    endif() #if (NOT
  endforeach()

endfunction() #func_install_xdk_files

function(func_install_support_files)

  #a convenient cmake var for storing <prefix>/bin
  set(PKG_STAGE_BIN_DIR "${PKG_STAGE_DIR}/bin")

  #<prefix>/share for gdal data files
  set(PKG_SHARE_DEST_DIR ${PKG_STAGE_DIR}/share)

  set(PKG_SHARE_SOURCE_DIR ${DEPENDENCIES_INSTALL_DIR}/share)

  set(GDAL_DATA ${PKG_SHARE_SOURCE_DIR}/gdal)
  #MSVC install gdal-data in in a different directory. So we don't spoil it
  if(MSVC)
    set(GDAL_DATA ${DEPENDENCIES_INSTALL_DIR}/data)
  endif()

  # Just check if required variables are defined.
  foreach(req
      DEPENDENCIES_INSTALL_DIR
      PKG_STAGE_DIR
      PACKAGE_SUPPORT_FILES_DIR
      OTB_INSTALL_DIR
      )
    if(NOT DEFINED ${req})
      message(FATAL_ERROR "you must set ${req} before calling this method")
    endif()
    set(vars "${vars}  ${req}=[${${req}}]\n")
  endforeach(req)

  # one for debugging..
  # install(CODE "message(\"CMake/PackageHelper.cmake:install_supoport_files(${outdir})\n${vars}\n\")")
  if(NOT PKG_GENERATE_XDK)
    func_install_otb_support_files()

    #check if monteverdi executable is built?
    if(EXISTS "${OTB_INSTALL_DIR}/bin/monteverdi${EXE_EXT}")
      func_install_monteverdi_support_files()
    endif()

  endif() #NOT PKG_GENERATE_XDK

  ####################### install GDAL data ############################
  if(NOT EXISTS "${GDAL_DATA}/epsg.wkt")
    message(FATAL_ERROR
      "Cannot generate package without GDAL_DATA : ${GDAL_DATA} ${DEPENDENCIES_INSTALL_DIR}")
  endif()


  # install(
  #   DIRECTORY ${GDAL_DATA}
  #   DESTINATION ${PKG_SHARE_DEST_DIR}
  #   )

  func_install_without_message("${GDAL_DATA}" "share" )
  ####################### install GeoTIFF data ########################
  #install( DIRECTORY ${PKG_SHARE_SOURCE_DIR}/epsg_csv DESTINATION ${PKG_SHARE_DEST_DIR}  )

  func_install_without_message("${PKG_SHARE_SOURCE_DIR}/epsg_csv" "share" )

  ####################### install OSSIM data ##########################
  #install( DIRECTORY ${PKG_SHARE_SOURCE_DIR}/ossim DESTINATION ${PKG_SHARE_DEST_DIR})

  func_install_without_message("${PKG_SHARE_SOURCE_DIR}/ossim" "share" )

  ####################### install proj share ##########################
  if(EXISTS ${PKG_SHARE_SOURCE_DIR}/proj)
    #install(DIRECTORY ${PKG_SHARE_SOURCE_DIR}/proj DESTINATION ${PKG_SHARE_DEST_DIR})
    func_install_without_message("${PKG_SHARE_SOURCE_DIR}/proj" "share" )
  endif()
  
  ####################### Install copyrights ##########################
  #install license for windows package
  #install(DIRECTORY ${PKG_SHARE_SOURCE_DIR}/copyright DESTINATION ${PKG_SHARE_DEST_DIR} )
  func_install_without_message("${PKG_SHARE_SOURCE_DIR}/copyright" "share" )
  install(FILES ${PKG_SHARE_SOURCE_DIR}/copyright/LICENSE DESTINATION ${PKG_STAGE_DIR})

  ####################### Install VERSION ##########################

  set(PKG_VERSION_FILE
    "${OTB_INSTALL_DIR}/share/doc/${PKG_OTB_VERSION_MAJOR}.${PKG_OTB_VERSION_MINOR}/VERSION")
  if(EXISTS ${PKG_VERSION_FILE} )
    install(FILES ${PKG_VERSION_FILE} DESTINATION ${PKG_STAGE_DIR})
  endif()

endfunction()

function(func_install_otb_support_files)
  foreach(req
      PKG_STAGE_DIR
      OTB_INSTALL_DIR
      DEPENDENCIES_INSTALL_DIR
      )
    if(NOT DEFINED ${req})
      message(FATAL_ERROR "you must set ${req} before calling this method")
    endif()
    set(vars "${vars}  ${req}=[${${req}}]\n")
  endforeach(req)

  #a convenient cmake var for storing <prefix>/bin
  set(PKG_STAGE_BIN_DIR "${PKG_STAGE_DIR}/bin")

  #<prefix>/share for gdal data files
  set(PKG_SHARE_DEST_DIR ${PKG_STAGE_DIR}/share)

  set(PKG_SHARE_SOURCE_DIR ${DEPENDENCIES_INSTALL_DIR}/share)

  #For Unixes we make them in the *pkgsetup.in
  ##################### install environment source ##########################
  if(WIN32)
    foreach(ENV_SOURCE_FILE
        "${PACKAGE_SUPPORT_FILES_DIR}/otbenv.cmd"
        "${PACKAGE_SUPPORT_FILES_DIR}/otbenv.profile")
      if(EXISTS ${ENV_SOURCE_FILE})
        install(FILES ${ENV_SOURCE_FILE} DESTINATION ${PKG_STAGE_DIR})
      endif()
    endforeach()

    #we need startup files for mapla monteverdi in the root directory
    #For Unixes, we make them inside pkgsetup script!
    foreach(exe_file mapla monteverdi)
      install(
        PROGRAMS ${PACKAGE_SUPPORT_FILES_DIR}/${exe_file}.bat
        DESTINATION "${PKG_STAGE_DIR}"
        )
    endforeach()
  endif()

  ####################### install cli and gui scripts ###########################
  file(GLOB PKG_APP_SCRIPTS
    ${OTB_INSTALL_DIR}/bin/otbcli*
    ${OTB_INSTALL_DIR}/bin/otbgui*) #

  list(LENGTH PKG_APP_SCRIPTS PKG_APP_SCRIPTS_LENGTH)
  if (PKG_APP_SCRIPTS_LENGTH LESS 1)
    message(WARNING "PKG_APP_SCRIPTS is empty: ${PKG_APP_SCRIPTS}")
  endif()

  ##################### install cli and gui scripts #######################
  install(PROGRAMS ${PKG_APP_SCRIPTS}  DESTINATION ${PKG_STAGE_BIN_DIR})


endfunction()

function(func_install_monteverdi_support_files)

  #name/ext of qt's sqlite plugin. Varies with platform/OS
  if(WIN32)
    set(PKG_QTSQLITE_FILENAME "qsqlite4.dll")
  elseif(APPLE)
    set(PKG_QTSQLITE_FILENAME "libqsqlite.dylib")
  elseif("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
    set(PKG_QTSQLITE_FILENAME "libqsqlite.so")
  else()
    message(FATAL_ERROR "func_install_monteverdi_support_files: Unknown OS/Platform")
  endif()

  #root folder where qt plugins are installed
  set(PKG_QTPLUGINS_DIR "${PKG_STAGE_DIR}/lib/qt4/plugins")

  #qt4's distributes some translation of common message string used in Qt.
  #This are provided with any qt installation. We reuse them in otb package
  #so as not to reinvent the wheels.
  set(PKG_QT_TRANSLATIONS_DIR "${PKG_STAGE_DIR}/lib/qt4/translations")

  #<prefix>/share for otb i18n directory. This is different from qt's i18N directory
  #which is <prefix>/share/qt4/translations.
  #set(PKG_xxOTB_Ixxx18N_DIR "${PKG_STAGE_DIR}/${PKG_OTB_INSTALL_DATA_DIR}/i18n")
  # we find this value by parsing ConfigureMonteverdi.h
  set(ConfigureMonteverdi_H "${OTB_BINARY_DIR}/Modules/Visualization/MonteverdiCore/ConfigureMonteverdi.h")
  if(NOT EXISTS "${ConfigureMonteverdi_H}")
    message(FATAL_ERROR "${ConfigureMonteverdi_H} does not exists. Cannot continue")
  endif()

  file(
    STRINGS "${ConfigureMonteverdi_H}"
    ConfigureMonteverdi_H_CONTENTS
    REGEX "^#define.Monteverdi_INSTALL_DATA_DIR")

  string(REGEX REPLACE
    "^#define.Monteverdi_INSTALL_DATA_DIR" ""
    ConfigureMonteverdi_H_CONTENTS
    ${ConfigureMonteverdi_H_CONTENTS} )

  if(NOT ConfigureMonteverdi_H_CONTENTS)
    message(FATAL_ERROR "Parse error in ${ConfigureMonteverdi_H}. Cannot continue")
  endif()

  string(
    REGEX REPLACE "\"" ""
    PKG_OTB_INSTALL_DATA_DIR
    "${ConfigureMonteverdi_H_CONTENTS}")

  if(NOT PKG_OTB_INSTALL_DATA_DIR)
    message(FATAL_ERROR "parse error in ${ConfigureMonteverdi_H_CONTENTS}. Cannot continue")
  endif()
  
  string(STRIP "${PKG_OTB_INSTALL_DATA_DIR}" PKG_OTB_INSTALL_DATA_DIR)

  set(PKG_OTB_TRANSLATIONS_DIRNAME "${PKG_OTB_INSTALL_DATA_DIR}/i18n")

  # Just check if required variables are defined.
  foreach(req
      PACKAGE_OTB_SRC_DIR
      PACKAGE_SUPPORT_FILES_DIR
      QT_PLUGINS_DIR
      PKG_STAGE_BIN_DIR
      PKG_QTSQLITE_FILENAME
      PKG_QTPLUGINS_DIR
      PKG_QT_TRANSLATIONS_DIR
      PKG_OTB_TRANSLATIONS_DIRNAME
      )
    if(NOT DEFINED ${req} OR "${${req}}" STREQUAL "")
      message(FATAL_ERROR "you must set ${req} before calling this method")
    endif()
    set(vars "${vars}  ${req}=[${${req}}]\n")
  endforeach(req)

  #install icon file for .app file. Monteverdi and Mapla has same icon!
  if(APPLE)
    install(FILES ${PACKAGE_SUPPORT_FILES_DIR}/Monteverdi.icns
      DESTINATION ${PKG_STAGE_DIR})
  endif()

  ####################### install sqldriver plugin ########################
  install(FILES ${QT_PLUGINS_DIR}/sqldrivers/${PKG_QTSQLITE_FILENAME}
    DESTINATION ${PKG_QTPLUGINS_DIR}/sqldrivers)

  ####################### install qt4 translations ###########################
  #get all translations already distributed with qt4
  get_qt_translation_files(QT_TRANSLATIONS_FILES)

  #install all files in ${QT_TRANSLATIONS_FILES}
  install(FILES ${QT_TRANSLATIONS_FILES}  DESTINATION ${PKG_QT_TRANSLATIONS_DIR})

  ####################### install monteverdi translations #####################
  #translation of monteverdi specific strings
  if(NOT EXISTS "${OTB_INSTALL_DIR}/${PKG_OTB_TRANSLATIONS_DIRNAME}")
    message(FATAL_ERROR "Error ${OTB_INSTALL_DIR}/${PKG_OTB_TRANSLATIONS_DIRNAME} not exists")
  endif()
  
  file(GLOB APP_TS_FILES ${PACKAGE_OTB_SRC_DIR}/i18n/*.ts) # qm files
  foreach(APP_TS_FILE ${APP_TS_FILES})
    get_filename_component(APP_TS_FILENAME ${APP_TS_FILE} NAME_WE)
    install(FILES ${OTB_INSTALL_DIR}/${PKG_OTB_TRANSLATIONS_DIRNAME}/${APP_TS_FILENAME}.qm
      DESTINATION ${PKG_STAGE_DIR}/${PKG_OTB_TRANSLATIONS_DIRNAME}
      )
  endforeach()
  
endfunction()
