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

#ifndef otbDecisionTree_h
#define otbDecisionTree_h

#include <map>
#include "itkObjectFactory.h"
#include "itkDataObject.h"

namespace otb
{

/**
 * \class DecisionTree
 * \brief Models a decision tree
 *
 * A decision tree holds an attribute that is being tested and 2
 * maps (STL): one for subtrees and anoter for labels (for the case
 * where there are no subtrees). These maps as keys a pair (STL) which
 * holds the value of the attribute being tested as well as the type
 * of test (LT, LE, EQ, GE, GT).
 *
 * In order to build a tree, one uses the method AddBranch. There are
 * 2 versions of this method: one for adding a subtree, and another
 * for adding labels in the case of leaf nodes. Each of these versions
 * has a syntactic sugar version for the case of EQ test.
 *
 * This implementation does not use different classes for terminal and
 * non terminal nodes for simplicity and follows the implementation
 * suggested by P. Norvig in the python version of the AIMA code which
 * is available at
 * http://aima-python.googlecode.com/svn/trunk/learning.py
 *
 * In the case of several terminal nodes being eligible, the first is returned.
 *
 * \sa DecisionTreeClassifier
 * \sa DecisionTreeEstimator
 *
 * \ingroup OTBLearningBase
 */

template <class AttributeValueType, class LabelType>
class ITK_EXPORT DecisionTree : public itk::DataObject
{
public:

  enum DecisionTreeTestType { MIN, LT=MIN, LE, EQ, GE, GT, MAX=GT};
  /** Standard typedefs */
  typedef DecisionTree                  Self;
  typedef itk::DataObject               Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Creation through object factory macro */
  itkNewMacro(Self);
  /** Runtime information macro */
  itkTypeMacro(DecisionTree, DataObject);

  typedef typename std::pair< AttributeValueType, DecisionTreeTestType > KeyType;
  typedef typename std::map< KeyType, Pointer >                          TreeMapType;
  typedef typename std::map< KeyType, LabelType >                        LabelMapType;
  typedef std::vector<AttributeValueType>                                ExampleType;

  itkSetMacro(Attribute, unsigned int);
  itkGetMacro(Attribute, unsigned int);

  /** Add a subtree on the tested attribute*/
  void AddBranch(AttributeValueType attr, DecisionTreeTestType testType, Pointer branch);
  /** Add a subtree on the tested attribute - syntactic sugar for the
      EQ case*/
  void AddBranch(AttributeValueType attr, Pointer branch);

  /** Add a leaf node on the tested attribute*/
  void AddBranch(AttributeValueType attr, DecisionTreeTestType testType, LabelType label);
  /** Add a leaf node on the tested attribute - syntactic sugar for the
      EQ case*/
  void AddBranch(AttributeValueType attr, LabelType label);

  LabelType Decide(const ExampleType example);

protected:
  /** Constructor */
  DecisionTree();
  /** Destructor */
  ~DecisionTree() override;
  /** Output information redefinition */

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  DecisionTree(const Self &) = delete;
  void operator =(const Self&) = delete;

  /** Map holding the subtrees */
  TreeMapType* m_TreeMap;
  /** Map holding the labels if final node */
  LabelMapType m_LabelMap;
  /** The attribute number (in the vector) being tested */
  unsigned int m_Attribute;

  /** Is the tree a final node? */
  bool m_IsFinal;

  LabelType m_Label;


};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbDecisionTree.hxx"
#endif

#endif
