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


#include <fstream>
#include <iomanip>

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbGenericRSTransform.h"
#include <ogr_spatialref.h>
#include "itkEuclideanDistance.h"
#include "otbGeographicalDistance.h"
#include "otbDEMHandler.h"

typedef otb::VectorImage<double, 2>       ImageType;
typedef otb::ImageFileReader<ImageType>   ReaderType;
typedef otb::GenericRSTransform<>         TransformType;
typedef TransformType::InputPointType     PointType;
typedef itk::Statistics::EuclideanDistance<ImageType::PointType> DistanceType;
typedef otb::GeographicalDistance<ImageType::PointType>          GeoDistanceType;

int otbGenericRSTransformFromImage(int argc, char* argv[])
{
  /*
   * This test checks if we can instanciate a GenericRSTransform from the image information
   * without throwing an exception
   */

  // Reader
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  reader->UpdateOutputInformation();
  
  // Build wgs ref
  OGRSpatialReference oSRS;
  oSRS.SetWellKnownGeogCS("WGS84");
  char * wgsRef = NULL;
  oSRS.exportToWkt(&wgsRef);

  // Instanciate WGS->Image transform
  TransformType::Pointer wgs2img = TransformType::New();
  wgs2img->SetInputProjectionRef(wgsRef);
  wgs2img->SetOutputProjectionRef(reader->GetOutput()->GetProjectionRef());
  wgs2img->SetOutputKeywordList(reader->GetOutput()->GetImageKeywordlist());
  wgs2img->InstanciateTransform();

  // Instanciate Image->WGS transform
  TransformType::Pointer img2wgs = TransformType::New();
  img2wgs->SetInputProjectionRef(reader->GetOutput()->GetProjectionRef());
  img2wgs->SetInputKeywordList(reader->GetOutput()->GetImageKeywordlist());
  img2wgs->SetOutputProjectionRef(wgsRef);
  img2wgs->InstanciateTransform();

  return EXIT_SUCCESS;
}

