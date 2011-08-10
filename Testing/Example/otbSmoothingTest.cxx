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

#include "otbSmoothing.cxx"
#include "otbWrapperInputImageParameter.h"
#include "otbWrapperOutputImageParameter.h"
#include "otbWrapperNumericalParameter.h"


int otbSmoothingNew(int argc, char* argv[])
{
  typedef otb::Wrapper::Smoothing SmoothingType;
  SmoothingType::Pointer appli = SmoothingType::New();

  return EXIT_SUCCESS;
}


int otbSmoothingTest1(int argc, char* argv[])
{
  typedef otb::Wrapper::Smoothing SmoothingType;
  SmoothingType::Pointer appli = SmoothingType::New();

  dynamic_cast<otb::Wrapper::InputImageParameter *>(appli->GetParameterByKey("in"))->SetFromFileName(argv[1]);
  dynamic_cast<otb::Wrapper::OutputImageParameter *>(appli->GetParameterByKey("out"))->SetFileName(argv[2]);
  
  appli->SetParameterString("type", "gaussian");
  appli->SetParameterInt("type.gaussian.radius", atoi(argv[3]));


  appli->ExecuteAndWriteOutput();

  return EXIT_SUCCESS;
}
