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
#include "itkMacro.h"
#include <iostream>

#include "otbConfusionMatrixToMassOfBelief.h"

int otbConfusionMatrixToMassOfBeliefNew(int argc, char* argv[])
{
  typedef unsigned int LabelType;
  typedef itk::VariableLengthVector<LabelType> RefLabelType;
  typedef itk::Statistics::ListSample<RefLabelType> RefListLabelType;
  typedef itk::VariableLengthVector<LabelType> ProdLabelType;
  typedef itk::Statistics::ListSample<ProdLabelType> ProdListLabelType;
  typedef otb::ConfusionMatrixCalculator<RefListLabelType, ProdListLabelType> ConfusionMatrixCalculatorType;

  // filter type
  typedef otb::ConfusionMatrixToMassOfBelief<ConfusionMatrixCalculatorType> ConfusionMatrixToMassOfBeliefType;

  // filter
  ConfusionMatrixToMassOfBeliefType::Pointer ConfMatToMOB = ConfusionMatrixToMassOfBeliefType::New();

  std::cout<<ConfMatToMOB<<std::endl;

  return EXIT_SUCCESS;
}
