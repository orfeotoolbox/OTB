/*
 * Copyright (C) 1999-2011 Insight Software Consortium
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

#ifndef otbPersistentImageToVectorDataFilter_hxx
#define otbPersistentImageToVectorDataFilter_hxx

#include "otbPersistentImageToVectorDataFilter.h"

namespace otb
{

template <class TImage, class TOutputVectorData>
PersistentImageToVectorDataFilter<TImage, TOutputVectorData>::PersistentImageToVectorDataFilter()
{
  m_ExtractFilter    = ExtractImageFilterType::New();
  m_OutputVectorData = OutputVectorDataType::New();

  m_VectorDataIO = OGRVectorDataIOType::New();
}

template <class TImage, class TOutputVectorData>
typename PersistentImageToVectorDataFilter<TImage, TOutputVectorData>::OutputVectorDataType*
PersistentImageToVectorDataFilter<TImage, TOutputVectorData>::GetOutputVectorData() const
{
  return m_OutputVectorData;
}

template <class TImage, class TOutputVectorData>
void PersistentImageToVectorDataFilter<TImage, TOutputVectorData>::AllocateOutputs()
{
  // Nothing that needs to be allocated for the outputs : the output is not meant to be used
}

template <class TImage, class TOutputVectorData>
void PersistentImageToVectorDataFilter<TImage, TOutputVectorData>::Reset()
{
  typedef typename OutputVectorDataType::DataNodeType DataNodeType;
  typedef typename DataNodeType::Pointer              DataNodePointerType;

  this->GetOutputVectorData()->Clear();
  DataNodePointerType root = DataNodeType::New();
  root->SetNodeId("Root");
  this->GetOutputVectorData()->GetDataTree()->SetRoot(root);

  DataNodePointerType folder = DataNodeType::New();
  folder->SetNodeType(otb::FOLDER);

  DataNodePointerType document = DataNodeType::New();
  document->SetNodeType(otb::DOCUMENT);

  this->GetOutputVectorData()->GetDataTree()->Add(document, this->GetOutputVectorData()->GetDataTree()->GetRoot()->Get());
  this->GetOutputVectorData()->GetDataTree()->Add(folder, document);
}

template <class TImage, class TOutputVectorData>
void PersistentImageToVectorDataFilter<TImage, TOutputVectorData>::Synthetize()
{
}

template <class TImage, class TOutputVectorData>
void PersistentImageToVectorDataFilter<TImage, TOutputVectorData>::GenerateData()
{
  // call the processing function for this tile
  OutputVectorDataPointerType currentTileVD = this->ProcessTile();

  // merge the result into the output vector data object
  OutputVectorDataPointerType output = GetOutputVectorData();


  ConcatenateVectorDataFilterPointerType concatenate = ConcatenateVectorDataFilterType::New();
  concatenate->AddInput(output);
  concatenate->AddInput(currentTileVD);
  concatenate->Update();

  concatenate->GetOutput()->SetMetaDataDictionary(currentTileVD->GetMetaDataDictionary());

  // copy metadata and reference the same data tree
  output->Graft(concatenate->GetOutput());
}

template <class TImage, class TOutputVectorData>
void PersistentImageToVectorDataFilter<TImage, TOutputVectorData>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb
#endif
