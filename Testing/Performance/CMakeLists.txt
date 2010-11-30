SET(PERFO_SVM_SRCS "otbReduceSpectralResponseSVMClassifier.cxx")
IF(WIN32)
  ADD_DEFINITIONS(-DWIN32)
ENDIF(WIN32)

ADD_EXECUTABLE(otbReduceSpectralResponseSVMClassifier ${PERFO_SVM_SRCS})


TARGET_LINK_LIBRARIES(otbReduceSpectralResponseSVMClassifier OTBIO OTBCommon OTBRadiometry OTBLearning)


INSTALL_TARGETS(/bin otbReduceSpectralResponseSVMClassifier)

