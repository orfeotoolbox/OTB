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

#include "otbWrapperNumericalParameter.h"

int otbWrapperNumericalParameterNew(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  typedef otb::Wrapper::NumericalParameter <int> NumericalParameterType;
  NumericalParameterType::Pointer parameter = NumericalParameterType::New();

  return EXIT_SUCCESS;
}


int otbWrapperNumericalParameterTest1(int itkNotUsed(argc), char* argv[])
{
  typedef otb::Wrapper::NumericalParameter <double> NumericalParameterType;
  NumericalParameterType::Pointer numParam = NumericalParameterType::New();

  const double value = atof (argv[1]);
  const std::string key = argv[2];
  const std::string desc = argv[3];

  numParam->SetValue(value);
  numParam->SetKey(key);
  numParam->SetDescription(desc);

  return EXIT_SUCCESS;
}
