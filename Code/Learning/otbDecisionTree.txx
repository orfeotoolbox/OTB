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
::AddBranch(AttributeValueType attr, DecisionTreeTestType testType, Pointer branch)
{
  m_IsFinal = false;
  KeyType key = KeyType(attr, testType);
  (*m_TreeMap)[key] = branch;
}

template <class AttributeValueType, class LabelType>
void
DecisionTree<AttributeValueType, LabelType>
::AddBranch(AttributeValueType attr, Pointer branch)
{
  m_IsFinal = false;
  KeyType key = KeyType(attr, EQ);
  (*m_TreeMap)[key] = branch;
}

template <class AttributeValueType, class LabelType>
void
DecisionTree<AttributeValueType, LabelType>
::AddBranch(AttributeValueType attr, DecisionTreeTestType testType, LabelType label)
{
  m_IsFinal = true;
  KeyType key = KeyType(attr, testType);
  m_LabelMap[key] = label;
}

template <class AttributeValueType, class LabelType>
void
DecisionTree<AttributeValueType, LabelType>
::AddBranch(AttributeValueType attr, LabelType label)
{
  m_IsFinal = true;
  KeyType key = KeyType(attr, EQ);
  m_LabelMap[key] = label;
}

template <class AttributeValueType, class LabelType>
LabelType
DecisionTree<AttributeValueType, LabelType>
::Decide(const ExampleType example)
{
  AttributeValueType attrValue = example[m_Attribute];
  bool found = false;
  KeyType key;
  if( m_IsFinal )
    {
    for(DecisionTreeTestType i = MIN; i <= MAX; i=DecisionTreeTestType(i+1) )
      {
      key = KeyType(attrValue, i);
      if( m_LabelMap.find(key) != m_LabelMap.end() )
	{
	found = true;
	break;
	}
      } 
    if (! found )                                      // attribute
                                                       // not found
						       // in the map 
      itkGenericExceptionMacro(<< "Example could not be handled by decision tree.");

    LabelType lab = m_LabelMap[key];
    return lab;
    }
  else
    {
    found = false;
    for(DecisionTreeTestType i = MIN; i <= MAX; i=DecisionTreeTestType(i+1) )
      {
      key = KeyType(attrValue, i);
      if( m_TreeMap->find(key) != m_TreeMap->end() )
	{
	found = true;
	break;
	}
      } 
    if( ! found )                                         // attribute
					                 // not found
					                 // in the map
      itkGenericExceptionMacro(<< "Example could not be handled by decision tree.");

    std::cout << key.first << " " << key.second << std::endl;
    Pointer child = (*m_TreeMap)[key];
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
