/*
 * Copyright (C) 2005-2022 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbVectorData_hxx
#define otbVectorData_hxx

#include "otbVectorData.h"
#include "otbMetaDataKey.h"

namespace otb
{

template <class TPrecision, unsigned int VDimension, class TValuePrecision>
VectorData<TPrecision, VDimension, TValuePrecision>::VectorData()
{
  m_root = DataNodeType::New();
  m_root->SetNodeId("Root");
  boost::add_vertex(m_root, m_DataTree);
  m_Origin.Fill(0);
  m_Spacing.Fill(1);
}

template <class TPrecision, unsigned int VDimension, class TValuePrecision>
void VectorData<TPrecision, VDimension, TValuePrecision>::SetProjectionRef(const std::string& projectionRef)
{
  itk::MetaDataDictionary& dict = this->GetMetaDataDictionary();

  itk::EncapsulateMetaData<std::string>(dict, MetaDataKey::ProjectionRefKey, projectionRef);
  this->Modified();
}

template <class TPrecision, unsigned int VDimension, class TValuePrecision>
std::string VectorData<TPrecision, VDimension, TValuePrecision>::GetProjectionRef() const
{
  const itk::MetaDataDictionary& dict = this->GetMetaDataDictionary();

  std::string projectionRef;
  itk::ExposeMetaData<std::string>(dict, MetaDataKey::ProjectionRefKey, projectionRef);

  return projectionRef;
}

template <class TPrecision, unsigned int VDimension, class TValuePrecision>
void VectorData<TPrecision, VDimension, TValuePrecision>::SetSpacing(const SpacingType& spacing)
{
  itkDebugMacro("setting Spacing to " << spacing);
  if (this->m_Spacing != spacing)
  {
    this->m_Spacing = spacing;
    this->Modified();
  }
}

template <class TPrecision, unsigned int VDimension, class TValuePrecision>
void VectorData<TPrecision, VDimension, TValuePrecision>::SetSpacing(const double spacing[2])
{
  SpacingType s(spacing);
  this->SetSpacing(s);
}

template <class TPrecision, unsigned int VDimension, class TValuePrecision>
void VectorData<TPrecision, VDimension, TValuePrecision>::SetSpacing(const float spacing[2])
{
  itk::Vector<float, 2> sf(spacing);
  SpacingType s;
  s.CastFrom(sf);
  this->SetSpacing(s);
}

template <class TPrecision, unsigned int VDimension, class TValuePrecision>
void VectorData<TPrecision, VDimension, TValuePrecision>::SetOrigin(const double origin[2])
{
  OriginType p(origin);
  this->SetOrigin(p);
}

template <class TPrecision, unsigned int VDimension, class TValuePrecision>
void VectorData<TPrecision, VDimension, TValuePrecision>::SetOrigin(const float origin[2])
{
  itk::Point<float, 2> of(origin);
  OriginType p;
  p.CastFrom(of);
  this->SetOrigin(p);
}

template <class TPrecision, unsigned int VDimension, class TValuePrecision>
void VectorData<TPrecision, VDimension, TValuePrecision>::Clear()
{
  m_DataTree.clear();
}

template <class TPrecision, unsigned int VDimension, class TValuePrecision>
int VectorData<TPrecision, VDimension, TValuePrecision>::Size() const
{
  return boost::num_vertices(m_DataTree);
}

template <class TPrecision, unsigned int VDimension, class TValuePrecision>
void VectorData<TPrecision, VDimension, TValuePrecision>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << "\n";

  typename boost::graph_traits<DataTreeType>::vertex_iterator it,it_end;
  boost::tie(it, it_end) = vertices(m_DataTree);

  for (;it!=it_end;it++)
  {
    typename boost::graph_traits<DataTreeType>::adjacency_iterator ai, a_end; 
    boost::tie(ai, a_end) = boost::adjacent_vertices(*it, m_DataTree);
    os << *it << " --> ";
    for (; ai != a_end; ai++) { 
        os << *ai << "\t";
    }
    os << "\n";
  }
}

template <class TPrecision, unsigned int VDimension, class TValuePrecision>
void VectorData<TPrecision, VDimension, TValuePrecision>::Graft(const itk::DataObject* data)
{
  // call the superclass' implementation
  Superclass::Graft(data);

  if (data)
  {
    // Attempt to cast data to an Image
    const Self* vdData = dynamic_cast<const Self*>(data);

    if (vdData)
    {
      // Copy all the needed data : DataTree, spacing, origin and
      // Projection Ref
      //m_DataTree = const_cast<DataTreeType*>(vdData->GetDataTree());
      this->SetSpacing(vdData->GetSpacing());
      this->SetOrigin(vdData->GetOrigin());
      this->SetProjectionRef(vdData->GetProjectionRef());
    }
    else
    {
      // pointer could not be cast back down
      itkExceptionMacro(<< "otb::VectorData::Graft() cannot cast " << typeid(data).name() << " to " << typeid(const Self*).name());
    }
  }
}
} // end namespace otb

#endif
