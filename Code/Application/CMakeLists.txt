#############################################################################
# PROJECT(Monteverdi2)

if(WIN32)
  add_definitions(-DWIN32)
endif(WIN32)

#############################################################################
# Qt4 settings
find_package( Qt4 REQUIRED QtCore QtGui)
include( ${QT_USE_FILE} )
add_definitions( ${QT_DEFINITIONS} )
# Add binary dir to build paths in order to include Qt4 wrapped files.
include_directories( ${CMAKE_CURRENT_BINARY_DIR} )
# Choose Qt4 libraries.
set( QT_USE_QTCORE TRUE )
set( QT_USE_QTGUI TRUE )

#############################################################################
# Input files.
file( GLOB Monteverdi2_SOURCES "*.cxx" )
file( GLOB Monteverdi2_HEADERS "*.h" )
file( GLOB Monteverdi2_FORMS "*.ui" )
file( GLOB Monteverdi2_RESOURCES "*.qrc" )

#############################################################################
# Qt4 wrapped files.
qt4_wrap_cpp( Monteverdi2_HEADERS_MOC ${Monteverdi2_HEADERS} )
qt4_wrap_ui( Monteverdi2_FORMS_HEADERS ${Monteverdi2_FORMS}  )
qt4_add_resources( Monteverdi2_RESOURCES_RCC ${Monteverdi2_RESOURCES} )

#############################################################################
add_executable(monteverdi2
  ${Monteverdi2_SOURCES}
  ${Monteverdi2_FORMS_HEADERS}
  ${Monteverdi2_HEADERS_MOC}
  ${Monteverdi2_RESOURCES_RCC}
)

#############################################################################
target_link_libraries(monteverdi2
  ${QT_LIBRARIES}
)

#############################################################################
# INSTALL Monteverdi2 application.
if(NOT OTB_INSTALL_NO_LIBRARIES)
  install(
    TARGETS monteverdi2
    RUNTIME DESTINATION ${Monteverdi2_INSTALL_BIN_DIR_CM24} COMPONENT Runtime
    #LIBRARY DESTINATION ${Monteverdi2_INSTALL_LIB_DIR_CM24} COMPONENT Runtime
    #ARCHIVE DESTINATION ${Monteverdi2_INSTALL_LIB_DIR_CM24} COMPONENT Development
)
endif(NOT OTB_INSTALL_NO_LIBRARIES)

#############################################################################
