/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbPersistentImageToVectorDataFilter_txx
#define __otbPersistentImageToVectorDataFilter_txx

#include "otbPersistentImageToVectorDataFilter.h"

namespace otb
{

template<class TImage, class TOutputVectorData>
PersistentImageToVectorDataFilter<TImage, TOutputVectorData>
::PersistentImageToVectorDataFilter()
{
  m_ExtractFilter = ExtractImageFilterType::New();
  m_OutputVectorData = OutputVectorDataType::New();

  m_VectorDataIO = OGRVectorDataIOType::New();
}

template<class TImage, class TOutputVectorData>
typename PersistentImageToVectorDataFilter<TImage, TOutputVectorData>::OutputVectorDataType *
PersistentImageToVectorDataFilter<TImage, TOutputVectorData>
::GetOutputVectorData() const
{
  return m_OutputVectorData;
}

template<class TImage, class TOutputVectorData>
void
PersistentImageToVectorDataFilter<TImage, TOutputVectorData>
::AllocateOutputs()
{
  // Nothing that needs to be allocated for the outputs : the output is not meant to be used
}

template<class TImage, class TOutputVectorData>
void
PersistentImageToVectorDataFilter<TImage, TOutputVectorData>
::Reset()
{
  typedef typename OutputVectorDataType::DataNodeType DataNodeType;
  typedef typename DataNodeType::Pointer DataNodePointerType;

  this->GetOutputVectorData()->Clear();
  DataNodePointerType root = DataNodeType::New();
  root->SetNodeId("Root");
  this->GetOutputVectorData()->GetDataTree()->SetRoot(root);

  DataNodePointerType folder = DataNodeType::New();
  folder->SetNodeType(otb::FOLDER);

  DataNodePointerType document = DataNodeType::New();
  document->SetNodeType(otb::DOCUMENT);

  this->GetOutputVectorData()->GetDataTree()->Add(document, this->GetOutputVectorData()->GetDataTree()->GetRoot()->Get());
  this->GetOutputVectorData()->GetDataTree()->Add(folder , document);
}

template<class TImage, class TOutputVectorData>
void
PersistentImageToVectorDataFilter<TImage, TOutputVectorData>
::Synthetize()
{

}

template<class TImage, class TOutputVectorData>
void
PersistentImageToVectorDataFilter<TImage, TOutputVectorData>
::GenerateData()
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
  output->Graft( concatenate->GetOutput() );
}

template<class TImage, class TOutputVectorData>
void
PersistentImageToVectorDataFilter<TImage, TOutputVectorData>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb
#endif
