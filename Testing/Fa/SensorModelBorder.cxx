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
  if (argc != 2)
    {
    std::cout << argv[0] << " <input filename>" << std::endl;

    return EXIT_FAILURE;
    }

  char * filename = argv[1];

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

  const int radius = 3;

  itk::Point<double, 2> imagePoint;

  // Test upper left corner
  std::cout << " --- upper left corner ---" << std::endl;
  for (int i = region.GetIndex(0) - radius; i < region.GetIndex(0) + radius; i++)
    {
    imagePoint[0] = i;
    for (int j = region.GetIndex(1) - radius; j < region.GetIndex(1) + radius; j++)
      {
      imagePoint[1] = j;

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
  for (int i = region.GetIndex(0) - radius; i < region.GetIndex(0) + radius; i++)
    {
    imagePoint[0] = i;
    for (int j = region.GetIndex(1) + region.GetSize(1) - radius; j < region.GetIndex(1) + region.GetSize(1) + radius; j++)
      {
      imagePoint[1] = j;

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
  for (int i = region.GetIndex(0) + region.GetSize(0) - radius; i < region.GetIndex(0) + region.GetSize(0) + radius; i++)
    {
    imagePoint[0] = i;
    for (int j = region.GetIndex(1) + region.GetSize(1) - radius; j < region.GetIndex(1) + region.GetSize(1) + radius; j++)
      {
      imagePoint[1] = j;

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
  for (int i = region.GetIndex(0) + region.GetSize(0) - radius; i < region.GetIndex(0) + region.GetSize(0) + radius; i++)
    {
    imagePoint[0] = i;
    for (int j = region.GetIndex(1) - radius; j < region.GetIndex(1) + radius; j++)
      {
      imagePoint[1] = j;

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

  return EXIT_SUCCESS;
}
