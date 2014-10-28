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


#include <iostream>
#include <iomanip>

#include "otbImage.h"
#include "otbImageFileReader.h"

#include "otbCompositeTransform.h"
#include "otbMapProjections.h"
#include "otbInverseSensorModel.h"

int otbCompositeTransform(int argc, char* argv[])
{

  if (argc != 3)
    {
    std::cout << argv[0] << " <input filename> <output filename>"
              << std::endl;

    return EXIT_FAILURE;
    }

  char * filename = argv[1];
  char * outFilename = argv[2];

  typedef otb::Image<double, 2>           ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(filename);
  reader->UpdateOutputInformation();

  typedef otb::UtmInverseProjection MapProjectionType;

  int  utmZone = 31;
  char utmHemisphere = 'N';

  MapProjectionType::Pointer mapProjection = MapProjectionType::New();
  mapProjection->SetZone(utmZone);
  mapProjection->SetHemisphere(utmHemisphere);

  typedef otb::InverseSensorModel<double> SensorModelType;
  SensorModelType::Pointer sensorModel = SensorModelType::New();
  sensorModel->SetImageGeometry(reader->GetOutput()->GetImageKeywordlist());

  if( sensorModel->IsValidSensorModel() == false )
   {
     std::cout<<"Invalid Model pointer m_Model == NULL!\n The ossim keywordlist is invalid!"<<std::endl;
     return EXIT_FAILURE;
   }

  typedef otb::CompositeTransform<MapProjectionType, SensorModelType> CompositeTransformType;
  CompositeTransformType::Pointer compositeTransform = CompositeTransformType::New();

  compositeTransform->SetFirstTransform(mapProjection);
  compositeTransform->SetSecondTransform(sensorModel);

  std::ofstream file;
  file.open(outFilename);

  file << std::setprecision(15);
  itk::Point<double, 2> point;
  point[0] = 374100;
  point[1] = 4829000;
  file << "Standard Composite transform: " << std::endl;
  file << point << " -> ";
  file << compositeTransform->TransformPoint(point);
  file << std::endl << std::endl;

  file.close();
  return EXIT_SUCCESS;
}
