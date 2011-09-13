file(GLOB INSTALLER_FILES *.py)
set(INSTALLER_FILES ${INSTALLER_FILES})

add_custom_target(otb ALL)

install(FILES ${INSTALLER_FILES} DESTINATION ${QGIS_DATA_DIR}/python/plugins/otb)


