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

#include "otbVectorData.h"
#include "otbVectorDataFileReader.h"
#include "otbVectorDataToLabelMapWithAttributesFilter.h"
#include "otbAttributesMapLabelObject.h"
#include "otbVectorImage.h"
#include "otbImageFileWriter.h"
#include "otbLabelMapToSimulatedImageFilter.h"
#include "otbProspectModel.h"
#include "otbSailModel.h"

int otbLabelMapToSimulatedImageFilterTest(int itkNotUsed(argc), char* argv[])
{
  const char*  infilename     = argv[1];
  const char*  satRSRFilename = argv[2];
  unsigned int nbBands        = static_cast<unsigned int>(atoi(argv[3]));
  const char*  rootPath       = argv[4];
  const char*  outfilename    = argv[5];

  typedef unsigned short LabelType;
  const unsigned int     Dimension = 2;
  typedef otb::VectorImage<double, Dimension> LabeledImageType;
  typedef otb::ImageFileWriter<LabeledImageType> WriterType;
  typedef otb::VectorData<double, Dimension> VectorDataType;
  typedef otb::AttributesMapLabelObject<LabelType, Dimension, std::string> LabelObjectType;
  typedef itk::LabelMap<LabelObjectType>            LabelMapType;
  typedef otb::VectorDataFileReader<VectorDataType> VectorDataFileReaderType;
  typedef otb::VectorDataToLabelMapWithAttributesFilter<VectorDataType, LabelMapType> VectorDataToLabelMapFilterType;

  typedef otb::ProspectModel ProspectType;
  typedef otb::SailModel     SailType;
  typedef otb::LabelMapToSimulatedImageFilter<LabelMapType, ProspectType, SailType, LabeledImageType> LabelMapToSimulatedImageFilterType;


  /** Instantiation of pointer objects*/
  VectorDataFileReaderType::Pointer           reader                = VectorDataFileReaderType::New();
  VectorDataToLabelMapFilterType::Pointer     vectorDataToLabelMap  = VectorDataToLabelMapFilterType::New();
  LabelMapToSimulatedImageFilterType::Pointer labelMapToImageFilter = LabelMapToSimulatedImageFilterType::New();
  WriterType::Pointer                         writer                = WriterType::New();


  reader->SetFileName(infilename);
  reader->Update();

  vectorDataToLabelMap->SetInput(reader->GetOutput());
  vectorDataToLabelMap->SetInitialLabel(1);
  vectorDataToLabelMap->SetBackgroundValue(0);

  labelMapToImageFilter->SetInput(vectorDataToLabelMap->GetOutput());
  labelMapToImageFilter->SetNumberOfComponentsPerPixel(nbBands);
  labelMapToImageFilter->SetSatRSRFilename(satRSRFilename);
  labelMapToImageFilter->SetNumberOfThreads(1); // must be set to 1
  labelMapToImageFilter->SetPathRoot(rootPath);

  // Write the result to an image file
  writer->SetFileName(outfilename);
  writer->SetInput(labelMapToImageFilter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
