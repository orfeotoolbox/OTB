SET(6SNEW_SRCS "otbSurfaceReflectanceToReflectanceFilterNew.cxx")

IF(WIN32)
  ADD_DEFINITIONS(-DWIN32)
ENDIF(WIN32)

ADD_EXECUTABLE(otbSurfaceReflectanceToReflectanceFilterNew ${6SNEW_SRCS})


TARGET_LINK_LIBRARIES(otbSurfaceReflectanceToReflectanceFilterNew OTBIO OTBCommon  OTBRadiometry)


INSTALL_TARGETS(/bin otbSurfaceReflectanceToReflectanceFilterNew)
