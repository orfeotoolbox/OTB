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
#include "itkPolyLineParametricPath.h"

#include "otbImageFileReader.h"
#include "otbPathListSource.h"
#include "otbImageToPathListAlignFilter.h"
#include "otbDrawPathListFilter.h"
#include "otbImageFileWriter.h"

int otbDrawPathAlign(int itkNotUsed(argc), char * argv[])
{
  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  typedef unsigned char InputPixelType;
  typedef unsigned char OutputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType,  Dimension> InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;

  typedef itk::PolyLineParametricPath<Dimension> PathType;

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  typedef otb::ImageToPathListAlignFilter<InputImageType, PathType> PathListType;
  typedef PathListType::OutputPathListType                          OutputPathListType;

  typedef otb::DrawPathListFilter<InputImageType, PathType, OutputImageType> DrawPathFilterType;

  ReaderType::Pointer reader         = ReaderType::New();
  WriterType::Pointer writer         = WriterType::New();

  DrawPathFilterType::Pointer DrawPath = DrawPathFilterType::New();
  PathType::Pointer           VertexList     = PathType::New();

  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);

  typedef otb::ImageToPathListAlignFilter<InputImageType, PathType> PathListAlignType;
  PathListAlignType::Pointer testList = PathListAlignType::New();

  //OTB-FA-00010-CS
  testList->SetInput(reader->GetOutput());
  testList->Update();

  OutputPathListType * sortiePath = testList->GetOutput();
  int                  nbPath = sortiePath->Size();
  std::cout << "NbPath: " << nbPath << std::endl;

  InputImageType::ConstPointer imageIn   = reader->GetOutput();

  DrawPath->SetInput(imageIn);
  DrawPath->SetInputPath(sortiePath);

  writer->SetInput(DrawPath->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
