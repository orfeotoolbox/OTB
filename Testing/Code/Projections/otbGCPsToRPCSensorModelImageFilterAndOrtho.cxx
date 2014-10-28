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
#include "otbImageFileReader.h"
#include "otbGCPsToRPCSensorModelImageFilter.h"
#include "otbImageFileWriter.h"
#include "otbMapProjections.h"
#include "otbOrthoRectificationFilter.h"
#include "otbMapProjections.h"

int otbGCPsToRPCSensorModelImageFilterAndOrtho(int argc, char* argv[])
{
  if (argc < 11)
    {
    std::cerr << "Usage: " << argv[0] << " infname outfname <origin easting> <origin northing> <x size> <y size> <x spacing> <y spacing> <UTM zone> <UTM hemisphere> a1x a1y b1x b1y b1z ... aNx aNy bNx bNy bNz " << std::endl;
    return EXIT_FAILURE;
    }
  else if ((argc - 11) % 5 != 0)
    {
    std::cerr << "Inconsistent GCPs description!" << std::endl;
    return EXIT_FAILURE;
    }

  const char * infname       = argv[1];
  const char * outfname      = argv[2];

  typedef otb::VectorImage<float, 2>                      ImageType;
  typedef otb::ImageFileReader<ImageType>                 ReaderType;
  typedef otb::GCPsToRPCSensorModelImageFilter<ImageType> GCPsToSensorModelFilterType;
  typedef GCPsToSensorModelFilterType::Point2DType        Point2DType;
  typedef GCPsToSensorModelFilterType::Point3DType        Point3DType;
  typedef otb::ImageFileWriter<ImageType>                                  WriterType;
  typedef otb::UtmInverseProjection                                                 UtmMapProjectionType;
  typedef otb::OrthoRectificationFilter<ImageType, ImageType, UtmMapProjectionType> OrthoRectifFilterType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);
  reader->UpdateOutputInformation();

  GCPsToSensorModelFilterType::Pointer rpcEstimator = GCPsToSensorModelFilterType::New();
  rpcEstimator->SetInput(reader->GetOutput());

  unsigned int nbGCPs = (argc - 11) / 5;

  std::cout << "Receiving " << nbGCPs << " from command line." << std::endl;

  for (unsigned int gcpId = 0; gcpId < nbGCPs; ++gcpId)
    {
    Point2DType sensorPoint;
    sensorPoint[0] = atof(argv[11 + gcpId * 5]);
    sensorPoint[1] = atof(argv[12 + gcpId * 5]);

    Point3DType geoPoint;
    geoPoint[0] = atof(argv[13 + 5 * gcpId]);
    geoPoint[1] = atof(argv[14 + 5 * gcpId]);
    geoPoint[2] = atof(argv[15 + 5 * gcpId]);

    std::cout << "Adding GCP sensor: " << sensorPoint << " <-> geo: " << geoPoint << std::endl;

    rpcEstimator->AddGCP(sensorPoint, geoPoint);
    }

  rpcEstimator->GetOutput()->UpdateOutputInformation();

  // Set floatfield to format properly
  std::cout.setf(std::ios::fixed, std::ios::floatfield);
  std::cout.precision(10);

  std::cout << rpcEstimator->GetOutput()->GetImageKeywordlist() << std::endl;
  std::cout << "Residual ground error: " << rpcEstimator->GetRMSGroundError() << std::endl;

  // Orthorectify the output image
  WriterType::Pointer writer = WriterType::New();

  OrthoRectifFilterType::Pointer orthoRectifFilter = OrthoRectifFilterType::New();
  UtmMapProjectionType::Pointer  utmMapProjection = UtmMapProjectionType::New();

  orthoRectifFilter->SetInput(rpcEstimator->GetOutput());

  ImageType::IndexType start;
  start[0] = 0;
  start[1] = 0;
  orthoRectifFilter->SetOutputStartIndex(start);

  ImageType::SizeType size;
  size[0] = atoi(argv[5]);      // X size
  size[1] = atoi(argv[6]);            //Y size
  orthoRectifFilter->SetOutputSize(size);

  ImageType::SpacingType spacing;
  spacing[0] = atof(argv[7]);
  spacing[1] = atof(argv[8]);
  orthoRectifFilter->SetOutputSpacing(spacing);

  ImageType::PointType origin;
  origin[0] = strtod(argv[3], NULL);         //Origin easting
  origin[1] = strtod(argv[4], NULL);         //Origin northing
  orthoRectifFilter->SetOutputOrigin(origin);

  utmMapProjection->SetZone(atoi(argv[9]));
  utmMapProjection->SetHemisphere(argv[10][0]);
  orthoRectifFilter->SetMapProjection(utmMapProjection);

  ImageType::PixelType no_data(reader->GetOutput()->GetNumberOfComponentsPerPixel());
  orthoRectifFilter->SetEdgePaddingValue(no_data);

  // Displacement Field spacing
  ImageType::SpacingType  gridSpacing;
  gridSpacing[0] = 16.0 * atof(argv[7]);
  gridSpacing[1] = 16.0 * atof(argv[8]);

  orthoRectifFilter->SetDisplacementFieldSpacing(gridSpacing);

  writer->SetInput(orthoRectifFilter->GetOutput());
  writer->SetFileName(outfname);
  writer->SetNumberOfDivisionsTiledStreaming(4);
  writer->Update();

  return EXIT_SUCCESS;
}
