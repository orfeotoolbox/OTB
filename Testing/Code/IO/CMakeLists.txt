# $Id$

IF( NOT OTB_DISABLE_CXX_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Images)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)

SET(IO_TESTS ${CXX_TEST_PATH}/otbIOTests)

#ADD_TEST(otbImageIOFactoryTest ${IO_TESTS} otbImageIOFactoryTest)
ADD_TEST(otbCAIImageIOTest ${IO_TESTS} otbCAIImageIOTest
        ${INPUTDATA}/poupees.jpg )

ADD_TEST(otbGDALImageIOTest ${IO_TESTS} otbGDALImageIOTest
        ${INPUTDATA}/cthead1.png )

ADD_TEST(otbImageFileReaderTestPNG2PNG ${IO_TESTS} otbImageFileReaderTest
        ${INPUTDATA}/cthead1.png
        ${TEMP}/otb_reader_cthead1.png )

ADD_TEST(otbImageFileReaderTestBSQ2PNG ${IO_TESTS} otbImageFileReaderTest
        ${INPUTDATA}/poupees.hd
        ${TEMP}/otb_reader_poupees.png )

ADD_TEST(otbImageFileReaderTestBSQ2TIFF ${IO_TESTS} otbImageFileReaderTest
        ${INPUTDATA}/poupees.hd
        ${TEMP}/otb_reader_poupees.tiff )

ADD_TEST(otbImageFileWriterTestPNG2PNG ${IO_TESTS} otbImageFileWriterTest
        ${INPUTDATA}/cthead1.png 
        ${TEMP}/otb_writer_cthead1.png )

ADD_TEST(otbImageFileWriterTestPNG2BSQ ${IO_TESTS} otbImageFileWriterTest
        ${INPUTDATA}/cthead1.png 
        ${TEMP}/otb_writer_cthead1.hd )

# TESTS RGB

ADD_TEST(otbImageFileReaderRGBTestPNG2PNG ${IO_TESTS} otbImageFileReaderRGBTest
        ${INPUTDATA}/sbuv.png 
        ${TEMP}/otb_reader_RGB_sbuv.png )

ADD_TEST(otbImageFileReaderRGBTestBSQ2PNG ${IO_TESTS} otbImageFileReaderRGBTest
        ${INPUTDATA}/poupees.hd 
        ${TEMP}/otb_reader_RGB_poupees.png )

ADD_TEST(otbImageFileWriterRGBTestPNG2PNG ${IO_TESTS} otbImageFileWriterRGBTest
        ${INPUTDATA}/sbuv.png 
        ${TEMP}/otb_writer_RGB_sbuv.png )

ADD_TEST(otbImageFileWriterRGBTestPNG2BSQ ${IO_TESTS} otbImageFileWriterRGBTest
        ${INPUTDATA}/sbuv.png 
        ${TEMP}/otb_writer_RGB_poupees.hd )

SET(BasicIO_SRCS
otbCAIImageIOTest.cxx
otbGDALImageIOTest.cxx
otbImageFileReaderTest.cxx
otbImageFileWriterTest.cxx
#otbImageIOFactoryTest.cxx
otbImageFileReaderRGBTest.cxx
otbImageFileWriterRGBTest.cxx
)


INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")
#LINK_DIRECTORIES( $ENV{CAI_DIR_LIB} )


ADD_EXECUTABLE(otbIOTests otbIOTests.cxx ${BasicIO_SRCS})
# CAI ${TIFF_LIBRARY}
TARGET_LINK_LIBRARIES(otbIOTests ${CAI_LIBRARY} ${GDAL_LIBRARY} OTBCommon OTBIO ${CAI_LIBRARY} ITKIO ITKCommon)


ENDIF( NOT OTB_DISABLE_CXX_TESTING )




