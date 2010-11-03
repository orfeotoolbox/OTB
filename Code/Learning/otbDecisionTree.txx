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
#ifndef __otbDecisionTree_txx
#define __otbDecisionTree_txx

#include "otbDecisionTree.h"
#include "otbMacro.h"

namespace otb
{
/**
 * Constructor
 */
template <class AttributeValueType, class LabelType>
DecisionTree<AttributeValueType, LabelType>
::DecisionTree()
{
}
/**
 * Destructor
 */
template <class AttributeValueType, class LabelType>
DecisionTree<AttributeValueType, LabelType>
::~DecisionTree()
{
}


/**
 *PrintSelf method
 */
template <class AttributeValueType, class LabelType>
void
DecisionTree<AttributeValueType, LabelType>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
