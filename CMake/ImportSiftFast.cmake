message(STATUS "Importing libSiftFast...")
option(OTB_USE_SIFTFAST "Use Fast SIFT Image Features Library" ON)
message(STATUS "  Using libSiftFast internal version")
mark_as_advanced(OTB_USE_SIFTFAST)
