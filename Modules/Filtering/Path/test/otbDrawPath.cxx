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
#include "itkPolyLineParametricPath.h"

#include "otbImageFileReader.h"
#include "otbDrawPathFilter.h"
#include "otbImageFileWriter.h"

int otbDrawPathDessinCarre(int itkNotUsed(argc), char * argv[])
{
  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  typedef unsigned char InputPixelType;
  typedef unsigned char OutputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType,  Dimension> InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;
  typedef InputImageType::PointType              InputImagePointType;

  typedef itk::PolyLineParametricPath<Dimension> PathType;

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  typedef otb::DrawPathFilter<InputImageType, PathType, OutputImageType> DrawPathFilterType;

  ReaderType::Pointer reader         = ReaderType::New();
  WriterType::Pointer writer         = WriterType::New();
  PathType::Pointer   VertexList     = PathType::New();

  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);

  InputImageType::ConstPointer image = reader->GetOutput();

  InputImagePointType           pos;
  PathType::ContinuousIndexType cindex;

  VertexList->Initialize();

  /*1er Segment*/
  pos[0] = 30;
  pos[1] = 30;
  image->TransformPhysicalPointToContinuousIndex(pos, cindex);
  VertexList->AddVertex(cindex);
  pos[0] = 30;
  pos[1] = 130;
  image->TransformPhysicalPointToContinuousIndex(pos, cindex);
  VertexList->AddVertex(cindex);

  /*2eme Segment*/
  pos[0] = 90;
  pos[1] = 130;
  image->TransformPhysicalPointToContinuousIndex(pos, cindex);
  VertexList->AddVertex(cindex);

  /*3eme Segment*/
  pos[0] = 90;
  pos[1] = 30;
  image->TransformPhysicalPointToContinuousIndex(pos, cindex);
  VertexList->AddVertex(cindex);

  /*4eme Segment*/
  pos[0] = 30;
  pos[1] = 30;
  image->TransformPhysicalPointToContinuousIndex(pos, cindex);
  VertexList->AddVertex(cindex);

  DrawPathFilterType::Pointer DrawPath = DrawPathFilterType::New();

  DrawPath->SetImageInput(reader->GetOutput());
  DrawPath->SetInputPath(VertexList);
  writer->SetInput(DrawPath->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
