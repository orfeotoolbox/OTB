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
  this->itk::ProcessObject::SetNumberOfRequiredOutputs(2);
  this->itk::ProcessObject::SetNthOutput(1, this->MakeOutput(1).GetPointer());

  m_ExtractFilter = ExtractImageFilterType::New();
}

template<class TImage, class TOutputVectorData>
typename PersistentImageToVectorDataFilter<TImage, TOutputVectorData>::DataObjectPointer
PersistentImageToVectorDataFilter<TImage, TOutputVectorData>
::MakeOutput(unsigned int i)
{
  if (i == 1)
    {
    return static_cast<itk::DataObject*>(OutputVectorDataType::New().GetPointer());
    }
  else
    {
    return Superclass::MakeOutput(i);
    }
}

template<class TImage, class TOutputVectorData>
typename PersistentImageToVectorDataFilter<TImage, TOutputVectorData>::OutputVectorDataType *
PersistentImageToVectorDataFilter<TImage, TOutputVectorData>
::GetOutputVectorData() const
{
  return static_cast<OutputVectorDataType*>(const_cast<itk::DataObject*>(this->itk::ProcessObject::GetOutput(1)));
}

template<class TImage, class TOutputVectorData>
void
PersistentImageToVectorDataFilter<TImage, TOutputVectorData>
::AllocateOutputs()
{
  // Nothing that needs to be allocated for the outputs : the output is not meant to be used
  //this->GetOutputVectorData()->Clear();
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

  this->GetOutputVectorData()->GetDataTree()->Add(folder, this->GetOutputVectorData()->GetDataTree()->GetRoot()->Get());
  this->GetOutputVectorData()->GetDataTree()->Add(document , folder);
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
  // make an extract to handle filter which request
  // the largest possible region of their input
  m_ExtractFilter = ExtractImageFilterType::New();
  m_ExtractFilter->SetInput( this->GetInput() );
  m_ExtractFilter->SetExtractionRegion( this->GetInput()->GetBufferedRegion() );
  m_ExtractFilter->Update();
  InputImagePointer image = m_ExtractFilter->GetOutput();

  // call the processing function for this tile
  OutputVectorDataPointerType currentTileVD = this->ProcessTile(image);

  // merge the result into the output vector data object
  OutputVectorDataPointerType output = GetOutputVectorData();

  ConcatenateVectorDataFilterPointerType concatenate = ConcatenateVectorDataFilterType::New();
  concatenate->AddInput(currentTileVD);
  concatenate->AddInput(output);
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
