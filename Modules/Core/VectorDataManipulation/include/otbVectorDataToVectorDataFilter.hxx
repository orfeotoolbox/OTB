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
VectorDataToVectorDataFilter<TInputVectorData, TOutputVectorData>::VectorDataToVectorDataFilter()
{
  this->SetNumberOfRequiredInputs(1);
}

template <class TInputVectorData, class TOutputVectorData>
void VectorDataToVectorDataFilter<TInputVectorData, TOutputVectorData>::SetInput(const InputVectorDataType* input)
{
  // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(0, const_cast<InputVectorDataType*>(input));
}

template <class TInputVectorData, class TOutputVectorData>
typename VectorDataToVectorDataFilter<TInputVectorData, TOutputVectorData>::InputVectorDataType*
VectorDataToVectorDataFilter<TInputVectorData, TOutputVectorData>::GetInput(void)
{
  if (this->GetNumberOfInputs() < 1)
  {
    return nullptr;
  }

  return static_cast<TInputVectorData*>(this->itk::ProcessObject::GetInput(0));
}

template <class TInputVectorData, class TOutputVectorData>
void VectorDataToVectorDataFilter<TInputVectorData, TOutputVectorData>::GenerateOutputInformation(void)
{
  Superclass::GenerateOutputInformation();

  OutputVectorDataPointer                    output = this->GetOutput();
  typename InputVectorDataType::ConstPointer input  = this->GetInput();
  output->SetMetaDataDictionary(input->GetMetaDataDictionary());
}

/**
   * GenerateData Performs the coordinate conversion for each element in the tree
 */
template <class TInputVectorData, class TOutputVectorData>
void VectorDataToVectorDataFilter<TInputVectorData, TOutputVectorData>::GenerateData(void)
{
  this->AllocateOutputs();
  InputVectorDataPointer  inputPtr  = this->GetInput();
  OutputVectorDataPointer outputPtr = this->GetOutput();

  // Get the input tree root
  typename InputDataNodeType::Pointer inputRoot = inputPtr->GetRoot();

  // // Create the output tree root
  // typedef typename OutputVectorDataType::DataNodePointerType OutputDataNodePointerType;
  // OutputDataNodePointerType outputRoot = OutputDataNodeType::New();
  // outputRoot->SetNodeType(inputRoot->GetNodeType());
  // outputRoot->SetNodeId(inputRoot->GetNodeId());
  // outputPtr->SetRoot(outputRoot);

  // Start recursive processing
  otb::Stopwatch chrono = otb::Stopwatch::StartNew();
  this->ProcessNode(inputPtr,inputRoot,outputPtr,outputPtr->GetRoot());
  chrono.Stop();
  otbMsgDevMacro(<< "VectoDataProjectionFilter: features processed in " << chrono.GetElapsedMilliseconds() << " ms.");
}

template <class TInputVectorData, class TOutputVectorData>
void VectorDataToVectorDataFilter<TInputVectorData, TOutputVectorData>::ProcessNode(InputVectorDataPointer inputVdata,
                                                                                    InputDataNodePointerType source,
                                                                                    OutputVectorDataPointer outputVdata,
                                                                                    OutputDataNodePointerType destination) const
{
  // Get the children list from the input node
  typedef typename InputVectorDataType::ChildrenListType InputChildrenListType;
  InputChildrenListType children = inputVdata->GetChildrenList(source);

  // For each child
  typename InputChildrenListType::const_iterator it = children.begin();
  while (it != children.end())
  {
    // Copy input DataNode info
    OutputDataNodePointerType newDataNode = OutputDataNodeType::New();
    newDataNode->SetNodeType((*it)->GetNodeType());
    newDataNode->SetNodeId((*it)->GetNodeId());
    newDataNode->SetMetaDataDictionary((*it)->GetMetaDataDictionary());

    switch ((*it)->GetNodeType())
    {
    case ROOT:
    {
      //outputVdata->Add(newDataNode,destination);
      //ProcessNode(inputVdata,(*it),outputVdata,newDataNode);
      break;
    }
    case DOCUMENT:
    {
      outputVdata->Add(newDataNode,destination);
      ProcessNode(inputVdata,(*it),outputVdata,newDataNode);
      break;
    }
    case FOLDER:
    {
      outputVdata->Add(newDataNode,destination);
      ProcessNode(inputVdata,(*it),outputVdata,newDataNode);
      break;
    }
    case FEATURE_POINT:
    {
      newDataNode->SetPoint(this->ProcessPoint((*it)->GetPoint()));
      outputVdata->Add(newDataNode,destination);
      break;
    }
    case FEATURE_LINE:
    {
      newDataNode->SetLine(this->ProcessLine((*it)->GetLine()));
      outputVdata->Add(newDataNode,destination);
      break;
    }
    case FEATURE_POLYGON:
    {
      newDataNode->SetPolygonExteriorRing(this->ProcessPolygon((*it)->GetPolygonExteriorRing()));
      newDataNode->SetPolygonInteriorRings(this->ProcessPolygonList((*it)->GetPolygonInteriorRings()));
      outputVdata->Add(newDataNode,destination);
      break;
    }
    case FEATURE_MULTIPOINT:
    {
      outputVdata->Add(newDataNode,destination);
      ProcessNode(inputVdata,(*it),outputVdata,newDataNode);
      break;
    }
    case FEATURE_MULTILINE:
    {
      outputVdata->Add(newDataNode,destination);
      ProcessNode(inputVdata,(*it),outputVdata,newDataNode);
      break;
    }
    case FEATURE_MULTIPOLYGON:
    {
      outputVdata->Add(newDataNode,destination);
      ProcessNode(inputVdata,(*it),outputVdata,newDataNode);
      break;
    }
    case FEATURE_COLLECTION:
    {
      outputVdata->Add(newDataNode,destination);
      ProcessNode(inputVdata,(*it),outputVdata,newDataNode);
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
void VectorDataToVectorDataFilter<TInputVectorData, TOutputVectorData>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
