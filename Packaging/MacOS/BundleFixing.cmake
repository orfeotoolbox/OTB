
include(BundleUtilities)
set(BU_CHMOD_BUNDLE_ITEMS ON)
fixup_bundle("${CMAKE_INSTALL_PREFIX}/bin/monteverdi2" "" "${CMAKE_INSTALL_PREFIX}/lib/otb")
