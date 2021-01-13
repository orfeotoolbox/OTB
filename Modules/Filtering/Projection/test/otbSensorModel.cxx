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


#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iterator>

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageMetadata.h"
#include "otbRPCForwardTransform.h"
#include "otbRPCInverseTransform.h"
#include "otbDEMHandler.h"
#include <otbImageKeywordlist.h>
#include "itkEuclideanDistanceMetric.h"
#include "otbGeographicalDistance.h"
#include "otbGenericRSTransform.h"
#include "otbImageMetadata.h"
#include "otbImageMetadataInterfaceFactory.h"
#include "otbGeomMetadataSupplier.h"
#include "otbMacro.h"

typedef std::list<itk::Point<double, 2>> pointsContainerType;
typedef std::list<itk::Point<double, 3>> geo3dPointsContainerType;
typedef otb::VectorImage<double, 2>      ImageType;
typedef itk::Statistics::EuclideanDistanceMetric<ImageType::PointType> DistanceType;
typedef otb::GeographicalDistance<ImageType::PointType>                GeographicalDistanceType;

int produceGCP(char* outputgcpfilename, const otb::ImageMetadata& imd, bool useForwardSensorModel = true, double z = 16.19688987731934)
{
  itk::Point<double, 2> imagePoint;
  itk::Point<double, 2> geoPoint;

  //  otbForwardSensorModel
  typedef otb::RPCForwardTransform<double, 2, 2> ForwardSensorModelType;
  ForwardSensorModelType::Pointer         forwardSensorModel = ForwardSensorModelType::New();
  forwardSensorModel->SetMetadata(imd);
  if (forwardSensorModel->IsValidSensorModel() == false)
  {
    otbLogMacro(Warning, << "Invalid Model pointer m_Model == NULL!\n The metadata is invalid!");
    return EXIT_FAILURE;
  }

  otb::DEMHandler::GetInstance().SetDefaultHeightAboveEllipsoid(z);

  std::ofstream file(outputgcpfilename, std::ios::out | std::ios::trunc);
  if (file)
  {
    for (double x = 10.0; x < 300.0; x += 50.)
      for (double y = 10.0; y < 300.0; y += 50.)
      {
        imagePoint[0] = x;
        imagePoint[1] = y;
	geoPoint = forwardSensorModel->TransformPoint(imagePoint);
	file << std::setprecision(16) << x << " " << y << " " << geoPoint[0] << " " << geoPoint[1] << " " << z << std::endl;
      }
  }
  else
  {
    std::cerr << "Couldn't open " << outputgcpfilename << std::endl;
    return EXIT_FAILURE;
  }

  file.close();

  return EXIT_SUCCESS;
}

bool provideGCP(char* gcpfilename, pointsContainerType& imgPt, geo3dPointsContainerType& geo3dPt, pointsContainerType& OSSIMPt)
{
  itk::Point<double, 2> imagePoint;
  itk::Point<double, 3> geo3dPoint;
  itk::Point<double, 2> OSSIMPoint;

  std::ifstream file(gcpfilename, std::ios::in);
  if (file)
  {
    std::string line;
    while (getline(file, line))
    {
      if (line.find_first_of("#") != 0)
      {
        std::istringstream iss(line);

        iss >> imagePoint[0] >> imagePoint[1] >> geo3dPoint[0] >> geo3dPoint[1] >> geo3dPoint[2] >> OSSIMPoint[0] >> OSSIMPoint[1];

        imgPt.push_back(imagePoint);
        geo3dPt.push_back(geo3dPoint);
	OSSIMPt.push_back(OSSIMPoint);
      }
    }
    file.close();
  }
  else
    return false;

  return true;
}

