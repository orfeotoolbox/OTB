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



#include "itkPointSet.h"
#include "itkPointSetToListSampleAdaptor.h"
#include "itkSubsample.h"
#include "otbSVMClassifier.h"

int otbSVMClassifierNew(int argc, char* argv[])
{
  typedef double                           InputPixelType;
  typedef int                              LabelPixelType;
  typedef itk::PointSet<InputPixelType, 2> PointSetType;

  typedef itk::Statistics::PointSetToListSampleAdaptor<PointSetType>
  DataSampleType;

  typedef otb::SVMClassifier<DataSampleType, LabelPixelType> ClassifierType;

  ClassifierType::Pointer classifier = ClassifierType::New();

  std::cout << classifier << std::endl;

  return EXIT_SUCCESS;
}
