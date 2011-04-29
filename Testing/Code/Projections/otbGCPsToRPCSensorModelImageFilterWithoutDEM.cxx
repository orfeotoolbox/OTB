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
#include "otbMacro.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbGCPsToRPCSensorModelImageFilter.h"

int otbGCPsToRPCSensorModelImageFilterWithoutDEM(int argc, char* argv[])
{
  if (argc < 3)
    {
    std::cerr << "Usage: " << argv[0] << " infname outfname a1x a1y b1x b1y b1z ... aNx aNy bNx bNy bNz" << std::endl;
    return EXIT_FAILURE;
    }
  else if ((argc - 3) % 5 != 0)
    {
    std::cerr << "Inconsistent GCPs description!" << std::endl;
    return EXIT_FAILURE;
    }

  const char * infname       = argv[1];
  const char * outfname      = argv[2];

  typedef otb::Image<float, 2>                            ImageType;
  typedef otb::ImageFileReader<ImageType>                 ReaderType;
  typedef otb::GCPsToRPCSensorModelImageFilter<ImageType> GCPsToSensorModelFilterType;
  typedef GCPsToSensorModelFilterType::Point2DType        Point2DType;
  typedef GCPsToSensorModelFilterType::Point3DType        Point3DType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);

  GCPsToSensorModelFilterType::Pointer rpcEstimator = GCPsToSensorModelFilterType::New();
  rpcEstimator->SetInput(reader->GetOutput());

  unsigned int nbGCPs = (argc - 3) / 5;

  std::cout << "Receiving " << nbGCPs << " from command line." << std::endl;

  for (unsigned int gcpId = 0; gcpId < nbGCPs; ++gcpId)
    {
    Point2DType sensorPoint;
    sensorPoint[0] = atof(argv[3 + gcpId * 5]);
    sensorPoint[1] = atof(argv[4 + gcpId * 5]);

    Point3DType geoPoint;
    geoPoint[0] = atof(argv[5 + 5 * gcpId]);
    geoPoint[1] = atof(argv[6 + 5 * gcpId]);
    geoPoint[2] = atof(argv[7 + 5 * gcpId]);

    std::cout << "Adding GCP sensor: " << sensorPoint << " <-> geo: " << geoPoint << std::endl;

    rpcEstimator->AddGCP(sensorPoint, geoPoint);
    }

  rpcEstimator->GetOutput()->UpdateOutputInformation();

  std::ofstream ofs;
  ofs.open(outfname);

  // Set floatfield to format properly
  ofs.setf(std::ios::fixed, std::ios::floatfield);
  ofs.precision(10);

  ofs << rpcEstimator->GetOutput()->GetImageKeywordlist() << std::endl;
  ofs << "Residual ground error: " << rpcEstimator->GetRMSGroundError() << std::endl;
  ofs.close();

  return EXIT_SUCCESS;
}
