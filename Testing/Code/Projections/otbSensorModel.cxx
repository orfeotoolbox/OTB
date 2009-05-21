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
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <iostream>

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbForwardSensorModel.h"
#include "otbInverseSensorModel.h"

int otbSensorModel( int argc, char* argv[] )
{
  if (argc!=3)
  {
    std::cout << argv[0] <<" <input filename> <output filename>"
    << std::endl;

    return EXIT_FAILURE;
  }

  char * filename = argv[1];
  char * outFilename = argv[2];

  std::ofstream file;
  file.open(outFilename);
  file << std::setprecision(15);


  typedef otb::VectorImage<double, 2> ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(filename);
  reader->UpdateOutputInformation();

  file << "*** KEYWORD LIST ***\n";
  file << reader->GetOutput()->GetImageKeywordlist();

  file << "\n*** TRANSFORM ***\n";

  typedef otb::ForwardSensorModel<double> ForwardSensorModelType;
  ForwardSensorModelType::Pointer forwardSensorModel = ForwardSensorModelType::New();
  forwardSensorModel->SetImageGeometry(reader->GetOutput()->GetImageKeywordlist());


  itk::Point<double,2> imagePoint;
  imagePoint[0]=10;
  imagePoint[1]=10;

  itk::Point<double,2> geoPoint;
  geoPoint = forwardSensorModel->TransformPoint(imagePoint);

  file << "Image to geo: " << imagePoint << " -> " << geoPoint << "\n";
  typedef otb::InverseSensorModel<double> InverseSensorModelType;
  InverseSensorModelType::Pointer inverseSensorModel = InverseSensorModelType::New();
  inverseSensorModel->SetImageGeometry(reader->GetOutput()->GetImageKeywordlist());


  itk::Point<double,2> reversedImagePoint;
  reversedImagePoint = inverseSensorModel->TransformPoint(geoPoint);

  file << "Geo to image: " << geoPoint << " -> " << reversedImagePoint << "\n";

  file.close();
  return EXIT_SUCCESS;
}


