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
#include "itkGradientImageFilter.h"
#include "otbHistogramOfOrientedGradientCovariantImageFunction.h"

typedef unsigned short InputPixelType;
const unsigned int     Dimension = 2;

typedef otb::Image<InputPixelType,  Dimension>                                  InputImageType;
typedef otb::ImageFileReader<InputImageType>                                    ReaderType;
typedef itk::GradientImageFilter<InputImageType>                                GradientFilterType;
typedef GradientFilterType::OutputImageType                                     CovariantImageType;
typedef otb::HistogramOfOrientedGradientCovariantImageFunction<CovariantImageType> FunctionType;


int otbHistogramOfOrientedGradientCovariantImageFunctionNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  // Instantiating object
  FunctionType::Pointer function       = FunctionType::New();

  std::cout << function << std::endl;

  return EXIT_SUCCESS;
}


int otbHistogramOfOrientedGradientCovariantImageFunction(int itkNotUsed(argc), char * argv[])
{
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  GradientFilterType::Pointer gradient = GradientFilterType::New();
  gradient->SetInput(reader->GetOutput());
  gradient->SetUseImageSpacing(false);
  gradient->Update();

  // Instantiating object
  FunctionType::Pointer function       = FunctionType::New();
  function->SetInputImage(gradient->GetOutput());

  InputImageType::IndexType index;

  unsigned int radius = atoi(argv[3]);
  index[0] = atoi(argv[4]);
  index[1] = atoi(argv[5]);

  function->SetNeighborhoodRadius(radius);

  FunctionType::OutputType value = function->EvaluateAtIndex(index);

  std::ofstream ofs(argv[2]);

  for(unsigned i = 0; i < 5; ++i)
    {
    for(unsigned j = 0; j<function->GetNumberOfOrientationBins(); ++j)
      {
      ofs<<value[i][j]<<"\t";
      }
    ofs<<std::endl;
    }

  ofs.close();

  return EXIT_SUCCESS;
}
