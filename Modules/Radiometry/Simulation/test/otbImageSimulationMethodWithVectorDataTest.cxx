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


#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbImageFileWriter.h"
#include "otbVectorDataFileReader.h"

#include "otbVectorDataToLabelMapWithAttributesFilter.h"
#include "otbImageSimulationMethod.h"
#include "otbAttributesMapLabelObject.h"
#include "otbImageFileReader.h"

int otbImageSimulationMethodWithVectorDataTest(int itkNotUsed(argc), char* argv[])
{
  const char*  infilename       = argv[1];
  const char*  satRSRFilename   = argv[2];
  unsigned int nbBands          = static_cast<unsigned int>(atoi(argv[3]));
  const char*  rootPath         = argv[4];
  unsigned int radius           = atoi(argv[5]);
  const char*  outfilename      = argv[6];
  const char*  outLabelfilename = argv[7];


  typedef unsigned short LabelType;
  const unsigned int     Dimension = 2;
  typedef otb::Image<LabelType, Dimension>    LabelImageType;
  typedef otb::VectorImage<double, Dimension> OutputImageType;
  typedef otb::ImageFileWriter<OutputImageType> ImageWriterType;
  typedef otb::ImageFileWriter<LabelImageType>  LabelImageWriterType;
  typedef otb::VectorData<double, Dimension> VectorDataType;
  typedef otb::AttributesMapLabelObject<LabelType, Dimension, std::string> LabelObjectType;
  typedef itk::LabelMap<LabelObjectType>            LabelMapType;
  typedef otb::VectorDataFileReader<VectorDataType> VectorDataFileReaderType;

  typedef otb::VectorDataToLabelMapWithAttributesFilter<VectorDataType, LabelMapType> SpatialisationFilterType;
  typedef otb::ProspectModel                                   SimulationStep1Type;
  typedef otb::SailModel                                       SimulationStep2Type;
  typedef otb::ProlateInterpolateImageFunction<LabelImageType> FTMType;
  typedef otb::ImageSimulationMethod<VectorDataType, SpatialisationFilterType, SimulationStep1Type, SimulationStep2Type, FTMType, OutputImageType>
      ImageSimulationMethodType;


  /** Instantiation of pointer objects*/
  VectorDataFileReaderType::Pointer  reader               = VectorDataFileReaderType::New();
  ImageWriterType::Pointer           writer               = ImageWriterType::New();
  LabelImageWriterType::Pointer      labelWriter          = LabelImageWriterType::New();
  ImageSimulationMethodType::Pointer imageSimulation      = ImageSimulationMethodType::New();
  SpatialisationFilterType::Pointer  spatialisationFilter = SpatialisationFilterType::New();


  reader->SetFileName(infilename);
  reader->Update();


  spatialisationFilter->SetInput(reader->GetOutput());
  spatialisationFilter->SetInitialLabel(1);
  spatialisationFilter->SetBackgroundValue(0);

  imageSimulation->SetSpatialisation(spatialisationFilter);
  imageSimulation->SetNumberOfComponentsPerPixel(nbBands);
  imageSimulation->SetSatRSRFilename(satRSRFilename);
  imageSimulation->SetPathRoot(rootPath);
  imageSimulation->SetRadius(radius);
  //    imageSimulation->SetMean();
  //    imageSimulation->SetVariance();
  imageSimulation->UpdateData();


  // Write the result to an image file
  writer->SetFileName(outfilename);
  writer->SetInput(imageSimulation->GetOutputReflectanceImage());
  writer->Update();

  labelWriter->SetFileName(outLabelfilename);
  labelWriter->SetInput(imageSimulation->GetOutputLabelImage());
  labelWriter->Update();


  return EXIT_SUCCESS;
}
