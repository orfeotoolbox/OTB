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

// Exercise the Spot5 sensor model on the image border
int main(int argc, char* argv[])
{
  if (argc != 3)
    {
    std::cout << argv[0] << " <input filename> <output filename>" << std::endl;

    return EXIT_FAILURE;
    }

  char * filename = argv[1];
  char*  outFilename = argv[2];


  typedef otb::VectorImage<double, 2> ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(filename);
  reader->UpdateOutputInformation();

  ImageType::Pointer image = reader->GetOutput();
  ImageType::RegionType region = image->GetLargestPossibleRegion();

  typedef otb::ForwardSensorModel<double> ForwardSensorModelType;
  ForwardSensorModelType::Pointer forwardSensorModel = ForwardSensorModelType::New();
  forwardSensorModel->SetImageGeometry(reader->GetOutput()->GetImageKeywordlist());
  if (forwardSensorModel->IsValidSensorModel() == false)
    {
    std::cout << "Invalid Model pointer m_Model == NULL!\n The ossim keywordlist is invalid!" << std::endl;
    return EXIT_FAILURE;
    }
  forwardSensorModel->SetAverageElevation(16.19688987731934);

  typedef otb::InverseSensorModel<double> InverseSensorModelType;
  InverseSensorModelType::Pointer inverseSensorModel = InverseSensorModelType::New();
  inverseSensorModel->SetImageGeometry(reader->GetOutput()->GetImageKeywordlist());
  if (inverseSensorModel->IsValidSensorModel() == false)
    {
    std::cout << "Invalid Model pointer m_Model == NULL!\n The ossim keywordlist is invalid!" << std::endl;
    return EXIT_FAILURE;
    }
  inverseSensorModel->SetAverageElevation(16.19688987731934);

  const int radius = 10;
  const double gridstep = 0.1;

  itk::Point<double, 2> imagePoint;

  // Test upper left corner
  std::cout << " --- upper left corner ---" << std::endl;
  for (imagePoint[0] = region.GetIndex(0) - radius; imagePoint[0] < region.GetIndex(0) + radius; imagePoint[0] += gridstep)
    {

    for (imagePoint[1] = region.GetIndex(1) - radius; imagePoint[1] < region.GetIndex(1) + radius; imagePoint[1] += gridstep)
      {

      itk::Point<double, 2> geoPoint;
      geoPoint = forwardSensorModel->TransformPoint(imagePoint);
      std::cout << "Image to geo: " << imagePoint << " -> " << geoPoint << "\n";

      if (vnl_math_isnan(geoPoint[0]) || vnl_math_isnan(geoPoint[1]))
        {
        return EXIT_FAILURE;
        }

      itk::Point<double, 2> reversedImagePoint;
      reversedImagePoint = inverseSensorModel->TransformPoint(geoPoint);

      std::cout << "Geo to image: " << geoPoint << " -> " << reversedImagePoint << "\n";

      if (vnl_math_isnan(reversedImagePoint[0]) || vnl_math_isnan(reversedImagePoint[1]))
        {
        return EXIT_FAILURE;
        }
      }
    }

  // Test lower left corner
  std::cout << " --- lower left corner ---" << std::endl;
  for (imagePoint[0] = region.GetIndex(0) - radius; imagePoint[0] < region.GetIndex(0) + radius; imagePoint[0] += gridstep)
    {
    for (imagePoint[1] = region.GetIndex(1) + region.GetSize(1) - radius; imagePoint[1] < region.GetIndex(1) + region.GetSize(1) + radius; imagePoint[1] += gridstep)
      {

      itk::Point<double, 2> geoPoint;
      geoPoint = forwardSensorModel->TransformPoint(imagePoint);
      std::cout << "Image to geo: " << imagePoint << " -> " << geoPoint << "\n";

      if (vnl_math_isnan(geoPoint[0]) || vnl_math_isnan(geoPoint[1]))
        {
        return EXIT_FAILURE;
        }

      itk::Point<double, 2> reversedImagePoint;
      reversedImagePoint = inverseSensorModel->TransformPoint(geoPoint);

      std::cout << "Geo to image: " << geoPoint << " -> " << reversedImagePoint << "\n";

      if (vnl_math_isnan(reversedImagePoint[0]) || vnl_math_isnan(reversedImagePoint[1]))
        {
        return EXIT_FAILURE;
        }
      }
    }

  // Test lower right corner
  std::cout << " --- lower right corner ---" << std::endl;
  for (imagePoint[0] = region.GetIndex(0) + region.GetSize(0) - radius; imagePoint[0] < region.GetIndex(0) + region.GetSize(0) + radius; imagePoint[0] += gridstep)
    {
    for (imagePoint[1] = region.GetIndex(1) + region.GetSize(1) - radius; imagePoint[1] < region.GetIndex(1) + region.GetSize(1) + radius; imagePoint[1] += gridstep)
      {

      itk::Point<double, 2> geoPoint;
      geoPoint = forwardSensorModel->TransformPoint(imagePoint);
      std::cout << "Image to geo: " << imagePoint << " -> " << geoPoint << "\n";

      if (vnl_math_isnan(geoPoint[0]) || vnl_math_isnan(geoPoint[1]))
        {
        return EXIT_FAILURE;
        }

      itk::Point<double, 2> reversedImagePoint;
      reversedImagePoint = inverseSensorModel->TransformPoint(geoPoint);

      std::cout << "Geo to image: " << geoPoint << " -> " << reversedImagePoint << "\n";

      if (vnl_math_isnan(reversedImagePoint[0]) || vnl_math_isnan(reversedImagePoint[1]))
        {
        return EXIT_FAILURE;
        }
      }
    }

  // Test upper right corner
  std::cout << " --- upper right corner ---" << std::endl;
  for (imagePoint[0] = region.GetIndex(0) + region.GetSize(0) - radius; imagePoint[0] < region.GetIndex(0) + region.GetSize(0) + radius; imagePoint[0] += gridstep)
    {
    for (imagePoint[1] = region.GetIndex(1) - radius; imagePoint[1] < region.GetIndex(1) + radius; imagePoint[1] += gridstep)
      {

      itk::Point<double, 2> geoPoint;
      geoPoint = forwardSensorModel->TransformPoint(imagePoint);
      std::cout << "Image to geo: " << imagePoint << " -> " << geoPoint << "\n";

      if (vnl_math_isnan(geoPoint[0]) || vnl_math_isnan(geoPoint[1]))
        {
        return EXIT_FAILURE;
        }

      itk::Point<double, 2> reversedImagePoint;
      reversedImagePoint = inverseSensorModel->TransformPoint(geoPoint);

      std::cout << "Geo to image: " << geoPoint << " -> " << reversedImagePoint << "\n";

      if (vnl_math_isnan(reversedImagePoint[0]) || vnl_math_isnan(reversedImagePoint[1]))
        {
        return EXIT_FAILURE;
        }
      }
    }



  // generat the output value along a segment crossing the lower image border
  // at the center position
  itk::Point<double, 2> imagePoint1;
  imagePoint1[0] = region.GetIndex(0) + region.GetSize(0)/2;
  imagePoint1[1] = region.GetIndex(1) + region.GetSize(1) - radius;

  itk::Point<double, 2> imagePoint2;
  imagePoint2[0] = region.GetIndex(0) + region.GetSize(0)/2;
  imagePoint2[1] = region.GetIndex(1) + region.GetSize(1) + radius;

  itk::Point<double, 2> geoPoint1, geoPoint2;
  geoPoint1 = forwardSensorModel->TransformPoint(imagePoint1);
  geoPoint2 = forwardSensorModel->TransformPoint(imagePoint2);

  itk::Vector<double, 2> geoDir;
  geoDir[0] = geoPoint2[0] - geoPoint1[0];
  geoDir[1] = geoPoint2[1] - geoPoint1[1];

  const int nbStep = 50;
  itk::Vector<double, 2> geoStep = geoDir / nbStep;

  std::ofstream file;
  file.open(outFilename);

  file << "# image_x image_y geo_x geo_y reversed_image_x reversed_image_y" << std::endl;
  file << std::setprecision(15);

  for (int i = 0; i < nbStep; ++i)
    {
    itk::Point<double, 2> geoPoint;
    geoPoint[0] = geoPoint1[0] + geoStep[0] * i;
    geoPoint[1] = geoPoint1[1] + geoStep[1] * i;

    itk::Point<double, 2> reversedImagePoint;
    reversedImagePoint = inverseSensorModel->TransformPoint(geoPoint);

    file << geoPoint[0] << "\t" << geoPoint[1]  << "\t"
         << reversedImagePoint[0] << "\t" << reversedImagePoint[1] << std::endl;

    if (vnl_math_isnan(geoPoint[0]) || vnl_math_isnan(geoPoint[1]))
      {
      return EXIT_FAILURE;
      }
    }

  file.close();

  return EXIT_SUCCESS;
}
