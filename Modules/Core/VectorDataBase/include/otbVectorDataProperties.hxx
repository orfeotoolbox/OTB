/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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


#ifndef otbVectorDataProperties_hxx
#define otbVectorDataProperties_hxx

#include "otbVectorDataProperties.h"
#include <algorithm>

namespace otb
{

template <class TVectorData>
bool VectorDataProperties<TVectorData>::IsBoundingRegionNull()
{
  for (unsigned int i = 0; i < VectorDataDimension; ++i)
  {
    if ((m_BoundingRegion.GetOrigin(i) != itk::NumericTraits<Type>::ZeroValue()) || (m_BoundingRegion.GetSize(i) != itk::NumericTraits<Type>::ZeroValue()))
      return false;
  }
  return true;
}
/**
  *   Add a region
 */
template <class TVectorData>
void VectorDataProperties<TVectorData>::AddRegion(const RegionType& region)
{
  //   std::cout << "add region: " << region << std::endl;
  if (this->IsBoundingRegionNull())
  {
    m_BoundingRegion = region;
  }
  else
  {
    Type index;
    for (unsigned int i = 0; i < VectorDataDimension; ++i)
    {
      index = std::min(m_BoundingRegion.GetOrigin(i), region.GetOrigin(i));
      m_BoundingRegion.SetSize(i, std::max(m_BoundingRegion.GetOrigin(i) + m_BoundingRegion.GetSize(i), region.GetOrigin(i) + region.GetSize(i)) - index);
      m_BoundingRegion.SetOrigin(i, index);
    }
  }
}
/**
 *   Compute the complete bounding box of the dataset
 */
template <class TVectorData>
void VectorDataProperties<TVectorData>::ComputeBoundingRegion()
{
  /** Initialize the bounding region to null*/
  IndexType index;
  SizeType  size;
  index.Fill(itk::NumericTraits<Type>::ZeroValue());
  size.Fill(itk::NumericTraits<Type>::ZeroValue());
  m_BoundingRegion.SetIndex(index);
  m_BoundingRegion.SetSize(size);

  /** Compute the bounding region*/
  InternalTreeNodeType* inputRoot = const_cast<InternalTreeNodeType*>(m_VectorDataObject->GetDataTree()->GetRoot());
  ProcessNode(inputRoot);
}

template <class TVectorData>
void VectorDataProperties<TVectorData>::ProcessNode(InternalTreeNodeType* source)
{
  // Get the children list from the input node
  ChildrenListType children = source->GetChildrenList();

  // For each child
  for (typename ChildrenListType::iterator it = children.begin(); it != children.end(); ++it)
  {
    // Copy input DataNode info
    DataNodePointerType dataNode = (*it)->Get();

    switch (dataNode->GetNodeType())
    {
    case otb::ROOT:
    {
      ProcessNode((*it));
      break;
    }
    case otb::DOCUMENT:
    {
      ProcessNode((*it));
      break;
    }
    case otb::FOLDER:
    {
      ProcessNode((*it));
      break;
    }
    case FEATURE_POINT:
    {
      //         otbGenericMsgDebugMacro(<<"Insert Point from vectorData");
      IndexType start;
      for (unsigned int i = 0; i < VectorDataDimension; ++i)
      {
        start[i] = dataNode->GetPoint()[i];
      }

      SizeType size;
      size.Fill(itk::NumericTraits<Type>::ZeroValue());
      RegionType region;
      region.SetSize(size);
      region.SetIndex(start);
      this->AddRegion(region);
      break;
    }
    case otb::FEATURE_LINE:
    {
      this->AddRegion(dataNode->GetLine()->GetBoundingRegion());
      break;
    }
    case FEATURE_POLYGON:
    {
      // otbGenericMsgDebugMacro(<<"Insert polygons from vectorData");
      this->AddRegion(dataNode->GetPolygonExteriorRing()->GetBoundingRegion());
      break;
    }
    case FEATURE_MULTIPOINT:
    {
      itkExceptionMacro(<< "This type (FEATURE_MULTIPOINT) is not handle (yet), please request for it");
      break;
    }
    case FEATURE_MULTILINE:
    {
      itkExceptionMacro(<< "This type (FEATURE_MULTILINE) is not handle (yet), please request for it");
      break;
    }
    case FEATURE_MULTIPOLYGON:
    {
      itkExceptionMacro(<< "This type (FEATURE_MULTIPOLYGON) is not handle (yet), please request for it");
      break;
    }
    case FEATURE_COLLECTION:
    {
      itkExceptionMacro(<< "This type (FEATURE_COLLECTION) is not handle (yet), please request for it");
      break;
    }
    }
  }
}

template <class TVectorData>
void VectorDataProperties<TVectorData>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Properties VectorData: " << m_VectorDataObject << std::endl;
  os << indent << "Properties Bounding Region: " << m_BoundingRegion << std::endl;
}

} // End namespace otb

#endif
