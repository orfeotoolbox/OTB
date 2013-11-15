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

#include "otbWrapperOutputImageParameter.h"
#include "otbImageFileReader.h"
#include "otbWrapperTypes.h"

int otbWrapperOutputImageParameterNew(int argc, char* argv[])
{
  typedef otb::Wrapper::OutputImageParameter OutputImageParameterType;
  OutputImageParameterType::Pointer parameter = OutputImageParameterType::New();

  return EXIT_SUCCESS;
}


int otbWrapperOutputImageParameterTest1(int argc, char* argv[])
{
  typedef otb::Wrapper::OutputImageParameter OutputImageParameterType;
  OutputImageParameterType::Pointer param = OutputImageParameterType::New();

  typedef otb::ImageFileReader< otb::Wrapper::FloatVectorImageType > ReaderType;

  ReaderType::Pointer reader = ReaderType::New();

  reader->SetFileName( argv[1] );
  reader->UpdateOutputInformation();

  param->SetValue(reader->GetOutput());
  param->SetFileName( argv[2] );

  param->SetKey(argv[3]);
  param->SetDescription(argv[4]);

  // an explicit writer initialization is needed
  param->InitializeWriters();
  param->Write();

  return EXIT_SUCCESS;
}
