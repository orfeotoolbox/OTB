if (BUILD_TESTING)
  set(OTB_Boost_OPTIONAL_COMPONENTS unit_test_framework)
endif()

# Required
find_package ( Boost
  1.35.0
  REQUIRED
  )

# Optional components
find_package ( Boost
  1.35.0
  COMPONENTS ${OTB_Boost_OPTIONAL_COMPONENTS}
  )
