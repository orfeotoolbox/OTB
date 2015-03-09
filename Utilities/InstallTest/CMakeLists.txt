# Test "make install"
if("x${CMAKE_INSTALL_PREFIX}" MATCHES "^x${OTB_BINARY_DIR}/InstallTest$")
  otb_add_test(NAME Install
    COMMAND ${CMAKE_COMMAND} -DCONFIGURATION=$<CONFIGURATION>
                             -DOTB_BINARY_DIR=${OTB_BINARY_DIR}
                             -P ${CMAKE_CURRENT_SOURCE_DIR}/InstallTest.cmake
    )
endif()
