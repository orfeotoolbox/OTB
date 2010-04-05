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
#include "itkExceptionObject.h"
#include "otbPrepareSRTMDirectory.h"

int otbPrepareSRTMDirectoryTest(int argc, char * argv[])
{

  typedef otb::PrepareSRTMDirectory PrepareSRTMDirectoryType;

  PrepareSRTMDirectoryType::Pointer prepareSRTM = PrepareSRTMDirectoryType::New();
  prepareSRTM->SetFullDEMDirectoryPath(argv[1]);
  prepareSRTM->SetDEMDirectoryPath(argv[2]);
  prepareSRTM->SetULLon(atof(argv[3]));
  prepareSRTM->SetULLat(atof(argv[4]));
  prepareSRTM->SetLRLon(atof(argv[5]));
  prepareSRTM->SetLRLat(atof(argv[6]));

  prepareSRTM->Evaluate();

  return EXIT_SUCCESS;
}
