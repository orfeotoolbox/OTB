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
#include "itkNumericTraits.h"

namespace otb
{
/**
 * Constructor
 */
template <class AttributeValueType, class LabelType>
DecisionTree<AttributeValueType, LabelType>
::DecisionTree()
{
  m_Map = new MapType;
  m_IsFinal = true;
  m_Label = static_cast<LabelType>(0);
}
/**
 * Destructor
 */
template <class AttributeValueType, class LabelType>
DecisionTree<AttributeValueType, LabelType>
::~DecisionTree()
{
  delete m_Map;
}

template <class AttributeValueType, class LabelType>
void
DecisionTree<AttributeValueType, LabelType>
::AddBranch(AttributeValueType attr, Pointer branch)
{
  m_IsFinal = false;
  (*m_Map)[attr] = branch;
}

template <class AttributeValueType, class LabelType>
void
DecisionTree<AttributeValueType, LabelType>
::AddBranch(AttributeValueType attr, LabelType label)
{
  m_IsFinal = true;
  m_Label = label;
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
