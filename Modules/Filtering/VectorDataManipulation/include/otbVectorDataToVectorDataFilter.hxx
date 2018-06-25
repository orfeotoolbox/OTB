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

#ifndef otbVectorDataToVectorDataFilter_hxx
#define otbVectorDataToVectorDataFilter_hxx

#include "otbVectorDataToVectorDataFilter.h"
#include "itkProgressReporter.h"
#include "otbDataNode.h"
#include "otbStopwatch.h"

namespace otb
{

/**
   * Constructor
 */
template <class TInputVectorData, class TOutputVectorData>
VectorDataToVectorDataFilter<TInputVectorData, TOutputVectorData>
::VectorDataToVectorDataFilter()
{
  this->SetNumberOfRequiredInputs(1);
}

template <class TInputVectorData, class TOutputVectorData>
void
VectorDataToVectorDataFilter<TInputVectorData, TOutputVectorData>
::SetInput(const InputVectorDataType *input)
{
// Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(0,
                                        const_cast<InputVectorDataType *>(input));
}

template <class TInputVectorData, class TOutputVectorData>
const typename VectorDataToVectorDataFilter<TInputVectorData, TOutputVectorData>::InputVectorDataType *
VectorDataToVectorDataFilter<TInputVectorData, TOutputVectorData>
::GetInput(void)
{
  if (this->GetNumberOfInputs() < 1)
    {
    return nullptr;
    }

  return static_cast<const TInputVectorData *>
           (this->itk::ProcessObject::GetInput(0));
}

template <class TInputVectorData, class TOutputVectorData>
void
VectorDataToVectorDataFilter<TInputVectorData, TOutputVectorData>
::GenerateOutputInformation(void)
{
  Superclass::GenerateOutputInformation();

  OutputVectorDataPointer  output = this->GetOutput();
  typename InputVectorDataType::ConstPointer input = this->GetInput();
  output->SetMetaDataDictionary(input->GetMetaDataDictionary());

}

/**
   * GenerateData Performs the coordinate conversion for each element in the tree
 */
template <class TInputVectorData, class TOutputVectorData>
void
VectorDataToVectorDataFilter<TInputVectorData, TOutputVectorData>
::GenerateData(void)
{
  this->AllocateOutputs();
  InputVectorDataPointer  inputPtr = this->GetInput();
  OutputVectorDataPointer outputPtr = this->GetOutput();

  typedef typename OutputVectorDataType::DataTreePointerType OutputDataTreePointerType;
  OutputDataTreePointerType tree = outputPtr->GetDataTree();

  // Get the input tree root
  InputInternalTreeNodeType * inputRoot = const_cast<InputInternalTreeNodeType *>(inputPtr->GetDataTree()->GetRoot());

  // Create the output tree root
  typedef typename OutputVectorDataType::DataNodePointerType OutputDataNodePointerType;
  OutputDataNodePointerType newDataNode = OutputDataNodeType::New();
  newDataNode->SetNodeType(inputRoot->Get()->GetNodeType());
  newDataNode->SetNodeId(inputRoot->Get()->GetNodeId());
  typename OutputInternalTreeNodeType::Pointer outputRoot = OutputInternalTreeNodeType::New();
  outputRoot->Set(newDataNode);
  tree->SetRoot(outputRoot);

  // Start recursive processing
  otb::Stopwatch chrono = otb::Stopwatch::StartNew();
  this->ProcessNode(inputRoot, outputRoot);
  chrono.Stop();
  otbMsgDevMacro(<< "VectoDataProjectionFilter: features processed in " << chrono.GetElapsedMilliseconds() << " ms.");
}

