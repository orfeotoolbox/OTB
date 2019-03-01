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


/* Example usage:
./ParallelLineDetectionExample Output/Lines.png Output/ParallelLines.png 20 2 10
*/


// This example illustrates the details of the \doxygen{otb}{ParallelLinePathListFilter}.
//

#include "itkPolyLineParametricPath.h"
#include "otbDrawPathListFilter.h"

#include "otbParallelLinePathListFilter.h"

#include "otbImage.h"
#include "otbImageFileWriter.h"

int main(int argc, char* argv[])
{
  if (argc != 6)
  {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " outputImage ";
    std::cerr << " outputParallelLineImage ";
    std::cerr << " distThreshParallel angThreshParallel commonDistThreshParallel" << std::endl;
    return EXIT_FAILURE;
  }

  double distThreshParallel       = atof(argv[3]);
  double angThreshParallel        = atof(argv[4]);
  double commonDistThreshParallel = atof(argv[5]);

  // We start by creating an empty image
  const unsigned int    Dimension = 2;
  typedef unsigned char PixelType;

  typedef otb::Image<PixelType, Dimension> ImageType;

  ImageType::Pointer image = ImageType::New();

  ImageType::IndexType start;

  start[0] = 0;
  start[1] = 0;

  ImageType::SizeType size;
  size[0] = 600;
  size[1] = 300;

  ImageType::RegionType region;

  region.SetSize(size);
  region.SetIndex(start);
  image->SetRegions(region);
  image->Allocate();
  image->FillBuffer(itk::NumericTraits<PixelType>::Zero);

  // We create some lines
  typedef itk::PolyLineParametricPath<Dimension> PathType;
  typedef otb::ObjectList<PathType>              PathListType;

  PathListType::Pointer lineList = PathListType::New();

  typedef PathType::ContinuousIndexType ContinuousIndexType;
  ContinuousIndexType                   cindex;

  /*-----*/
  PathType::Pointer aLine = PathType::New();
  aLine->Initialize();
  cindex[0] = 1;
  cindex[1] = 41;
  aLine->AddVertex(cindex);

  cindex[0] = 175;
  cindex[1] = 204;
  aLine->AddVertex(cindex);

  lineList->PushBack(aLine);

  /*-----*/
  aLine = PathType::New();
  aLine->Initialize();
  cindex[0] = 60;
  cindex[1] = 18;
  aLine->AddVertex(cindex);

  cindex[0] = 203;
  cindex[1] = 164;
  aLine->AddVertex(cindex);

  lineList->PushBack(aLine);

  /*-----*/
  aLine = PathType::New();
  aLine->Initialize();
  cindex[0] = 174;
  cindex[1] = 99;
  aLine->AddVertex(cindex);

  cindex[0] = 281;
  cindex[1] = 1;
  aLine->AddVertex(cindex);

  lineList->PushBack(aLine);

  /*-----*/
  aLine = PathType::New();
  aLine->Initialize();
  cindex[0] = 3;
  cindex[1] = 233;
  aLine->AddVertex(cindex);

  cindex[0] = 191;
  cindex[1] = 227;
  aLine->AddVertex(cindex);

  lineList->PushBack(aLine);

  /*-----*/
  aLine = PathType::New();
  aLine->Initialize();
  cindex[0] = 254;
  cindex[1] = 279;
  aLine->AddVertex(cindex);

  cindex[0] = 351;
  cindex[1] = 110;
  aLine->AddVertex(cindex);

  lineList->PushBack(aLine);

  /*-----*/
  aLine = PathType::New();
  aLine->Initialize();
  cindex[0] = 270;
  cindex[1] = 287;
  aLine->AddVertex(cindex);

  cindex[0] = 368;
  cindex[1] = 120;
  aLine->AddVertex(cindex);

  lineList->PushBack(aLine);

  /*-----*/
  aLine = PathType::New();
  aLine->Initialize();
  cindex[0] = 355;
  cindex[1] = 204;
  aLine->AddVertex(cindex);

  cindex[0] = 528;
  cindex[1] = 199;
  aLine->AddVertex(cindex);

  lineList->PushBack(aLine);

  /*-----*/
  aLine = PathType::New();
  aLine->Initialize();
  cindex[0] = 437;
  cindex[1] = 243;
  aLine->AddVertex(cindex);

  cindex[0] = 591;
  cindex[1] = 237;
  aLine->AddVertex(cindex);

  lineList->PushBack(aLine);

  // Polylines are drawn on a black
  typedef otb::DrawPathListFilter<ImageType, PathType, ImageType> DrawPathType;
  DrawPathType::Pointer                                           drawPathListFilter = DrawPathType::New();
  drawPathListFilter->SetInput(image);
  drawPathListFilter->SetInputPath(lineList);
  drawPathListFilter->SetPathValue(itk::NumericTraits<PixelType>::max());

  typedef otb::ImageFileWriter<ImageType> WriterType;
  WriterType::Pointer                     writer = WriterType::New();
  writer->SetInput(drawPathListFilter->GetOutput());
  writer->SetFileName(argv[1]);
  writer->Update();

  // Parallel lines are detected. A minimum common length, an angular
  // threshold and a maximum distance threshold have to specified.
  // The input is a pathList of the previously extracted line segments.
  typedef otb::ParallelLinePathListFilter<PathType> ParallelLinePathType;
  ParallelLinePathType::Pointer                     parallelLinePathListFilter = ParallelLinePathType::New();
  parallelLinePathListFilter->SetDistanceThreshold(distThreshParallel);
  parallelLinePathListFilter->SetAngularThreshold(angThreshParallel);
  parallelLinePathListFilter->SetCommonDistanceThreshold(commonDistThreshParallel);
  parallelLinePathListFilter->SetInput(lineList);
  parallelLinePathListFilter->Update();

  // A black background image is built to draw the path on.
  ImageType::Pointer outputParallel = ImageType::New();
  outputParallel->SetRegions(image->GetLargestPossibleRegion());
  outputParallel->Allocate();
  outputParallel->FillBuffer(itk::NumericTraits<PixelType>::Zero);

  // Parallel lines are drawn on a black background image with \doxygen{otb}{DrawPathListFilter}.
  // The \code{SetUseIternalValues()} tells the drawing filter to draw the path with its likelihood
  // value.
  // typedef otb::DrawPathListFilter<ImageType, PathType, ImageType> DrawPathType;
  DrawPathType::Pointer drawPathListFilterParallel = DrawPathType::New();
  drawPathListFilterParallel->SetInput(outputParallel);
  drawPathListFilterParallel->SetInputPath(parallelLinePathListFilter->GetOutput());
  drawPathListFilter->SetPathValue(itk::NumericTraits<PixelType>::max());
  drawPathListFilterParallel->SetUseInternalPathValue(false);

  // Write the Line image
  WriterType::Pointer writerParallel = WriterType::New();
  writerParallel->SetInput(drawPathListFilterParallel->GetOutput());
  writerParallel->SetFileName(argv[2]);
  writerParallel->Update();

  return EXIT_SUCCESS;
}
