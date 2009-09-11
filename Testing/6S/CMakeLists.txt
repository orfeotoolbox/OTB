SET(6SNEW_SRCS "otbSurfaceReflectanceToReflectanceFilterNew.cxx")
SET(6SATM_SRCS "otbAtmosphericCorrectionsReduceSpectralResponse.cxx")
IF(WIN32)
  ADD_DEFINITIONS(-DWIN32)
ENDIF(WIN32)

ADD_EXECUTABLE(otbSurfaceReflectanceToReflectanceFilterNew ${6SNEW_SRCS})
ADD_EXECUTABLE(otbAtmosphericCorrectionsReduceSpectralResponse ${6SATM_SRCS})

TARGET_LINK_LIBRARIES(otbSurfaceReflectanceToReflectanceFilterNew OTBIO OTBCommon  OTBRadiometry)
TARGET_LINK_LIBRARIES(otbAtmosphericCorrectionsReduceSpectralResponse OTBIO OTBCommon  OTBRadiometry)

INSTALL_TARGETS(/bin otbSurfaceReflectanceToReflectanceFilterNew)
INSTALL_TARGETS(/bin otbAtmosphericCorrectionsReduceSpectralResponse)

