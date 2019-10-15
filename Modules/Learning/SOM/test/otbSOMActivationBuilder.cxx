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
#include "otbSOMMap.h"
#include "otbSOMActivationBuilder.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkListSample.h"
#include "itkImageRegionIterator.h"

int otbSOMActivationBuilder(int itkNotUsed(argc), char* argv[])
{
  const unsigned int Dimension         = 2;
  char*              vectorSetFileName = argv[1];
  char*              mapFileName       = argv[2];
  char*              outputFileName    = argv[3];

  typedef float                                               ComponentType;
  typedef unsigned char                                       OutputPixelType;
  typedef itk::VariableLengthVector<ComponentType>            PixelType;
  typedef itk::Statistics::EuclideanDistanceMetric<PixelType> DistanceType;

  typedef otb::SOMMap<PixelType, DistanceType, Dimension> MapType;
  typedef otb::ImageFileReader<MapType> MapReaderType;

  typedef otb::VectorImage<ComponentType, Dimension> InputImageType;
  typedef otb::ImageFileReader<InputImageType>   ReaderType;
  typedef itk::Statistics::ListSample<PixelType> ListSampleType;

  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  typedef otb::SOMActivationBuilder<ListSampleType, MapType, OutputImageType> SOMActivationBuilderType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(vectorSetFileName);
  reader->Update();

  ListSampleType::Pointer listSample = ListSampleType::New();
  listSample->SetMeasurementVectorSize(reader->GetOutput()->GetNumberOfComponentsPerPixel());

  itk::ImageRegionIterator<InputImageType> it(reader->GetOutput(), reader->GetOutput()->GetLargestPossibleRegion());

  it.GoToBegin();

  while (!it.IsAtEnd())
  {
    listSample->PushBack(it.Get());
    ++it;
  }

  MapReaderType::Pointer mapReader = MapReaderType::New();
  mapReader->SetFileName(mapFileName);

  SOMActivationBuilderType::Pointer somAct = SOMActivationBuilderType::New();
  somAct->SetInput(mapReader->GetOutput());
  somAct->SetListSample(listSample);

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFileName);
  writer->SetInput(somAct->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
