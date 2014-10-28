/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "otbCommandLineArgumentParser.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbGCPsToRPCSensorModelImageFilter.h"
#include "otbGenericRSTransform.h"
#include "otbGeographicalDistance.h"

int otbGCPsToRPCSensorModelImageFilterCheckRpcModel(int argc, char * argv[])
{
  // Set command line arguments
  typedef otb::CommandLineArgumentParser ParserType;
  ParserType::Pointer parser = ParserType::New();

  parser->AddInputImage();
  parser->AddOption("--DEMDirectory", "Path to the DEM directory ", "-dem", 1, false);
  parser->AddOptionNParams("--GroudControlPoints",
                           "Ground Control Points to estimate sensor model a1x a1y b1x b1y b1z ... aNx aNy aNz bNx bNy bNz",
                           "-gcp", true);
  parser->AddOption("--ErrorAllowed", "Error allowed to declare a point not good ", "-err", 1, false);

  // Parse the command line
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
    std::cerr << "Inconsistent GCPs description!" << std::endl;
    return EXIT_FAILURE;
    }

  typedef otb::VectorImage<float, 2>                      ImageType;
  typedef otb::ImageFileReader<ImageType>                 ReaderType;
  typedef otb::GCPsToRPCSensorModelImageFilter<ImageType> GCPsToSensorModelFilterType;
  typedef GCPsToSensorModelFilterType::Point2DType        Point2DType;
  typedef GCPsToSensorModelFilterType::Point3DType        Point3DType;
  typedef otb::GenericRSTransform<double,3, 3>           GenericRSTransformType;
  typedef otb::GeographicalDistance<ImageType::PointType>          GeoDistanceType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(parseResult->GetInputImage());
  reader->UpdateOutputInformation();

  GCPsToSensorModelFilterType::Pointer rpcEstimator = GCPsToSensorModelFilterType::New();
  rpcEstimator->SetInput(reader->GetOutput());

  unsigned int nbGCPs = nbPoints / 5;
  std::cout << "Receiving " << nbPoints << " from command line." << std::endl;

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

  // Estimate the rpc model
  rpcEstimator->GetOutput()->UpdateOutputInformation();

  // Instancicate a GenericRSTransform in order to transform the
  // indexes, using the rpcModel estimated, into geographical
  // coordiantes.
  // The test will check for nan coordinates, and the distance between
  // geographical coordinates.

  GenericRSTransformType::Pointer grsTrasnform = GenericRSTransformType::New();
  grsTrasnform->SetInputKeywordList(rpcEstimator->GetKeywordlist());
  std::cout<<rpcEstimator->GetKeywordlist()<<std::endl;
  grsTrasnform->SetOutputProjectionRef("4326");

  // Set the DEM Directory if any
  if(parseResult->IsOptionPresent("--DEMDirectory"))
    {
    otb::DEMHandler::Instance()->OpenDEMDirectory(parseResult->GetParameterString("--DEMDirectory"));
    }
  else
    {
    otb::DEMHandler::Instance()->SetDefaultHeightAboveEllipsoid(0);
    }

  grsTrasnform->InstanciateTransform();

  // Test
  GeoDistanceType::Pointer geoDistance = GeoDistanceType::New();
  bool isErrorDetected = false;

  for (unsigned int gcpId = 0; gcpId < nbGCPs; ++gcpId)
    {
    Point3DType point;
    point[0] = parseResult->GetParameterFloat("--GroudControlPoints",     gcpId * 5);
    point[1] = parseResult->GetParameterFloat("--GroudControlPoints", 1 + gcpId * 5);
    point[2] = parseResult->GetParameterFloat("--GroudControlPoints", 4 + gcpId * 5);

    Point3DType transformedPoint;
    transformedPoint = grsTrasnform->TransformPoint(point);

    Point2DType transformedPoint2D;
    transformedPoint2D[0] = transformedPoint[0];
    transformedPoint2D[1] = transformedPoint[1];

    // reference point
    Point2DType geoPoint;
    geoPoint[0] = parseResult->GetParameterFloat("--GroudControlPoints", 2 + gcpId * 5);
    geoPoint[1] = parseResult->GetParameterFloat("--GroudControlPoints", 3 + gcpId * 5);

    // Search for nans
    if ( vnl_math_isnan(transformedPoint2D[0]) || vnl_math_isnan(transformedPoint2D[1]) )
      {
      std::cout << "Reference : "<< geoPoint
                <<" --> Result of the reprojection using the estimated RpcModel "<<  transformedPoint2D
                << std::endl;
      std::cout<<"The result of the projection is nan, there is a problem with the estimated RpcModel "
               << std::endl<<std::endl;
      isErrorDetected = true;
      }

    // Search for wrong projection results
    double residual = geoDistance->Evaluate(geoPoint, transformedPoint2D);
    if( residual > parseResult->GetParameterFloat("--ErrorAllowed"))
      {
      std::cout << "Reference : "<< geoPoint
                <<" --> Result of the reprojection using the estimated RpcModel "
                << transformedPoint2D
                << std::endl
                << " Residual ["<< residual << "] is higher than the tolerance ["
                << parseResult->GetParameterFloat("--ErrorAllowed")
                <<"], there is a problem with the estimated RpcModel"
                <<std::endl<<std::endl;
      isErrorDetected = true;
      }
    }

  // Is there an error
  if ( isErrorDetected )
    return EXIT_FAILURE;

  return EXIT_SUCCESS;
}
