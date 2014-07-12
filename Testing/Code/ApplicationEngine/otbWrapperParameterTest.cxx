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

#include "otbWrapperParameter.h"

int otbWrapperParameterNew(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  typedef otb::Wrapper::Parameter ParameterBaseType;
  ParameterBaseType::Pointer parameter = ParameterBaseType::New();

  //std::cout << parameter << std::endl;

  return EXIT_SUCCESS;
}

int otbWrapperParameterTest1(int itkNotUsed(argc), char* argv[])
{
  typedef otb::Wrapper::Parameter ParameterBaseType;
  ParameterBaseType::Pointer parameter = ParameterBaseType::New();

  const std::string name = argv[1];

  parameter->SetName(name);

  if (name == parameter->GetName())
    {
    return EXIT_SUCCESS;
    }
  else
    {
    return EXIT_FAILURE;
    }
}
