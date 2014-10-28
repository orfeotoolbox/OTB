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



#include <fstream>
#include "otbSOMMap.h"
#include "otbSOMClassifier.h"
#include "itkListSample.h"

int otbSOMClassifierNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  typedef double InputPixelType;
  typedef int    LabelPixelType;
  const unsigned int Dimension = 2;

  typedef itk::VariableLengthVector<InputPixelType>                  PixelType;
  typedef itk::Statistics::EuclideanDistanceMetric<PixelType>              DistanceType;
  typedef otb::SOMMap<PixelType, DistanceType, Dimension>            SOMMapType;
  typedef itk::Statistics::ListSample<PixelType>                     SampleType;
  typedef otb::SOMClassifier<SampleType, SOMMapType, LabelPixelType> ClassifierType;

  ClassifierType::Pointer classifier = ClassifierType::New();

  std::cout << classifier << std::endl;

  return EXIT_SUCCESS;
}
