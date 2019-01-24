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


#include "otbSarRadiometricCalibrationToImageFilter.h"
#include "otbImageFileReader.h"
#include "otbExtractROI.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkComplexToModulusImageFilter.h"
#include  "otbStreamingCompareImageFilter.h"

int otbSarRadiometricCalibrationToImageFilterCompareTest(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  typedef float                                                                         RealType;
  typedef std::complex<RealType>                                                        PixelType;
  typedef otb::Image<PixelType, Dimension>                                              InputImageType;
  typedef otb::Image<RealType, Dimension>                                               OutputImageType;
  typedef otb::ImageFileReader<InputImageType>                                          ReaderType;
  typedef otb::SarRadiometricCalibrationToImageFilter<InputImageType, OutputImageType>  FilterComplexType;
  typedef otb::SarRadiometricCalibrationToImageFilter<OutputImageType, OutputImageType> FilterRealType;
  typedef otb::ExtractROI<RealType, RealType>                                           ExtractorType;
  typedef itk::ComplexToModulusImageFilter<InputImageType, OutputImageType>             ModulusType;
  typedef otb::StreamingCompareImageFilter<OutputImageType>                             CompareFilterType;

  // Instantiating object
  FilterComplexType::Pointer filterComplex = FilterComplexType::New();
  FilterRealType::Pointer filterReal = FilterRealType::New();
  ReaderType::Pointer reader = ReaderType::New();
  ExtractorType::Pointer extractorComplex = ExtractorType::New();
  ExtractorType::Pointer extractorReal = ExtractorType::New();
  ModulusType::Pointer modulus = ModulusType::New();
  CompareFilterType::Pointer compare = CompareFilterType::New();

  reader->SetFileName(argv[1]);
  filterComplex->SetInput(reader->GetOutput());

  modulus->SetInput(reader->GetOutput());
  filterReal->SetInput(modulus->GetOutput());

  filterComplex->UpdateOutputInformation();
  filterReal->UpdateOutputInformation();

  if (argc > 2)
    {
    // Generate an extract from the large input
    OutputImageType::RegionType region;
    OutputImageType::IndexType  id;
    id[0] = atoi(argv[2]);   id[1] = atoi(argv[3]);
    OutputImageType::SizeType size;
    size[0] = atoi(argv[4]);   size[1] = atoi(argv[5]);
    region.SetIndex(id);
    region.SetSize(size);

    extractorComplex->SetExtractionRegion(region);
    extractorComplex->SetInput(filterComplex->GetOutput());

    extractorReal->SetExtractionRegion(region);
    extractorReal->SetInput(filterReal->GetOutput());

    compare->SetInput1(extractorComplex->GetOutput());
    compare->SetInput2(extractorReal->GetOutput());
    }
  else
    {
    compare->SetInput1(filterComplex->GetOutput());
    compare->SetInput2(filterReal->GetOutput());
    }

  compare->Update();
  if(compare->GetMAE() > 0.00000001)
    {
    std::cout << "MAE : " << compare->GetMAE() << std::endl;
    return EXIT_FAILURE;
    }


  return EXIT_SUCCESS;
}