template <class TInputVectorData, class TOutputVectorData>
void
VectorDataToVectorDataFilter<TInputVectorData, TOutputVectorData>
::ProcessNode(InputInternalTreeNodeType * source, OutputInternalTreeNodeType * destination) const
{
  // Get the children list from the input node
  typedef typename InputInternalTreeNodeType::ChildrenListType      InputChildrenListType;
  InputChildrenListType children = source->GetChildrenList();

  // For each child
  typename InputChildrenListType::const_iterator it = children.begin();
  while (it != children.end())
    {
    typename OutputInternalTreeNodeType::Pointer newContainer;
    typedef typename InputVectorDataType::DataNodePointerType  InputDataNodePointerType;
    typedef typename OutputVectorDataType::DataNodePointerType OutputDataNodePointerType;
    // Copy input DataNode info
    InputDataNodePointerType  dataNode = (*it)->Get();
    OutputDataNodePointerType newDataNode   = OutputDataNodeType::New();
    newDataNode->SetNodeType(dataNode->GetNodeType());
    newDataNode->SetNodeId(dataNode->GetNodeId());
    newDataNode->SetMetaDataDictionary(dataNode->GetMetaDataDictionary());

    switch (dataNode->GetNodeType())
      {
      case ROOT:
        {
        newContainer = OutputInternalTreeNodeType::New();
        newContainer->Set(newDataNode);
        destination->AddChild(newContainer);
        ProcessNode((*it), newContainer);
        break;
        }
      case DOCUMENT:
        {
        newContainer = OutputInternalTreeNodeType::New();
        newContainer->Set(newDataNode);
        destination->AddChild(newContainer);
        ProcessNode((*it), newContainer);
        break;
        }
      case FOLDER:
        {
        newContainer = OutputInternalTreeNodeType::New();
        newContainer->Set(newDataNode);
        destination->AddChild(newContainer);
        ProcessNode((*it), newContainer);
        break;
        }
      case FEATURE_POINT:
        {
        newDataNode->SetPoint(this->ProcessPoint(dataNode->GetPoint()));
        newContainer = OutputInternalTreeNodeType::New();
        newContainer->Set(newDataNode);
        destination->AddChild(newContainer);
        break;
        }
      case FEATURE_LINE:
        {
        newDataNode->SetLine(this->ProcessLine(dataNode->GetLine()));
        newContainer = OutputInternalTreeNodeType::New();
        newContainer->Set(newDataNode);
        destination->AddChild(newContainer);
        break;
        }
      case FEATURE_POLYGON:
        {
        newDataNode->SetPolygonExteriorRing(this->ProcessPolygon(dataNode->GetPolygonExteriorRing()));
        newDataNode->SetPolygonInteriorRings(this->ProcessPolygonList(dataNode->GetPolygonInteriorRings()));
        newContainer = OutputInternalTreeNodeType::New();
        newContainer->Set(newDataNode);
        destination->AddChild(newContainer);
        break;
        }
      case FEATURE_MULTIPOINT:
        {
        newContainer = OutputInternalTreeNodeType::New();
        newContainer->Set(newDataNode);
        destination->AddChild(newContainer);
        ProcessNode((*it), newContainer);
        break;
        }
      case FEATURE_MULTILINE:
        {
        newContainer = OutputInternalTreeNodeType::New();
        newContainer->Set(newDataNode);
        destination->AddChild(newContainer);
        ProcessNode((*it), newContainer);
        break;
        }
      case FEATURE_MULTIPOLYGON:
        {
        newContainer = OutputInternalTreeNodeType::New();
        newContainer->Set(newDataNode);
        destination->AddChild(newContainer);
        ProcessNode((*it), newContainer);
        break;
        }
      case FEATURE_COLLECTION:
        {
        newContainer = OutputInternalTreeNodeType::New();
        newContainer->Set(newDataNode);
        destination->AddChild(newContainer);
        ProcessNode((*it), newContainer);
        break;
        }
      }
    ++it;
    }
}

/**
   * PrintSelf Method
 */
template <class TInputVectorData, class TOutputVectorData>
void
VectorDataToVectorDataFilter<TInputVectorData, TOutputVectorData>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
