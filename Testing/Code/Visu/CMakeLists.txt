
IF( NOT OTB_DISABLE_CXX_TESTING )

# SUBDIRS( ImageViewer ImageColorViewer )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Images)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)


#Tolerance sur diff pixel image
SET(TOL 0.0)

#SET(VISU_TESTS ${CXX_TEST_PATH}/otbVisuTests)


# -------            otb::ExtractROI   ------------------------------



# -------       Fichiers sources CXX -----------------------------------
#SET(BasicVisu_SRCS
#)


INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

# ${TIFF_LIBRARY}

#ADD_EXECUTABLE(otbVisuTests otbVisuTests.cxx ${BasicVisu_SRCS})
#TARGET_LINK_LIBRARIES(otbVisuTests OTBIO OTBCommon ${CAI_LIBRARY}  ${GDAL_LIBRARY} ITKIO ITKCommon)




#ADD_EXECUTABLE(otbFLTKImageViewer otbFLTKImageViewer.cxx)
#TARGET_LINK_LIBRARIES(otbFLTKImageViewer ITKFltkImageViewer OTBIO OTBCommon ITKIO ITKCommon)

ADD_EXECUTABLE(otbImageViewer otbImageViewer.cxx)
TARGET_LINK_LIBRARIES(otbImageViewer ITKFltkImageViewer OTBVisu OTBIO OTBCommon ITKIO ITKCommon)

ADD_EXECUTABLE(otbColorImageViewer otbColorImageViewer.cxx)
TARGET_LINK_LIBRARIES(otbColorImageViewer ITKFltkImageViewer OTBVisu OTBIO OTBCommon ITKIO ITKCommon)



ENDIF( NOT OTB_DISABLE_CXX_TESTING )


# -----------------------------------------------------------------------
# Exemple test simple avec un "main"
# 

