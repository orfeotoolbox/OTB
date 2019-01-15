/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbDecisionTree_hxx
#define otbDecisionTree_hxx

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

  otbMsgDevMacro( << "Trying to match attribute " << m_Attribute << " with value " << attrValue );

  bool found = false;
  if( m_IsFinal )
    {
    typename LabelMapType::const_iterator lmIt = m_LabelMap.begin();
    while( lmIt != m_LabelMap.end() )
      {
      KeyType theKey = lmIt->first;
      DecisionTreeTestType theTest = theKey.second;
      AttributeValueType theValue = theKey.first;
      switch( theTest  )
        {
        case LT:
          if( attrValue < theValue )
            return lmIt->second;
          break;
        case LE:
          if( attrValue <= theValue )
            return lmIt->second;
          break;
        case EQ:
          if( attrValue == theValue )
            return lmIt->second;
          break;
        case GE:
          if( attrValue >= theValue )
            return lmIt->second;
          break;
        case GT:
          if( attrValue > theValue )
            return lmIt->second;
          break;
        }
      ++lmIt;
      }

    // if we get here it means that a verified test was  not found
    itkGenericExceptionMacro(<< "Example could not be handled by decision tree.");
    }
  else
    {
    found = false;

    // Look for branches matching the test on the attribute
    std::vector< KeyType > candidateKeys;

    typename TreeMapType::const_iterator tmIt = m_TreeMap->begin();
    while( tmIt != m_TreeMap->end() )
      {
      KeyType theKey = tmIt->first;
      DecisionTreeTestType theTest = theKey.second;
      AttributeValueType theValue = theKey.first;
      switch( theTest  )
        {
        case LT:
          if( attrValue < theValue )
            {
            candidateKeys.push_back( theKey );
            found = true;
            }
          break;
        case LE:
          if( attrValue <= theValue )
            {
            candidateKeys.push_back( theKey );
            found = true;
            }
          break;
        case EQ:
          if( attrValue == theValue )
            {
            candidateKeys.push_back( theKey );
            found = true;
            }
          break;
        case GE:
          if( attrValue >= theValue )
            {
            candidateKeys.push_back( theKey );
            found = true;
            }
          break;
        case GT:
          if( attrValue > theValue )
            {
            candidateKeys.push_back( theKey );
            found = true;
            }
          break;
        }
      ++tmIt;
      }

    if( ! found )                                         // attribute
      // not found
      // in the map
      {
      itkGenericExceptionMacro(<< "Example could not be handled by decision tree.");
      }
    else
      {
      // If we found one or several matching tests
      typename std::vector< KeyType >::const_iterator ckIt = candidateKeys.begin();
      while ( ckIt != candidateKeys.end() )
        {
        otbMsgDevMacro(<< (*ckIt).first << " " << (*ckIt).second);
        Pointer child = (*m_TreeMap)[(*ckIt)];
        return child->Decide(example);
        }
      }

    }
  //added return statement to suppress warning, but if/then/else
  //conditions make sure it is never reached
    return static_cast<LabelType>(0);
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
