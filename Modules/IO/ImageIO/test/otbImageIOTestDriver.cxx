/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "otbTestMain.h"

void RegisterTests()
{
  REGISTER_TEST(otbImageFileWriterWithExtendedOptionBox);
  REGISTER_TEST(otbImageFileReaderONERAComplex);
  REGISTER_TEST(otbImageFileReaderRADComplexFloat);
  REGISTER_TEST(otbShortRGBImageIOTest);
  REGISTER_TEST(otbPipelineMetadataHandlingWithUFFilterTest);
  REGISTER_TEST(otbImageMetadataFileWriterTest);
  REGISTER_TEST(otbImageScalarFileWriterTestWithoutInputShort);
  REGISTER_TEST(otbImageScalarFileWriterTestWithoutInputInt);
  REGISTER_TEST(otbImageScalarFileWriterTestWithoutInputFloat);
  REGISTER_TEST(otbImageScalarFileWriterTestWithoutInputDouble);
  REGISTER_TEST(otbImageComplexFileWriterTestWithoutInputShort);
  REGISTER_TEST(otbImageComplexFileWriterTestWithoutInputInt);
  REGISTER_TEST(otbImageComplexFileWriterTestWithoutInputFloat);
  REGISTER_TEST(otbImageComplexFileWriterTestWithoutInputDouble);
  REGISTER_TEST(otbVectorImageStreamingFileWriterScalarTestWithoutInputShort);
  REGISTER_TEST(otbVectorImageStreamingFileWriterScalarTestWithoutInputInt);
  REGISTER_TEST(otbVectorImageStreamingFileWriterScalarTestWithoutInputFloat);
  REGISTER_TEST(otbVectorImageStreamingFileWriterScalarTestWithoutInputDouble);
  REGISTER_TEST(otbVectorImageStreamingFileWriterComplexTestWithoutInputShort);
  REGISTER_TEST(otbVectorImageStreamingFileWriterComplexTestWithoutInputInt);
  REGISTER_TEST(otbVectorImageStreamingFileWriterComplexTestWithoutInputFloat);
  REGISTER_TEST(otbVectorImageStreamingFileWriterComplexTestWithoutInputDouble);
  REGISTER_TEST(otbReadingComplexDataIntoComplexImageTest);
  REGISTER_TEST(otbImageFileReaderRADInt);
  REGISTER_TEST(otbImageFileReaderRGBTest);
  REGISTER_TEST(otbImageMetadataStreamingFileWriterTest);
  REGISTER_TEST(otbFloatImageIOTest);
  REGISTER_TEST(otbImageFileReaderMSTAR);
  REGISTER_TEST(otbGDALDriverDoubleWritingTest);
  REGISTER_TEST(otbDoubleImageIOTest);
  REGISTER_TEST(otbImageFileWriterTestCalculateNumberOfDivisions);
  REGISTER_TEST(otbImageFileReaderRADComplexFloatExtract);
  REGISTER_TEST(otbImageFileReaderRADComplexInt);
  REGISTER_TEST(otbImageScalarStreamingFileWriterTestWithoutInputShort);
  REGISTER_TEST(otbImageScalarStreamingFileWriterTestWithoutInputInt);
  REGISTER_TEST(otbImageScalarStreamingFileWriterTestWithoutInputFloat);
  REGISTER_TEST(otbImageScalarStreamingFileWriterTestWithoutInputDouble);
  REGISTER_TEST(otbImageComplexStreamingFileWriterTestWithoutInputShort);
  REGISTER_TEST(otbImageComplexStreamingFileWriterTestWithoutInputInt);
  REGISTER_TEST(otbImageComplexStreamingFileWriterTestWithoutInputFloat);
  REGISTER_TEST(otbImageComplexStreamingFileWriterTestWithoutInputDouble);
  REGISTER_TEST(otbScalarBufferToImageFileWriterTest);
  REGISTER_TEST(otbImageFileWriterONERAComplex);
  REGISTER_TEST(otbPipelineMetadataHandlingTest);
  REGISTER_TEST(otbMultiResolutionReadingInfo);
  REGISTER_TEST(otbImageFileReaderTestFloat);
  REGISTER_TEST(otbVectorImageComplexFloatTest);
  REGISTER_TEST(otbVectorImageComplexDoubleTest);
  REGISTER_TEST(otbImageComplexFloatTest);
  REGISTER_TEST(otbImageComplexDoubleTest);
  REGISTER_TEST(otbVectorImageComplexIntoRealFloatTest);
  REGISTER_TEST(otbVectorImageComplexIntoRealDoubleTest);
  REGISTER_TEST(otbImageFileReaderRADChar);
  REGISTER_TEST(otbShortImageIOTest);
  REGISTER_TEST(otbImageSeriesFileReader);
  REGISTER_TEST(otbStreamingShortImageFileWriterTest);
  REGISTER_TEST(otbImageFileWriterStreamingONERAComplex);
  REGISTER_TEST(otbImageFileReaderWithComplexPixelTest);
  REGISTER_TEST(otbVectorImageFileReaderWithComplexPixelTest);
  REGISTER_TEST(otbImageFileReaderRADFloat);
  REGISTER_TEST(otbVectorImageFileReaderWriterTest);
  REGISTER_TEST(otbIntImageIOTest);
  REGISTER_TEST(otbImageFileReaderONERATest);
  REGISTER_TEST(otbPNGIndexedNbBandsTest);
  REGISTER_TEST(otbImageFileReaderTest);
  REGISTER_TEST(otbVectorImageFileWriterScalarTestWithoutInputShort);
  REGISTER_TEST(otbVectorImageFileWriterScalarTestWithoutInputInt);
  REGISTER_TEST(otbVectorImageFileWriterScalarTestWithoutInputFloat);
  REGISTER_TEST(otbVectorImageFileWriterScalarTestWithoutInputDouble);
  REGISTER_TEST(otbVectorImageFileWriterComplexTestWithoutInputShort);
  REGISTER_TEST(otbVectorImageFileWriterComplexTestWithoutInputInt);
  REGISTER_TEST(otbVectorImageFileWriterComplexTestWithoutInputFloat);
  REGISTER_TEST(otbVectorImageFileWriterComplexTestWithoutInputDouble);
  REGISTER_TEST(otbWritingComplexDataWithComplexImageTest);
  REGISTER_TEST(otbImageFileWriterWithFilterTest);
  REGISTER_TEST(otbImageFileReaderRADComplexDouble);
  REGISTER_TEST(otbPipeline);
  REGISTER_TEST(otbStreamingImageFilterTest);
  REGISTER_TEST(otbStreamingImageFileWriterTest);
  REGISTER_TEST(otbImageFileWriterRGBTest);
  REGISTER_TEST(otbMonobandScalarToImageComplexFloat);
  REGISTER_TEST(otbMonobandScalarToImageComplexDouble);
  REGISTER_TEST(otbMonobandScalarToImageComplexInt);
  REGISTER_TEST(otbMonobandScalarToImageComplexShort);
  REGISTER_TEST(otbMonobandComplexToImageScalarFloat);
  REGISTER_TEST(otbMonobandComplexToImageScalarDouble);
  REGISTER_TEST(otbMonobandComplexToImageScalarInt);
  REGISTER_TEST(otbMonobandComplexToImageScalarShort);
  REGISTER_TEST(otbMonobandComplexToImageComplexFloat);
  REGISTER_TEST(otbMonobandComplexToImageComplexDouble);
  REGISTER_TEST(otbMonobandComplexToImageComplexInt);
  REGISTER_TEST(otbMonobandComplexToImageComplexShort);
  REGISTER_TEST(otbMonobandComplexToVectorImageScalarFloat);
  REGISTER_TEST(otbMonobandComplexToVectorImageScalarDouble);
  REGISTER_TEST(otbMonobandComplexToVectorImageScalarInt);
  REGISTER_TEST(otbMonobandComplexToVectorImageScalarShort);
  REGISTER_TEST(otbMonobandComplexToVectorImageComplexFloat);
  REGISTER_TEST(otbMonobandComplexToVectorImageComplexDouble);
  REGISTER_TEST(otbMonobandComplexToVectorImageComplexInt);
  REGISTER_TEST(otbMonobandComplexToVectorImageComplexShort);
  REGISTER_TEST(otbMultibandScalarToImageComplexFloat);
  REGISTER_TEST(otbMultibandScalarToImageComplexDouble);
  REGISTER_TEST(otbMultibandScalarToImageComplexInt);
  REGISTER_TEST(otbMultibandScalarToImageComplexShort);
  REGISTER_TEST(otbMultibandScalarToVectorImageComplexFloat);
  REGISTER_TEST(otbMultibandScalarToVectorImageComplexDouble);
  REGISTER_TEST(otbMultibandScalarToVectorImageComplexInt);
  REGISTER_TEST(otbMultibandScalarToVectorImageComplexShort);
  REGISTER_TEST(otbMultibandComplexToVectorImageScalarFloat);
  REGISTER_TEST(otbMultibandComplexToVectorImageScalarDouble);
  REGISTER_TEST(otbMultibandComplexToVectorImageScalarInt);
  REGISTER_TEST(otbMultibandComplexToVectorImageScalarShort);
  REGISTER_TEST(otbMultibandComplexToVectorImageComplexFloat);
  REGISTER_TEST(otbMultibandComplexToVectorImageComplexDouble);
  REGISTER_TEST(otbMultibandComplexToVectorImageComplexShort);
  REGISTER_TEST(otbMultibandComplexToVectorImageComplexInt);
  REGISTER_TEST(otbMonobandScalarToVectorImageComplexFloat);
  REGISTER_TEST(otbMonobandScalarToVectorImageComplexDouble);
  REGISTER_TEST(otbMonobandScalarToVectorImageComplexShort);
  REGISTER_TEST(otbMonobandScalarToVectorImageComplexInt);
  REGISTER_TEST(otbMultibandComplexToImageScalarFloat);
  REGISTER_TEST(otbMultibandComplexToImageScalarDouble);
  REGISTER_TEST(otbMultibandComplexToImageScalarShort);
  REGISTER_TEST(otbMultibandComplexToImageScalarInt);
  REGISTER_TEST(otbImageFileWriterTest);
  REGISTER_TEST(otbCompareWritingComplexImageTest);
  REGISTER_TEST(otbImageFileReaderOptBandTest);
  REGISTER_TEST(otbImageFileWriterOptBandTest);
  REGISTER_TEST(otbMultiImageFileWriterTest);
  REGISTER_TEST(otbWriteGeomFile);
}
