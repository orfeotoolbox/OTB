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


#include <iostream>
#include <iomanip>

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbGenericMapProjection.h"
#include "otbSpatialReference.h"
#include "otbCompositeTransform.h"
#include "otbRPCInverseTransform.h"
#include "otbMetaDataKey.h"

int otbCompositeTransform(int argc, char* argv[])
{

  if (argc != 3)
  {
    std::cout << argv[0] << " <input filename> <output filename>" << std::endl;

    return EXIT_FAILURE;
  }

  char* filename    = argv[1];
  char* outFilename = argv[2];

  typedef otb::Image<double, 2> ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer                     reader = ReaderType::New();
  reader->SetFileName(filename);
  reader->UpdateOutputInformation();

  typedef otb::GenericMapProjection<otb::TransformDirection::INVERSE> MapProjectionType;

  MapProjectionType::Pointer mapProjection = MapProjectionType::New();
  // UTM31N
  mapProjection->SetWkt(otb::SpatialReference::FromEPSG(32631).ToWkt());

  typedef otb::RPCInverseTransform<double, 2, 2> SensorModelType;
  SensorModelType::Pointer                sensorModel = SensorModelType::New();
  sensorModel->SetMetadata(reader->GetOutput()->GetImageMetadata());

  if (sensorModel->IsValidSensorModel() == false)
  {
    std::cout << "Invalid Model pointer m_Model == NULL!\n The ossim keywordlist is invalid!" << std::endl;
    return EXIT_FAILURE;
  }

  typedef otb::CompositeTransform<MapProjectionType, SensorModelType> CompositeTransformType;
  CompositeTransformType::Pointer compositeTransform = CompositeTransformType::New();

  compositeTransform->SetFirstTransform(mapProjection);
  compositeTransform->SetSecondTransform(sensorModel);

  std::ofstream file;
  file.open(outFilename);

  file << std::setprecision(15);
  itk::Point<double, 2> point;
  point[0] = 374100;
  point[1] = 4829000;
  file << "Standard Composite transform: " << std::endl;
  file << point << " -> ";
  file << compositeTransform->TransformPoint(point);
  file << std::endl << std::endl;

  file.close();
  return EXIT_SUCCESS;
}
