/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "itkMacro.h"
#include "otbImage.h"
#include "otbFunctionToImageFilter.h"
#include "otbSarParametricMapFunction.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"


int otbSarParametricMapFunctionToImageFilter(int itkNotUsed(argc), char * argv[])
{
  const unsigned int Dimension = 2;
  typedef float                                                                     PixelType;
  typedef otb::Image<PixelType, Dimension>                                          InputImageType;
  typedef InputImageType::IndexType                                                 IndexType;
  typedef otb::Image<PixelType, Dimension>                                          OutputImageType;
  typedef otb::ImageFileReader<InputImageType>                                      ReaderType;
  typedef otb::ImageFileWriter<OutputImageType>                            WriterType;
  typedef otb::SarParametricMapFunction<InputImageType>                             FunctionType;
  typedef otb::FunctionToImageFilter<InputImageType, OutputImageType, FunctionType> FilterType;
  typedef FunctionType::PointSetType                                                PointSetType;

  // Instantiating object
  FilterType::Pointer filter = FilterType::New();
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  PointSetType::Pointer points = PointSetType::New();

  InputImageType::Pointer image = InputImageType::New();

  InputImageType::RegionType region;
  InputImageType::IndexType index;
  InputImageType::SizeType size;

  size[0] = (unsigned int)( ::atoi(argv[2]) );
  size[1] = (unsigned int)( ::atoi(argv[3]) );
  region.SetSize(size);

  index[0] = 0;
  index[1] = 0;
  region.SetIndex(index);
  image->SetRegions(region);
  image->Allocate();
  image->UpdateOutputInformation();

  std::cout << "main image size : " << image->GetLargestPossibleRegion().GetSize() << std::endl;
  writer->SetFileName(argv[1]);

  filter->SetInput(image);

  FunctionType* function = filter->GetFunction();
  function->SetInputImage(image);

  /** Test on some indexes and some physical coordinates*/
  FunctionType::PointType  p0;
  PixelType                value;

  p0[0] = static_cast<unsigned int>(0);
  p0[1] = static_cast<unsigned int>(0);
  value = 10.0;
  points->SetPoint(0, p0);
  points->SetPointData(0, value);

  p0[0] = static_cast<unsigned int>(size[0] / 2.);
  p0[1] = static_cast<unsigned int>(size[1] / 2.);
  value = 100.0;
  points->SetPoint(1, p0);
  points->SetPointData(1, value);

  p0[0] = static_cast<unsigned int>(size[0] / 4.);
  p0[1] = static_cast<unsigned int>(size[1] / 4.);
  value = 50.0;
  points->SetPoint(2, p0);
  points->SetPointData(2, value);


  function->SetPointSet(points);

  IndexType       polynomalSize;
  polynomalSize[0] = 2;
  polynomalSize[1] = 0;
  function->SetPolynomalSize(polynomalSize);

  function->EvaluateParametricCoefficient();
  function->Print(std::cout);
//  FunctionType::IndexType index;
  index[0] = 0;
  index[1] = 0;
  std::cout << index << " -> " << function->EvaluateAtIndex(index) << std::endl;
  index[0] = 50;
  index[1] = 100;
  std::cout << index << " -> " << function->EvaluateAtIndex(index) << std::endl;
  index[0] = 100;
  index[1] = 50;
  std::cout << index << " -> " << function->EvaluateAtIndex(index) << std::endl;

  writer->SetInput(filter->GetOutput());
  writer->SetNumberOfDivisionsStrippedStreaming(2);
  writer->Update();

  return EXIT_SUCCESS;
}
