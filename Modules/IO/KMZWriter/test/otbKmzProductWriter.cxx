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
#include "otbCommandLineArgumentParser.h"


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
    sensorPoint[0] = atof(argv[4 + gcpId * 5]);
    sensorPoint[1] = atof(argv[5 + gcpId * 5]);

    Point3DType geoPoint;
    geoPoint[0] = atof(argv[6 + 5 * gcpId]);
    geoPoint[1] = atof(argv[7 + 5 * gcpId]);
    geoPoint[2] = atof(argv[8 + 5 * gcpId]);

    std::cout << "Adding GCP sensor: " << sensorPoint << " <-> geo: " << geoPoint << std::endl;

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
  // Parse command line parameters
  typedef otb::CommandLineArgumentParser ParserType;
  ParserType::Pointer parser = ParserType::New();

  parser->SetProgramDescription("Chain that Estimate a sensor model in order to export the input image to Google Earth understandable format Kmz");
  parser->AddInputImage();
  parser->AddOption("--OutputProductName", "Output Kmz product ", "-kmz", 1, true);
  parser->AddOption("--LogoImage", "Output Kmz product ", "-lo", 1, false);
  parser->AddOption("--LegendImage", "Output Kmz product ", "-le", 1, false);
  parser->AddOption("--DEMDirectory", "Path to the DEM directory ", "-dem", 1, true);
  parser->AddOptionNParams("--GroudControlPoints",
                           "Ground Control Points to estimate sensor model a1x a1y b1x b1y b1z ... aNx aNy aNz bNx bNy bNz",
                           "-gcp", true);

  typedef otb::CommandLineArgumentParseResult ParserResultType;
  ParserResultType::Pointer  parseResult = ParserResultType::New();

  try
  {
    parser->ParseCommandLine(argc, argv, parseResult);
  }
  catch ( itk::ExceptionObject & err )
  {
    std::string descriptionException = err.GetDescription();
    if (descriptionException.find("ParseCommandLine(): Help Parser") != std::string::npos)
    {
    return EXIT_SUCCESS;
    }
    if (descriptionException.find("ParseCommandLine(): Version Parser") != std::string::npos)
      {
      return EXIT_SUCCESS;
      }
    return EXIT_FAILURE;
  }

  // Check if the number of gcp pairs point is consistent
  unsigned int nbPoints = parseResult->GetNumberOfParameters("--GroudControlPoints");

  if (nbPoints % 5 != 0)
    {
    std::cout <<"argc " << argc << std::endl;
    std::cerr << "Inconsistent GCPs description!" << std::endl;
    return EXIT_FAILURE;
    }

  // Instantiate reader
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(parseResult->GetInputImage());

  GCPsToSensorModelFilterType::Pointer rpcEstimator = GCPsToSensorModelFilterType::New();
  rpcEstimator->SetInput(reader->GetOutput());

  std::cout << "Receiving " << nbPoints << " from command line." << std::endl;

  unsigned int nbGCPs =  nbPoints/5;

  for (unsigned int gcpId = 0; gcpId < nbGCPs; ++gcpId)
    {
    Point2DType sensorPoint;
    sensorPoint[0] = parseResult->GetParameterFloat("--GroudControlPoints",     gcpId * 5);
    sensorPoint[1] = parseResult->GetParameterFloat("--GroudControlPoints", 1 + gcpId * 5);

    Point3DType geoPoint;
    geoPoint[0] = parseResult->GetParameterFloat("--GroudControlPoints", 2 + gcpId * 5);
    geoPoint[1] = parseResult->GetParameterFloat("--GroudControlPoints", 3 + gcpId * 5);
    geoPoint[2] = parseResult->GetParameterFloat("--GroudControlPoints", 4 + gcpId * 5);

    std::cout << "Adding GCP sensor: " << sensorPoint << " <-> geo: " << geoPoint << std::endl;

    rpcEstimator->AddGCP(sensorPoint, geoPoint);
    }

  rpcEstimator->GetOutput()->UpdateOutputInformation();
  std::cout << "Residual ground error: " << rpcEstimator->GetRMSGroundError() << std::endl;

  // Second part : Image To Kmz
  KmzProductWriterType::Pointer    kmzWriter  = KmzProductWriterType::New();
  kmzWriter->SetInput(rpcEstimator->GetOutput());
  kmzWriter->SetPath(parseResult->GetParameterString("--OutputProductName"));

  // Read the logo if any
  if(parseResult->IsOptionPresent("--LogoImage"))
    {
    ReaderType::Pointer logoReader  = ReaderType::New();
    logoReader->SetFileName(parseResult->GetParameterString("--LogoImage"));
    logoReader->Update();
    kmzWriter->SetLogo(logoReader->GetOutput());
    }
  // Read the legend if any
  if(parseResult->IsOptionPresent("--LegendImage"))
    {
    ReaderType::Pointer legendReader  = ReaderType::New();
    legendReader->SetFileName(parseResult->GetParameterString("--LegendImage"));
    legendReader->Update();
    kmzWriter->AddLegend("Input Legend", legendReader->GetOutput());
    kmzWriter->AddLegend(legendReader->GetOutput());
    }
  // Set the DEM Directory if any
  if(parseResult->IsOptionPresent("--DEMDirectory"))
    {
    otb::DEMHandler::Instance()->OpenDEMDirectory(parseResult->GetParameterString("--DEMDirectory"));
    }

  // trigger the writing
  kmzWriter->Update();

  return EXIT_SUCCESS;
}
