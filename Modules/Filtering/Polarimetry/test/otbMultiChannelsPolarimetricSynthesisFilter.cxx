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
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbMultiChannelsPolarimetricSynthesisFilter.h"

int otbMultiChannelsPolarimetricSynthesisFilter(int itkNotUsed(argc), char* argv[])
{
  const char* inputFilename1 = argv[1];
  const char* inputFilename2 = argv[2];
  const char* inputFilename3 = argv[3];

  const char* outputFilename = argv[4];

  double PsiI = strtod(argv[5], nullptr);
  double KhiI = strtod(argv[6], nullptr);
  double PsiR = strtod(argv[7], nullptr);
  double KhiR = strtod(argv[8], nullptr);

  typedef std::complex<double> InputPixelType;
  typedef double               OutputPixelType;
  const unsigned int           Dimension = 2;
  typedef otb::Image<InputPixelType, Dimension>  InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;

  typedef otb::VectorImage<InputPixelType, Dimension> InputVectorImageType;

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  typedef otb::MultiChannelsPolarimetricSynthesisFilter<InputVectorImageType, OutputImageType> FilterType;

  FilterType::Pointer polarimetricSynthesis = FilterType::New();

  polarimetricSynthesis->SetPsiI(PsiI);
  polarimetricSynthesis->SetKhiI(KhiI);
  polarimetricSynthesis->SetPsiR(PsiR);
  polarimetricSynthesis->SetKhiR(KhiR);

  ReaderType::Pointer reader1 = ReaderType::New();
  ReaderType::Pointer reader2 = ReaderType::New();
  ReaderType::Pointer reader3 = ReaderType::New();
  WriterType::Pointer writer  = WriterType::New();

  reader1->SetFileName(inputFilename1);
  reader2->SetFileName(inputFilename2);
  reader3->SetFileName(inputFilename3);
  writer->SetFileName(outputFilename);

  reader1->Update();
  reader2->Update();
  reader3->Update();

  InputVectorImageType::Pointer outputPtr = InputVectorImageType::New();

  InputVectorImageType::IndexType start;
  start[0] = 0;
  start[1] = 0;

  InputVectorImageType::SizeType size = reader1->GetOutput()->GetLargestPossibleRegion().GetSize();

  InputVectorImageType::RegionType region;
  region.SetSize(size);
  region.SetIndex(start);
  outputPtr->SetRegions(region);
  outputPtr->SetNumberOfComponentsPerPixel(3);
  outputPtr->Allocate();
  InputVectorImageType::PixelType pix0;
  pix0.SetSize(3);
  InputPixelType complexpix(0, 0);
  pix0.Fill(complexpix);

  outputPtr->FillBuffer(pix0);

  InputVectorImageType::Pointer vectorImage = InputVectorImageType::New();

  // Copy of the pictures in a vector
  itk::ImageRegionConstIterator<InputImageType>  inputIt1(reader1->GetOutput(), reader1->GetOutput()->GetLargestPossibleRegion());
  itk::ImageRegionConstIterator<InputImageType>  inputIt2(reader2->GetOutput(), reader2->GetOutput()->GetLargestPossibleRegion());
  itk::ImageRegionConstIterator<InputImageType>  inputIt3(reader3->GetOutput(), reader3->GetOutput()->GetLargestPossibleRegion());
  itk::ImageRegionIterator<InputVectorImageType> outputIt(outputPtr, outputPtr->GetLargestPossibleRegion());

  inputIt1.GoToBegin();
  inputIt2.GoToBegin();
  inputIt3.GoToBegin();
  outputIt.GoToBegin();

  while (!inputIt1.IsAtEnd())
  {
    InputVectorImageType::PixelType pix;
    pix.SetSize(4);
    pix[0] = inputIt1.Get();
    pix[1] = inputIt2.Get();
    pix[2] = inputIt3.Get();

    outputIt.Set(pix);
    ++inputIt1;
    ++inputIt2;
    ++inputIt3;
    ++outputIt;
  }

  polarimetricSynthesis->SetInput(outputPtr);
  writer->SetInput(polarimetricSynthesis->GetOutput());

  writer->Update();

  return EXIT_SUCCESS;
}