int otbSensorModel(int argc, char* argv[])
{
  if (argc != 9)
  {
    std::cout << argv[0] << " <input geom filename> <input gcp filename> <output gcp filename> "
              << " <needed keywords> <imgTol> <geoTol> <writeBaseline> <only check needed keywords>" << std::endl;

    return EXIT_FAILURE;
  }

  char*              geomfilename = argv[1];
  char*              gcpfilename  = argv[2];
  char*              outFilename  = argv[3];
  std::istringstream iss(argv[4]);
  double             imgTol        = atof(argv[5]);
  double             geoTol        = atof(argv[6]);
  int                writeBaseline = atoi(argv[7]);
  bool               checkNeededKw = atoi(argv[8]);

  // -------------------
  // Some instantiations
  // -------------------
  otb::ImageKeywordlist kwlist = otb::ReadGeometryFromGEOMFile(geomfilename);
  otb::ImageMetadata imd;
  otb::GeomMetadataSupplier geomSupplier(geomfilename);
  for (int loop = 0 ; loop < geomSupplier.GetNbBands() ; ++loop)
    imd.Bands.emplace_back();
  otb::ImageMetadataInterfaceBase::Pointer imi = otb::ImageMetadataInterfaceFactory::CreateIMI(imd, geomSupplier);
  imd = imi->GetImageMetadata();
  geomSupplier.FetchRPC(imd);

  if (!(kwlist.GetSize() > 0))
  {
    std::cerr << "ImageKeywordlist is empty." << std::endl;
    return EXIT_FAILURE;
  }

  if (writeBaseline)
  {
    return produceGCP(outFilename, imd);
  }

  typedef otb::ImageKeywordlist::KeywordlistMap KeywordlistMapType;
  KeywordlistMapType                            kwmap = kwlist.GetKeywordlist();

  //  otbForwardSensorModel
  typedef otb::RPCForwardTransform<double, 2, 2> ForwardSensorModelType;
  ForwardSensorModelType::Pointer         forwardSensorModel = ForwardSensorModelType::New();
  if (!forwardSensorModel)
  {
    std::cerr << "Invalid sensor model (ForwardSensorModelType::Pointer is NULL)" << std::endl;
    return EXIT_FAILURE;
  }
  forwardSensorModel->SetMetadata(imd);
  if (forwardSensorModel->IsValidSensorModel() == false)
  {
    std::cerr << "Invalid Model pointer m_Model == NULL!\n The metadata is invalid!" << std::endl;
    return EXIT_FAILURE;
  }

  // otbInverseSensorModel
  typedef otb::RPCInverseTransform<double, 2, 2> InverseSensorModelType;
  InverseSensorModelType::Pointer         inverseSensorModel = InverseSensorModelType::New();
  if (!inverseSensorModel)
  {
    std::cerr << "Invalid sensor model (InverseSensorModelType::Pointer is NULL)" << std::endl;
    return EXIT_FAILURE;
  }
  inverseSensorModel->SetMetadata(imd);
  if (inverseSensorModel->IsValidSensorModel() == false)
  {
    std::cerr << "Invalid Model pointer m_Model == NULL!\n The metadata is invalid!" << std::endl;
    return EXIT_FAILURE;
  }

  // otbGenericRSTransform
  // Build wgs ref
  OGRSpatialReference oSRS;
  oSRS.SetWellKnownGeogCS("WGS84");
  char* wgsRef = nullptr;
  oSRS.exportToWkt(&wgsRef);

  typedef otb::GenericRSTransform<> GRSTransformType;

  // Instantiate Image->WGS transform
  GRSTransformType::Pointer img2wgs = GRSTransformType::New();
  if (!img2wgs)
  {
    std::cerr << "Invalid sensor model (GRSTransformType::Pointer is NULL)" << std::endl;
    return EXIT_FAILURE;
  }
  img2wgs->SetInputProjectionRef("");
  img2wgs->SetOutputProjectionRef(wgsRef);
  img2wgs->SetInputImageMetadata(&imd);
  img2wgs->InstantiateTransform();


  // Instantiate WGS->Image transform
  GRSTransformType::Pointer wgs2img = GRSTransformType::New();
  if (!wgs2img)
  {
    std::cerr << "Invalid sensor model (GRSTransformType::Pointer is NULL)" << std::endl;
    return EXIT_FAILURE;
  }
  wgs2img->SetInputProjectionRef(wgsRef);
  wgs2img->SetOutputProjectionRef("");
  wgs2img->SetOutputImageMetadata(&imd);
  wgs2img->InstantiateTransform();

  itk::Point<double, 2> imagePoint;
  itk::Point<double, 2> geoPoint, geoPointGCP;
  itk::Point<double, 3> geo3dPoint;
  itk::Point<double, 2> geoPointGRS;
  itk::Point<double, 2> geoPointOSSIM;
  itk::Point<double, 2> reversedImagePoint;
  itk::Point<double, 2> reversedImagePointGRS;


  pointsContainerType      pointsContainer;
  geo3dPointsContainerType geo3dPointsContainer;
  pointsContainerType      ossimContainer;
  bool                     okStatus = provideGCP(gcpfilename, pointsContainer, geo3dPointsContainer, ossimContainer);
  if (!okStatus)
  {
    std::cerr << "File" << gcpfilename << " couldn't be opened" << std::endl;
    return EXIT_FAILURE;
  }

  DistanceType::Pointer             distance    = DistanceType::New();
  GeographicalDistanceType::Pointer geoDistance = GeographicalDistanceType::New();

  //--------------------------
  // Some instantiations (end)
  //--------------------------


  //-----------
  // Tests core
  //-----------

  // #1 keywordlist, only check the needed keywords
  /*-------------------------------------*/
  if (checkNeededKw)
  {
    // Split the string into many tokens, with whitespaces as separators
    std::list<std::string> neededKw;
    copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(), back_inserter(neededKw));


    std::list<std::string> missingKw;
    for (std::list<std::string>::iterator neededIt = neededKw.begin(); neededIt != neededKw.end(); ++neededIt)
    {
      bool foundNeededKw = false;
      for (KeywordlistMapType::iterator kwlistIt = kwmap.begin(); kwlistIt != kwmap.end(); ++kwlistIt)
      {
        std::size_t found = kwlistIt->first.find(*neededIt);
        if (found != std::string::npos)
        {
          foundNeededKw = true;
        }
      }

      if (!foundNeededKw)
        missingKw.push_back(*neededIt);
    }

    if ((neededKw.size() > 0) && (missingKw.size() > 0))
    {
      std::cerr << "Some keywords were not found; missing keywords : " << std::endl;
      for (std::list<std::string>::iterator itm = missingKw.begin(); itm != missingKw.end(); ++itm)
        std::cerr << *itm << std::endl;
      return EXIT_FAILURE;
    }
  }
  /*-------------------------------------*/


  pointsContainerType::iterator      pointsIt      = pointsContainer.begin();
  geo3dPointsContainerType::iterator geo3dPointsIt = geo3dPointsContainer.begin();
  while ((pointsIt != pointsContainer.end()) && (geo3dPointsIt != geo3dPointsContainer.end()))
  {
    imagePoint = *pointsIt;
    geo3dPoint = *geo3dPointsIt;
    geoPointOSSIM = *ossimIt;
    
    double z = geo3dPoint[2];
    otb::DEMHandler::GetInstance().SetDefaultHeightAboveEllipsoid(z);

    // otbForwardSensorModel and otbInverseSensorModel
    geoPoint           = forwardSensorModel->TransformPoint(imagePoint);
    reversedImagePoint = inverseSensorModel->TransformPoint(geoPoint);

    // otbGenericRSTransform
    geoPointGRS           = img2wgs->TransformPoint(imagePoint);
    reversedImagePointGRS = wgs2img->TransformPoint(geoPointGRS);

    // Just for debug purpose
    otbLogMacro(Debug, << "------------------------------------------------");
    otbLogMacro(Debug, << geomfilename);
    otbLogMacro(Debug, << std::setprecision(15) << "Image to geo (Inverse/Forward SensorModel): " << imagePoint << " -> " << geoPoint);
    otbLogMacro(Debug, << std::setprecision(15) << "Geo to image (Inverse/Forward SensorModel): " << geoPoint << " -> " << reversedImagePoint);
    otbLogMacro(Debug, << std::setprecision(15) << "Image to geo (GenericRSTransform): " << imagePoint << " -> " << geoPointGRS);
    otbLogMacro(Debug, << std::setprecision(15) << "Geo to image (GenericRSTransform): " << geoPointGRS << " -> " << reversedImagePointGRS);
    otbLogMacro(Debug, << std::setprecision(15) << "Image to geo (OSSIM): " << imagePoint << " -> " << geoPointOSSIM);


    // 3. Results should be plausible (not clearly out of bound results)
    /*-------------------------------------*/
    if (!((geoPoint[0] >= -180.0) && (geoPoint[0] <= 180.0)) || !((geoPoint[1] >= -90.0) && (geoPoint[1] <= 90.0)))
    {
      std::cerr << "GeoPoint out of bound (otbForwardSensorModel otbInverseSensorModel) : " << geoPoint << "\n";
      return EXIT_FAILURE;
    }

    if (!((geoPointGRS[0] >= -180.0) && (geoPointGRS[0] <= 180.0)) || !((geoPointGRS[1] >= -90.0) && (geoPointGRS[1] <= 90.0)))
    {
      std::cerr << "GeoPoint out of bound (otbGenericRSTransform) : " << geoPointGRS << "\n";
      return EXIT_FAILURE;
    }
    /*-------------------------------------*/

    // 2. Check that the encapsulation does not modify results
    /*-------------------------------------*/
    // Inverse/Forward SensorModel VS GenericRSTransform
    double dist1 = geoDistance->Evaluate(geoPoint, geoPointGRS);
    if (dist1 > geoTol)
    {
      std::cerr << "Geo distance between otbForwardSensorModel and otbGenericRSTransform too high : "
                << "dist = " << dist1 << " (tol = " << geoTol << ")" << std::endl;
      return EXIT_FAILURE;
    }

    // Inverse/Forward SensorModel VS OSSIM
    double dist2 = geoDistance->Evaluate(geoPoint, geoPointOSSIM);
    if (dist2 > geoTol)
    {
      std::cerr << "Geo distance between otbForwardSensorModel and OSSIM too high : "
                << "dist = " << dist2 << " (tol = " << geoTol << ")" << std::endl;
      return EXIT_FAILURE;
    }
    /*-------------------------------------*/

    // 4. Round tripping error
    /*-------------------------------------*/
    // for otbForwardSensorModel otbInverseSensorModel classes
    double dist3 = distance->Evaluate(imagePoint, reversedImagePoint);
    if (dist3 > imgTol)
    {
      std::cerr << "Round tripping error for otbForwardSensorModel / otbInverseSensorModel too high : "
                << "dist(imagePoint,reversedImagePoint) = " << dist3 << " (tol = " << imgTol << ")" << std::endl;
      return EXIT_FAILURE;
    }

    // for otbGenericRSTransform class
    double dist4 = distance->Evaluate(imagePoint, reversedImagePointGRS);
    if (dist4 > imgTol)
    {
      std::cerr << "Round tripping error for otbGenericRSTransform too high : "
                << "dist(imagePoint,reversedImagePoint) = " << dist4 << " (tol = " << imgTol << ")" << std::endl;
      return EXIT_FAILURE;
    }
    /*-------------------------------------*/

    // 5. Stability check
    /*-------------------------------------*/
    geoPointGCP[0] = geo3dPoint[0];
    geoPointGCP[1] = geo3dPoint[1];

    double dist5 = geoDistance->Evaluate(geoPoint, geoPointGCP);
    double dist6 = geoDistance->Evaluate(geoPointGRS, geoPointGCP);
    double dist7 = geoDistance->Evaluate(geoPointOSSIM, geoPointGCP);

    otbLogMacro(Debug, << "Forward SensorModel VS GCP : " << dist5);
    otbLogMacro(Debug, << "GenericRSTransform VS GCP : " << dist6);
    otbLogMacro(Debug, << "OSSIM VS GCP : " << dist7);

    if (dist5 > geoTol)
    {
      std::cerr << "Result for Forward SensorModel is unstable : dist from baseline = " << dist5 << " (tol = " << geoTol << ")" << std::endl;
      return EXIT_FAILURE;
    }
    if (dist6 > geoTol)
    {
      std::cerr << "Result for GenericRSTransform is unstable : dist from baseline = " << dist6 << " (tol = " << geoTol << ")" << std::endl;
      return EXIT_FAILURE;
    }
    /*-------------------------------------*/

    ++pointsIt;
    ++geo3dPointsIt;
    ++ossimIt;
  }
  //-----------------
  // Tests core (end)
  //-----------------

  return EXIT_SUCCESS;
}
