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

#include "otbGenericRSTransform.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "itkEuclideanDistanceMetric.h"
#include "otbGeoInformationConversion.h"
#include "otbGeographicalDistance.h"

typedef otb::Image<unsigned short>      ImageType;
typedef otb::ImageFileReader<ImageType> ReaderType;
typedef ImageType::PointType            PointType;
typedef otb::GenericRSTransform<>       TransformType;
typedef itk::Statistics::EuclideanDistanceMetric<PointType> PointEuclideanDistanceType;
typedef otb::GeographicalDistance<PointType> GeographicalDistanceType;

int otbGenericRSTransformGenericTest(int argc, char * argv[])
{
  if(argc<14)
    {
    std::cerr<<"Usage: "<<argv[0]<<"input_point_x input_point_y output_point_x output_point_y input_projection_type[IMAGE, WKT, EPSG] input_projection_source output_projection_type[IMAGE, WKT, EPSG] output_projection_source input_distance[PHYSICAL, GEO] input_threshold output_distance[PHYSICAL, GEO] ouptut_threshold elevation_flag[NOELEV, AVERAGE, DEM] elevation_source";
    return EXIT_FAILURE;
    }

  PointType refInPoint, refOutPoint, inPoint, outPoint;
  refInPoint[0] = atof(argv[1]);
  refInPoint[1] = atof(argv[2]);
  refOutPoint[0] = atof(argv[3]);
  refOutPoint[1] = atof(argv[4]);

  TransformType::Pointer transform = TransformType::New();

  std::cout<<std::endl;

  std::cout<<"Input reference point: "<<refInPoint<<std::endl;

  std::cout<<std::endl;

  std::cout<<"Reading input projection ..."<<std::endl;

  std::string inputType = argv[5];
  std::string outputType = argv[7];
  std::string inDistType = argv[9];
  std::string outDistType = argv[11];
  std::string elevType = argv[13];

  ReaderType::Pointer reader = ReaderType::New();

  if(inputType == "IMAGE")
    {
    reader->SetFileName(argv[6]);
    reader->UpdateOutputInformation();

    transform->SetInputProjectionRef(reader->GetOutput()->GetProjectionRef());
    transform->SetInputKeywordList(reader->GetOutput()->GetImageKeywordlist());

    std::cout<<"Input projection read from image: "<<argv[6]<<std::endl;
    }
  else if(inputType == "WKT")
    {
    transform->SetInputProjectionRef(argv[6]);
    std::cout<<"Input projection read from wkt: "<<argv[6]<<std::endl;
    }
  else if(inputType == "EPSG")
    {
    std::string wktFromEpsg = otb::GeoInformationConversion::ToWKT(atoi(argv[6]));
    transform->SetInputProjectionRef(wktFromEpsg);

    std::cout<<"Input projection read from epsg code: "<<atoi(argv[6])<<" and converted to wkt: "<<wktFromEpsg<<std::endl;
    }
  else
    {
    std::cerr<<"Failed to read input projection flag "<<inputType<<". Should be one of IMAGE, WKT, EPSG"<<std::endl;
    return EXIT_FAILURE;
    }

  std::cout<<std::endl;

  std::cout<<"Output reference point: "<<refOutPoint<<std::endl;

  std::cout<<std::endl;

  std::cout<<"Reading output projection ..."<<std::endl;

  if(outputType == "IMAGE")
    {
    reader->SetFileName(argv[8]);
    reader->UpdateOutputInformation();

    transform->SetOutputProjectionRef(reader->GetOutput()->GetProjectionRef());
    transform->SetOutputKeywordList(reader->GetOutput()->GetImageKeywordlist());

    std::cout<<"Output projection read from image: "<<argv[8]<<std::endl;
    }
  else if(outputType == "WKT")
    {
    transform->SetOutputProjectionRef(argv[6]);
    std::cout<<"Output projection read from wkt: "<<argv[8]<<std::endl;
    }
  else if(outputType == "EPSG")
    {
    std::string wktFromEpsg = otb::GeoInformationConversion::ToWKT(atoi(argv[8]));
    transform->SetOutputProjectionRef(wktFromEpsg);

    std::cout<<"Output projection read from epsg code: "<<atoi(argv[8])<<" and converted to wkt: "<<wktFromEpsg<<std::endl;
    }
  else
    {
    std::cerr<<"Failed to read output projection flag "<<outputType<<". Should be one of IMAGE, WKT, EPSG"<<std::endl;
    return EXIT_FAILURE;
    }

  std::cout<<std::endl;

  std::cout<<"Reading elevation ..."<<std::endl;

  if(elevType == "NOELEV")
    {
    // Do noting
    std::cout<<"No elevation used."<<std::endl;
    }
  else if(elevType == "AVERAGE")
    {
    if(argc != 15)
      {
      std::cerr<<"AVERAGE elevation not provided."<<std::endl;
      return EXIT_FAILURE;
      }

    double averageElevation = atof(argv[14]);
    otb::DEMHandler::Instance()->SetDefaultHeightAboveEllipsoid(averageElevation);

    std::cout<<"Average elevation "<<averageElevation<<" used."<<std::endl;
    }
  else if(elevType == "DEM")
    {
    if(argc != 15)
      {
      std::cerr<<"DEM directory not provided."<<std::endl;
      return EXIT_FAILURE;
      }

    otb::DEMHandler::Instance()->OpenDEMDirectory(argv[14]);
    
    std::cout<<"Elevation from DEM "<<argv[14]<<" used."<<std::endl;
    }
  else
    {
    std::cerr<<"Failed to read the elevation flag. Should be one of NOELEV, AVERAGE, DEM"<<std::endl;
    return EXIT_FAILURE;
    }

  // Instanciate the transform
  transform->InstanciateTransform();

  // Generate the inverse transform
  TransformType::Pointer inverseTransform = TransformType::New();
  transform->GetInverse(inverseTransform);

  std::cout<<std::endl;

  // Compute transformed points
  outPoint = transform->TransformPoint(refInPoint);
  std::cout<<"transform("<<refInPoint<<") = "<<outPoint<<" (reference: "<<refOutPoint<<")"<<std::endl;

  std::cout<<std::endl;

  inPoint  = inverseTransform->TransformPoint(refOutPoint);
  std::cout<<"transform("<<refOutPoint<<") = "<<inPoint<<" (reference: "<<refInPoint<<")"<<std::endl;

  std::cout<<std::endl;

  // Build metrics
  PointEuclideanDistanceType::Pointer physDistance = PointEuclideanDistanceType::New();
  GeographicalDistanceType::Pointer geoDistance = GeographicalDistanceType::New();

  // Check precision on output
  double outThreshold = atof(argv[12]);
  double outRes;

  bool failed = false;

  if(outDistType == "PHYSICAL")
    {
    std::cout<<"Output precision checked using physical distance."<<std::endl;
    outRes = physDistance->Evaluate(outPoint, refOutPoint);
    }
  else if(outDistType == "GEO")
    {
    std::cout<<"Output precision checked using Haversine distance."<<std::endl;
    outRes = geoDistance->Evaluate(outPoint, refOutPoint);
    }
  else
    {
    std::cerr<<"Failed to read out distance flag. Should be one of PHYSICAL, GEO."<<std::endl;
    return EXIT_FAILURE;
    }

  std::cout<<std::endl;
  
  if(outRes>outThreshold)
    {
    failed = true;
    std::cerr<<"Output residual ("<<outRes<<") outpasses the threshold ("<<outThreshold<<")"<<std::endl;
    std::cerr<<std::endl;
    }

  // Check precision on input
  double inThreshold = atof(argv[10]);
  double inRes;

  if(inDistType == "PHYSICAL")
    {
    std::cout<<"Input precision checked using physical distance."<<std::endl;
    inRes = physDistance->Evaluate(inPoint, refInPoint);
    }
  else if(inDistType == "GEO")
    {
    std::cout<<"Input precision checked using Haversine distance."<<std::endl;
    inRes = geoDistance->Evaluate(inPoint, refInPoint);
    }
  else
    {
    std::cerr<<"Failed to read in distance flag. Should be one of PHYSICAL, GEO."<<std::endl;
    return EXIT_FAILURE;
    }
  
  std::cout<<std::endl;

  if(inRes>inThreshold)
    {
    failed = true;
    std::cerr<<"Input residual ("<<outRes<<") outpasses the threshold ("<<inThreshold<<")"<<std::endl;
    std::cerr<<std::endl;
    }


  if(failed)
    {
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
