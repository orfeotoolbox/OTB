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

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbKmzProductWriter.h"
#include "otbGCPsToRPCSensorModelImageFilter.h"


typedef otb::VectorImage<float, 2>                      ImageType;
typedef otb::KmzProductWriter<ImageType>                KmzProductWriterType;
typedef otb::ImageFileReader<ImageType>                 ReaderType;
typedef otb::GCPsToRPCSensorModelImageFilter<ImageType> GCPsToSensorModelFilterType;
typedef GCPsToSensorModelFilterType::Point2DType        Point2DType;
typedef GCPsToSensorModelFilterType::Point3DType        Point3DType;

int otbKmzProductWriter(int argc, char* argv[])
{

  if (argc < 3)
    {
    std::cerr << "Usage: " << argv[0] << " infname demPath kmzFileName "
              <<"a1x a1y b1x b1y b1z ... aNx aNy aNz bNx bNy bNz"
              << std::endl;
    return EXIT_FAILURE;
    }
  else if ((argc - 4) % 5 != 0)
    {
    std::cout <<"argc " << argc << std::endl;
    std::cerr << "Inconsistent GCPs description!" << std::endl;
    return EXIT_FAILURE;
    }

  // Get the cli arguments
  const char * infname       = argv[1];
  const char * demPath       = argv[2];
  const char * kmzFileName   = argv[3];

  // Instantiate reader
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);

  otb::DEMHandler::Instance()->OpenDEMDirectory(demPath);

  GCPsToSensorModelFilterType::Pointer rpcEstimator = GCPsToSensorModelFilterType::New();
  rpcEstimator->SetInput(reader->GetOutput());

  unsigned int nbGCPs = (argc - 4) / 5;

  std::cout << "Receiving " << nbGCPs << " from command line." << std::endl;

  for (unsigned int gcpId = 0; gcpId < nbGCPs; ++gcpId)
    {
    Point2DType sensorPoint;
    sensorPoint[0] = std::stof(argv[4 + gcpId * 5]);
    sensorPoint[1] = std::stof(argv[5 + gcpId * 5]);

    Point3DType geoPoint;
    geoPoint[0] = std::stof(argv[6 + 5 * gcpId]);
    geoPoint[1] = std::stof(argv[7 + 5 * gcpId]);
    geoPoint[2] = std::stof(argv[8 + 5 * gcpId]);
#ifdef DEBUG
    std::cout << "Adding GCP sensor: " << sensorPoint << " <-> geo: " << geoPoint << std::endl;
#endif
    rpcEstimator->AddGCP(sensorPoint, geoPoint);
    }

  rpcEstimator->GetOutput()->UpdateOutputInformation();

  std::cout << "Residual ground error: " << rpcEstimator->GetRMSGroundError() << std::endl;

  // Second part : Image To Kmz
  KmzProductWriterType::Pointer    kmzWriter  = KmzProductWriterType::New();

  kmzWriter->SetInput(rpcEstimator->GetOutput());
  kmzWriter->SetPath(kmzFileName);

  kmzWriter->Update();

  return EXIT_SUCCESS;
}


int otbKmzProductWriterWithLogoAndLegend(int argc, char* argv[])
{

  if (argc < 3)
    {
    std::cerr << "Usage: " << argv[0] << " input logo_path legend_path "
              <<"DEM_path output "
              <<"a1x a1y b1x b1y b1z ... aNx aNy aNz bNx bNy bNz"
              << std::endl;
    return EXIT_FAILURE;
    }
  else if ((argc - 6) % 5 != 0)
    {
    std::cout <<"argc " << argc << std::endl;
    std::cerr << "Inconsistent GCPs description!" << std::endl;
    return EXIT_FAILURE;
    }
    
  // Instantiate reader
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  GCPsToSensorModelFilterType::Pointer rpcEstimator = GCPsToSensorModelFilterType::New();
  rpcEstimator->SetInput(reader->GetOutput());

  unsigned int nbPoints = argc-6;
  std::cout << "Receiving " << nbPoints << " from command line." << std::endl;

  unsigned int nbGCPs =  nbPoints/5;

  for (unsigned int gcpId = 0; gcpId < nbGCPs; ++gcpId)
    {
    Point2DType sensorPoint;
    sensorPoint[0] = std::stof(argv[6 + 5 * gcpId]);
    sensorPoint[1] = std::stof(argv[7 + 5 * gcpId]);

    Point3DType geoPoint;
    geoPoint[0] = std::stof(argv[8 + 5 * gcpId]);
    geoPoint[1] = std::stof(argv[9 + 5 * gcpId]);
    geoPoint[2] = std::stof(argv[10 + 5 * gcpId]);
#ifdef DEBUG
    std::cout << "Adding GCP sensor: " << sensorPoint << " <-> geo: " << geoPoint << std::endl;
#endif
    rpcEstimator->AddGCP(sensorPoint, geoPoint);
    }

  rpcEstimator->GetOutput()->UpdateOutputInformation();
  std::cout << "Residual ground error: " << rpcEstimator->GetRMSGroundError() << std::endl;

  // Second part : Image To Kmz
  KmzProductWriterType::Pointer    kmzWriter  = KmzProductWriterType::New();
  kmzWriter->SetInput(rpcEstimator->GetOutput());
  kmzWriter->SetPath(argv[5]);

  // Read the logo
  ReaderType::Pointer logoReader  = ReaderType::New();
  logoReader->SetFileName(argv[2]);
  logoReader->Update();
  kmzWriter->SetLogo(logoReader->GetOutput());

  // Read the legend
  ReaderType::Pointer legendReader  = ReaderType::New();
  legendReader->SetFileName(argv[3]);
  legendReader->Update();
  kmzWriter->AddLegend("Input Legend", legendReader->GetOutput());
  kmzWriter->AddLegend(legendReader->GetOutput());

  // Set the DEM Directory
  otb::DEMHandler::Instance()->OpenDEMDirectory(argv[4]);

  // trigger the writing
  kmzWriter->Update();

  return EXIT_SUCCESS;
}
