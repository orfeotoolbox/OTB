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

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "otbTestMain.h"

void RegisterTests()
{
  REGISTER_TEST(otbImageTest);
  REGISTER_TEST(otbVectorImageLegacyTest);
  REGISTER_TEST(otbVectorImageTest);
  REGISTER_TEST(otbVectorImageComplexNew);
  REGISTER_TEST(otbVectorImageComplexFloatTest);
  REGISTER_TEST(otbVectorImageComplexDoubleTest);
  REGISTER_TEST(otbVectorImageComplexIntoRealFloatTest);
  REGISTER_TEST(otbVectorImageComplexIntoRealDoubleTest);
  REGISTER_TEST(otbImageComplexFloatTest);
  REGISTER_TEST(otbImageComplexDoubleTest);
  REGISTER_TEST(otbStreamingImageFilterTest);
  REGISTER_TEST(otbVectorImageFileReaderWriterTest);
  REGISTER_TEST(otbStreamingShortImageFileWriterTest);
  REGISTER_TEST(otbMultiDatasetReadingInfo);
}
