INCLUDE_ONCE_MACRO(BOOST)

SETUP_SUPERBUILD(BOOST)

#all we loose is one single test from not building boost unit-testing-framework.
#That is single test code. I don't see why we can't use something in-house
#change test code to not use boost unit-testing-framework and all is well.

if(MSVC)
  set(BOOST_URL "http://download.sourceforge.net/project/boost/boost/1.60.0/boost_1_60_0.tar.bz2")
  set(BOOST_URL_MD5 "65a840e1a0b13a558ff19eeb2c4f0cbe")
else()
  set(BOOST_URL "http://download.sourceforge.net/project/boost/boost/1.50.0/boost_1_50_0.tar.bz2")
  set(BOOST_URL_MD5 "52dd00be775e689f55a987baebccc462")
endif()

ExternalProject_Add(BOOST
  PREFIX BOOST
  URL "${BOOST_URL}"
  URL_MD5 ${BOOST_URL_MD5}
  BINARY_DIR ${BOOST_SB_BUILD_DIR}
  INSTALL_DIR ${SB_INSTALL_PREFIX}
  DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
  CMAKE_CACHE_ARGS ${SB_CMAKE_CACHE_ARGS}
  PATCH_COMMAND ${CMAKE_COMMAND} -E copy
  ${CMAKE_SOURCE_DIR}/patches/BOOST/CMakeLists.txt
  ${BOOST_SB_SRC}
  )

set(_SB_Boost_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
set(_SB_Boost_LIBRARY_DIR ${SB_INSTALL_PREFIX}/lib)
