/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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



#include "otbStandardMetaImageFunctionBuilder.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"

#include "otbImageFunctionAdaptor.h"
#include "otbFlusserMomentsImageFunction.h"
#include "otbRadiometricMomentsImageFunction.h"
#include "otbFourierMellinDescriptorsImageFunction.h"
#include "otbLocalHistogramImageFunction.h"
#include "otbHaralickTexturesImageFunction.h"



int otbStandardMetaImageFunctionBuilder(int itkNotUsed(argc), char* argv[])
{
  const char * inputFilename  = argv[1];

  std::vector<double> p1, p2, p3, p4, p5;

  typedef double                                                PrecisionType;
  typedef double                                                CoordRepType;
  typedef otb::StandardMetaImageFunctionBuilder<PrecisionType,
    CoordRepType>                                               BuilderType;

  typedef otb::Image<PrecisionType, 2>                          ImageType;
  typedef otb::VectorImage<PrecisionType, 2>                    VectorImageType;
  typedef otb::ImageFileReader<ImageType>                       ImageReaderType;
  typedef otb::ImageFileReader<VectorImageType>                 VectorImageReaderType;

  typedef BuilderType::MetaImageFunctionType                    MetaImageFunctionType;
  typedef BuilderType::DataObjectContainerType                  DataObjectContainerType;

  typedef otb::FlusserMomentsImageFunction<ImageType, CoordRepType>  FlusserMomentsIF;
  typedef otb::ImageFunctionAdaptor<FlusserMomentsIF, PrecisionType> AdaptedFlusserMomentsIF;

  typedef otb::RadiometricMomentsImageFunction<ImageType, CoordRepType>  RadiometricMomentsIF;
  typedef otb::ImageFunctionAdaptor<RadiometricMomentsIF, PrecisionType>                AdaptedRadiometricMomentsIF;
  typedef otb::FourierMellinDescriptorsImageFunction<ImageType, CoordRepType>  FourierMellinDescriptorsIF;
  typedef otb::ImageFunctionAdaptor<FourierMellinDescriptorsIF, PrecisionType> AdaptedFourierMellinDescriptorsIF;

  p1.push_back(7);
  p1.push_back(128);
  p1.push_back(0);
  p1.push_back(1500);

  p2.push_back(6);

  p3.push_back(5);

  p4.push_back(9);
  p4.push_back(5);
  p4.push_back(5);

  p5.push_back(10);
  p5.push_back(1);
  p5.push_back(1500);
  p5.push_back(16);
  p5.push_back(4);

  // instantiation
  ImageReaderType::Pointer       Ireader  = ImageReaderType::New();
  VectorImageReaderType::Pointer VIreader = VectorImageReaderType::New();

  BuilderType::Pointer builder = BuilderType::New();

  Ireader->SetFileName(inputFilename);
  VIreader->SetFileName(inputFilename);

  builder->AddImage(Ireader->GetOutput());
  builder->SetNeighborhoodRadius(8);

  builder->SetLocalHistogramIFParameters(p1);
  builder->SetFlusserMomentsIFParameters(p2);
  builder->SetRadiometricMomentsIFParameters(p3);
  builder->SetFourierMellinDescriptorsIFParameters(p4);
  builder->SetHaralickTexturesIFParameters(p5);

  builder->AddImage(VIreader->GetOutput());

  MetaImageFunctionType::Pointer MIF = builder->GetMetaImageFunction();
  std::cout << MIF << std::endl;

  DataObjectContainerType DOC = builder->GetDataObjectContainer();
  std::cout << "DataObjectContainer.size(): " << DOC.size() << std::endl;

  std::cout << static_cast<AdaptedRadiometricMomentsIF *>(MIF->GetNthFunction(0))->GetInternalImageFunction()
            << std::endl;
  std::cout << static_cast<AdaptedRadiometricMomentsIF *>(MIF->GetNthFunction(3))->GetInternalImageFunction()
            << std::endl;


  std::cout << static_cast<AdaptedFlusserMomentsIF *>(MIF->GetNthFunction(1))->GetInternalImageFunction()
            << std::endl;
  std::cout << static_cast<AdaptedFlusserMomentsIF *>(MIF->GetNthFunction(7))->GetInternalImageFunction()
            << std::endl;

  std::cout << static_cast<AdaptedFourierMellinDescriptorsIF *>(MIF->GetNthFunction(2))->GetInternalImageFunction()
            << std::endl;
  std::cout << static_cast<AdaptedFourierMellinDescriptorsIF *>(MIF->GetNthFunction(11))->GetInternalImageFunction()
            << std::endl;

/*
  std::cout << static_cast<AdaptedLocalHistogramIF *>(MIF->GetNthFunction())->GetInternalImageFunction()
            << std::endl;
  std::cout << static_cast<AdaptedLocalHistogramIF *>(MIF->GetNthFunction(5))->GetInternalImageFunction()
            << std::endl;

  std::cout << static_cast<AdaptedHaralickTexturesIF *>(MIF->GetNthFunction(4))->GetInternalImageFunction()
            << std::endl;
  std::cout << static_cast<AdaptedHaralickTexturesIF *>(MIF->GetNthFunction(21))->GetInternalImageFunction()
            << std::endl;
*/

  return EXIT_SUCCESS;
}
