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
#include "itkEuclideanDistanceMetric.h"
#include "otbDEMHandler.h"
#include "itkMersenneTwisterRandomVariateGenerator.h"

#include <ogr_spatialref.h>

typedef otb::VectorImage<double, 2>       ImageType;
typedef otb::ImageFileReader<ImageType>   ReaderType;
typedef otb::GenericRSTransform<>         TransformType;
typedef TransformType::InputPointType     PointType;
typedef itk::Statistics::EuclideanDistanceMetric<ImageType::PointType> DistanceType;
typedef itk::Statistics::MersenneTwisterRandomVariateGenerator   RandomGeneratorType;

int otbForwardBackwardProjectionConsistency(int argc, char* argv[])
{
  std::string infname   = argv[1];
  std::string demDir    = argv[2];
  std::string geoidFile = argv[3]; //TODO: use geoid

  unsigned int nbTestedPoint = atoi(argv[4]);

  double imgTol = atof(argv[5]);

  unsigned int verbose = atoi(argv[6]);

  bool pass = true;

  double accSumPx=0.0;
  double accSum2Px=0.0;

  unsigned int identityCounter = 0;

  // Reader
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  reader->UpdateOutputInformation();

  // Image Size
  ImageType::SizeType size;
  size[0] = reader->GetOutput()->GetLargestPossibleRegion().GetSize()[0];
  size[1] = reader->GetOutput()->GetLargestPossibleRegion().GetSize()[1];

  // Build wgs ref
  OGRSpatialReference oSRS;
  oSRS.SetWellKnownGeogCS("WGS84");
  char * wgsRef = NULL;
  oSRS.exportToWkt(&wgsRef);

  // Distances
  DistanceType::Pointer distance = DistanceType::New();

  otb::DEMHandler::Pointer demHandler = otb::DEMHandler::Instance();
  demHandler->OpenDEMDirectory(demDir);
  // demHandler->OpenGeoidFile(geoidFile); TODO: use geoid

  // Instantiate Image->WGS transform
  TransformType::Pointer img2wgs = TransformType::New();
  img2wgs->SetInputProjectionRef(reader->GetOutput()->GetProjectionRef());
  img2wgs->SetInputKeywordList(reader->GetOutput()->GetImageKeywordlist());
  img2wgs->SetOutputProjectionRef(wgsRef);
  img2wgs->InstanciateTransform();

  // Instantiate WGS->Image transform
  TransformType::Pointer wgs2img = TransformType::New();
  wgs2img->SetInputProjectionRef(wgsRef);
  wgs2img->SetOutputProjectionRef(reader->GetOutput()->GetProjectionRef());
  wgs2img->SetOutputKeywordList(reader->GetOutput()->GetImageKeywordlist());
  wgs2img->InstanciateTransform();

  // Random Point Generator
  RandomGeneratorType::Pointer randomGenerator = RandomGeneratorType::GetInstance();

  for (unsigned int testID = 0; testID < nbTestedPoint; testID++)
    {
    ImageType::PointType ImgPoint, estimatedGeoPoint, estimatedImgPoint;

    // References
    ImgPoint[0] = vcl_floor(randomGenerator->GetUniformVariate(0., (double)(size[0])));
    ImgPoint[1] = vcl_floor(randomGenerator->GetUniformVariate(0., (double)(size[1])));

    // Estimations
    estimatedGeoPoint = img2wgs->TransformPoint(ImgPoint);
    estimatedImgPoint = wgs2img->TransformPoint(estimatedGeoPoint);

    // Distance Computation
    double imgRes = distance->Evaluate(ImgPoint, estimatedImgPoint);

    if (distance->Evaluate(ImgPoint, estimatedGeoPoint) == 0)
      {
      identityCounter ++;
      }

    if (verbose)
      {
       std::cerr<<"Point #"<<testID<<": ["<<ImgPoint[0]<<","<<ImgPoint[1]<<"]"<<std::endl;
       std::cerr<<"Estimated Positions: "<<std::endl;
       std::cerr<<"estimatedGeoPoint: "<<" ["<<estimatedGeoPoint[0]<<","<<estimatedGeoPoint[1]<<"]"<<std::endl;
       std::cerr<<"estimatedImgPoint: "<<" ["<<estimatedImgPoint[0]<<","<<estimatedImgPoint[1]<<"]"<<std::endl;

       if((imgRes > imgTol) || vnl_math_isnan(imgRes))
         {
         std::cerr<<"!!! Image residual is too high: "<<imgRes<<" pixels !!!"<<std::endl;
         pass = false;
         }
       std::cerr<<"## ---------- ##"<<std::endl;
      }
    else
      {
      if((imgRes > imgTol) || vnl_math_isnan(imgRes))
        {
        pass = false;
        }
      }

    accSumPx  += imgRes;
    accSum2Px += (imgRes * imgRes);

    }

  if (identityCounter == nbTestedPoint)
    {
    std::cerr<<"ERROR: img2wgs projection is identity!"<<std::endl;
    pass = false;
    }

  if(pass)
     {
     return EXIT_SUCCESS;
     }
   else
     {
     std::cerr<<"There were imprecise results."<<std::endl;
     std::cerr<<"Total image residual (in pixels): "<< accSumPx
         << " - Mean: " <<accSumPx/(double)(nbTestedPoint)
         //<< " - Standard Deviation: "<< vcl_sqrt((accSum2Px/(double)(nbTestedPoint)) - ((accSumPx/(double)(nbTestedPoint)) * (accSumPx/(double)(nbTestedPoint))))
         <<std::endl;
     return EXIT_FAILURE;
     }
}
