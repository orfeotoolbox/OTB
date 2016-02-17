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
#include <sstream>
#include <fstream>
#include <iterator>

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbForwardSensorModel.h"
#include "otbInverseSensorModel.h"
#include "otbDEMHandler.h"
#include <otbImageKeywordlist.h>
#include "itkEuclideanDistanceMetric.h"
#include "otbGeographicalDistance.h"
#include "otbGenericRSTransform.h"

#include "ossim/projection/ossimProjection.h"
#include "ossim/projection/ossimSensorModelFactory.h"
#include "ossim/ossimPluginProjectionFactory.h"


typedef std::list< itk::Point<double, 2> > pointsContainerType;
typedef std::list< itk::Point<double, 3> > geo3dPointsContainerType;
typedef otb::VectorImage<double, 2>     ImageType;
typedef itk::Statistics::EuclideanDistanceMetric<ImageType::PointType> DistanceType;
typedef otb::GeographicalDistance<ImageType::PointType> GeographicalDistanceType;

int produceGCP(char * outputgcpfilename, const otb::ImageKeywordlist& kwlist, bool useForwardSensorModel=true, double z=16.19688987731934)
{
   itk::Point<double, 2> imagePoint;
   itk::Point<double, 2> geoPoint;
	
  //  otbForwardSensorModel
   typedef otb::ForwardSensorModel<double> ForwardSensorModelType;
   ForwardSensorModelType::Pointer forwardSensorModel = ForwardSensorModelType::New();
   forwardSensorModel->SetImageGeometry(kwlist);
   if( forwardSensorModel->IsValidSensorModel() == false )
   {
     std::cout<<"Invalid Model pointer m_Model == NULL!\n The ossim keywordlist is invalid!"<<std::endl;
     return EXIT_FAILURE;
   } 
   
   otb::DEMHandler::Instance()->SetDefaultHeightAboveEllipsoid(z);

  // ossim classes
  ossimKeywordlist ossimKwlist;
  kwlist.convertToOSSIMKeywordlist(ossimKwlist);
  
  ossimProjection* ossimSensorModel = ossimSensorModelFactory::instance()->createProjection(ossimKwlist);
  if (ossimSensorModel == NULL)
  {
      ossimSensorModel = ossimplugins::ossimPluginProjectionFactory::instance()->createProjection(ossimKwlist);
  }
  if (ossimSensorModel == NULL) // Model validity
  {
	std::cerr << "Invalid sensor model (ossimSensorModel is NULL)" << std::endl;
    return EXIT_FAILURE;
  }
	
  std::ofstream file(outputgcpfilename, std::ios::out | std::ios::trunc);
  if (file)
	{
    for(double x=10.0; x<300.0; x+=50.)
		for(double y=10.0; y<300.0; y+=50.)
		{
		  imagePoint[0]=x;
		  imagePoint[1]=y;	
		  if (useForwardSensorModel) // otbForwardSensorModel 
		  {
	          geoPoint = forwardSensorModel->TransformPoint(imagePoint);
	      }
	      else // ossim classes
	      {
			  ossimDpt ossimPoint(otb::internal::ConvertToOSSIMFrame(imagePoint[0]),  //x
								  otb::internal::ConvertToOSSIMFrame(imagePoint[1])); //y
			  ossimGpt ossimGPoint;
			  ossimSensorModel->lineSampleHeightToWorld(ossimPoint,z, ossimGPoint);
			  geoPoint[0] = ossimGPoint.lon;
			  geoPoint[1] = ossimGPoint.lat;
		  }
	      
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

bool provideGCP(char * gcpfilename, pointsContainerType& imgPt, geo3dPointsContainerType& geo3dPt)
{
	itk::Point<double, 2> imagePoint;
	itk::Point<double, 3> geo3dPoint;
	
	std::ifstream file(gcpfilename,std::ios::in);
	if (file)
		{
			std::string line;
			while(getline(file,line))
			{
			   if (line.find_first_of("#") != 0)
			   {  
				   std::istringstream iss(line);
			 
				   iss >> imagePoint[0] >> imagePoint[1] >> geo3dPoint[0] >> geo3dPoint[1] >> geo3dPoint[2];
				  
				   imgPt.push_back(imagePoint);
				   geo3dPt.push_back(geo3dPoint);
		       }
		      
		    }
			file.close();
		}
		else
		   return false;
	
	return true;
}

int otbNewSensorModel(int argc, char* argv[])
{
  if (argc != 10)
    {
    std::cout << argv[0] 
              << " <input geom filename> <input gcp filename> <output gcp filename> "
              <<" <needed keywords> <imgTol> <geoTol> <writeBaseline> <modeVerbose> <only check needed keywords>" << std::endl;

    return EXIT_FAILURE;
    }
    
  char * geomfilename = argv[1];
  char * gcpfilename = argv[2];
  char * outFilename = argv[3];
  std::istringstream iss(argv[4]);
  double imgTol = atof(argv[5]);
  double geoTol = atof(argv[6]);
  int writeBaseline = atoi(argv[7]);
  int modeVerbose = atoi(argv[8]);
  bool checkNeededKw = atoi(argv[9]);

  // -------------------
  // Some instanciations  
  // -------------------
  otb::ImageKeywordlist kwlist = otb::ReadGeometryFromGEOMFile(geomfilename);
    
  if (!(kwlist.GetSize() > 0))
  {
       std::cerr<<"ImageKeywordlist is empty."<<std::endl;
       return EXIT_FAILURE;
  }   
     
  if (writeBaseline)
  {
	  return produceGCP(outFilename,kwlist);
  }   
     
  typedef otb::ImageKeywordlist::KeywordlistMap KeywordlistMapType;
  KeywordlistMapType kwmap = kwlist.GetKeywordlist();
  
  //  otbForwardSensorModel
  typedef otb::ForwardSensorModel<double> ForwardSensorModelType;
  ForwardSensorModelType::Pointer forwardSensorModel = ForwardSensorModelType::New();
  if( !forwardSensorModel )
   {
     std::cerr<<"Invalid sensor model (ForwardSensorModelType::Pointer is NULL)"<<std::endl;
     return EXIT_FAILURE;
   }
  forwardSensorModel->SetImageGeometry(kwlist);
  if( forwardSensorModel->IsValidSensorModel() == false )
   {
     std::cerr<<"Invalid Model pointer m_Model == NULL!\n The ossim keywordlist is invalid!"<<std::endl;
     return EXIT_FAILURE;
   }
   
  // otbInverseSensorModel 
  typedef otb::InverseSensorModel<double> InverseSensorModelType;
  InverseSensorModelType::Pointer inverseSensorModel = InverseSensorModelType::New();
  if( !inverseSensorModel )
   {
     std::cerr<<"Invalid sensor model (InverseSensorModelType::Pointer is NULL)"<<std::endl;
     return EXIT_FAILURE;
   }
  inverseSensorModel->SetImageGeometry(kwlist);
  if( inverseSensorModel->IsValidSensorModel() == false )
   {
     std::cerr<<"Invalid Model pointer m_Model == NULL!\n The ossim keywordlist is invalid!"<<std::endl;
     return EXIT_FAILURE;
   }
   
  // otbGenericRSTransform 
  // Build wgs ref
  OGRSpatialReference oSRS;
  oSRS.SetWellKnownGeogCS("WGS84");
  char * wgsRef = NULL;
  oSRS.exportToWkt(&wgsRef);

  typedef otb::GenericRSTransform<>       GRSTransformType;
  
  // Instantiate Image->WGS transform
  GRSTransformType::Pointer img2wgs = GRSTransformType::New();
  if( !img2wgs )
   {
     std::cerr<<"Invalid sensor model (GRSTransformType::Pointer is NULL)"<<std::endl;
     return EXIT_FAILURE;
   }
  img2wgs->SetInputProjectionRef("");
  img2wgs->SetOutputProjectionRef(wgsRef);
  img2wgs->SetInputKeywordList(kwlist);
  img2wgs->InstanciateTransform();


  // Instantiate WGS->Image transform
  GRSTransformType::Pointer wgs2img = GRSTransformType::New();
  if( !wgs2img )
   {
     std::cerr<<"Invalid sensor model (GRSTransformType::Pointer is NULL)"<<std::endl;
     return EXIT_FAILURE;
   }
  wgs2img->SetInputProjectionRef("");
  wgs2img->SetOutputProjectionRef(wgsRef);
  wgs2img->SetOutputKeywordList(kwlist);
  wgs2img->InstanciateTransform();

  // ossim classes
  ossimKeywordlist ossimKwlist;
  kwlist.convertToOSSIMKeywordlist(ossimKwlist);
  
  ossimProjection* ossimSensorModel = ossimSensorModelFactory::instance()->createProjection(ossimKwlist);
  if (ossimSensorModel == NULL)
  {
      ossimSensorModel = ossimplugins::ossimPluginProjectionFactory::instance()->createProjection(ossimKwlist);
  }
  if (ossimSensorModel == NULL) // Model validity
  {
	std::cerr << "Invalid sensor model (ossimSensorModel is NULL)" << std::endl;
    return EXIT_FAILURE;
  }

  itk::Point<double, 2> imagePoint;
  itk::Point<double, 2> geoPoint,geoPointGCP;
  itk::Point<double, 3> geo3dPoint;
  itk::Point<double, 2> geoPointGRS;
  itk::Point<double, 2> geoPointOSSIM;
  itk::Point<double, 2> reversedImagePoint;
  itk::Point<double, 2> reversedImagePointGRS;
  

  pointsContainerType pointsContainer;
  geo3dPointsContainerType geo3dPointsContainer;
  bool okStatus = provideGCP(gcpfilename,pointsContainer,geo3dPointsContainer);
  if (!okStatus)
  {
     std::cerr << "File" << gcpfilename << " couldn't be opened" << std::endl;
     return EXIT_FAILURE;
  }
 
  DistanceType::Pointer distance = DistanceType::New();
  GeographicalDistanceType::Pointer geoDistance = GeographicalDistanceType::New();
  
  //--------------------------	
  // Some instanciations (end)
  //--------------------------
	
	
	
  //-----------	
  // Tests core
  //-----------	

  // #1 keywordlist, only check the needed keywords
  /*-------------------------------------*/
  if (checkNeededKw)
  {
      //Split the string into many tokens, with whitespaces as separators
      std::list<std::string> neededKw;
      copy(std::istream_iterator<std::string>(iss),
         std::istream_iterator<std::string>(),
         back_inserter(neededKw));
                                 
      
      std::list<std::string> missingKw;
      for(std::list<std::string>::iterator neededIt=neededKw.begin(); neededIt!=neededKw.end(); ++neededIt)
      {
        bool foundNeededKw = false;
        for(KeywordlistMapType::iterator kwlistIt=kwmap.begin(); kwlistIt!=kwmap.end(); ++kwlistIt)
        {
            std::size_t found = kwlistIt->first.find(*neededIt);
            if (found!=std::string::npos)
            {   
                foundNeededKw = true;
            }
        }
        
        if (!foundNeededKw)
            missingKw.push_back(*neededIt);
      }
      
      if ( (neededKw.size()>0) && (missingKw.size()>0) )
      {
        std::cerr << "Some keywords were not found; missing keywords : " << std::endl;
        for (std::list<std::string>::iterator itm = missingKw.begin(); itm != missingKw.end(); ++itm)  
           std::cerr << *itm << std::endl;
        return EXIT_FAILURE;
      }
  }
  /*-------------------------------------*/


  pointsContainerType::iterator pointsIt=pointsContainer.begin();	
  geo3dPointsContainerType::iterator geo3dPointsIt=geo3dPointsContainer.begin();	
  //for(; pointsIt!=pointsContainer.end(); ++pointsIt)
  while( (pointsIt!=pointsContainer.end()) && (geo3dPointsIt!=geo3dPointsContainer.end())  )
  {
	  imagePoint = *pointsIt;
	  geo3dPoint = *geo3dPointsIt;
	  
	  double z=geo3dPoint[2];
	  otb::DEMHandler::Instance()->SetDefaultHeightAboveEllipsoid(z);
	  
	  // otbForwardSensorModel and otbInverseSensorModel
	  geoPoint = forwardSensorModel->TransformPoint(imagePoint);
	  reversedImagePoint = inverseSensorModel->TransformPoint(geoPoint);
	  
	  // otbGenericRSTransform 
	  geoPointGRS = img2wgs->TransformPoint(imagePoint);
	  reversedImagePointGRS = wgs2img->TransformPoint(geoPointGRS);
	  
	  // ossim classes
	  ossimDpt ossimPoint(otb::internal::ConvertToOSSIMFrame(imagePoint[0]),  //x
						  otb::internal::ConvertToOSSIMFrame(imagePoint[1])); //y
	  ossimGpt ossimGPoint;
	  ossimSensorModel->lineSampleHeightToWorld(ossimPoint,z, ossimGPoint);
	  geoPointOSSIM[0] = ossimGPoint.lon;
      geoPointOSSIM[1] = ossimGPoint.lat;

	  // Just for debug purpose
	  if (modeVerbose)
	  {
		  std::cout << ">>>>>>>>>>>>>> ---------------------" << std::endl;
		  std::cout << ">>>>>>>>>>>>>>" << geomfilename << std::endl;
		  std::cout << ">>>>>>>>>>>>>>" << std::setprecision(15) 
					<<  "Image to geo (Inverse/Forward SensorModel): " << imagePoint << " -> " << geoPoint << "\n";
		  std::cout << ">>>>>>>>>>>>>>" << std::setprecision(15) 
					<<  "Geo to image (Inverse/Forward SensorModel): " << geoPoint << " -> " << reversedImagePoint << "\n";
		  std::cout << ">>>>>>>>>>>>>>" << std::setprecision(15) 
					<<  "Image to geo (GenericRSTransform): " << imagePoint << " -> " << geoPointGRS << "\n";
		  std::cout << ">>>>>>>>>>>>>>" << std::setprecision(15) 
					<<  "Geo to image (GenericRSTransform): " << geoPointGRS << " -> " << reversedImagePointGRS << "\n";
		  std::cout << ">>>>>>>>>>>>>>" << std::setprecision(15) 
					<<  "Image to geo (OSSIM): " << imagePoint << " -> " << geoPointOSSIM << "\n";
	  }

	  	  
	  // 3. Results should be plausible (no NaN and no clearly out of bound results)
	  /*-------------------------------------*/
	  if ( (ossim::isnan(geoPoint[0])) || (ossim::isnan(geoPoint[1])) )
	  {
		 std::cerr << " Nan results (otbForwardSensorModel otbInverseSensorModel) : image to geo: " << imagePoint << " -> " << geoPoint << "\n";
		 return EXIT_FAILURE;
	  }
	  
	  if (  (ossim::isnan(geoPointGRS[0])) || (ossim::isnan(geoPointGRS[1])) )
	  {
		 std::cerr << "Nan results (otbGenericRSTransform) : image to geo: " << imagePoint << " -> " << geoPointGRS << "\n";
		 return EXIT_FAILURE;
	  }
	  
	  if ( (ossim::isnan(reversedImagePoint[0])) || (ossim::isnan(reversedImagePoint[1])) )
	  {
		 std::cerr << "Nan results (otbForwardSensorModel otbInverseSensorModel) : geo to image: " << geoPoint << " -> " << reversedImagePoint << "\n";
		 return EXIT_FAILURE;
	  }
	  
	  if (  (ossim::isnan(reversedImagePointGRS[0])) || (ossim::isnan(reversedImagePointGRS[1])) )
	  {
		 std::cerr << "Nan results (otbGenericRSTransform) : geo to image: " << geoPointGRS << " -> " << reversedImagePointGRS << "\n";
		 return EXIT_FAILURE;
	  }
	  
	  if ( !((geoPoint[0]>=-180.0) && (geoPoint[0]<=180.0)) || !((geoPoint[1]>=-90.0) && (geoPoint[1]<=90.0)) )
	  {
		 std::cerr << "GeoPoint out of bound (otbForwardSensorModel otbInverseSensorModel) : " << geoPoint << "\n";
		 return EXIT_FAILURE;
	  }
	  
	  if ( !((geoPointGRS[0]>=-180.0) && (geoPointGRS[0]<=180.0)) || !((geoPointGRS[1]>=-90.0) && (geoPointGRS[1]<=90.0)) )
	  {
		 std::cerr << "GeoPoint out of bound (otbGenericRSTransform) : " << geoPointGRS << "\n";
		 return EXIT_FAILURE;
	  }
	  /*-------------------------------------*/
	  
	  //2. Check that the encapsulation does not modify results 
	  /*-------------------------------------*/
	  // Inverse/Forward SensorModel VS GenericRSTransform
	  double dist1 = geoDistance->Evaluate(geoPoint, geoPointGRS); 
	  if (dist1>geoTol)
	  {
		 std::cerr << "Geo distance between otbForwardSensorModel and otbGenericRSTransform too high : "
		           << "dist = " << dist1 << " (tol = " << geoTol << ")" << std::endl;
		 return EXIT_FAILURE;
	  }
	  
	  // Inverse/Forward SensorModel VS OSSIM
	  double dist2 = geoDistance->Evaluate(geoPoint, geoPointOSSIM); 
	  if (dist2>geoTol)
	  {
		 std::cerr << "Geo distance between otbForwardSensorModel and OSSIM too high : "
		           << "dist = " << dist2 << " (tol = " << geoTol << ")" << std::endl;
		 return EXIT_FAILURE;
	  }
	  /*-------------------------------------*/
	  
	  //4. Round tripping error 
	  /*-------------------------------------*/
	  //for otbForwardSensorModel otbInverseSensorModel classes
	  double dist3 = distance->Evaluate(imagePoint, reversedImagePoint);
	  if (dist3>imgTol)
	  {
		 std::cerr << "Round tripping error for otbForwardSensorModel / otbInverseSensorModel too high : "
		           << "dist(imagePoint,reversedImagePoint) = " << dist3 << " (tol = " << imgTol << ")" << std::endl;
		 return EXIT_FAILURE;
	  }
	  
	  //for otbGenericRSTransform class
	  double dist4 = distance->Evaluate(imagePoint, reversedImagePointGRS);
	  if (dist4>imgTol)
	  {
		 std::cerr << "Round tripping error for otbGenericRSTransform too high : "
		           << "dist(imagePoint,reversedImagePoint) = " << dist4 << " (tol = " << imgTol << ")" << std::endl;
		 return EXIT_FAILURE;
	  }
	  /*-------------------------------------*/

	  //5. Stability check
	  /*-------------------------------------*/
	  geoPointGCP[0] = geo3dPoint[0];
	  geoPointGCP[1] = geo3dPoint[1];
	  
	  double dist5 = geoDistance->Evaluate(geoPoint, geoPointGCP);
	  double dist6 = geoDistance->Evaluate(geoPointGRS, geoPointGCP);
	  double dist7 = geoDistance->Evaluate(geoPointOSSIM, geoPointGCP);
	  
	  if (modeVerbose)
	  {
		  std::cout << ">>>>>>>>>>>>>>" << "Forward SensorModel VS GCP : " <<  dist5 << std::endl;
		  std::cout << ">>>>>>>>>>>>>>" << "GenericRSTransform VS GCP : " <<  dist6 << std::endl;
		  std::cout << ">>>>>>>>>>>>>>" << "OSSIM VS GCP : " <<  dist7 << std::endl;
	  }
	  
	  if (dist5>geoTol)
	  {
		 std::cerr << "Result for Forward SensorModel is unstable : dist from baseline = " 
		           <<  dist5 << " (tol = " << geoTol << ")" << std::endl;
		 return EXIT_FAILURE;
	  }
	  if (dist6>geoTol)
	  {
		 std::cerr << "Result for GenericRSTransform is unstable : dist from baseline = " 
		           <<  dist6 << " (tol = " << geoTol << ")" << std::endl;
		 return EXIT_FAILURE;
	  }
	 /*-------------------------------------*/
 
  ++pointsIt;
  ++geo3dPointsIt;
  }
  //-----------------	
  // Tests core (end)
  //-----------------	
  
  return EXIT_SUCCESS;
}
