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

#include "otbWrapperOutputImageListParameter.h"
#include "otbImageFileReader.h"
#include "otbWrapperTypes.h"

int otbWrapperOutputImageListParameterNew(int argc, char* argv[])
{
  typedef otb::Wrapper::OutputImageListParameter ParameterType;
  ParameterType::Pointer parameter = ParameterType::New();

  return EXIT_SUCCESS;
}


int otbWrapperOutputImageListParameterTest1(int argc, char* argv[])
{
  typedef otb::Wrapper::OutputImageListParameter ParameterType;
  ParameterType::Pointer param = ParameterType::New();

  typedef otb::ImageFileReader< otb::Wrapper::VectorImageType > ReaderType;

  ReaderType::Pointer reader1 = ReaderType::New();
  ReaderType::Pointer reader2 = ReaderType::New();

  reader1->SetFileName( argv[1] );
  reader2->SetFileName( argv[2] );
  reader1->UpdateOutputInformation();
  reader2->UpdateOutputInformation();

  param->AddImage(reader1->GetOutput());
  param->AddImage(reader2->GetOutput());
  param->AddFileName( argv[3] );
  param->AddFileName( argv[4] );

  param->SetKey(argv[5]);
  param->SetDescription(argv[6]);

  param->Write();

  return EXIT_SUCCESS;
}
