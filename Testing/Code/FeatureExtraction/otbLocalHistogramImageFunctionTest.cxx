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



#include <iomanip>

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbLocalHistogramImageFunction.h"

int otbLocalHistogramImageFunctionTest(int argc, char * argv[])
{
  const char * inputFilename  = argv[1];
  const char * outputFilename  = argv[2];

  typedef float InputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType,  Dimension>                  InputImageType;
  typedef otb::ImageFileReader<InputImageType>                    ReaderType;
  typedef otb::LocalHistogramImageFunction<InputImageType>        FunctionType;


  ReaderType::Pointer reader         = ReaderType::New();
  FunctionType::Pointer function     = FunctionType::New();

  reader->SetFileName(inputFilename);
  reader->Update();

  function->SetInputImage(reader->GetOutput());

  InputImageType::IndexType index;
  index[0] = atoi(argv[3]);
  index[1] = atoi(argv[4]);

  function->SetNeighborhoodRadius(atoi(argv[5]));
  function->SetHistogramMin(atoi(argv[6]));
  function->SetHistogramMax(atoi(argv[7]));
  function->GaussianSmoothingOff();

  FunctionType::OutputType Result;
  Result = function->EvaluateAtIndex(index);


  std::ofstream outputStream(outputFilename);
  outputStream << std::setprecision(10) << std::endl;
  outputStream << "Index: " << index << std::endl;

  for (unsigned int i = 0; i < function->GetNumberOfHistogramBins(); ++i)
    {
    outputStream << "Bin[" << i << "] = " << "[" << Result->GetBinMin(0, i) << ", " << Result->GetBinMax(0, i)
        << "[ -> " << Result->GetFrequency(i, 0) << std::endl;
    }

  outputStream.close();

  return EXIT_SUCCESS;
}
