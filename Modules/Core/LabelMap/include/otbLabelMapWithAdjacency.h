/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbLabelMapWithAdjacency_h
#define otbLabelMapWithAdjacency_h

#include "itkLabelMap.h"
#include "otbMergeLabelObjectFunctor.h"

#include <set>

namespace otb
{
/** \class LabelMapWithAdjacency
*   \brief This class is a LabelMap with additional adjacency information.
*
*   The adjacency information is stored as a map of set of labels, in
*   order to avoid duplicated neighbors.
 *
 * \ingroup OTBLabelMap
*/

template <class TLabelObject>
class ITK_EXPORT LabelMapWithAdjacency : public itk::LabelMap<TLabelObject>
{
public:
  /** Standard class typedefs */
  typedef LabelMapWithAdjacency         Self;
  typedef itk::LabelMap<TLabelObject>   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  typedef itk::WeakPointer<const Self>  ConstWeakPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(LabelMapWithAdjacency, LabelMap);

  /** Typedef of the LabelObject */
  typedef TLabelObject LabelObjectType;

  /** Dimension of the image.  This constant is used by functions that are
   * templated over image type (as opposed to being templated over pixel type
   * and dimension) when they need compile time access to the dimension of
   * the image. */
  itkStaticConstMacro(ImageDimension, unsigned int, LabelObjectType::ImageDimension);

  /** Label typedef support. */
  typedef typename LabelObjectType::LabelType LabelType;

  /** A set containing adjacent labels */
  typedef std::set<LabelType> AdjacentLabelsContainerType;

  /** A map containing the set of adjacent labels per label */
  typedef std::map<LabelType, AdjacentLabelsContainerType> AdjacencyMapType;

  /** The merging functor */
  typedef Functor::MergeLabelObjectFunctor<TLabelObject> MergeFunctorType;

  /** A pair of labels */
  typedef std::pair<LabelType, LabelType> LabelPairType;

  /** A vector of pair of labels */
  typedef std::vector<LabelPairType> LabelPairVectorType;

  /** Set/Get the adjacency map */
  void SetAdjacencyMap(const AdjacencyMapType& amap)
  {
    m_AdjacencyMap = amap;
  }

  const AdjacencyMapType& GetAdjacencyMap() const
  {
    return m_AdjacencyMap;
  }

  /** Add a given label to the adjacent labels set of another label */
  void AddAdjacentLabel(LabelType label1, LabelType label2)
  {
    if (m_AdjacencyMap.find(label1) != m_AdjacencyMap.end())
    {
      m_AdjacencyMap[label1].insert(label2);
    }
    else
    {
      AdjacentLabelsContainerType newContainer;
      newContainer.insert(label2);
      m_AdjacencyMap[label1] = newContainer;
    }
  }

  /** Clear the adjacent labels of a given label */
  void ClearAdjacentLabels(LabelType label)
  {
    if (m_AdjacencyMap.find(label) != m_AdjacencyMap.end())
    {
      m_AdjacencyMap[label].clear();
    }
  }

  /** Remove the given adjacent label from the given label */
  void RemoveAdjacentLabel(LabelType label1, LabelType label2)
  {
    if (m_AdjacencyMap.find(label1) != m_AdjacencyMap.end())
    {
      m_AdjacencyMap[label1].erase(label2);
    }
  }

  /** Get the set of adjacent labels from a given label */
  const AdjacentLabelsContainerType& GetAdjacentLabels(LabelType label) const
  {
    typename AdjacencyMapType::const_iterator it = m_AdjacencyMap.find(label);
    if (it != m_AdjacencyMap.end())
    {
      return it->second;
    }
    else
    {
      itkExceptionMacro(<< "No Adjacency set for label " << label << ".");
    }
  }

  /** Merge two label objects. The first label will be the one retained */
  void MergeLabels(const LabelType& label1, const LabelType& label2)
  {
    // Check if source and destination labels are the same
    if (label1 == label2)
    {
      // If so, there is nothing to merge
      return;
    }

    // Check if two labels are adjacent
    if (m_AdjacencyMap.find(label1) != m_AdjacencyMap.end() && !m_AdjacencyMap[label1].count(label2))
    {
      itkExceptionMacro(<< "Labels " << label1 << " and " << label2 << " are not adjacent, can not merge.");
    }

    // Retrieve the two label objects
    typename LabelObjectType::Pointer lo1 = this->GetLabelObject(label1);
    typename LabelObjectType::Pointer lo2 = this->GetLabelObject(label2);

    // Merges label object
    MergeFunctorType                  mergeFunctor;
    typename LabelObjectType::Pointer loOut = mergeFunctor(lo1, lo2);

    // Move every occurrence of label2 to label1 in adjacency map
    for (typename AdjacentLabelsContainerType::iterator it = m_AdjacencyMap[label2].begin(); it != m_AdjacencyMap[label2].end(); ++it)
    {
      m_AdjacencyMap[*it].erase(label2);
      if (*it != label1)
      {
        m_AdjacencyMap[*it].insert(label1);
        m_AdjacencyMap[label1].insert(*it);
      }
    }

    // Remove label2 from adjancency map
    m_AdjacencyMap.erase(label2);

    // Remove label object corresponding to label2
    this->RemoveLabel(label2);

    // Replace label object corresponding to label1
    this->RemoveLabel(label1);
    this->AddLabelObject(loOut);
  }

  /** Merge a list of pairs of labels. For each pair, the first label
   * will be the one retained */
  void MergeLabels(const LabelPairVectorType& labels)
  {
    LabelPairVectorType internalLabelPairs = labels;

    typename LabelPairVectorType::iterator lpit1, lpit2;

    for (lpit1 = internalLabelPairs.begin(); lpit1 != internalLabelPairs.end(); ++lpit1)
    {
      // Merge the current label pair
      this->MergeLabels(lpit1->first, lpit1->second);

      // Update the remaining label pairs
      for (lpit2 = lpit1 + 1; lpit2 != internalLabelPairs.end(); ++lpit2)
      {
        if (lpit2->first == lpit1->second)
        {
          lpit2->first = lpit1->first;
        }
        if (lpit2->second == lpit1->second)
        {
          lpit2->second = lpit1->first;
        }
      }
    }
  }

protected:
  /** Constructor */
  LabelMapWithAdjacency()
  {
  }
  /** Destructor */
  ~LabelMapWithAdjacency() override
  {
  }
  /** Printself */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override
  {
    Superclass::PrintSelf(os, indent);
  }

  /** Re-implement CopyInformation to pass the adjancency graph
   * through */
  void CopyInformation(const itk::DataObject* data) override
  {
    // Call superclass implementation
    Superclass::CopyInformation(data);

    // Try to cast to LabelMapWithAdjacency
    const Self* selfData = dynamic_cast<const Self*>(data);

    // If cast succeed
    if (selfData)
      m_AdjacencyMap = selfData->m_AdjacencyMap;
  }

private:
  LabelMapWithAdjacency(const Self&) = delete;
  void operator=(const Self&) = delete;

  /** The adjacency map */
  AdjacencyMapType m_AdjacencyMap;
};

} // end namespace otb

#endif
