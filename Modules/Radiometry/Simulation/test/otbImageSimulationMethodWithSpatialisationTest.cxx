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


#include "otbVectorImage.h"
#include "otbImageFileWriter.h"
#include "otbVectorDataFileReader.h"

#include "otbSpatialisationFilter.h"
#include "otbImageSimulationMethod.h"
#include "otbAttributesMapLabelObject.h"
#include "otbImageFileReader.h"

int otbImageSimulationMethodWithSpatialisationTest(int itkNotUsed(argc), char* argv[])
{
  const char*  satRSRFilename   = argv[1];
  unsigned int nbBands          = static_cast<unsigned int>(atoi(argv[2]));
  const char*  rootPath         = argv[3];
  unsigned int radius           = atoi(argv[4]);
  const char*  outfilename      = argv[5];
  const char*  outLabelfilename = argv[6];


  typedef unsigned short LabelType;
  const unsigned int     Dimension = 2;
  typedef otb::Image<LabelType, Dimension>    LabelImageType;
  typedef otb::VectorImage<double, Dimension> OutputImageType;
  typedef otb::ImageFileWriter<OutputImageType> ImageWriterType;
  typedef otb::ImageFileWriter<LabelImageType>  LabelImageWriterType;
  typedef otb::VectorData<double, Dimension> VectorDataType;
  typedef otb::AttributesMapLabelObject<LabelType, Dimension, std::string> LabelObjectType;
  typedef itk::LabelMap<LabelObjectType> LabelMapType;


  typedef otb::SpatialisationFilter<LabelMapType>              SpatialisationFilterType;
  typedef otb::ProspectModel                                   SimulationStep1Type;
  typedef otb::SailModel                                       SimulationStep2Type;
  typedef otb::ProlateInterpolateImageFunction<LabelImageType> FTMType;
  typedef otb::ImageSimulationMethod<VectorDataType, SpatialisationFilterType, SimulationStep1Type, SimulationStep2Type, FTMType, OutputImageType>
      ImageSimulationMethodType;


  /** Instantiation of pointer objects*/
  ImageWriterType::Pointer           writer               = ImageWriterType::New();
  LabelImageWriterType::Pointer      labelWriter          = LabelImageWriterType::New();
  ImageSimulationMethodType::Pointer imageSimulation      = ImageSimulationMethodType::New();
  SpatialisationFilterType::Pointer  spatialisationFilter = SpatialisationFilterType::New();

  SpatialisationFilterType::SizeType objectSize;
  objectSize[0] = 300;
  objectSize[1] = 300;

  SpatialisationFilterType::SizeType nbOjects;
  nbOjects[0] = 3;
  nbOjects[1] = 3;

  std::vector<std::string> pathVector(9);
  pathVector[0] = "JHU/becknic/rocks/sedimentary/powder/0_75/txt/greywa1f.txt";
  pathVector[1] = "";
  pathVector[2] = "JHU/becknic/manmade/txt/0092uuu.txt";
  pathVector[3] = "JHU/becknic/vegetation/txt/conifers.txt";
  pathVector[4] = "JHU/becknic/manmade/txt/0834uuu.txt";
  pathVector[5] = "JHU/becknic/vegetation/txt/grass.txt";
  pathVector[6] = "JHU/becknic/water/txt/coarse.txt";
  pathVector[7] = "JHU/becknic/rocks/igneous/solid/txt/andesi1s.txt";
  pathVector[8] = "JHU/becknic/soils/txt/0015c.txt";

  std::vector<std::string> areaVector(9);
  areaVector[0] = "sedimentaryRock";
  areaVector[1] = "prosail";
  areaVector[2] = "manmade";
  areaVector[3] = "conifers";
  areaVector[4] = "manmade";
  areaVector[5] = "grass";
  areaVector[6] = "water";
  areaVector[7] = "igneousRocks";
  areaVector[8] = "soils";

  std::vector<LabelType> labels(9);
  labels[0] = 1;
  labels[1] = 2;
  labels[2] = 1;
  labels[3] = 2;
  labels[4] = 3;
  labels[5] = 2;
  labels[6] = 4;
  labels[7] = 5;
  labels[8] = 3;

  spatialisationFilter->SetObjectSize(objectSize);
  spatialisationFilter->SetNumberOfObjects(nbOjects);
  spatialisationFilter->SetPathVector(pathVector);
  spatialisationFilter->SetAreaVector(areaVector);
  spatialisationFilter->SetLabels(labels);

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
