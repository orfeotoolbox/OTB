cmake_minimum_required(VERSION 2.8)

add_custom_target(build_with_nmake 
ALL DEPENDS 
COMMAND
nmake /f ${GEOS_SB_SRC}/makefile.vc
WORKING_DIRECTORY ${GEOS_SB_SRC}
)

install(FILES  ${GEOS_SB_SRC}/capi/geos_c.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
install(FILES  ${GEOS_SB_SRC}/include/geos.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
install(DIRECTORY ${GEOS_SB_SRC}/include/geos DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
install(FILES  ${GEOS_SB_SRC}/src/geos.dll DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(FILES  ${GEOS_SB_SRC}/src/geos_c.dll DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(FILES  ${GEOS_SB_SRC}/src/geos.lib DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
install(FILES  ${GEOS_SB_SRC}/src/geos_i.lib DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
install(FILES  ${GEOS_SB_SRC}/src/geos_c_i.lib DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
