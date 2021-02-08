/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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


#include "otbVectorDataTransformFilter.h"
#include "itkAffineTransform.h"

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbVectorDataFileReader.h"
#include "otbVectorDataFileWriter.h"

#include "otbVectorDataProjectionFilter.h"

// common typedefs
typedef otb::VectorData<> VectorDataType;
typedef otb::VectorDataTransformFilter<VectorDataType, VectorDataType> VectorDataTransformType;

int otbVectorDataTransformFilter(int itkNotUsed(argc), char* argv[])
{
  typedef otb::VectorImage<double, 2> ImageType;

  typedef otb::ImageFileReader<ImageType>           ReaderType;
  typedef otb::VectorDataFileReader<VectorDataType> VectorDataFileReaderType;
  typedef otb::VectorDataFileWriter<VectorDataType> VectorDataFileWriterType;

  typedef otb::VectorDataProjectionFilter<VectorDataType, VectorDataType> VDProjectionFilterType;

  // Instantiate the image reader
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  reader->UpdateOutputInformation();

  VectorDataFileReaderType::Pointer vdreader = VectorDataFileReaderType::New();
  vdreader->SetFileName(argv[2]);
  vdreader->Update();

  // Reproject the VectorData In the image coordinate system
  VDProjectionFilterType::Pointer vdproj = VDProjectionFilterType::New();
  vdproj->SetInput(vdreader->GetOutput());
  vdproj->SetInputProjectionRef(vdreader->GetOutput()->GetProjectionRef());
  vdproj->SetOutputImageMetadata(&reader->GetOutput()->GetImageMetadata());
  vdproj->SetOutputProjectionRef(reader->GetOutput()->GetProjectionRef());

  // Test the translation using the ApplyTransformTo
  typedef itk::AffineTransform<double, 2> TransformType;
  typedef TransformType::OutputVectorType TranslationParamType;

  // Set up the transform (Apply a translation of 8 pixels in the y direction)
  TransformType::Pointer transform = TransformType::New();
  TranslationParamType   translationParam;
  translationParam[0] = 0;
  translationParam[1] = 8. * reader->GetOutput()->GetSignedSpacing()[1];
  transform->SetTranslation(translationParam);

  VectorDataTransformType::Pointer transformFilter = VectorDataTransformType::New();
  transformFilter->SetInput(vdproj->GetOutput());
  transformFilter->SetTransform(transform);

  // retransform int the input vector projection
  VDProjectionFilterType::Pointer reverseVdProj = VDProjectionFilterType::New();
  reverseVdProj->SetInput(transformFilter->GetOutput());
  reverseVdProj->SetOutputProjectionRef(vdreader->GetOutput()->GetProjectionRef());
  reverseVdProj->SetInputImageMetadata(&reader->GetOutput()->GetImageMetadata());
  reverseVdProj->SetInputProjectionRef(reader->GetOutput()->GetProjectionRef());

  // Write the vectordata
  VectorDataFileWriterType::Pointer vdwriter = VectorDataFileWriterType::New();
  vdwriter->SetInput(reverseVdProj->GetOutput());
  vdwriter->SetFileName(argv[3]);
  vdwriter->Update();

  return EXIT_SUCCESS;
}
