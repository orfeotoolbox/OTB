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
#include "otbAeronetData.h"
#include "otbAeronetFileReader.h"
#include "otbSystem.h"

int otbAeronetExtractDataBadData(int itkNotUsed(argc), char * argv[])
{
  otb::AeronetFileReader::Pointer reader = otb::AeronetFileReader::New();

  reader->SetFileName(argv[1]);
  reader->SetDay(atoi(argv[2]));
  reader->SetMonth(atoi(argv[3]));
  reader->SetYear(atoi(argv[4]));
  reader->SetHour(atoi(argv[5]));
  reader->SetMinute(atoi(argv[6]));
  reader->SetEpsilon(atof(argv[7]));

  try
    {
    reader->Update();
    }
  catch (itk::ExceptionObject e)
    {
    return EXIT_SUCCESS;
    }
  return EXIT_FAILURE;
}
