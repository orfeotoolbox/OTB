IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
IF(OTB_DATA_USE_LARGEINPUT)
  SET(IMAGEDATA ${OTB_DATA_LARGEINPUT_ROOT} )
ENDIF(OTB_DATA_USE_LARGEINPUT)
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)


# Tolerances
SET(TOL 0.0)
SET(EPSILON 0.000001)

# Test programs
SET(VISUREFAC_TESTS1 ${CXX_TEST_PATH}/otbVisuRefacTests1)

#------------ otb::ImageWidget -------------------------

ADD_TEST(vrTuImageWidgetNew ${VISUREFAC_TESTS1}
otbImageWidgetNew
)

ADD_TEST(vrTvImageWidgetWithoutGlAcceleration ${VISUREFAC_TESTS1}
otbImageWidget
${INPUTDATA}/poupees.png 0
)

IF(OTB_GL_USE_ACCEL)
ADD_TEST(vrTvImageWidgetWithGlAcceleration ${VISUREFAC_TESTS1}
otbImageWidget
${INPUTDATA}/poupees.png 1
)
ENDIF(OTB_GL_USE_ACCEL)

#------------ otb::ImageWidgetActionHandler -------------

ADD_TEST(vrTuImageWidgetActionHandlerNew ${VISUREFAC_TESTS1}
otbImageWidgetActionHandlerNew
)

#------------ otb::ImageWidgetController ----------------

ADD_TEST(vrTuImageWidgetControllerNew ${VISUREFAC_TESTS1}
otbImageWidgetControllerNew
)

ADD_TEST(vrTvImageWidgetController ${VISUREFAC_TESTS1}
otbImageWidgetController
)

#------------ otb::ImageLayer ----------------------------

ADD_TEST(vrTuImageLayerNew ${VISUREFAC_TESTS1}
otbImageLayerNew
)

#------------ otb::GrayscaleRenderingFunction ------------

ADD_TEST(vrTuGrayscaleRenderingFunctionNew ${VISUREFAC_TESTS1}
otbGrayscaleRenderingFunctionNew
)

#------------ otb::RenderingImageFilter ------------

ADD_TEST(vrTuRenderingImageFilterNew ${VISUREFAC_TESTS1}
otbRenderingImageFilterNew
)

ADD_TEST(vrTvRenderingImageFilterScalar ${VISUREFAC_TESTS1}
--compare-image ${TOL}
${BASELINE}/vrTvRenderingImageFilterScalarOutput.png
${TEMP}/vrTvRenderingImageFilterScalarOutput.png
otbRenderingImageFilterScalar
${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
${TEMP}/vrTvRenderingImageFilterScalarOutput.png
127 417
)

ADD_TEST(vrTvRenderingImageFilterVector ${VISUREFAC_TESTS1}
--compare-image ${TOL}
${BASELINE}/vrTvRenderingImageFilterVectorOutput.png
${TEMP}/vrTvRenderingImageFilterVectorOutput.png
otbRenderingImageFilterVector
${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
${TEMP}/vrTvRenderingImageFilterVectorOutput.png
0 179 0 0 0 802 0 0 0
)

SET(VisuRefac_SRCS1
otbImageWidgetNew.cxx
otbImageWidget.cxx
otbImageWidgetActionHandlerNew.cxx
otbImageWidgetControllerNew.cxx
otbImageWidgetController.cxx
otbImageLayerNew.cxx
otbGrayscaleRenderingFunctionNew.cxx
otbRenderingImageFilterNew.cxx
otbRenderingImageFilterScalar.cxx
otbRenderingImageFilterVector.cxx
)

INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}") 

ADD_EXECUTABLE(otbVisuRefacTests1 otbVisuRefacTests1.cxx ${VisuRefac_SRCS1})
TARGET_LINK_LIBRARIES(otbVisuRefacTests1 OTBVisuRefac OTBIO)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
