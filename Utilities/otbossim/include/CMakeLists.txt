FILE(GLOB ossim_base_HDRS "ossim/base/*.h")
FILE(GLOB ossim_elevation_HDRS "ossim/elevation/*.h")
FILE(GLOB ossim_font_HDRS "ossim/font/*.h")
FILE(GLOB ossim_imaging_HDRS "ossim/imaging/*.h")
FILE(GLOB ossim_init_HDRS "ossim/init/*.h")
FILE(GLOB ossim_kbool_HDRS "ossim/kbool/*.h")
FILE(GLOB ossim_matrix_HDRS "ossim/matrix/*.h")
FILE(GLOB ossim_parallel_HDRS "ossim/parallel/*.h")
FILE(GLOB ossim_plugin_HDRS "ossim/plugin/*.h")
FILE(GLOB ossim_projection_HDRS "ossim/projection/*.h")
FILE(GLOB ossim_support_data_HDRS "ossim/support_data/*.h")
FILE(GLOB ossim_vec_HDRS "ossim/vec/*.h")
FILE(GLOB ossim_vpfutil_HDRS "ossim/vpfutil/*.h")


INSTALL(FILES ${ossim_base_HDRS}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbossim/include/ossim/base
    COMPONENT Development)
INSTALL(FILES ${ossim_elevation_HDRS}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbossim/include/ossim/elevation
    COMPONENT Development)
INSTALL(FILES ${ossim_font_HDRS}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbossim/include/ossim/font
    COMPONENT Development)
INSTALL(FILES ${ossim_imaging_HDRS}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbossim/include/ossim/imaging
    COMPONENT Development)
INSTALL(FILES ${ossim_init_HDRS}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbossim/include/ossim/init
    COMPONENT Development)
INSTALL(FILES ${ossim_kbool_HDRS}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbossim/include/ossim/kbool
    COMPONENT Development)
INSTALL(FILES ${ossim_matrix_HDRS}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbossim/include/ossim/matrix
    COMPONENT Development)
INSTALL(FILES ${ossim_parallel_HDRS}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbossim/include/ossim/parallel
    COMPONENT Development)
INSTALL(FILES ${ossim_plugin_HDRS}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbossim/include/ossim/plugin
    COMPONENT Development)
INSTALL(FILES ${ossim_projection_HDRS}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbossim/include/ossim/projection
    COMPONENT Development)
INSTALL(FILES ${ossim_support_data_HDRS}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbossim/include/ossim/support_data
    COMPONENT Development)
INSTALL(FILES ${ossim_vec_HDRS}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbossim/include/ossim/vec
    COMPONENT Development)
INSTALL(FILES ${ossim_vpfutil_HDRS}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbossim/include/ossim/vpfutil
    COMPONENT Development)

