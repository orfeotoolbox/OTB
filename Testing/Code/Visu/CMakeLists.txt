
IF( NOT OTB_DISABLE_CXX_TESTING )

# SUBDIRS( ImageViewer ImageColorViewer )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Images)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)


#Tolerance sur diff pixel image
SET(TOL 0.0)

SET(VISU_TESTS ${CXX_TEST_PATH}/otbVisuTests)

ADD_TEST(viTuImageViewNew ${VISU_TESTS} 
        otbImageView
        )

ADD_TEST(viTuImageViewerNew ${VISU_TESTS} 
        otbImageViewerNew
        )

ADD_TEST(viTvImageViewerRGB ${CXX_TEST_PATH}/otbImageViewer 
        ${INPUTDATA}/sbuv.png
        0 )

ADD_TEST(viTvImageViewerNB ${CXX_TEST_PATH}/otbImageViewer 
        ${INPUTDATA}/cthead1.png
        0 )

# -------       Fichiers sources CXX -----------------------------------
SET(BasicVisu_SRCS
otbImageView.cxx
otbImageViewerNew.cxx
)


INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbVisuTests otbVisuTests.cxx ${BasicVisu_SRCS})
TARGET_LINK_LIBRARIES(otbVisuTests OTBVisu OTBIO OTBCommon ${CAI_LIBRARY}  ${GDAL_LIBRARY} ITKIO ITKCommon)

ADD_EXECUTABLE(otbImageViewer otbImageViewer.cxx)
TARGET_LINK_LIBRARIES(otbImageViewer OTBVisu OTBIO OTBCommon ITKIO ITKCommon)

ADD_EXECUTABLE(otbImageViewerWithitkVectorImage otbImageViewerWithitkVectorImage.cxx)
TARGET_LINK_LIBRARIES(otbImageViewerWithitkVectorImage OTBVisu OTBIO OTBCommon ITKIO ITKCommon)

ADD_EXECUTABLE(otbImageViewerWithitkImage otbImageViewerWithitkImage.cxx)
TARGET_LINK_LIBRARIES(otbImageViewerWithitkImage OTBVisu OTBIO OTBCommon ITKIO ITKCommon)


ENDIF( NOT OTB_DISABLE_CXX_TESTING )


# -----------------------------------------------------------------------
# Exemple test simple avec un "main"
# 

