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
#include "itkMacro.h"
#include "otbImage.h"
#include "otbFunctionWithNeighborhoodToImageFilter.h"
#include "itkVarianceImageFunction.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbFunctionWithNeighborhoodToImageFilter(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  typedef double                                                                    PixelType;
  typedef otb::Image<PixelType, Dimension>                                          InputImageType;
  typedef otb::Image<PixelType, Dimension>                                          OutputImageType;
  typedef otb::ImageFileReader<InputImageType>                                      ReaderType;
  typedef otb::ImageFileWriter<OutputImageType>                            WriterType;
  typedef itk::VarianceImageFunction<InputImageType>                                FunctionType;
  typedef otb::FunctionWithNeighborhoodToImageFilter<InputImageType, OutputImageType, FunctionType> FilterType;

  // Instantiating object
  FilterType::Pointer filter = FilterType::New();
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(argv[1]);
  writer->SetFileName(argv[2]);
  filter->SetInput(reader->GetOutput());

  FunctionType::Pointer function = FunctionType::New();
  function->SetNeighborhoodRadius(atoi(argv[3]));
  function->SetInputImage(reader->GetOutput());

  filter->SetFunction(function);

  writer->SetInput(filter->GetOutput());
  writer->SetNumberOfDivisionsStrippedStreaming(2);
  writer->Update();

  return EXIT_SUCCESS;
}
