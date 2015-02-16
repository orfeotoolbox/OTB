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


#include "otbTestMain.h"

void RegisterTests()
{
  REGISTER_TEST(otbVectorDataIOFactory);
  //REGISTER_TEST(otbVectorDataSourceNew);
  REGISTER_TEST(otbVectorDataFileReaderNew);
  REGISTER_TEST(otbVectorDataFileReader);
  REGISTER_TEST(otbVectorDataFileWriterNew);
  REGISTER_TEST(otbVectorDataFileWriter);
  REGISTER_TEST(otbVectorDataFileWriterPolygons);
  REGISTER_TEST(otbVectorDataFileWriterMultiPolygons);
  REGISTER_TEST(otbVectorDataFileReaderWriter);
  REGISTER_TEST(otbVectorDataFileGeoReaderWriter);
  REGISTER_TEST(otbOGRVectorDataIONew);
  REGISTER_TEST(otbKMLVectorDataIONew);
  REGISTER_TEST(otbOGRVectorDataIOTestCanRead);
  REGISTER_TEST(otbOGRVectorDataIOCanWrite);
  REGISTER_TEST(otbKMLVectorDataIOTestCanRead);
  REGISTER_TEST(otbKMLVectorDataIOTestCanWrite);
  REGISTER_TEST(otbKMLVectorDataIOTestFileReader);
  REGISTER_TEST(otbVectorDataKeywordlistNew);
  REGISTER_TEST(otbVectorDataKeywordlist);
}
