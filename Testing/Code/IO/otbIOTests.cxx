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

// this file defines the otbCommonTest for the test driver
// and all it expects is that you have a function called RegisterTests
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <iostream>
#include "otbTestMain.h" 


void RegisterTests()
{
REGISTER_TEST(otbGDALImageIOTestCanRead);
REGISTER_TEST(otbGDALImageIOTestCanWrite);
REGISTER_TEST(otbONERAImageIOTestCanRead);
REGISTER_TEST(otbMSTARImageIOTestCanRead);
REGISTER_TEST(otbImageFileReaderMSTAR);
REGISTER_TEST(otbImageFileReaderTest);
REGISTER_TEST(otbImageFileWriterTest);
REGISTER_TEST(otbImageMetadataFileWriterTest);
REGISTER_TEST(otbImageMetadataStreamingFileWriterTest);
REGISTER_TEST(otbVectorImageFileReaderWriterTest);
REGISTER_TEST(otbImageFileReaderRGBTest);
REGISTER_TEST(otbImageFileWriterRGBTest);
REGISTER_TEST(otbImageTest);
REGISTER_TEST(otbMultiToMonoChannelExtractROISAR);
REGISTER_TEST(otbImageFileReaderERS);
REGISTER_TEST(otbImageFileReaderONERATest);
REGISTER_TEST(otbImageFileReaderONERAComplex);
REGISTER_TEST(otbImageFileWriterONERAComplex);
REGISTER_TEST(otbImageFileWriterStreamingONERAComplex);
REGISTER_TEST(otbStreamingImageFilterTest);
REGISTER_TEST(otbStreamingImageFileWriterTest);
REGISTER_TEST(otbStreamingShortImageFileWriterTest);
REGISTER_TEST(otbStreamingImageFileWriterWithFilterTest);
REGISTER_TEST(otbPipelineMetadataHandlingTest);
REGISTER_TEST(otbPipelineMetadataHandlingWithUFFilterTest);
REGISTER_TEST(otbStreamingImageFileWriterTestCalculateNumberOfDivisions);
REGISTER_TEST(otbVectorImageTest);

REGISTER_TEST(otbShortImageIOTest);
REGISTER_TEST(otbShortRGBImageIOTest);
REGISTER_TEST(otbIntImageIOTest);
REGISTER_TEST(otbFloatImageIOTest);
REGISTER_TEST(otbDoubleImageIOTest);

REGISTER_TEST(otbLUMImageIOTestCanRead);
REGISTER_TEST(otbLUMImageIOTestCanWrite);
REGISTER_TEST(otbBSQImageIOTestCanRead);
REGISTER_TEST(otbBSQImageIOTestCanWrite);

REGISTER_TEST(otbDXFToSpatialObjectGroupFilterNew);

REGISTER_TEST(otbSpatialObjectDXFReaderNew);
REGISTER_TEST(otbSpatialObjectDXFReader);

REGISTER_TEST(otbOSSIMImageMetaDataReaderTest);

REGISTER_TEST(otbDEMReaderNew);
REGISTER_TEST(otbDEMReaderTest);
REGISTER_TEST(otbImageGeometryHandlerNew);
}
