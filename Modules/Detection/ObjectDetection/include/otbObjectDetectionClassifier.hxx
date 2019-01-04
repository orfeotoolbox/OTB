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

#ifndef otbObjectDetectionClassifier_hxx
#define otbObjectDetectionClassifier_hxx

#include "otbObjectDetectionClassifier.h"

#include "itkContinuousIndex.h"

namespace otb
{

template <class TInputImage, class TOutputVectorData, class TLabel, class TFunctionType>
PersistentObjectDetectionClassifier<TInputImage, TOutputVectorData, TLabel, TFunctionType>
::PersistentObjectDetectionClassifier()
  : m_NeighborhoodRadius(0),
    m_ClassKey("Class"),
    m_NoClassLabel(0),
    m_GridStep(10)
{
  this->SetNumberOfRequiredInputs(1);

  // Have 2 outputs : the image created by Superclass, a vector data with points
  this->SetNumberOfRequiredOutputs(3);
  this->itk::ProcessObject::SetNthOutput(1, this->MakeOutput(1).GetPointer());
  this->itk::ProcessObject::SetNthOutput(2, this->MakeOutput(2).GetPointer());
}

template <class TInputImage, class TOutputVectorData, class TLabel, class TFunctionType>
PersistentObjectDetectionClassifier<TInputImage, TOutputVectorData, TLabel, TFunctionType>
::~PersistentObjectDetectionClassifier()
{
}

template <class TInputImage, class TOutputVectorData, class TLabel, class TFunctionType>
void
PersistentObjectDetectionClassifier<TInputImage, TOutputVectorData, TLabel, TFunctionType>
::AllocateOutputs()
{
  // This is commented to prevent the streaming of the whole image for the first stream strip
  // It shall not cause any problem because the output image of this filter is not intended to be used.
  //InputImagePointer image = const_cast< TInputImage * >( this->GetInput() );
  //this->GraftOutput( image );
  // Nothing that needs to be allocated for the remaining outputs
}

template <class TInputImage, class TOutputVectorData, class TLabel, class TFunctionType>
void
PersistentObjectDetectionClassifier<TInputImage, TOutputVectorData, TLabel, TFunctionType>
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();
  if (this->GetInput())
    {
    this->GetOutput()->CopyInformation(this->GetInput());
    this->GetOutput()->SetLargestPossibleRegion(this->GetInput()->GetLargestPossibleRegion());

    if (this->GetOutput()->GetRequestedRegion().GetNumberOfPixels() == 0)
      {
      this->GetOutput()->SetRequestedRegion(this->GetOutput()->GetLargestPossibleRegion());
      }
    }
}

template <class TInputImage, class TOutputVectorData, class TLabel, class TFunctionType>
void
PersistentObjectDetectionClassifier<TInputImage, TOutputVectorData, TLabel, TFunctionType>
::SetModel(ModelType* model)
{
  if (model != m_Model)
    {
    m_Model = model;
    this->Modified();
    }
}

template <class TInputImage, class TOutputVectorData, class TLabel, class TFunctionType>
const typename PersistentObjectDetectionClassifier<TInputImage, TOutputVectorData, TLabel, TFunctionType>::ModelType*
PersistentObjectDetectionClassifier<TInputImage, TOutputVectorData, TLabel, TFunctionType>
::GetModel(void) const
{
  return m_Model;
}

template <class TInputImage, class TOutputVectorData, class TLabel, class TFunctionType>
typename PersistentObjectDetectionClassifier<TInputImage, TOutputVectorData, TLabel, TFunctionType>::VectorDataType*
PersistentObjectDetectionClassifier<TInputImage, TOutputVectorData, TLabel, TFunctionType>
::GetOutputVectorData()
{
  return static_cast<VectorDataType*>(this->itk::ProcessObject::GetOutput(1));
}

template <class TInputImage, class TOutputVectorData, class TLabel, class TFunctionType>
itk::DataObject::Pointer
PersistentObjectDetectionClassifier<TInputImage, TOutputVectorData, TLabel, TFunctionType>
::MakeOutput(DataObjectPointerArraySizeType idx)
{
  itk::DataObject::Pointer output;
  switch (idx)
    {
    case 0:
      output = static_cast<itk::DataObject*>(InputImageType::New().GetPointer());
      break;
    case 1:
      {
      output = static_cast<itk::DataObject*>(VectorDataType::New().GetPointer());
      break;
      }
    default:
      output = static_cast<itk::DataObject*>(InputImageType::New().GetPointer());
      break;
    }
  return output;
}

template <class TInputImage, class TOutputVectorData, class TLabel, class TFunctionType>
void
PersistentObjectDetectionClassifier<TInputImage, TOutputVectorData, TLabel, TFunctionType>
::Reset()
{
  m_ThreadPointArray = PointArrayContainer(this->GetNumberOfThreads());

}

