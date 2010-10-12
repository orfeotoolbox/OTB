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

#include <iostream>
#include <iomanip>
#include <fstream>
#include "itkExceptionObject.h"
#include "itkImage.h"

#include "otbImageFileReader.h"
#include "otbHuImageFunction.h"

int otbHuImage(int argc, char * argv[])
{
  const char * inputFilename  = argv[1];
  const char * outputFilename  = argv[2];

  typedef unsigned char InputPixelType;
  const unsigned int Dimension = 2;

  typedef itk::Image<InputPixelType,  Dimension>             InputImageType;
  typedef otb::ImageFileReader<InputImageType>               ReaderType;
  typedef otb::HuImageFunction<InputImageType>               FunctionType;
  typedef FunctionType::RealType                             RealType;
  
  InputImageType::RegionType region;
  InputImageType::SizeType   size;
  InputImageType::IndexType  start;

  start.Fill(0);
  size[0] = 50;
  size[1] = 50;

  ReaderType::Pointer   reader         = ReaderType::New();
  FunctionType::Pointer function = FunctionType::New();

  reader->SetFileName(inputFilename);

  InputImageType::Pointer image = reader->GetOutput();

  region.SetIndex(start);
  region.SetSize(size);

  image->SetRegions(region);
  image->Update();
  function->SetInputImage(image);
  function->SetNeighborhoodRadius(3);

  InputImageType::IndexType index;
  index[0] = 10;
  index[1] = 10;

  RealType Result = function->EvaluateAtIndex(index);

  std::ofstream outputStream(outputFilename);
  outputStream << std::setprecision(10) << "Hu Image moments: [10]" << std::endl;
  for (unsigned int j = 1; j < 8; j++)
    {
    outputStream << "Hu(" << j << ") = " << Result[j-1] << std::endl;
    }
  outputStream.close();

  return EXIT_SUCCESS;
}
