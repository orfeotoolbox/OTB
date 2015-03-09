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


#include "otbRandomPointSetSource.h"
#include "itkPointSet.h"

int otbRandomPointSetSourceTest(int, char*[])
{
  typedef double                                  PixelType;
  typedef itk::PointSet<PixelType, 2>             PointSetType;
  typedef otb::RandomPointSetSource<PointSetType> PointSetSourceType;
  typedef PointSetType::PointsContainer           PointsContainerType;

  PointSetSourceType::Pointer pointSet = PointSetSourceType::New();
  pointSet->SetNumberOfPoints(10);
  PointSetType::PointType minPoint, maxPoint;
  minPoint[0] = 10;
  minPoint[1] = 20;
  pointSet->SetMinPoint(minPoint);
  maxPoint[0] = 100;
  maxPoint[1] = 50;
  pointSet->SetMaxPoint(maxPoint);

  //To get deterministic results
  pointSet->SetSeed(100);

  pointSet->Update();

  // Get the the point container
  PointSetSourceType::PointsContainerPointer
    points = pointSet->GetOutput()->GetPoints();

  PointsContainerType::ConstIterator it = points->Begin();
  while (it != points->End())
    {
    PointSetType::PointType p = it.Value();
    std::cout.width(5); std::cout << p[0] << ", ";
    std::cout.width(5); std::cout << p[1] << std::endl;
    ++it;
    }

  // All objects should be automatically destroyed at this point

  return EXIT_SUCCESS;

}