template <class TInputImage, class TOutputVectorData, class TLabel, class TFunctionType>
void
PersistentObjectDetectionClassifier<TInputImage, TOutputVectorData, TLabel, TFunctionType>
::Synthetize()
{
  // merge all points in a single vector data
  //std::copy(m_ThreadPointArray[0].begin(), m_ThreadPointArray[0].end(),
  //std::ostream_iterator<DescriptorsFunctionPointType>(std::cout, "\n") );

  VectorDataType* vdata = this->GetOutputVectorData();

  // Retrieving root node
  VectorDataNodePointerType root = vdata->GetDataTree()->GetRoot()->Get();
  // Create the document node
  VectorDataNodePointerType document = VectorDataNodeType::New();
  document->SetNodeType(otb::DOCUMENT);
  VectorDataNodePointerType folder = VectorDataNodeType::New();
  folder->SetNodeType(otb::FOLDER);
  // Adding the layer to the data tree
  vdata->GetDataTree()->Add(document, root);
  vdata->GetDataTree()->Add(folder, document);

  for (itk::ThreadIdType threadId = 0; threadId < m_ThreadPointArray.size(); ++threadId)
    {
    PointArray& pointArray = m_ThreadPointArray[threadId];
    typename PointArray::const_iterator it = pointArray.begin();
    typename PointArray::const_iterator end = pointArray.end();

    for (; it != end; ++it)
      {
      VectorDataNodePointerType currentGeometry = VectorDataNodeType::New();
      currentGeometry->SetNodeId("FEATURE_POINT");
      currentGeometry->SetNodeType(otb::FEATURE_POINT);
      VectorDataPointType p;
      p[0] = it->first[0];
      p[1] = it->first[1];
      currentGeometry->SetPoint(p);
      currentGeometry->SetFieldAsInt(m_ClassKey, it->second);
      vdata->GetDataTree()->Add(currentGeometry, folder);
      }
    }
}

template <class TInputImage, class TOutputVectorData, class TLabel, class TFunctionType>
void
PersistentObjectDetectionClassifier<TInputImage, TOutputVectorData, TLabel, TFunctionType>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

template <class TInputImage, class TOutputVectorData, class TLabel, class TFunctionType>
void
PersistentObjectDetectionClassifier<TInputImage, TOutputVectorData, TLabel, TFunctionType>
::GenerateInputRequestedRegion()
{
  Superclass::GenerateInputRequestedRegion();

  // get pointers to the input and output
  typename Superclass::InputImagePointer inputPtr =
    const_cast< TInputImage * >( this->GetInput() );
  typename Superclass::OutputImagePointer outputPtr = this->GetOutput();

  if ( !inputPtr || !outputPtr )
    {
    return;
    }

  // get a copy of the input requested region (should equal the output
  // requested region)
  typename TInputImage::RegionType inputRequestedRegion;
  inputRequestedRegion = inputPtr->GetRequestedRegion();

  // pad the input requested region by the operator radius
  inputRequestedRegion.PadByRadius( m_NeighborhoodRadius + 1 );

  // crop the input requested region at the input's largest possible region
  if ( inputRequestedRegion.Crop(inputPtr->GetLargestPossibleRegion()) )
    {
    inputPtr->SetRequestedRegion( inputRequestedRegion );
    return;
    }
  else
    {
    // Couldn't crop the region (requested region is outside the largest
    // possible region).  Throw an exception.

    // store what we tried to request (prior to trying to crop)
    inputPtr->SetRequestedRegion( inputRequestedRegion );
    }
}

template <class TInputImage, class TOutputVectorData, class TLabel, class TFunctionType>
void
PersistentObjectDetectionClassifier<TInputImage, TOutputVectorData, TLabel, TFunctionType>
::BeforeThreadedGenerateData()
{
  // Compute the 1/(sigma) vector
  m_InvertedScales = m_Scales;
  for(unsigned int idx = 0; idx < m_Scales.Size(); ++idx)
    {
    if(m_Scales[idx]-1e-10 < 0.)
      m_InvertedScales[idx] = 0.;
    else
      m_InvertedScales[idx] = 1 / m_Scales[idx];
    }

}

template <class TInputImage, class TOutputVectorData, class TLabel, class TFunctionType>
void
PersistentObjectDetectionClassifier<TInputImage, TOutputVectorData, TLabel, TFunctionType>
::ThreadedGenerateData(const RegionType& outputRegionForThread,
                       itk::ThreadIdType threadId)
{
  InputImageType* input = static_cast<InputImageType*>(this->itk::ProcessObject::GetInput(0));
  const ModelType*   model = this->GetModel();

  IndexType begin = outputRegionForThread.GetIndex();
  IndexType end = begin;
  end[0] += outputRegionForThread.GetSize(0);
  end[1] += outputRegionForThread.GetSize(1);

  IndexType current = begin;
  for (; current[1] != end[1]; current[1]++)
    {
    if (current[1] % m_GridStep == 0)
      {
      for(current[0] = begin[0]; current[0] != end[0]; current[0]++)
        {
        if (current[0] % m_GridStep == 0)
          {
          DescriptorsFunctionPointType point;
          input->TransformIndexToPhysicalPoint(current, point);

          DescriptorType descriptor = m_DescriptorsFunction->Evaluate(point);
          ModelMeasurementType modelMeasurement(descriptor.GetSize());
          for (unsigned int i = 0; i < descriptor.GetSize(); ++i)
            {
            modelMeasurement[i] = (descriptor[i] - m_Shifts[i]) * m_InvertedScales[i];
            }
          LabelType label = (model->Predict(modelMeasurement))[0];

          if (label != m_NoClassLabel)
            {
            m_ThreadPointArray[threadId].push_back(std::make_pair(point, label));
            }
          }
        }
      }
    }
}


template <class TInputImage, class TOutputVectorData, class TLabel, class TFunctionPrecision, class TCoordRep>
ObjectDetectionClassifier<TInputImage, TOutputVectorData, TLabel, TFunctionPrecision, TCoordRep>
::ObjectDetectionClassifier()
{

}

template <class TInputImage, class TOutputVectorData, class TLabel, class TFunctionPrecision, class TCoordRep>
ObjectDetectionClassifier<TInputImage, TOutputVectorData, TLabel, TFunctionPrecision, TCoordRep>
::~ObjectDetectionClassifier()
{

}


} // end namespace otb

#endif
