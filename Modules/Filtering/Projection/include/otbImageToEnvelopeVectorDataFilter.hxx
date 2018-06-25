/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbImageToEnvelopeVectorDataFilter_hxx
#define otbImageToEnvelopeVectorDataFilter_hxx

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
  if (this->GetNumberOfInputs() < 1) return nullptr;

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
  m_Transform->InstantiateTransform();
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
  itk::ContinuousIndex<double,2> ul(
    inputPtr->GetLargestPossibleRegion().GetIndex());
  // move 'ul' to the corner of the first pixel
  ul[0] += -0.5;
  ul[1] += -0.5;

  itk::ContinuousIndex<double,2> ur(ul);
  itk::ContinuousIndex<double,2> lr(ul);
  itk::ContinuousIndex<double,2> ll(ul);

  typename InputImageType::SizeType size = inputPtr->GetLargestPossibleRegion().GetSize();
  ur[0]+=size[0];
  ll[1]+=size[1];
  lr[0]+=size[0];
  lr[1]+=size[1];

  // Get corners as physical points
  typename InputImageType::PointType ulp, urp, lrp, llp, current;
  inputPtr->TransformContinuousIndexToPhysicalPoint(ul, ulp);
  inputPtr->TransformContinuousIndexToPhysicalPoint(ur, urp);
  inputPtr->TransformContinuousIndexToPhysicalPoint(lr, lrp);
  inputPtr->TransformContinuousIndexToPhysicalPoint(ll, llp);

  this->InstantiateTransform();

  itk::ContinuousIndex<double,2> edgeIndex;
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
      inputPtr->TransformContinuousIndexToPhysicalPoint(edgeIndex, edgePoint);
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
      inputPtr->TransformContinuousIndexToPhysicalPoint(edgeIndex, edgePoint);
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
      inputPtr->TransformContinuousIndexToPhysicalPoint(edgeIndex, edgePoint);
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
      inputPtr->TransformContinuousIndexToPhysicalPoint(edgeIndex, edgePoint);
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
