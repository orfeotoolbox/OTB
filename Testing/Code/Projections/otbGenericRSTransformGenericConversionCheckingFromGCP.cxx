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


#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbGenericRSTransform.h"
#include "itkEuclideanDistance.h"
#include "otbGeographicalDistance.h"
#include "otbDEMHandler.h"

#include <ogr_spatialref.h>

typedef otb::VectorImage<double, 2>       ImageType;
typedef otb::ImageFileReader<ImageType>   ReaderType;
typedef otb::GenericRSTransform<>         TransformType;
typedef TransformType::InputPointType     PointType;
typedef itk::Statistics::EuclideanDistance<ImageType::PointType> DistanceType;
typedef otb::GeographicalDistance<ImageType::PointType>          GeoDistanceType;

int otbGenericRSTransformGenericConversionCheckingFromGCP(int argc, char* argv[])
{
  std::string infname   = argv[1];
  std::string demDir    = argv[2];
  std::string geoidFile = argv[3]; //TODO: use geoid

  double imgTol = atof(argv[4]);
  double geoTol = atof(argv[5]);

  int verbose = atoi(argv[6]);

  bool pass = true;

  double accSumPx=0.0;
  double accSum2Px=0.0;
  double accSumM=0.0;
  double accSum2M=0.0;

  // Reader
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  reader->UpdateOutputInformation();

  // Build wgs ref
  OGRSpatialReference oSRS;
  oSRS.SetWellKnownGeogCS("WGS84");
  char * wgsRef = NULL;
  oSRS.exportToWkt(&wgsRef);

  // Distances
  DistanceType::Pointer distance = DistanceType::New();
  GeoDistanceType::Pointer geoDistance = GeoDistanceType::New();

  otb::DEMHandler::Pointer demHandler = otb::DEMHandler::Instance();
  // demHandler->OpenDEMDirectory(demDir);
  // demHandler->OpenGeoidFile(geoidFile); TODO: use geoid

  // Instantiate Image->WGS transform
  TransformType::Pointer img2wgs = TransformType::New();
  img2wgs->SetInputProjectionRef(reader->GetOutput()->GetProjectionRef());
  img2wgs->SetInputKeywordList(reader->GetOutput()->GetImageKeywordlist());
  img2wgs->SetOutputProjectionRef(wgsRef);
  img2wgs->SetDEMDirectory(demDir);
  img2wgs->InstanciateTransform();

  // Instanciate WGS->Image transform
  TransformType::Pointer wgs2img = TransformType::New();
  wgs2img->SetInputProjectionRef(wgsRef);
  wgs2img->SetOutputProjectionRef(reader->GetOutput()->GetProjectionRef());
  wgs2img->SetOutputKeywordList(reader->GetOutput()->GetImageKeywordlist());
  wgs2img->SetDEMDirectory(demDir);
  wgs2img->InstanciateTransform();

  std::cout << "GCPProjection is : " << reader->GetOutput()->GetGCPProjection() << std::endl;

  unsigned int nbGCP = reader->GetOutput()->GetGCPCount();
  std::cerr << "There is " << nbGCP << " GCPs embedded in this image" << std::endl;
  if (nbGCP == 0)
    {
    return EXIT_FAILURE;
    }

  std::cerr<<"## ---------- ##"<<std::endl;

  for (unsigned int GCPid = 0; GCPid < nbGCP; GCPid++)
    {
    ImageType::PointType ImgGCP, GeoGCP, estimatedImgGCP, estimatedGeoGCP;

    // References
    ImgGCP[0] = reader->GetOutput()->GetGCPCol(GCPid);
    ImgGCP[1] = reader->GetOutput()->GetGCPRow(GCPid);

    GeoGCP[0] = reader->GetOutput()->GetGCPX(GCPid);
    GeoGCP[1] = reader->GetOutput()->GetGCPY(GCPid);

    // Estimations
    estimatedImgGCP = wgs2img->TransformPoint(GeoGCP);
    estimatedGeoGCP = img2wgs->TransformPoint(ImgGCP);

    // Distances Computation
    double geoRes = geoDistance->Evaluate(GeoGCP, estimatedGeoGCP);
    double imgRes = distance->Evaluate(ImgGCP, estimatedImgGCP);


    if (verbose)
      {
       std::cerr<<"GCP #"<<GCPid<<": ["<<ImgGCP[0]<<","<<ImgGCP[1]<<"] -> "<<"["<<GeoGCP[0]<<","<<GeoGCP[1]<<"]"<<std::endl;
       std::cerr<<"Estimated Positions: "<<std::endl;
       std::cerr<<"Geographic (WGS84): "<<" ["<<estimatedGeoGCP[0]<<","<<estimatedGeoGCP[1]<<"]"<<std::endl;

       if((geoRes > geoTol) || vnl_math_isnan(geoRes))
         {
         std::cerr<<"!!! Geographic (WGS84) residual is too high: "<<geoRes<<" meters !!!"<<std::endl;
         pass = false;
         }
       std::cerr<<"Image index: "<<" ["<<estimatedImgGCP[0]<<","<<estimatedImgGCP[1]<<"]"<<std::endl;
       if((imgRes > imgTol) || vnl_math_isnan(imgRes))
         {
         std::cerr<<"!!! Image residual is too high: "<<imgRes<<" pixels !!!"<<std::endl;
         pass = false;
         }
       std::cerr<<"## ---------- ##"<<std::endl;
      }
    else
      {
      if((geoRes > geoTol) || vnl_math_isnan(geoRes) || (imgRes > imgTol) || vnl_math_isnan(imgRes))
        {
        pass = false;
        }
      }

    accSumPx  += imgRes;
    accSum2Px += (imgRes * imgRes);

    accSumM  += geoRes;
    accSum2M += (geoRes * geoRes);

    }


  if(pass)
     {
     return EXIT_SUCCESS;
     }
   else
     {
     std::cerr<<"There were imprecise results."<<std::endl;
     std::cerr<<"Total geographic residual (in meters): "<<accSumM
         << " - Mean: "<<accSumM/(double)(nbGCP)
         << " - Standard Deviation: "<< vcl_sqrt((accSum2M/(double)(nbGCP)) - ((accSumM/(double)(nbGCP)) * (accSumM/(double)(nbGCP))))
         <<std::endl;
     std::cerr<<"Total image residual (in pixels): "<< accSumPx
         << " - Mean: " <<accSumPx/(double)(nbGCP)
         << " - Standard Deviation: "<< vcl_sqrt((accSum2Px/(double)(nbGCP)) - ((accSumPx/(double)(nbGCP)) * (accSumPx/(double)(nbGCP))))
         <<std::endl;
     return EXIT_FAILURE;
     }
}
