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


#include "otbTransformPointSetFilter.h"
#include "itkPointSet.h"
#include "itkAffineTransform.h"

int otbTransformPointSetFilterTest(int, char*[])
{

  // Declare the PointSet pixel type.
  // Those are the values associated
  // with each PointSet point. (not used on this filter test)
  typedef int PixelType;

  // Declare the types of the PointSet
  // By default it is a 3D PointSet using itk::Point<float, 3>
  // on the vertices, and an itk::VectorContainter
  // as containter for points
  typedef itk::PointSet<PixelType> PointSetType;

  // Declare the type for PointsContainer
  typedef PointSetType::PointsContainer PointsContainerType;

  // Declare the type for PointsContainerPointer
  typedef PointSetType::PointsContainerPointer
  PointsContainerPointer;
  // Declare the type for Points
  typedef PointSetType::PointType PointType;

  // Create an input PointSet
  PointSetType::Pointer inputPointSet  = PointSetType::New();

  // Insert data on the PointSet
  PointsContainerPointer points = inputPointSet->GetPoints();

  // Fill a cube with points , just to get some data
  int                                    n = 1; // let's start with a few of them
  PointsContainerType::ElementIdentifier count = 0;  // count them

  for (int x = -n; x <= n; x++)
    {
    for (int y = -n; y <= n; y++)
      {
      for (int z = -n; z <= n; z++)
        {
        PointType p;
        p[0] = x;
        p[1] = y;
        p[2] = z;
        std::cout << "Inserting point # ";
        std::cout.width(3); std::cout << count << "  = ";
        std::cout.width(4); std::cout << p[0] << ", ";
        std::cout.width(4); std::cout << p[1] << ", ";
        std::cout.width(4); std::cout << p[2] << std::endl;
        points->InsertElement(count, p);
        count++;
        }
      }
    }

  std::cout << "Input PointSet has " << inputPointSet->GetNumberOfPoints();
  std::cout << "   points " << std::endl;

  // Declare the transform type
  typedef itk::AffineTransform<float, 3> TransformType;

  // Declare the type for the filter
  typedef otb::TransformPointSetFilter<
      PointSetType,
      PointSetType,
      TransformType>       FilterType;

  // Create a Filter
  FilterType::Pointer filter = FilterType::New();

  // Create an  Transform
  // (it doesn't use smart pointers)
  TransformType::Pointer affineTransform = TransformType::New();
  affineTransform->Scale(3.5);
  TransformType::OffsetType::ValueType tInit[3] = {100, 200, 300};
  TransformType::OffsetType            translation = tInit;
  affineTransform->Translate(translation);

  // Connect the inputs
  filter->SetInput(inputPointSet);
  filter->SetTransform(affineTransform);

  // Execute the filter
  filter->Update();
  std::cout << "Filter: " << filter;

  // Get the Smart Pointer to the Filter Output
  PointSetType::Pointer outputPointSet = filter->GetOutput();

  std::cout << "Output PointSet has " << outputPointSet->GetNumberOfPoints();
  std::cout << "   points " << std::endl;

  // Get the the point container
  PointSetType::PointsContainerPointer
    transformedPoints = outputPointSet->GetPoints();

  PointsContainerType::ConstIterator it = transformedPoints->Begin();
  while (it != transformedPoints->End())
    {
    PointType p = it.Value();
    std::cout.width(5); std::cout << p[0] << ", ";
    std::cout.width(5); std::cout << p[1] << ", ";
    std::cout.width(5); std::cout << p[2] << std::endl;
    ++it;
    }

  // All objects should be automatically destroyed at this point

  return EXIT_SUCCESS;

}
