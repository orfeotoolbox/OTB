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


#include <iostream>
#include "itkPolyLineParametricPath.h"

#include "otbImageFileWriter.h"
#include "otbImageFileReader.h"
#include "otbExtractROI.h"
#include "otbImageToPathListAlignFilter.h"
#include "otbDrawPathFilter.h"
//#include "otbColorImageViewer.h"

#include <stdio.h>

int main(int argc, char ** argv)
{
  if (argc != 3)
    {

    std::cout << "Usage : " << argv[0] << " inputImage outputImage" << std::endl;
    return 1;

    }

  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  typedef unsigned char InputPixelType;
  typedef unsigned char OutputPixelType;

  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType,  Dimension> InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);

  reader->Update();

  std::cout << "Lecture terminee" << std::endl;

  typedef otb::ExtractROI<InputPixelType, InputPixelType> ROIFilterType;

  ROIFilterType::Pointer roiFilter = ROIFilterType::New();

  roiFilter->SetInput(reader->GetOutput());
  roiFilter->SetStartX(10);
  roiFilter->SetStartY(0);
  roiFilter->SetSizeX(256);
  roiFilter->SetSizeY(256);

  roiFilter->Update();

  std::cout << "Extraction ROI" << std::endl;

  typedef itk::PolyLineParametricPath<Dimension>                    PathType;
  typedef otb::ImageToPathListAlignFilter<InputImageType, PathType> ListAlignFilterType;

  ListAlignFilterType::Pointer alignFilter = ListAlignFilterType::New();

  alignFilter->SetInput(roiFilter->GetOutput());

  alignFilter->Update();

  std::cout << "Alignements termines" << std::endl;

  typedef ROIFilterType::OutputImageType BackgroundImageType;

  typedef otb::DrawPathFilter<BackgroundImageType, PathType, OutputImageType> DrawPathFilterType;

  DrawPathFilterType::Pointer drawPathFilter = DrawPathFilterType::New();

  typedef ListAlignFilterType::OutputPathListType ListType;

  ListType* listePaths = alignFilter->GetOutput();

  ListType::Iterator listIt = listePaths->Begin();

  BackgroundImageType::Pointer backgroundImage = roiFilter->GetOutput();

  roiFilter->Update();

  unsigned int color = 0;

  while (listIt != listePaths->End())
    {

    drawPathFilter->SetImageInput(backgroundImage);
    drawPathFilter->SetInputPath(listIt.Get());
    //drawPathFilter->SetPathValue( color );

    drawPathFilter->Update();

    backgroundImage = drawPathFilter->GetOutput();

    ++listIt;
    ++color;

    }

  writer->SetInput(drawPathFilter->GetOutput());

  writer->Update();

/*  typedef otb::ColorImageViewer<unsigned char, double>    ViewerType;
  ViewerType                      viewer;

  viewer.SetLabel( "Input Image" );
  viewer.SetImage( drawPathFilter->GetOutput() );

  viewer.Show();
  Fl::run();
*/

  return EXIT_SUCCESS;
}
