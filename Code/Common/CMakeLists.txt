############################################################################
# Sub-directory entries
add_subdirectory( Core )
add_subdirectory( Gui )

if (OTB_WRAP_QT)
  message("-- Generating Qt Wrapped Applications.")
  add_subdirectory(ApplicationsWrapper)
endif()
