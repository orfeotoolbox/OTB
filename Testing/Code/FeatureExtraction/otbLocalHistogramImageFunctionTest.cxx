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

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbLocalHistogramImageFunction.h"

int otbLocalHistogramImageFunctionTest(int argc, char * argv[])
{
  const char * inputFilename  = argv[1];
  const char * outputFilename  = argv[2];

  typedef unsigned char InputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType,  Dimension>                  InputImageType;
  typedef otb::ImageFileReader<InputImageType>                    ReaderType;
  typedef otb::LocalHistogramImageFunction<InputImageType>        FunctionType;

  ReaderType::Pointer   reader         = ReaderType::New();
  FunctionType::Pointer function       = FunctionType::New();

  reader->SetFileName(inputFilename);
  reader->Update();
  function->SetInputImage(reader->GetOutput());

  InputImageType::IndexType index;
  index[0] = 100;
  index[1] = 100;

  function->SetNeighborhoodRadius(3);  
  FunctionType::OutputType Result;
  Result = function->EvaluateAtIndex(index);

  std::ofstream outputStream(outputFilename);
  outputStream << std::setprecision(10) << "Local Histogram Image : " << Result << std::endl;

  outputStream.close();

  return EXIT_SUCCESS;
}
