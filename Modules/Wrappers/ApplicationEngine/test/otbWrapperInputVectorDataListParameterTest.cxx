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

#include "otbWrapperInputVectorDataListParameter.h"

int otbWrapperInputVectorDataListParameterNew(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  typedef otb::Wrapper::InputVectorDataListParameter ParameterType;
  ParameterType::Pointer parameter = ParameterType::New();

  return EXIT_SUCCESS;
}


int otbWrapperInputVectorDataListParameterTest1(int itkNotUsed(argc), char* argv[])
{
  typedef otb::Wrapper::InputVectorDataListParameter ParameterType;
  ParameterType::Pointer param = ParameterType::New();

  const std::string inputFilename1 = argv[1];
  const std::string inputFilename2  = argv[2];

  param->AddFromFileName(inputFilename1);
  param->AddFromFileName(inputFilename2);
  param->SetKey(argv[3]);
  param->SetDescription(argv[4]);

  return EXIT_SUCCESS;
}
