############################################################################
# Sub-directory entries
add_subdirectory( Core )
add_subdirectory( Gui )

if (OTBQtWidget_LOADED)
  message("-- Generating Qt Wrapped Applications.")
  add_subdirectory(ApplicationsWrapper)
endif()
