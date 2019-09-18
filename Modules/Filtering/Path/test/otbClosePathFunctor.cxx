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
#include <iostream>

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkConnectedComponentImageFilter.h"
#include "otbPolygon.h"
#include "otbObjectList.h"
#include "otbImageToEdgePathFilter.h"
#include "otbClosePathFunctor.h"
#include "otbUnaryFunctorObjectListFilter.h"

int otbClosePathFunctor(int argc, char* argv[])
{

  if (argc != 3)
  {
    std::cout << "Usage: " << argv[0];
    std::cout << " inputImage outputFile" << std::endl;
    return 1;
  }

  typedef unsigned char  InputPixelType;
  typedef unsigned short LabelPixelType; // FIXME doesn't seem to work with long int (64 bits problem ?)

  typedef otb::Image<InputPixelType, 2> InputImageType;
  typedef otb::Image<LabelPixelType, 2> LabelImageType;

  typedef otb::ImageFileReader<InputImageType> ReaderType;
  ReaderType::Pointer                          reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  typedef itk::ConnectedComponentImageFilter<InputImageType, LabelImageType> ConnectedComponentsFilterType;
  ConnectedComponentsFilterType::Pointer connectedComponentsFilter = ConnectedComponentsFilterType::New();

  //   connectedComponentsFilter->SetInput(reader->GetOutput());
  connectedComponentsFilter->SetInput(reader->GetOutput());
  connectedComponentsFilter->Update();

  std::cout << "# regions: " << connectedComponentsFilter->GetObjectCount() << std::endl;

  typedef otb::Polygon<double>         PolygonType;
  typedef otb::ObjectList<PolygonType> PolygonListType;
  typedef otb::ImageToEdgePathFilter<LabelImageType, PolygonType> PolygonFilterType;

  PolygonListType::Pointer polygonList = PolygonListType::New();

  for (LabelPixelType label = 1; label <= connectedComponentsFilter->GetObjectCount(); ++label)
  {
    std::cerr << ".";
    PolygonFilterType::Pointer polygonFilter = PolygonFilterType::New();
    polygonFilter->SetInput(connectedComponentsFilter->GetOutput());
    polygonFilter->SetForegroundValue(label);
    polygonFilter->Update();

    polygonList->PushBack(polygonFilter->GetOutput());
  }

  typedef otb::ClosePathFunctor<PolygonType, PolygonType> LengthFunctorType;
  typedef otb::UnaryFunctorObjectListFilter<PolygonListType, PolygonListType, LengthFunctorType> ClosePathFilterType;
  ClosePathFilterType::Pointer closePathFilter = ClosePathFilterType::New();
  closePathFilter->SetInput(polygonList);
  closePathFilter->Update();

  const char*   outfile = argv[2];
  std::ofstream file;
  file.open(outfile);

  typedef PolygonType::VertexListType   VertexListType;
  typedef VertexListType::ConstIterator IteratorType;

  typedef PolygonListType::ConstIterator PolygonListIteratorType;

  for (PolygonListIteratorType pIt = closePathFilter->GetOutput()->Begin(); pIt != closePathFilter->GetOutput()->End(); ++pIt)
  {
    file << "--- New Polygon ---" << std::endl;
    PolygonType::Pointer polygon = pIt.Get();
    IteratorType         it;
    for (it = polygon->GetVertexList()->Begin(); it != polygon->GetVertexList()->End(); ++it)
    {
      file << it.Value() << std::endl;
    }
  }

  file.close();

  return EXIT_SUCCESS;
}
