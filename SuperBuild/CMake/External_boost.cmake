INCLUDE_ONCE_MACRO(BOOST)

SETUP_SUPERBUILD(BOOST)

set(_SB_BOOST_LIBRARYDIR ${SB_INSTALL_PREFIX}/lib)

set(BOOST_SB_CONFIG)

if(OTB_COMPILER_ARCH_IS_X64)
  set(BOOST_SB_CONFIG architecture=x86 address-model=64)
else()
  set(BOOST_SB_CONFIG architecture=x86)
endif()

set(BOOST_SB_CONFIG
  ${BOOST_SB_CONFIG}
  variant=release
  link=shared
  threading=multi
  runtime-link=shared
  --prefix=${SB_INSTALL_PREFIX}
  --includedir=${SB_INSTALL_PREFIX}/include
  --libdir=${_SB_BOOST_LIBRARYDIR}
  --with-system
  --with-serialization
  --with-filesystem
  --with-test
  --with-date_time
  --with-program_options
  --with-thread
  -d0
  )

if(UNIX)
  set(BOOST_BOOTSTRAP_FILE "./bootstrap.sh")
  set(BOOST_B2_EXE "./b2")
else()
  set(BOOST_BOOTSTRAP_FILE "bootstrap.bat")
  set(BOOST_B2_EXE "b2.exe")
endif()

set(BOOST_CONFIGURE_COMMAND ${CMAKE_COMMAND}
  -E chdir ${BOOST_SB_SRC}
  ${BOOST_BOOTSTRAP_FILE}
  --prefix=${SB_INSTALL_PREFIX}
  )

set(BOOST_BUILD_COMMAND ${CMAKE_COMMAND}
  -E chdir ${BOOST_SB_SRC}
  ${BOOST_B2_EXE}
  ${BOOST_SB_CONFIG}
  install
  )

#NOTE: update _SB_Boost_INCLUDE_DIR below when you change version number
ExternalProject_Add(BOOST
  PREFIX BOOST
  URL "http://download.sourceforge.net/project/boost/boost/1.60.0/boost_1_60_0.tar.bz2"
  URL_MD5 65a840e1a0b13a558ff19eeb2c4f0cbe
  BINARY_DIR ${BOOST_SB_BUILD_DIR}
  INSTALL_DIR ${SB_INSTALL_PREFIX}
  DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
  CONFIGURE_COMMAND ${BOOST_CONFIGURE_COMMAND}
  BUILD_COMMAND ${BOOST_BUILD_COMMAND}
  INSTALL_COMMAND ""
)

#HINT: avoid all uses of  _SB_* in External_<project>.cmake
# and depend on much saner CMAKE_PREFIX_PATH for cmake projects.
if(MSVC)
  set(_SB_Boost_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include/boost-1_60)
else()
  set(_SB_Boost_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
endif()
SUPERBUILD_PATCH_SOURCE(BOOST)