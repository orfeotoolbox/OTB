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

#include <iomanip>
#include <iostream>

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbForwardSensorModel.h"
#include "otbInverseSensorModel.h"
#include "otbDEMHandler.h"
#include <otbImageKeywordlist.h>
#include "itkEuclideanDistanceMetric.h"
#include "otbGeographicalDistance.h"
#include "otbGenericRSTransform.h"

int otbNewSensorModel(int argc, char* argv[])
{
  if (argc != 5)
    {
    std::cout << argv[0] << " <input filename> <output filename> <imgTol>" << std::endl;

    return EXIT_FAILURE;
    }

  char * filename = argv[1];
  char * outFilename = argv[2];
  double imgTol = atof(argv[3]);
  double geoTol = atof(argv[4]);


  typedef otb::VectorImage<double, 2>     ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef itk::Statistics::EuclideanDistanceMetric<ImageType::PointType> DistanceType;
  typedef otb::GeographicalDistance<ImageType::PointType> GeographicalDistanceType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(filename);
  reader->UpdateOutputInformation();

  // #1 keywordlist, only check the needed keywords
  /*-------------------------------------*/
  std::ofstream file;
  file.open(outFilename);
  file << std::setprecision(15);
  file << "**  keywords list  **" << std::endl;
  otb::ImageKeywordlist kwlist = reader->GetOutput()->GetImageKeywordlist();
  
  if (!kwlist.GetSize() > 0)
  {
       std::cout<<"ImageKeywordlist is empty."<<std::endl;
       return EXIT_FAILURE;
  }   
     
  typedef otb::ImageKeywordlist::KeywordlistMap KeywordlistMapType;
  KeywordlistMapType kwmap = kwlist.GetKeywordlist();
  
  // list of needed keywords
  std::string allowedKw[]={"support_data.sensorID","line_den_coeff",
                           "line_den_coeff","samp_den_coeff","samp_num_coeff"};
                           
  int n = static_cast<int>(sizeof(allowedKw) / sizeof(std::string) );
  
  std::list<std::string> missingKw;
  for (int i=0; i<n; i++)
  {
	bool foundAllowedKw = false;
    for(KeywordlistMapType::iterator it=kwmap.begin(); it!=kwmap.end(); ++it)
	{
		std::size_t found = it->first.find(allowedKw[i]);
		if (found!=std::string::npos)
		{   
			file << it->first << " " << it->second << std::endl;
			foundAllowedKw = true;
		}
	}
	
	if (!foundAllowedKw)
	    missingKw.push_back(allowedKw[i]);
  }
  file.close();
  
  if (missingKw.size()>0)
  {
	std::cout << "Missing keywords : " << std::endl;
	for (std::list<std::string>::iterator itm = missingKw.begin(); itm != missingKw.end(); ++itm)  
	   std::cout << *itm << std::endl;
	return EXIT_FAILURE;
  }
  /*-------------------------------------*/

  otb::DEMHandler::Instance()->SetDefaultHeightAboveEllipsoid(16.19688987731934);

  typedef otb::ForwardSensorModel<double> ForwardSensorModelType;
  ForwardSensorModelType::Pointer forwardSensorModel = ForwardSensorModelType::New();
  forwardSensorModel->SetImageGeometry(kwlist);
  if( forwardSensorModel->IsValidSensorModel() == false )
   {
     std::cout<<"Invalid Model pointer m_Model == NULL!\n The ossim keywordlist is invalid!"<<std::endl;
     return EXIT_FAILURE;
   }
   
  typedef otb::InverseSensorModel<double> InverseSensorModelType;
  InverseSensorModelType::Pointer inverseSensorModel = InverseSensorModelType::New();
  inverseSensorModel->SetImageGeometry(kwlist);
  if( inverseSensorModel->IsValidSensorModel() == false )
   {
     std::cout<<"Invalid Model pointer m_Model == NULL!\n The ossim keywordlist is invalid!"<<std::endl;
     return EXIT_FAILURE;
   }
   
  // Build wgs ref
  OGRSpatialReference oSRS;
  oSRS.SetWellKnownGeogCS("WGS84");
  char * wgsRef = NULL;
  oSRS.exportToWkt(&wgsRef);

  typedef otb::GenericRSTransform<>       GRSTransformType;
  GRSTransformType::Pointer transform = GRSTransformType::New();
  
  // Instantiate Image->WGS transform
  GRSTransformType::Pointer img2wgs = GRSTransformType::New();
  img2wgs->SetInputProjectionRef(reader->GetOutput()->GetProjectionRef());
  img2wgs->SetInputKeywordList(reader->GetOutput()->GetImageKeywordlist());
  img2wgs->SetOutputProjectionRef(wgsRef);
  img2wgs->InstanciateTransform();

  // Instantiate WGS->Image transform
  GRSTransformType::Pointer wgs2img = GRSTransformType::New();
  wgs2img->SetInputProjectionRef(wgsRef);
  wgs2img->SetOutputProjectionRef(reader->GetOutput()->GetProjectionRef());
  wgs2img->SetOutputKeywordList(reader->GetOutput()->GetImageKeywordlist());
  wgs2img->InstanciateTransform();

  itk::Point<double, 2> imagePoint;
  imagePoint[0] = 10;
  imagePoint[1] = 10;
  itk::Point<double, 2> geoPoint;
  itk::Point<double, 2> geoPointGRS;
  itk::Point<double, 2> reversedImagePoint;
  itk::Point<double, 2> reversedImagePointGRS;
  
  geoPoint = forwardSensorModel->TransformPoint(imagePoint);
  reversedImagePoint = inverseSensorModel->TransformPoint(geoPoint);
  
  geoPointGRS = img2wgs->TransformPoint(imagePoint);
  reversedImagePointGRS = wgs2img->TransformPoint(geoPointGRS);

  // TODO : remove this, just for debug purpose
  std::cout << ">>>>>>>>>>>>>>" << std::endl;
  std::cout << filename << std::endl;
  std::cout << outFilename << std::endl;
  std::cout << "ProjectionRef = " << reader->GetOutput()->GetProjectionRef() << std::endl;
  std::cout << "Image to geo: " << imagePoint << " -> " << geoPoint << "\n";
  std::cout << "Geo to image: " << geoPoint << " -> " << reversedImagePoint << "\n";
  std::cout << "Image to geo: " << imagePoint << " -> " << geoPointGRS << "\n";
  std::cout << "Geo to image: " << geoPointGRS << " -> " << reversedImagePointGRS << "\n";
  // TODO (end)
  
  DistanceType::Pointer distance = DistanceType::New();
  GeographicalDistanceType::Pointer geoDistance = GeographicalDistanceType::New();
  
  // 3. Results should be plausible (no NaN and no clearly out of bound results)
  /*-------------------------------------*/
  if ( (!geoPoint[0]==geoPoint[0]) || (!geoPoint[1]==geoPoint[1]) )
  {
     std::cout << "Image to geo: " << imagePoint << " -> " << geoPoint << "\n";
     return EXIT_FAILURE;
  }
  
  if ( (!geoPointGRS[0]==geoPointGRS[0]) || (!geoPointGRS[1]==geoPointGRS[1]) )
  {
     std::cout << "Image to geo: " << imagePoint << " -> " << geoPointGRS << "\n";
     return EXIT_FAILURE;
  }
  
  if ( (!reversedImagePoint[0]==reversedImagePoint[0]) || (!reversedImagePoint[1]==reversedImagePoint[1]) )
  {
     std::cout << "Geo to image: " << geoPoint << " -> " << reversedImagePoint << "\n";
     return EXIT_FAILURE;
  }
  
  if ( (!reversedImagePointGRS[0]==reversedImagePointGRS[0]) || (!reversedImagePointGRS[1]==reversedImagePointGRS[1]) )
  {
     std::cout << "Geo to image: " << geoPointGRS << " -> " << reversedImagePointGRS << "\n";
     return EXIT_FAILURE;
  }
  
  if ( !((geoPoint[0]>=-180.0) && (geoPoint[0]<=180.0)) || !((geoPoint[0]>=-90.0) && (geoPoint[0]<=90.0)) )
  {
	 std::cout << "GeoPoint out of bound (otbForwardSensorModel otbInverseSensorModel) : " << geoPointGRS << "\n";
     return EXIT_FAILURE;
  }
  
  if ( !((geoPointGRS[0]>=-180.0) && (geoPointGRS[0]<=180.0)) || !((geoPointGRS[0]>=-90.0) && (geoPointGRS[0]<=90.0)) )
  {
	 std::cout << "GeoPoint out of bound (otbGenericRSTransform) : " << geoPointGRS << "\n";
     return EXIT_FAILURE;
  }
  /*-------------------------------------*/
  
  //2. GenericRSTransform encapsulate Inverse/Forward SensorModel: check that the encapsulation does not modify results 
  /*-------------------------------------*/
  double dist = geoDistance->Evaluate(geoPoint, geoPointGRS);
  if (dist>geoTol)
  {
     std::cout << "Geo distance between otbForwardSensorModel and otbGenericRSTransform too high : " << dist << "\n";
     return EXIT_FAILURE;
  }
  /*-------------------------------------*/
  
  //4. Round tripping error 
  /*-------------------------------------*/
  //for otbForwardSensorModel otbInverseSensorModel classes
  dist = distance->Evaluate(imagePoint, reversedImagePoint);
  if (dist>imgTol)
  {
     std::cout << "Round tripping for otbForwardSensorModel otbInverseSensorModel : dist(imagePoint,reversedImagePoint) = " << dist << "\n";
     return EXIT_FAILURE;
  }
  
  //for otbGenericRSTransform class
  dist = distance->Evaluate(imagePoint, reversedImagePointGRS);
  if (dist>imgTol)
  {
     std::cout << "Round tripping for otbGenericRSTransform : dist(imagePoint,reversedImagePoint) = " << dist << "\n";
     return EXIT_FAILURE;
  }
  /*-------------------------------------*/

  //5. Stability check
  /*-------------------------------------*/
  file.open(outFilename,std::ios::app);
  file << "** geo point **" << std::endl;
  file << geoPoint << std::endl;
  file << geoPointGRS << std::endl;
  file.close();
 /*-------------------------------------*/
  return EXIT_SUCCESS;
}