int otbGenericRSTransformImageAndMNTToWGS84ConversionChecking(int argc, char* argv[])
{
  std::string infname = argv[1];
  std::string demdir = argv[2];
  
  ImageType::PointType refImgPt, refGeoPt, estimatedImgPt, estimatedGeoPt;
  refImgPt[0] = atof(argv[4]);
  refImgPt[1] = atof(argv[5]);
  refGeoPt[0] = atof(argv[6]);
  refGeoPt[1] = atof(argv[7]);
  double refHeight = atof(argv[8]);
  double imgTol = atof(argv[9]);
  double geoTol = atof(argv[10]);

  bool pass = true;

// Reader
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  reader->UpdateOutputInformation();
  
  // Build wgs ref
  OGRSpatialReference oSRS;
  oSRS.SetWellKnownGeogCS("WGS84");
  char * wgsRef = NULL;
  oSRS.exportToWkt(&wgsRef);

  DistanceType::Pointer distance = DistanceType::New();
  GeoDistanceType::Pointer geoDistance = GeoDistanceType::New();

  otb::DEMHandler::Pointer demHandler = otb::DEMHandler::Instance();
  demHandler->OpenDEMDirectory(argv[2]);
  demHandler->OpenGeoidFile(argv[3]);
  double heightAboveMSL = demHandler->GetHeightAboveMSL(refGeoPt);
  double heightAboveEllipsoid = demHandler->GetHeightAboveEllipsoid(refGeoPt);

  // Instanciate WGS->Image transform
  TransformType::Pointer wgs2img = TransformType::New();
  wgs2img->SetInputProjectionRef(wgsRef);
  wgs2img->SetOutputProjectionRef(reader->GetOutput()->GetProjectionRef());
  wgs2img->SetOutputKeywordList(reader->GetOutput()->GetImageKeywordlist());
  wgs2img->SetDEMDirectory(argv[2]);
  wgs2img->InstanciateTransform();

  // Instanciate Image->WGS transform
  TransformType::Pointer img2wgs = TransformType::New();
  img2wgs->SetInputProjectionRef(reader->GetOutput()->GetProjectionRef());
  img2wgs->SetInputKeywordList(reader->GetOutput()->GetImageKeywordlist());
  img2wgs->SetOutputProjectionRef(wgsRef);
  img2wgs->SetDEMDirectory(argv[2]);
  img2wgs->InstanciateTransform();

  std::cout<< std::setprecision(8) << std::endl;

  std::cout<<"References: "<<std::endl;
  std::cout<<"Geo (wgs84): "<<refGeoPt<<std::endl;
  std::cout<<"Image: "<<refImgPt<<std::endl;
  std::cout<<"Height: "<<refHeight<<std::endl;

  std::cout<<"-------------------- "<<std::endl;
  std::cout<<"Using elevation from "<<demdir<<std::endl;
  
  estimatedImgPt = wgs2img->TransformPoint(refGeoPt);
  std::cout<<"Inverse(refGeo): "<<refGeoPt<<" -> "<< estimatedImgPt <<std::endl;

  double imgRes = distance->Evaluate(refImgPt, estimatedImgPt);
  std::cout<<"Residual: "<<imgRes<<" pixels"<<std::endl;

  if(imgRes > imgTol)
    {
    pass = false;
    std::cerr<<"Image residual is too high: "<<imgRes<<" pixels"<<std::endl;
    }


  estimatedGeoPt = img2wgs->TransformPoint(refImgPt);
  std::cout<<"Forward(refImg): "<<refImgPt<<" -> " << estimatedGeoPt<<std::endl;

  double geoRes = geoDistance->Evaluate(refGeoPt, estimatedGeoPt);
  std::cout<<"Residual: "<<geoRes<<" meters"<<std::endl;

  if(geoRes > geoTol)
    {
    pass = false;
    std::cerr<<"Geographic (WGS84) residual is too high: "<<geoRes<<" meters"<<std::endl;
    }

  std::cout<<"-------------------- "<<std::endl;
  std::cout<<"Using reference elevation: "<<std::endl;
  wgs2img = TransformType::New();
  wgs2img->SetInputProjectionRef(wgsRef);
  wgs2img->SetOutputProjectionRef(reader->GetOutput()->GetProjectionRef());
  wgs2img->SetOutputKeywordList(reader->GetOutput()->GetImageKeywordlist());
  wgs2img->SetAverageElevation(refHeight);
  wgs2img->InstanciateTransform();

  img2wgs = TransformType::New();
  img2wgs->SetInputProjectionRef(reader->GetOutput()->GetProjectionRef());
  img2wgs->SetInputKeywordList(reader->GetOutput()->GetImageKeywordlist());
  img2wgs->SetOutputProjectionRef(wgsRef);
  img2wgs->SetAverageElevation(refHeight);
  img2wgs->InstanciateTransform();

  estimatedGeoPt = img2wgs->TransformPoint(refImgPt);
  std::cout<<"Forward(refImg): "<<refImgPt<<" -> " << estimatedGeoPt<<std::endl;
  geoRes = geoDistance->Evaluate(refGeoPt, estimatedGeoPt);
  std::cout<<"Residual: "<<geoRes<<" meters"<<std::endl;

  if(geoRes > geoTol)
    {
    pass = false;
    std::cerr<<"Geographic (WGS84) residual is too high: "<<geoRes<<" meters"<<std::endl;
    }
  
  estimatedImgPt = wgs2img->TransformPoint(refGeoPt);
  std::cout<<"Inverse(refGeo): "<<refGeoPt<<" -> "<< estimatedImgPt <<std::endl;
  imgRes = distance->Evaluate(refImgPt, estimatedImgPt);
  std::cout<<"Residual: "<<imgRes<<" pixels"<<std::endl;

  if(imgRes > imgTol)
    {
    pass = false;
    std::cerr<<"Image residual is too high: "<<imgRes<<" pixels"<<std::endl;
    }

  std::cout<<"-------------------- "<<std::endl;
  std::cout<<"Using fixed height above ellipsoid: "<<heightAboveEllipsoid<<std::endl;
  wgs2img = TransformType::New();
  wgs2img->SetInputProjectionRef(wgsRef);
  wgs2img->SetOutputProjectionRef(reader->GetOutput()->GetProjectionRef());
  wgs2img->SetOutputKeywordList(reader->GetOutput()->GetImageKeywordlist());
  wgs2img->SetAverageElevation(heightAboveEllipsoid);
  wgs2img->InstanciateTransform();

  img2wgs = TransformType::New();
  img2wgs->SetInputProjectionRef(reader->GetOutput()->GetProjectionRef());
  img2wgs->SetInputKeywordList(reader->GetOutput()->GetImageKeywordlist());
  img2wgs->SetOutputProjectionRef(wgsRef);
  img2wgs->SetAverageElevation(heightAboveEllipsoid);
  img2wgs->InstanciateTransform();

  estimatedGeoPt = img2wgs->TransformPoint(refImgPt);
  std::cout<<"Forward(refImg): "<<refImgPt<<" -> " << estimatedGeoPt<<std::endl;
  geoRes = geoDistance->Evaluate(refGeoPt, estimatedGeoPt);
  std::cout<<"Residual: "<<geoRes<<" meters"<<std::endl;

  if(geoRes > geoTol)
    {
    pass = false;
    std::cerr<<"Geographic (WGS84) residual is too high: "<<geoRes<<" meters"<<std::endl;
    }
  
  estimatedImgPt = wgs2img->TransformPoint(refGeoPt);
  std::cout<<"Inverse(refGeo): "<<refGeoPt<<" -> "<< estimatedImgPt <<std::endl;
  imgRes = distance->Evaluate(refImgPt, estimatedImgPt);
  std::cout<<"Residual: "<<imgRes<<" pixels"<<std::endl;

  if(imgRes > imgTol)
    {
    pass = false;
    std::cerr<<"Image residual is too high: "<<imgRes<<" pixels"<<std::endl;
    }

  std::cout<<"-------------------- "<<std::endl;
  std::cout<<"Using fixed height above MSL: "<<heightAboveMSL<<std::endl;
  wgs2img = TransformType::New();
  wgs2img->SetInputProjectionRef(wgsRef);
  wgs2img->SetOutputProjectionRef(reader->GetOutput()->GetProjectionRef());
  wgs2img->SetOutputKeywordList(reader->GetOutput()->GetImageKeywordlist());
  wgs2img->SetAverageElevation(heightAboveMSL);
  wgs2img->InstanciateTransform();

  img2wgs = TransformType::New();
  img2wgs->SetInputProjectionRef(reader->GetOutput()->GetProjectionRef());
  img2wgs->SetInputKeywordList(reader->GetOutput()->GetImageKeywordlist());
  img2wgs->SetOutputProjectionRef(wgsRef);
  img2wgs->SetAverageElevation(heightAboveMSL);
  img2wgs->InstanciateTransform();

  estimatedGeoPt = img2wgs->TransformPoint(refImgPt);
  std::cout<<"Forward(refImg): "<<refImgPt<<" -> " << estimatedGeoPt<<std::endl;
  geoRes = geoDistance->Evaluate(refGeoPt, estimatedGeoPt);
  std::cout<<"Residual: "<<geoRes<<" meters"<<std::endl;

  if(geoRes > geoTol)
    {
    pass = false;
    std::cerr<<"Geographic (WGS84) residual is too high: "<<geoRes<<" meters"<<std::endl;
    }
  
  estimatedImgPt = wgs2img->TransformPoint(refGeoPt);
  std::cout<<"Inverse(refGeo): "<<refGeoPt<<" -> "<< estimatedImgPt <<std::endl;
  imgRes = distance->Evaluate(refImgPt, estimatedImgPt);
  std::cout<<"Residual: "<<imgRes<<" pixels"<<std::endl;

  if(imgRes > imgTol)
    {
    pass = false;
    std::cerr<<"Image residual is too high: "<<imgRes<<" pixels"<<std::endl;
    }


  if(pass)
    {
    return EXIT_SUCCESS;
    }
  else
    {
    std::cerr<<"There were imprecise results."<<std::endl;
    return EXIT_FAILURE;
    }
}
