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
REGISTER_TEST(otbSystemTest);
REGISTER_TEST(otbTestCommandLineArgumentParser);
REGISTER_TEST(otbTestCommandLineArgumentParserHelp);
REGISTER_TEST(otbTestCommandLineArgumentParserList);
REGISTER_TEST(otbTestCommandLineArgumentParserWithError);
REGISTER_TEST(otbExtractROI);
REGISTER_TEST(otbExtractROINew);
REGISTER_TEST(otbExtractROI_RGB);
REGISTER_TEST(otbMultiChannelExtractROI );
REGISTER_TEST(otbMultiChannelExtractROINew );
REGISTER_TEST(otbMultiToMonoChannelExtractROI );
REGISTER_TEST(otbMultiToMonoChannelExtractROINew );
REGISTER_TEST(otbLineSpatialObjectList);
REGISTER_TEST(otbPointSetSourceTest);
REGISTER_TEST(otbDrawLineSpatialObjectNew);
REGISTER_TEST(otbDrawLineSpatialObject);
REGISTER_TEST(otbDrawLineSpatialObjectListNew);
REGISTER_TEST(otbDrawLineSpatialObjectList);
REGISTER_TEST(otbImageToLineSpatialObjectListNew);
REGISTER_TEST(otbPathListToHistogramGeneratorNew);
REGISTER_TEST(otbPathListToHistogramGenerator);
REGISTER_TEST(otbHistogramStatisticsFunction);
REGISTER_TEST(otbListNew);
REGISTER_TEST(otbList);
REGISTER_TEST(otbObjectListNew);
REGISTER_TEST(otbObjectList);
REGISTER_TEST(otbImageListNew);
REGISTER_TEST(otbImageList);
REGISTER_TEST(otbImageListSourceNew);
REGISTER_TEST(otbImageToImageListFilterNew);
REGISTER_TEST(otbImageListToImageListFilterNew);
}
