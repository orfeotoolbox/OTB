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

#include "otbSimplePointCountStrategy.h"
#include "itkPointSet.h"
#include "otbImage.h"
#include "itkVariableLengthVector.h"

int otbSimplePointCountStrategyTest(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{

  const unsigned int Dimension = 2;
  typedef float RealType;

  typedef otb::Image<RealType, Dimension>     ImageType;
  typedef itk::VariableLengthVector<RealType> RealVectorType;

  typedef itk::PointSet<RealVectorType, Dimension> PointSetType;
  typedef PointSetType::PointsContainer            PointsContainerType;
  typedef ImageType::IndexType                     IndexType;

  typedef otb::Count<PointSetType, unsigned int, IndexType> counterType;

  /*pointSet de test*/
  PointSetType::Pointer                  pointset =  PointSetType::New();
  PointsContainerType::ElementIdentifier count = 0;
  PointSetType::PointType                Point;
  Point[0] = 12.14;
  Point[1] = 14.14;

  for (int i = 0; i < 10; ++i)
    {
    pointset->SetPoint(count, Point);
    count++;
    } /** Fin creation pointset de test */

  /*Test du filtre*/
  IndexType index;
  index[0] = 12;
  index[1] = 14;
  unsigned int rad = 2;
  counterType  comptemoica;

  std::cout << "Le resultat retourne est " << comptemoica(pointset, rad, index) << std::endl;

  return EXIT_SUCCESS;

}
