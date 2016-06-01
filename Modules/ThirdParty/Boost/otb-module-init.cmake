# Mandatory components
# - date_time -> ossim plugins
set(OTB_Boost_COMPONENTS date_time)
# Optional components
if (BUILD_TESTING)
  list(APPEND OTB_Boost_COMPONENTS unit_test_framework)
endif()

message(STATUS "Looking for boost components: ${OTB_Boost_COMPONENTS}")
find_package ( Boost
  1.35.0
  COMPONENTS ${OTB_Boost_COMPONENTS}
  REQUIRED
  )
