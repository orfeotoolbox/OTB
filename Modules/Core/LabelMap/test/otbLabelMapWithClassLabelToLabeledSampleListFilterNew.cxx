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


#include "otbAttributesMapLabelObjectWithClassLabel.h"
#include "itkLabelMap.h"
#include "otbLabelMapWithClassLabelToLabeledSampleListFilter.h"
#include "itkVariableLengthVector.h"
#include "itkListSample.h"

int otbLabelMapWithClassLabelToLabeledSampleListFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  typedef otb::AttributesMapLabelObjectWithClassLabel
    <unsigned short, 2, double, unsigned short>                     LabelObjectType;
  typedef itk::LabelMap<LabelObjectType>                         LabelMapType;
  typedef itk::FixedArray<unsigned short, 1>                      LabelVectorType;
  typedef itk::VariableLengthVector<double>                      VectorType;
  typedef itk::Statistics::ListSample<VectorType>                ListSampleType;
  typedef itk::Statistics::ListSample<LabelVectorType>           TrainingListSampleType;
  typedef otb::LabelMapWithClassLabelToLabeledSampleListFilter<LabelMapType, ListSampleType, TrainingListSampleType> LabelMapFilterType;

  // instantiation
  LabelMapFilterType::Pointer filter = LabelMapFilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
