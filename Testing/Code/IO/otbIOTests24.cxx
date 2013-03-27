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
  REGISTER_TEST(otbExtendedFilenameToReaderOptions);
  REGISTER_TEST(otbExtendedFilenameToWriterOptions);
  REGISTER_TEST(otbImageFileReaderWithExtendedFilename);
  REGISTER_TEST(otbImageFileWriterWithExtendedFilename);
  REGISTER_TEST(otbImageFileWriterWithExtendedOptionBox);
}
