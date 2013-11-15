/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/



#include "otbTestMain.h"


void RegisterTests()
{
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
  REGISTER_TEST(otbMonobandScalarToVectorImageComplexFloat);
  REGISTER_TEST(otbMonobandScalarToVectorImageComplexDouble);
  REGISTER_TEST(otbMonobandScalarToVectorImageComplexInt);
  REGISTER_TEST(otbMonobandScalarToVectorImageComplexShort);

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
  REGISTER_TEST(otbMultibandComplexToVectorImageComplexInt);
  REGISTER_TEST(otbMultibandComplexToVectorImageComplexShort);

  // Weird cases not tested (commented in CMakeLists.txt and not implemented, need spec):
  REGISTER_TEST(otbMultibandComplexToImageScalarFloat);
  REGISTER_TEST(otbMultibandComplexToImageScalarDouble);
  REGISTER_TEST(otbMultibandComplexToImageScalarInt);
  REGISTER_TEST(otbMultibandComplexToImageScalarShort);


  // Read complex data with GDAL only
  REGISTER_TEST(otbGDALReadPxlComplexDouble);
  REGISTER_TEST(otbGDALReadPxlComplexFloat);


  // Write Complex data
  REGISTER_TEST(otbImageScalarFileWriterTestWithoutInputShort);
  REGISTER_TEST(otbImageScalarFileWriterTestWithoutInputInt);
  REGISTER_TEST(otbImageScalarFileWriterTestWithoutInputFloat);
  REGISTER_TEST(otbImageScalarFileWriterTestWithoutInputDouble);

  REGISTER_TEST(otbImageComplexFileWriterTestWithoutInputShort);
  REGISTER_TEST(otbImageComplexFileWriterTestWithoutInputInt);
  REGISTER_TEST(otbImageComplexFileWriterTestWithoutInputFloat);
  REGISTER_TEST(otbImageComplexFileWriterTestWithoutInputDouble);


  REGISTER_TEST(otbVectorImageFileWriterScalarTestWithoutInputShort);
  REGISTER_TEST(otbVectorImageFileWriterScalarTestWithoutInputInt);
  REGISTER_TEST(otbVectorImageFileWriterScalarTestWithoutInputFloat);
  REGISTER_TEST(otbVectorImageFileWriterScalarTestWithoutInputDouble);

  REGISTER_TEST(otbVectorImageFileWriterComplexTestWithoutInputShort);
  REGISTER_TEST(otbVectorImageFileWriterComplexTestWithoutInputInt);
  REGISTER_TEST(otbVectorImageFileWriterComplexTestWithoutInputFloat);
  REGISTER_TEST(otbVectorImageFileWriterComplexTestWithoutInputDouble);

  // Streaming Write Complex data
  REGISTER_TEST(otbImageScalarStreamingFileWriterTestWithoutInputShort);
  REGISTER_TEST(otbImageScalarStreamingFileWriterTestWithoutInputInt);
  REGISTER_TEST(otbImageScalarStreamingFileWriterTestWithoutInputFloat);
  REGISTER_TEST(otbImageScalarStreamingFileWriterTestWithoutInputDouble);

  REGISTER_TEST(otbImageComplexStreamingFileWriterTestWithoutInputShort);
  REGISTER_TEST(otbImageComplexStreamingFileWriterTestWithoutInputInt);
  REGISTER_TEST(otbImageComplexStreamingFileWriterTestWithoutInputFloat);
  REGISTER_TEST(otbImageComplexStreamingFileWriterTestWithoutInputDouble);


  REGISTER_TEST(otbVectorImageStreamingFileWriterScalarTestWithoutInputShort);
  REGISTER_TEST(otbVectorImageStreamingFileWriterScalarTestWithoutInputInt);
  REGISTER_TEST(otbVectorImageStreamingFileWriterScalarTestWithoutInputFloat);
  REGISTER_TEST(otbVectorImageStreamingFileWriterScalarTestWithoutInputDouble);

  REGISTER_TEST(otbVectorImageStreamingFileWriterComplexTestWithoutInputShort);
  REGISTER_TEST(otbVectorImageStreamingFileWriterComplexTestWithoutInputInt);
  REGISTER_TEST(otbVectorImageStreamingFileWriterComplexTestWithoutInputFloat);
  REGISTER_TEST(otbVectorImageStreamingFileWriterComplexTestWithoutInputDouble);

  //merge filter function values tests in IOTests23
  REGISTER_TEST(otbSpectralSensitivityReaderNew);
  REGISTER_TEST(otbSpectralSensitivityReaderTest);
  REGISTER_TEST(otbSpectralSensitivityReaderGenericTest);
  REGISTER_TEST(otbFilterFunctionValuesNew);
  REGISTER_TEST(otbWavelengthSpectralBandsNew);

}
