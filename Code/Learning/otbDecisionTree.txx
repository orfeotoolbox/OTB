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
  m_TreeMap = new TreeMapType;
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
  delete m_TreeMap;
}

template <class AttributeValueType, class LabelType>
void
DecisionTree<AttributeValueType, LabelType>
::AddBranch(AttributeValueType attr, Pointer branch)
{
  m_IsFinal = false;
  (*m_TreeMap)[attr] = branch;
}

template <class AttributeValueType, class LabelType>
void
DecisionTree<AttributeValueType, LabelType>
::AddBranch(AttributeValueType attr, LabelType label)
{
  m_IsFinal = true;
  m_LabelMap[attr] = label;
}

template <class AttributeValueType, class LabelType>
LabelType
DecisionTree<AttributeValueType, LabelType>
::Decide(const ExampleType example)
{
  if( m_IsFinal )
    {
    AttributeValueType attrValue = example[m_Attribute];
    if( m_LabelMap.find(attrValue) == m_LabelMap.end() ) // attribute
							 // not found
							 // in the map 
      itkGenericExceptionMacro(<< "Example could not be handled by decision tree.");
    LabelType lab = m_LabelMap[attrValue];
    return lab;
    }
  else
    {
    AttributeValueType attrValue = example[m_Attribute];
    if( m_TreeMap->find(attrValue) == m_TreeMap->end() ) // attribute
					                 // not found
					                 // in the map
      itkGenericExceptionMacro(<< "Example could not be handled by decision tree.");
    Pointer child = (*m_TreeMap)[attrValue];
    return child->Decide(example);
    }

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
