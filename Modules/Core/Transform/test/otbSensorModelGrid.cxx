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

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbForwardSensorModel.h"
#include "otbInverseSensorModel.h"

int otbSensorModelGrid(int argc, char* argv[])
{
  if (argc != 6)
    {
    std::cout << argv[0] << " <input filename> <output filename in x> <output filename in y> <size_x> <size_y>" << std::endl;

    return EXIT_FAILURE;
    }

  char * filename = argv[1];
  char * outFilenameX = argv[2];
  char * outFilenameY = argv[3];
  unsigned int size_x((unsigned int)atoi(argv[4]));
  unsigned int size_y((unsigned int)atoi(argv[5]));

  typedef otb::VectorImage<double, 2>           VectorImageType;
  typedef otb::Image<double, 2>                 ImageType;
  typedef otb::ImageFileReader<VectorImageType> ReaderType;
  typedef otb::ImageFileWriter<ImageType> WriterType;
  typedef VectorImageType::SizeType             SizeType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(filename);
  reader->UpdateOutputInformation();

  WriterType::Pointer writeErrorX = WriterType::New();
  writeErrorX->SetFileName(outFilenameX);

  WriterType::Pointer writeErrorY = WriterType::New();
  writeErrorY->SetFileName(outFilenameY);

  SizeType sizeIn = reader->GetOutput()->GetLargestPossibleRegion().GetSize();
  SizeType sizeOut;
  sizeOut[0] = size_x;
  sizeOut[1] = size_y;

  ImageType::RegionType region;
  region.SetIndex(0, 0);
  region.SetSize(sizeOut);

  ImageType::Pointer outputErrorX = ImageType::New();
  outputErrorX->SetRegions(region);
  outputErrorX->Allocate();

  ImageType::Pointer outputErrorY = ImageType::New();
  outputErrorY->SetRegions(region);
  outputErrorY->Allocate();

  const double averageElevation = 16.19688987731934;
  otb::DEMHandler::Instance()->SetDefaultHeightAboveEllipsoid(averageElevation);

  typedef otb::ForwardSensorModel<double> ForwardSensorModelType;
  ForwardSensorModelType::Pointer forwardSensorModel = ForwardSensorModelType::New();
  forwardSensorModel->SetImageGeometry(reader->GetOutput()->GetImageKeywordlist());
  if( forwardSensorModel->IsValidSensorModel() == false )
    {
      std::cout<<"Invalid Model pointer m_Model == NULL!\n The ossim keywordlist is invalid!"<<std::endl;
      return EXIT_FAILURE;
    }

  typedef otb::InverseSensorModel<double> InverseSensorModelType;
  InverseSensorModelType::Pointer inverseSensorModel = InverseSensorModelType::New();
  inverseSensorModel->SetImageGeometry(reader->GetOutput()->GetImageKeywordlist());
  if( inverseSensorModel->IsValidSensorModel() == false )
    {
      std::cout<<"Invalid Model pointer m_Model == NULL!\n The ossim keywordlist is invalid!"<<std::endl;
      return EXIT_FAILURE;
    }

  double deltaX = static_cast<double>(sizeIn[0]) / static_cast<double>(size_x);
  double deltaY = static_cast<double>(sizeIn[1]) / static_cast<double>(size_y);

  for(unsigned int i = 0; i < size_x; ++i)
    {
      for(unsigned int j = 0; j < size_y; ++j)
        {
          itk::Point<double, 2> imagePoint;
          imagePoint[0] = deltaX * i;
          imagePoint[1] = deltaY * j;

          itk::Point<double, 2> geoPoint;
          geoPoint = forwardSensorModel->TransformPoint(imagePoint);

          itk::Point<double, 2> reversedImagePoint;
          reversedImagePoint = inverseSensorModel->TransformPoint(geoPoint);

          double errorX = imagePoint[0] - reversedImagePoint[0];
          double errorY = imagePoint[1] - reversedImagePoint[1];

          ImageType::IndexType index;
          index[0]= i;
          index[1]= j;

          outputErrorX->SetPixel(index, errorX);
          outputErrorY->SetPixel(index, errorY);
        }
    }

  writeErrorX->SetInput(outputErrorX);
  writeErrorX->Update();

  writeErrorY->SetInput(outputErrorY);
  writeErrorY->Update();

  return EXIT_SUCCESS;
}
