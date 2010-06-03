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

#include "itkListSample.h"
#include "otbConfusionMatrixCalculator.h"



int otbConfusionMatrixCalculatorNew(int argc, char* argv[])
{

  typedef itk::FixedArray<int, 1>                 LabelType;
  typedef itk::Statistics::ListSample<LabelType>  ListLabelType;
  typedef otb::ConfusionMatrixCalculator< ListLabelType > CalculatorType;

  CalculatorType::Pointer calculator = CalculatorType::New();
  
  return EXIT_SUCCESS;
}
