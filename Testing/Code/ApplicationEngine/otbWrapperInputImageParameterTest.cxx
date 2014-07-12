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

#include "otbWrapperInputImageParameter.h"

int otbWrapperInputImageParameterNew(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  typedef otb::Wrapper::InputImageParameter InputImageParameterType;
  InputImageParameterType::Pointer parameter = InputImageParameterType::New();

  return EXIT_SUCCESS;
}

int otbWrapperInputImageParameterTest1(int itkNotUsed(argc), char* argv[])
{
  typedef otb::Wrapper::InputImageParameter InputImageParameterType;
  InputImageParameterType::Pointer param = InputImageParameterType::New();

  param->SetFromFileName(argv[1]);
  param->SetKey(argv[2]);
  param->SetDescription(argv[3]);

  return EXIT_SUCCESS;
}
