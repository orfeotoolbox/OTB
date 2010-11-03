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
#ifndef __otbDecisionTree_h
#define __otbDecisionTree_h

#include <map>
#include "itkObjectFactory.h"
#include "itkDataObject.h"

namespace otb
{
/**
 * \class DecisionTree
 * \brief Models a decision tree
 *
 * 
 *
 * \sa DecisionTreeClassifier
 * \sa DecisionTreeEstimator
 */

template <class AttributeValueType, class LabelType>
class ITK_EXPORT DecisionTree : public itk::DataObject
{
public:
  /** Standard typedefs */
  typedef DecisionTree                  Self;
  typedef itk::DataObject               Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Creation through object factory macro */
  itkNewMacro(Self);
  /** Runtime informations macro */
  itkTypeMacro(DecisionTree, DataObject);  

  typedef typename std::map< AttributeValueType, ConstPointer > MapType;
  void AddBranch(const DecisionTree& branch);

  LabelType Decide(const std::vector<AttributeValueType> example);

protected:
  /** Constructor */
  DecisionTree();
  /** Destructor */
  virtual ~DecisionTree();
  /** Output information redefinition */

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  DecisionTree(const Self &); // purposely not implemented
  void operator =(const Self&); // purposely not implemented

  /** Map holding the tree */
  MapType m_Map;
  /** The attribute number (in the vector) being tested */
  unsigned int m_Attribute;

  
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbDecisionTree.txx"
#endif

#endif
