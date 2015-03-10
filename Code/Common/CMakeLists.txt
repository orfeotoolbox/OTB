############################################################################
# Sub-directory entries
add_subdirectory( Core )
add_subdirectory( Gui )

if (OTB_USE_QT4)
  message("-- Generating Qt Wrapped Applications.")
  add_subdirectory(ApplicationsWrapper)
endif()
