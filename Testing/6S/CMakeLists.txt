SET(6SNEW_SRCS "otbSurfaceReflectanceToReflectanceFilterNew.cxx")
SET(6SATM_SRCS "otbAtmosphericCorrectionsSpectralResponse.cxx")
IF(WIN32)
  ADD_DEFINITIONS(-DWIN32)
ENDIF(WIN32)

ADD_EXECUTABLE(otbSurfaceReflectanceToReflectanceFilterNew ${6SNEW_SRCS})
ADD_EXECUTABLE(otbAtmosphericCorrectionsSpectralResponse ${6SATM_SRCS})

TARGET_LINK_LIBRARIES(otbSurfaceReflectanceToReflectanceFilterNew OTBIO OTBCommon  OTBRadiometry)
TARGET_LINK_LIBRARIES(otbAtmosphericCorrectionsSpectralResponse OTBIO OTBCommon  OTBRadiometry)

INSTALL_TARGETS(/bin otbSurfaceReflectanceToReflectanceFilterNew)
INSTALL_TARGETS(/bin otbAtmosphericCorrectionsSpectralResponse)

