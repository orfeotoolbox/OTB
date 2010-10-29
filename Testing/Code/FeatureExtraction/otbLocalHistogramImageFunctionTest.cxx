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
#include "otbStreamingMinMaxImageFilter.h"

int otbLocalHistogramImageFunctionTest(int argc, char * argv[])
{
  const char * inputFilename  = argv[1];
  const char * outputFilename  = argv[2];

  typedef unsigned char InputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType,  Dimension>                  InputImageType;
  typedef otb::ImageFileReader<InputImageType>                    ReaderType;
  typedef otb::StreamingMinMaxImageFilter<InputImageType>         MinMaxFilterType;
  typedef otb::LocalHistogramImageFunction<InputImageType>        FunctionType;

  
  ReaderType::Pointer reader         = ReaderType::New();
  MinMaxFilterType::Pointer filter   = MinMaxFilterType::New();
  FunctionType::Pointer function     = FunctionType::New();

  reader->SetFileName(inputFilename);
  reader->Update();
  
  filter->SetInput(reader->GetOutput());
  filter->Update();

  function->SetInputImage(reader->GetOutput());

  InputImageType::IndexType index;
  index[0] = 100;
  index[1] = 100;

  function->SetNeighborhoodRadius(3);
  function->SetHistogramMin(filter->GetMinimum());
  function->SetHistogramMax(filter->GetMaximum());
  FunctionType::OutputType Result;
  Result = function->EvaluateAtIndex(index);

  std::ofstream outputStream(outputFilename);
  outputStream << std::setprecision(10) << std::endl;
  for(unsigned int i = 0 ; i < function->GetNumberOfHistogramBins() ; ++i)
    {
      outputStream << "Pos[" <<i <<"] = " << Result->GetFrequency(i) << " -> " << Result->GetMeasurement(i,0) << std::endl;
    }

  outputStream.close();

  return EXIT_SUCCESS;
}
