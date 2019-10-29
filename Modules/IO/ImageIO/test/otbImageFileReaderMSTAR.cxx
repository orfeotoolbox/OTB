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


#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbVectorImage.h"
#include "otbImage.h"

#include "itkUnaryFunctorImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkRescaleIntensityImageFilter.h"

int otbImageFileReaderMSTAR(int itkNotUsed(argc), char* argv[])
{
  typedef float         InputPixelType;
  typedef unsigned char OutputPixelType;
  const unsigned int    InputDimension = 2;

  typedef otb::Image<itk::FixedArray<InputPixelType, 2>, InputDimension> InputImageType;
  typedef otb::Image<InputPixelType, InputDimension>  InternalImageType;
  typedef otb::Image<OutputPixelType, InputDimension> OutputImageType;

  typedef otb::ImageFileReader<InputImageType> ReaderType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  reader->Update();

  typedef itk::ImageRegionConstIterator<InputImageType> ConstIteratorType;
  typedef itk::ImageRegionIterator<InternalImageType>   IteratorType;

  InputImageType::RegionType inputRegion;

  InputImageType::RegionType::IndexType inputStart;
  InputImageType::RegionType::SizeType  size;

  inputStart[0] = 0;
  inputStart[1] = 0;

  size[0] = reader->GetOutput()->GetLargestPossibleRegion().GetSize()[0];
  size[1] = reader->GetOutput()->GetLargestPossibleRegion().GetSize()[1];

  inputRegion.SetSize(size);
  inputRegion.SetIndex(inputStart);

  InternalImageType::RegionType outputRegion;

  InternalImageType::RegionType::IndexType outputStart;

  outputStart[0] = 0;
  outputStart[1] = 0;

  outputRegion.SetSize(size);
  outputRegion.SetIndex(outputStart);

  InternalImageType::Pointer magnitude = InternalImageType::New();
  magnitude->SetRegions(inputRegion);
  const InternalImageType::SpacingType& spacing     = reader->GetOutput()->GetSignedSpacing();
  const InternalImageType::PointType&   inputOrigin = reader->GetOutput()->GetOrigin();
  double                                outputOrigin[InputDimension];

  for (unsigned int i = 0; i < InputDimension; ++i)
  {
    outputOrigin[i] = inputOrigin[i] + spacing[i] * inputStart[i];
  }

  magnitude->SetSignedSpacing(spacing);
  magnitude->SetOrigin(outputOrigin);
  magnitude->Allocate();

  ConstIteratorType inputIt(reader->GetOutput(), inputRegion);
  IteratorType      outputIt(magnitude, outputRegion);

  for (inputIt.GoToBegin(), outputIt.GoToBegin(); !inputIt.IsAtEnd(); ++inputIt, ++outputIt)
  {
    outputIt.Set(inputIt.Get()[0]);
    //    std::cout << inputIt.Get()[0] << " - " << inputIt.Get()[1] << std::endl;
  }

  typedef itk::RescaleIntensityImageFilter<InternalImageType, OutputImageType> RescalerType;

  RescalerType::Pointer rescaler = RescalerType::New();
  rescaler->SetOutputMinimum(itk::NumericTraits<OutputPixelType>::min());
  rescaler->SetOutputMaximum(itk::NumericTraits<OutputPixelType>::max());
  rescaler->SetInput(magnitude);

  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  WriterType::Pointer writer = WriterType::New();

  writer->SetInput(rescaler->GetOutput());
  writer->SetFileName(argv[2]);

  writer->Update();

  return EXIT_SUCCESS;
}
