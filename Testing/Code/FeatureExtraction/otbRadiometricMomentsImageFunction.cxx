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



#include <iostream>
#include <iomanip>
#include <fstream>
#include "itkMacro.h"
#include "otbImage.h"

#include "otbImageFileReader.h"
#include "otbRadiometricMomentsImageFunction.h"

int otbRadiometricMomentsImageFunctionNew(int itkNotUsed(argc), char ** itkNotUsed(argv))
{
  typedef unsigned char InputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType,  Dimension>                  InputImageType;
  typedef otb::RadiometricMomentsImageFunction<InputImageType>    FunctionType;

  // Instantiating object
  FunctionType::Pointer function       = FunctionType::New();

  std::cout << function << std::endl;

  return EXIT_SUCCESS;
}


int otbRadiometricMomentsImageFunction(int itkNotUsed(argc), char * argv[])
{
  const char * inputFilename  = argv[1];
  const char * outputFilename  = argv[2];

  typedef unsigned char InputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType,  Dimension>                  InputImageType;
  typedef otb::ImageFileReader<InputImageType>                    ReaderType;
  typedef otb::RadiometricMomentsImageFunction<InputImageType>    FunctionType;
  typedef FunctionType::OutputType                                OutputType;

  ReaderType::Pointer   reader         = ReaderType::New();
  FunctionType::Pointer function       = FunctionType::New();

  reader->SetFileName(inputFilename);
  reader->Update();
  function->SetInputImage(reader->GetOutput());

  InputImageType::IndexType index;
  index[0] = 100;
  index[1] = 100;

  function->SetNeighborhoodRadius(3);
  OutputType Result;
  Result = function->EvaluateAtIndex(index);

  std::ofstream outputStream(outputFilename);
  outputStream << std::setprecision(10) << "Radiometric moments: [10]" << std::endl;

  for (unsigned int j = 1; j < 5; ++j)
    {
    outputStream << "Radiometric Moment(" << j << ") = " << Result[j-1] << std::endl;
    }

  outputStream.close();

  return EXIT_SUCCESS;
}
