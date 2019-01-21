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

#include "otbROIdataConversion.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkImageRegionIterator.h"
#include <iostream>

int otbROIdataConversion(int itkNotUsed(argc), char* argv[])
{
  typedef double PixelType;
  const unsigned int Dimension = 2;
  typedef otb::Image<PixelType,  Dimension>                      InputImageType;
  typedef otb::Image<PixelType,  Dimension>                      ROIImageType;
  typedef otb::ROIdataConversion<InputImageType, ROIImageType> ConverterType;
  typedef ConverterType::OutputImageType                         OutputImageType;

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileReader<ROIImageType>    ROIReaderType;

  ConverterType::Pointer converter = ConverterType::New();
  ReaderType::Pointer    readerIm = ReaderType::New();
  ReaderType::Pointer    readerTr = ROIReaderType::New();

  readerIm->SetFileName(argv[1]);
  readerTr->SetFileName(argv[2]);
  readerIm->Update();
  readerTr->Update();

  converter->SetInput(readerIm->GetOutput());
  converter->SetROIImage(readerTr->GetOutput());
  converter->Update();

  std::ofstream file;
  file.open(argv[3]);
  itk::ImageRegionIterator<OutputImageType> outputIter(converter->GetOutput(),
                                                       converter->GetOutput()->GetLargestPossibleRegion());
  outputIter.GoToBegin();

  while (!outputIter.IsAtEnd())
    {
    file << outputIter.Get() << "  ";
    ++outputIter;
    }
  file << std::endl;
  file.close();

  return EXIT_SUCCESS;
}
