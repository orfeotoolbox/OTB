/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbConcatenateVectorDataFilter_txx
#define __otbConcatenateVectorDataFilter_txx

#include "otbConcatenateVectorDataFilter.h"

#include "otbMath.h"

namespace otb
{
template<class TVectorData>
ConcatenateVectorDataFilter<TVectorData>
::ConcatenateVectorDataFilter()
{
  this->SetNumberOfRequiredInputs(2);
  this->SetNumberOfRequiredOutputs(1);

  m_Folder        = DataNodeType::New();
  m_Folder->SetNodeType(otb::FOLDER);

  m_Document        = DataNodeType::New();
  m_Document->SetNodeType(otb::DOCUMENT);

}

template <class TVectorData>
void
ConcatenateVectorDataFilter<TVectorData>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
}

template <class TVectorData>
void
ConcatenateVectorDataFilter<TVectorData>
::AddInput(const VectorDataType * vectorData)
{
  this->Superclass::SetNthInput(this->GetNumberOfInputs(),
                                const_cast<VectorDataType *>(vectorData));
}

template <class TVectorData>
const TVectorData *
ConcatenateVectorDataFilter<TVectorData>
::GetInput(unsigned int idx) const
{
  if (this->GetNumberOfInputs() < idx)
    {
    return 0;
    }
  return static_cast<const VectorDataType *>(this->Superclass::GetInput(idx));
}

template <class TVectorData>
void
ConcatenateVectorDataFilter<TVectorData>
::GenerateData()
{
  // TODO : no checking is done on the inputs, do checking to avoid
  // TODO : Check if they are in the same coordinate system (tricky)
  
  // Start recursive processing

  // Copy the input MetaDataDictionary in the output VectorData
  //this->GetOutput()->SetMetaDataDictionary(this->GetInput(0)->GetMetaDataDictionary());
  
  // Prepare the output
  typename DataNodeType::Pointer outputRoot = this->GetOutput()->GetDataTree()->GetRoot()->Get();
  
  // Adding the layer to the data tree
  this->GetOutput()->GetDataTree()->Add(m_Document, outputRoot);
  this->GetOutput()->GetDataTree()->Add(m_Folder, m_Document);
  
  // Retrieve all the inputs
  for(unsigned int idx = 0; idx < this->GetNumberOfInputs(); ++idx)
    {
    // Add the current vectordata
    TreeNodeType *
      inputRoot = const_cast<TreeNodeType *>(this->GetInput(idx)->GetDataTree()->GetRoot());
    //
    ProcessNode(inputRoot);
    }

}


template <class TVectorData>
void
ConcatenateVectorDataFilter<TVectorData>
::ProcessNode(TreeNodeType * source)
{
  if (source == 0)
    return;

  // Get the children list from the input node
  ChildrenListType children = source->GetChildrenList();

  // For each child
  typename ChildrenListType::iterator it;
  for (it = children.begin(); it != children.end(); ++it)
    {
    // get the data node
    DataNodePointerType  dataNode = (*it)->Get();
    
    switch (dataNode->GetNodeType())
      {
      case ROOT:
        {
        ProcessNode((*it));
        break;
        }
      case DOCUMENT:
        {
        ProcessNode((*it));
        break;
        }
      case FOLDER:
        {
        ProcessNode((*it));
        break;
        }
      case FEATURE_POINT:
        {
        this->GetOutput()->GetDataTree()->Add(dataNode, m_Document);
        break;
        }
      case FEATURE_LINE:
        {
        this->GetOutput()->GetDataTree()->Add(dataNode, m_Document);
        break;
        }
      case FEATURE_POLYGON:
        {
        this->GetOutput()->GetDataTree()->Add(dataNode, m_Document);
        break;
        }
    case FEATURE_MULTIPOINT:
        {
        ProcessNode((*it));
        break;
        }
      case FEATURE_MULTILINE:
        {
        ProcessNode((*it));
        break;
        }
      case FEATURE_MULTIPOLYGON:
        {
        ProcessNode((*it));
        break;
        }
      case FEATURE_COLLECTION:
        {
        ProcessNode((*it));
        break;
        }
      }
    }
}


} // end namespace otb

#endif
