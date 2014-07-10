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
#include "otbDecisionTree.h"

enum WheatTypes { WinterWheat, SummerWheat };

int otbDecisionTreeNew(int itkNotUsed(argc), char ** itkNotUsed(argv))
{

  typedef bool AttributeValueType;

  typedef otb::DecisionTree< AttributeValueType, WheatTypes > DecisionTreeType;

  DecisionTreeType::Pointer decisionTree = DecisionTreeType::New();

  return EXIT_SUCCESS;
}
