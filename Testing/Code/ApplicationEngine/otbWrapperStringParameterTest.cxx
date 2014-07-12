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

#include "otbWrapperStringParameter.h"

int otbWrapperStringParameterNew(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  typedef otb::Wrapper::StringParameter StringParameterType;
  StringParameterType::Pointer parameter = StringParameterType::New();

  return EXIT_SUCCESS;
}


int otbWrapperStringParameterTest1(int itkNotUsed(argc), char* argv[])
{
  typedef otb::Wrapper::StringParameter StringParameterType;
  StringParameterType::Pointer numParam = StringParameterType::New();

  const std::string value = argv[1];
  const std::string key   = argv[2];
  const std::string desc  = argv[3];

  numParam->SetValue(value);
  numParam->SetKey(key);
  numParam->SetDescription(desc);

  return EXIT_SUCCESS;
}
