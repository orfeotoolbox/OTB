/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbConcatenateVectorDataFilter_hxx
#define otbConcatenateVectorDataFilter_hxx

#include "otbConcatenateVectorDataFilter.h"

#include "otbMath.h"

namespace otb
{
template <class TVectorData>
ConcatenateVectorDataFilter<TVectorData>::ConcatenateVectorDataFilter()
{
  this->SetNumberOfRequiredInputs(2);
  this->SetNumberOfRequiredOutputs(1);

  m_Folder = DataNodeType::New();
  m_Folder->SetNodeType(otb::FOLDER);

  m_Document = DataNodeType::New();
  m_Document->SetNodeType(otb::DOCUMENT);
}

template <class TVectorData>
void ConcatenateVectorDataFilter<TVectorData>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
}

template <class TVectorData>
void ConcatenateVectorDataFilter<TVectorData>::AddInput(const VectorDataType* vectorData)
{
  for (unsigned idx = 0; idx < this->GetNumberOfIndexedInputs(); ++idx)
  {
    if (!this->GetInput(idx))
    {
      this->Superclass::SetNthInput(idx, const_cast<VectorDataType*>(vectorData));
      return;
    }
  }
  this->Superclass::SetNthInput(this->GetNumberOfIndexedInputs(), const_cast<VectorDataType*>(vectorData));
}

template <class TVectorData>
const TVectorData* ConcatenateVectorDataFilter<TVectorData>::GetInput(unsigned int idx) const
{
  if (this->GetNumberOfInputs() < idx)
  {
    return nullptr;
  }
  return static_cast<const VectorDataType*>(this->Superclass::GetInput(idx));
}

template <class TVectorData>
void ConcatenateVectorDataFilter<TVectorData>::GenerateData()
{
  // TODO : no checking is done on the inputs, do checking to avoid
  // TODO : Check if they are in the same coordinate system (tricky)

  // Start recursive processing

  // Copy the input MetaDataDictionary in the output VectorData
  // this->GetOutput()->SetMetaDataDictionary(this->GetInput(0)->GetMetaDataDictionary());

  // Prepare the output
  // typename DataNodeType::Pointer outputRoot = this->GetOutput()->GetDataTree()->GetRoot()->Get();


  typename DataTreeType::Pointer outputTree = this->GetOutput()->GetDataTree();
  typename TreeNodeType::Pointer inputRoot  = const_cast<TreeNodeType*>(this->GetInput(0)->GetDataTree()->GetRoot());

  outputTree->SetRoot(inputRoot);

  typename DataNodeType::Pointer outputDocument = this->GetOutput()->GetDataTree()->GetRoot()->GetChild(0)->Get();

  // Adding the layer to the data tree
  //   this->GetOutput()->GetDataTree()->Add(m_Document, outputRoot);
  //   this->GetOutput()->GetDataTree()->Add(m_Folder, m_Document);

  // Retrieve all the inputs
  for (unsigned int idx = 1; idx < this->GetNumberOfInputs(); ++idx)
  {
    // Add the current vectordata
    TreeNodeType* currentInputRoot = const_cast<TreeNodeType*>(this->GetInput(idx)->GetDataTree()->GetRoot());

    ProcessNode(currentInputRoot, outputDocument);
  }
}


template <class TVectorData>
void ConcatenateVectorDataFilter<TVectorData>::ProcessNode(TreeNodeType* source, DataNodeType* outputDocument)
{
  if (source == nullptr)
    return;


  // Get the children list from the input node
  ChildrenListType children = source->GetChildrenList();

  // For each child
  typename ChildrenListType::iterator it;
  for (it = children.begin(); it != children.end(); ++it)
  {
    // get the data node
    DataNodePointerType dataNode = (*it)->Get();

    switch (dataNode->GetNodeType())
    {
    case ROOT:
    {
      ProcessNode((*it), outputDocument);
      break;
    }
    case DOCUMENT:
    {
      ProcessNode((*it), outputDocument);
      break;
    }
    case FOLDER:
    {
      ProcessNode((*it), outputDocument);
      break;
    }
    case FEATURE_POINT:
    {
      this->GetOutput()->GetDataTree()->Add(dataNode, outputDocument);
      break;
    }
    case FEATURE_LINE:
    {
      this->GetOutput()->GetDataTree()->Add(dataNode, outputDocument);
      break;
    }
    case FEATURE_POLYGON:
    {
      this->GetOutput()->GetDataTree()->Add(dataNode, outputDocument);
      break;
    }
    case FEATURE_MULTIPOINT:
    {
      ProcessNode((*it), outputDocument);
      break;
    }
    case FEATURE_MULTILINE:
    {
      ProcessNode((*it), outputDocument);
      break;
    }
    case FEATURE_MULTIPOLYGON:
    {
      ProcessNode((*it), outputDocument);
      break;
    }
    case FEATURE_COLLECTION:
    {
      ProcessNode((*it), outputDocument);
      break;
    }
    }
  }
}


} // end namespace otb

#endif
