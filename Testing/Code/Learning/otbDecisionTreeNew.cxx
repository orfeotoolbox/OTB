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
#include "itkExceptionObject.h"
#include "otbDecisionTree.h"

int otbDecisionTreeNew(int argc, char* argv[])
{

  enum WheatTypes { WinterWheat, SummerWheat };

  typedef bool AttributeValueType;

  typedef otbDecisionTree< AttributeValueType, WheatTypes > DecisionTreeType;

  DecisionTreeType::Pointer decisionTree = DecisionTreeType::New();

  return EXIT_SUCCESS;
}
