/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom; Telecom bretagne. All rights reserved.
  See ITCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "otbVectorImage.h"
#include "itkVariableLengthVector.h"
#include "itkListSample.h"

#include "otbSOMMap.h"
//#include "otbPeriodicSOM.h"
#include "otbSOMWithMissingValue.h"

#include "otbFlexibleDistanceWithMissingValue.h"

int otbSOMWithMissingValueNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  const unsigned int Dimension = 2;
  typedef double                                 PixelType;
  typedef otb::VectorImage<PixelType, Dimension> ImageType;
  typedef ImageType::PixelType                   VectorType;

  typedef otb::Statistics::FlexibleDistanceWithMissingValue<VectorType> DistanceType;

  typedef otb::SOMMap<VectorType, DistanceType, Dimension>  MapType;
  typedef itk::Statistics::ListSample<VectorType>           SampleListType;
  typedef otb::Functor::CzihoSOMLearningBehaviorFunctor     LearningBehaviorFunctorType;
  typedef otb::Functor::CzihoSOMNeighborhoodBehaviorFunctor NeighborhoodBehaviorFunctorType;
  typedef otb::SOMWithMissingValue<SampleListType, MapType,
      LearningBehaviorFunctorType, NeighborhoodBehaviorFunctorType> SOMType;

  SOMType::Pointer som = SOMType::New();

  std::cout << som << std::endl;

  return EXIT_SUCCESS;
}
