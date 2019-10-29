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


#include "otbHooverMatrixFilter.h"
#include "otbHooverInstanceFilter.h"
#include "otbLabelMapToAttributeImageFilter.h"

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkLabelImageToLabelMapFilter.h"

int otbHooverInstanceFilterToAttributeImage(int argc, char* argv[])
{
  typedef otb::AttributesMapLabelObject<unsigned int, 2, float> LabelObjectType;
  typedef itk::LabelMap<LabelObjectType>        LabelMapType;
  typedef otb::HooverMatrixFilter<LabelMapType> HooverMatrixFilterType;
  typedef otb::Image<unsigned int, 2>                              ImageType;
  typedef otb::VectorImage<float, 2>                               VectorImageType;
  typedef itk::LabelImageToLabelMapFilter<ImageType, LabelMapType> ImageToLabelMapFilterType;
  typedef otb::ImageFileReader<ImageType>    ImageReaderType;
  typedef HooverMatrixFilterType::MatrixType MatrixType;

  typedef otb::HooverInstanceFilter<LabelMapType> InstanceFilterType;
  typedef otb::LabelMapToAttributeImageFilter<LabelMapType, VectorImageType> AttributeImageFilterType;

  typedef otb::ImageFileWriter<VectorImageType> WriterType;

  if (argc != 4)
  {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " segmentationGT segmentationMS outputAttributeImage" << std::endl;
    return EXIT_FAILURE;
  }

  ImageReaderType::Pointer gt_reader = ImageReaderType::New();
  gt_reader->SetFileName(argv[1]);

  ImageReaderType::Pointer ms_reader = ImageReaderType::New();
  ms_reader->SetFileName(argv[2]);

  ImageToLabelMapFilterType::Pointer gt_filter = ImageToLabelMapFilterType::New();
  gt_filter->SetInput(gt_reader->GetOutput());
  gt_filter->SetBackgroundValue(0);

  ImageToLabelMapFilterType::Pointer ms_filter = ImageToLabelMapFilterType::New();
  ms_filter->SetInput(ms_reader->GetOutput());
  ms_filter->SetBackgroundValue(0);

  HooverMatrixFilterType::Pointer hooverFilter = HooverMatrixFilterType::New();
  hooverFilter->SetGroundTruthLabelMap(gt_filter->GetOutput());
  hooverFilter->SetMachineSegmentationLabelMap(ms_filter->GetOutput());

  hooverFilter->Update();

  MatrixType& mat = hooverFilter->GetHooverConfusionMatrix();

  InstanceFilterType::Pointer instances = InstanceFilterType::New();
  instances->SetGroundTruthLabelMap(gt_filter->GetOutput());
  instances->SetMachineSegmentationLabelMap(ms_filter->GetOutput());
  instances->SetThreshold(0.75);
  instances->SetHooverMatrix(mat);
  instances->SetUseExtendedAttributes(false);

  AttributeImageFilterType::Pointer attributeImageGT = AttributeImageFilterType::New();
  attributeImageGT->SetInput(instances->GetOutputGroundTruthLabelMap());
  attributeImageGT->SetAttributeForNthChannel(0, InstanceFilterType::GetNameFromAttribute(InstanceFilterType::ATTRIBUTE_RC));
  attributeImageGT->SetAttributeForNthChannel(1, InstanceFilterType::GetNameFromAttribute(InstanceFilterType::ATTRIBUTE_RF));
  attributeImageGT->SetAttributeForNthChannel(2, InstanceFilterType::GetNameFromAttribute(InstanceFilterType::ATTRIBUTE_RA));
  attributeImageGT->SetAttributeForNthChannel(3, InstanceFilterType::GetNameFromAttribute(InstanceFilterType::ATTRIBUTE_RM));

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(attributeImageGT->GetOutput());
  writer->SetFileName(argv[3]);

  writer->Update();

  std::cout << "Mean RC =" << instances->GetMeanRC() << std::endl;
  std::cout << "Mean RF =" << instances->GetMeanRF() << std::endl;
  std::cout << "Mean RA =" << instances->GetMeanRA() << std::endl;
  std::cout << "Mean RM =" << instances->GetMeanRM() << std::endl;
  std::cout << "Mean RN =" << instances->GetMeanRN() << std::endl;

  return EXIT_SUCCESS;
}
