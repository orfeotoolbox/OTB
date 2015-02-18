if (BUILD_TESTING)
  set(OTB_Boost_OPTIONAL_COMPONENTS unit_test_framework)
endif()

# Required
message(STATUS "Looking for required Boost headers")
find_package ( Boost
  1.35.0
  REQUIRED
  )

# Optional components
message(STATUS "Looking for optional Boost commponents : ${OTB_Boost_OPTIONAL_COMPONENTS}")
find_package ( Boost
  1.35.0
  COMPONENTS ${OTB_Boost_OPTIONAL_COMPONENTS}
  )
