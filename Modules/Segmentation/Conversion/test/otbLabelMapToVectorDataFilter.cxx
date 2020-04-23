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


#include "otbImageFileReader.h"
#include "otbVectorDataFileWriter.h"
#include "otbVectorData.h"
#include "otbVectorDataProjectionFilter.h"

#include <fstream>
#include <iostream>

#include "otbImage.h"
#include "otbLabelMapToVectorDataFilter.h"
#include "otbAttributesMapLabelObject.h"
#include "itkLabelImageToLabelMapFilter.h"

int otbLabelMapToVectorDataFilter(int argc, char* argv[])
{
  /** Use the labelObjecttopolygon functor (not thread safe) only polygon conversion is available yet*/
  if (argc != 3)
  {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputImageFile outputVectorfile(shp)" << std::endl;
    return EXIT_FAILURE;
  }
  const char* infname  = argv[1];
  const char* outfname = argv[2];

  // Labeled image type
  const unsigned int     Dimension = 2;
  typedef unsigned short LabelType;
  typedef otb::Image<LabelType, Dimension> LabeledImageType;
  typedef otb::ImageFileReader<LabeledImageType> LabeledReaderType;

  // Label map typedef
  typedef otb::AttributesMapLabelObject<LabelType, Dimension, double> LabelObjectType;
  typedef itk::LabelMap<LabelObjectType> LabelMapType;
  typedef itk::LabelImageToLabelMapFilter<LabeledImageType, LabelMapType> LabelMapFilterType;
  typedef otb::VectorData<double, 2>                                      VectorDataType;
  typedef otb::VectorDataFileWriter<VectorDataType> WriterType;
  typedef otb::VectorDataProjectionFilter<VectorDataType, VectorDataType> VectorDataFilterType;

  LabeledReaderType::Pointer lreader = LabeledReaderType::New();
  lreader->SetFileName(infname);

  LabelMapFilterType::Pointer labelMapFilter = LabelMapFilterType::New();
  labelMapFilter->SetInput(lreader->GetOutput());
  labelMapFilter->SetBackgroundValue(itk::NumericTraits<LabelType>::min());
  labelMapFilter->Update();

  WriterType::Pointer writer = WriterType::New();

  typedef otb::LabelMapToVectorDataFilter<LabelMapType, VectorDataType> LabelMapToVectorDataFilterType;

  LabelMapToVectorDataFilterType::Pointer MyFilter = LabelMapToVectorDataFilterType::New();

  MyFilter->SetInput(labelMapFilter->GetOutput());
  MyFilter->Update();

  MyFilter->GetOutput()->SetProjectionRef(lreader->GetOutput()->GetProjectionRef());

  VectorDataFilterType::Pointer vectorDataProjection = VectorDataFilterType::New();
  vectorDataProjection->SetInputOrigin(lreader->GetOutput()->GetOrigin());
  vectorDataProjection->SetInputSpacing(lreader->GetOutput()->GetSignedSpacing());
  vectorDataProjection->SetInput(MyFilter->GetOutput());

  writer->SetFileName(outfname);
  writer->SetInput(vectorDataProjection->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
