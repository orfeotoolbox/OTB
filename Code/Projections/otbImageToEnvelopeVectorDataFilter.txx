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
#ifndef __otbImageToEnvelopeVectorDataFilter_txx
#define __otbImageToEnvelopeVectorDataFilter_txx

#include "otbImageToEnvelopeVectorDataFilter.h"
#include "otbDataNode.h"

namespace otb
{
/**
   * Constructor
 */
template <class TInputImage, class TOutputVectorData>
ImageToEnvelopeVectorDataFilter<TInputImage, TOutputVectorData>
::ImageToEnvelopeVectorDataFilter() : m_SamplingRate(0)
{
  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredOutputs(1);
  m_OutputProjectionRef.clear();

  // Build output
  this->SetNthOutput(0, OutputVectorDataType::New());
}

template <class TInputImage, class TOutputVectorData>
void
ImageToEnvelopeVectorDataFilter<TInputImage, TOutputVectorData>
::SetInput(const InputImageType *input)
{
  // process object is not const-correct, the const_cast
  // is required here.
  this->itk::ProcessObject::SetNthInput(0,
                                       const_cast<InputImageType *>(input));
}

template <class TInputImage, class TOutputVectorData>
const TInputImage *
ImageToEnvelopeVectorDataFilter<TInputImage, TOutputVectorData>
::GetInput(void)
{
  if (this->GetNumberOfInputs() < 1) return 0;

  return dynamic_cast<const InputImageType*>
           (this->itk::ProcessObject::GetInput(0));
}

template <class TInputImage, class TOutputVectorData>
void
ImageToEnvelopeVectorDataFilter<TInputImage, TOutputVectorData>
::GenerateOutputInformation(void)
{
  // Call superclass implementation
  Superclass::GenerateOutputInformation();

  // Ensure transform instantiation
  this->InstantiateTransform();

  // Add projection info to output
  OutputVectorDataPointer  output = this->GetOutput();
  itk::MetaDataDictionary& dict = output->GetMetaDataDictionary();
  itk::EncapsulateMetaData<std::string>(dict, MetaDataKey::ProjectionRefKey, m_Transform->GetOutputProjectionRef());
}

template <class TInputImage, class TOutputVectorData>
void
ImageToEnvelopeVectorDataFilter<TInputImage, TOutputVectorData>
::GenerateInputRequestedRegion(void)
{
  // Call superclass implementation
  Superclass::GenerateInputRequestedRegion();

  // Retrieve input image pointer
  typename InputImageType::Pointer inputPtr = const_cast<TInputImage *>(this->GetInput());

  typename InputImageType::RegionType requestedRegion = inputPtr->GetRequestedRegion();
  typename InputImageType::SizeType   size = requestedRegion.GetSize();
  size.Fill(0);
  requestedRegion.SetSize(size);

  typename InputImageType::IndexType   index = requestedRegion.GetIndex();
  index.Fill(0);
  requestedRegion.SetIndex(index);

  inputPtr->SetRequestedRegion(requestedRegion);
}

template <class TInputImage, class TOutputVectorData>
void
ImageToEnvelopeVectorDataFilter<TInputImage, TOutputVectorData>
::InstantiateTransform(void)
{
  // Project into output projection
  typename InputImageType::ConstPointer inputPtr = this->GetInput();
  m_Transform = InternalTransformType::New();
  m_Transform->SetOutputProjectionRef(m_OutputProjectionRef);
  m_Transform->SetInputProjectionRef(inputPtr->GetProjectionRef());
  m_Transform->SetInputKeywordList(inputPtr->GetImageKeywordlist());
  m_Transform->InstanciateTransform();
}


template <class TInputImage, class TOutputVectorData>
void
ImageToEnvelopeVectorDataFilter<TInputImage, TOutputVectorData>
::GenerateData(void)
{
  // Retrieve input and output pointers
  typename InputImageType::ConstPointer inputPtr = this->GetInput();
  OutputVectorDataPointer outputPtr = this->GetOutput();

  // Compute corners as index
  typename InputImageType::IndexType ul, ur, lr, ll;
  ul = inputPtr->GetLargestPossibleRegion().GetIndex();
  ur = ul;
  ll=ul;
  lr=ul;
  typename InputImageType::SizeType size = inputPtr->GetLargestPossibleRegion().GetSize();
  ur[0]+=size[0];
  ll[1]+=size[1];
  lr[0]+=size[0];
  lr[1]+=size[1];

  // Get corners as physical points
  typename InputImageType::PointType ulp, urp, lrp, llp, current;
  inputPtr->TransformIndexToPhysicalPoint(ul, ulp);
  inputPtr->TransformIndexToPhysicalPoint(ur, urp);
  inputPtr->TransformIndexToPhysicalPoint(lr, lrp);
  inputPtr->TransformIndexToPhysicalPoint(ll, llp);

  this->InstantiateTransform();
  
  typename InputImageType::IndexType edgeIndex;
  typename InputImageType::PointType edgePoint;

  // Build envelope polygon
  typename PolygonType::Pointer envelope = PolygonType::New();
  typename PolygonType::VertexType vertex;
  current = m_Transform->TransformPoint(ulp);
  vertex[0] = current[0];
  vertex[1] = current[1];
  envelope->AddVertex(vertex);
  
  if (m_SamplingRate>0)
    {
    edgeIndex = ul;
    edgeIndex[0]+=m_SamplingRate;
    while (edgeIndex[0]<ur[0])
      {
      inputPtr->TransformIndexToPhysicalPoint(edgeIndex, edgePoint);
      current = m_Transform->TransformPoint(edgePoint);
      vertex[0] = current[0];
      vertex[1] = current[1];
      envelope->AddVertex(vertex);
      edgeIndex[0]+=m_SamplingRate;
      }
    }
  
  current = m_Transform->TransformPoint(urp);
  vertex[0] = current[0];
  vertex[1] = current[1];
  envelope->AddVertex(vertex);
  
  if (m_SamplingRate>0)
    {
    edgeIndex = ur;
    edgeIndex[1]+=m_SamplingRate;
    while (edgeIndex[1]<lr[1])
      {
      inputPtr->TransformIndexToPhysicalPoint(edgeIndex, edgePoint);
      current = m_Transform->TransformPoint(edgePoint);
      vertex[0] = current[0];
      vertex[1] = current[1];
      envelope->AddVertex(vertex);
      edgeIndex[1]+=m_SamplingRate;
      }
    }
  
  current = m_Transform->TransformPoint(lrp);
  vertex[0] = current[0];
  vertex[1] = current[1];
  envelope->AddVertex(vertex);
  
  if (m_SamplingRate>0)
    {
    edgeIndex = lr;
    edgeIndex[0]-=m_SamplingRate;
    while (edgeIndex[0]>ll[0])
      {
      inputPtr->TransformIndexToPhysicalPoint(edgeIndex, edgePoint);
      current = m_Transform->TransformPoint(edgePoint);
      vertex[0] = current[0];
      vertex[1] = current[1];
      envelope->AddVertex(vertex);
      edgeIndex[0]-=m_SamplingRate;
      }
    }
  
  current = m_Transform->TransformPoint(llp);
  vertex[0] = current[0];
  vertex[1] = current[1];
  envelope->AddVertex(vertex);
  
  if (m_SamplingRate>0)
    {
    edgeIndex = ll;
    edgeIndex[1]-=m_SamplingRate;
    while (edgeIndex[1]>ul[1])
      {
      inputPtr->TransformIndexToPhysicalPoint(edgeIndex, edgePoint);
      current = m_Transform->TransformPoint(edgePoint);
      vertex[0] = current[0];
      vertex[1] = current[1];
      envelope->AddVertex(vertex);
      edgeIndex[1]-=m_SamplingRate;
      }
    }

  // Add polygon to the VectorData tree
  OutputDataTreePointerType tree = outputPtr->GetDataTree();

  // Create the output tree root
  OutputDataNodePointerType root = tree->GetRoot()->Get();

  OutputDataNodePointerType document = OutputDataNodeType::New();
  document->SetNodeType(DOCUMENT);
  tree->Add(document, root);

  OutputDataNodePointerType newDataNode = OutputDataNodeType::New();
  newDataNode->SetPolygonExteriorRing(envelope);

  tree->Add(newDataNode, document);
}

} // end namespace otb

#endif
