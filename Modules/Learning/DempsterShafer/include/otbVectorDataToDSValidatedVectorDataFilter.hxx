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

#ifndef otbVectorDataToDSValidatedVectorDataFilter_hxx
#define otbVectorDataToDSValidatedVectorDataFilter_hxx

#include "otbVectorDataToDSValidatedVectorDataFilter.h"
#include <iostream>

namespace otb
{

// Constructor
template <class TVectorData, class TPrecision>
  VectorDataToDSValidatedVectorDataFilter<TVectorData, TPrecision>
::VectorDataToDSValidatedVectorDataFilter() :
  m_Bel(0.0),
  m_Plau(0.0),
  m_CriterionFormula("((Belief + Plausibility)/2.)"),
  m_CriterionThreshold(0.5),
  m_CurrentID(0)
{
  this->SetNumberOfRequiredInputs(1);

  m_Parser =  ParserType::New();
}


template <class TVectorData, class TPrecision>
void
VectorDataToDSValidatedVectorDataFilter<TVectorData, TPrecision>
::AddDescriptor(const std::string& key, std::vector<double>  model)
{
  bool alreadyExist = false;

  if (model.size() != 4)
    {
     itkExceptionMacro(<< "Wrong model! Size(=" << model.size() << ") > 4" )
    }
  else if (model[0]<0 || model[1]<model[0] || model[2]<model[1] || model[2]>1)
    {
     itkExceptionMacro(<< "Wrong model! Values have to be 0<=v1<=v2<=v3<=1" )
    }
  else if (model[3]<.75 || model[3]>1.0)
    {
      itkExceptionMacro(<< "Wrong model! Values have to be 0.75<=v4<=1" )
    }
  else
    {
    for (unsigned int i=0; i<m_DescriptorModels.size(); ++i)
      {
      if (m_DescriptorModels[i].first.compare(key) == 0)
        {
        for (unsigned int j=0; j<4; ++j)
          {
           m_DescriptorModels[i].second.at(j) = model[j];
          }
        alreadyExist = true;
        }
      }
    if (!alreadyExist)
      {
      m_DescriptorModels.push_back(std::pair<std::string, std::vector<double> >(key, model));
      }
    }

  this->Modified();
}


template <class TVectorData, class TPrecision>
void
VectorDataToDSValidatedVectorDataFilter<TVectorData, TPrecision>
::ClearDescriptors()
{
  m_DescriptorModels.clear();
}

template <class TVectorData, class TPrecision>
void
VectorDataToDSValidatedVectorDataFilter<TVectorData, TPrecision>
::SetDescriptorModels(DescriptorModelsType model)
{
  for (unsigned int i=0; i<model.size(); ++i)
    {
     this->AddDescriptor(model[i].first, model[i].second);
    }
}


template <class TVectorData, class TPrecision>
void
VectorDataToDSValidatedVectorDataFilter<TVectorData, TPrecision>
::GenerateData()
{
  //Initialize Dempster Shafer tools
  if( m_DescriptorModels.size() == 0 )
    {
    itkExceptionMacro(<< "No defined Descriptor. Please add a descriptor model.");
    }

  for (unsigned int i=0; i<m_DescriptorModels.size(); ++i)
    {
    typename FuzzyVarType::Pointer fuz = FuzzyVarType::New();
    std::string fuzName, fuzName_;
    fuzName  = m_DescriptorModels[i].first;
    fuzName_ = m_DescriptorModels[i].first + "_";

    fuz->SetMembership(fuzName,
                       0.0,
                       0.0,
                       m_DescriptorModels[i].second[0],
                       m_DescriptorModels[i].second[1],
                       0.0,
                       m_DescriptorModels[i].second[3]);
    fuz->SetMembership(fuzName_,
                       m_DescriptorModels[i].second[1],
                       m_DescriptorModels[i].second[2],
                       1.0,
                       1.0,
                       0.0,
                       m_DescriptorModels[i].second[3]);

    m_FuzzyVars.push_back(fuz);

    m_Universe.insert(fuzName);
    m_Universe.insert(fuzName_);
    }

  //Initialize parser
  m_Parser->SetExpr(m_CriterionFormula);
  m_Parser->DefineVar("Belief", &m_Bel);
  m_Parser->DefineVar("Plausibility", &m_Plau);

  // Output
  this->GetOutput(0)->SetMetaDataDictionary(this->GetInput()->GetMetaDataDictionary());
  // Retrieving root node
  typename DataNodeType::Pointer root = this->GetOutput(0)->GetDataTree()->GetRoot()->Get();
  // Create the document node
  typename DataNodeType::Pointer document = DataNodeType::New();
  document->SetNodeType(otb::DOCUMENT);
  // Adding the layer to the data tree
  this->GetOutput(0)->GetDataTree()->Add(document, root);
  // Create the folder node
  typename DataNodeType::Pointer folder = DataNodeType::New();
  folder->SetNodeType(otb::FOLDER);
  // Adding the layer to the data tree
  this->GetOutput(0)->GetDataTree()->Add(folder, document);
  this->GetOutput(0)->SetProjectionRef(this->GetInput()->GetProjectionRef());

  TreeIteratorType itVector(this->GetInput()->GetDataTree());
  itVector.GoToBegin();
  while (!itVector.IsAtEnd())
    {
    if (!itVector.Get()->IsRoot() && !itVector.Get()->IsDocument() && !itVector.Get()->IsFolder())
      {
      typename DataNodeType::Pointer currentGeometry = itVector.Get();
      typename JointMassOfBeliefFilterType::Pointer jointMassFilter = JointMassOfBeliefFilterType::New();

      for (unsigned int i=0; i<m_DescriptorModels.size(); ++i)
        {
        if (currentGeometry->HasField(m_DescriptorModels[i].first))
          {
          LabelSetType H, H_;
          std::string fuzName, fuzName_;
          fuzName  = m_DescriptorModels[i].first;
          fuzName_ = m_DescriptorModels[i].first + "_";

          typename MassOfBeliefFunctionType::Pointer mass = MassOfBeliefFunctionType::New();
          mass->InitializePowerSetMasses(m_Universe);

          H.insert(fuzName);
          H_.insert(fuzName_);

          mass->SetMass(H, m_FuzzyVars[i]->GetMembership(fuzName, currentGeometry->GetFieldAsDouble(fuzName)));
          mass->SetMass(H_, m_FuzzyVars[i]->GetMembership(fuzName_, currentGeometry->GetFieldAsDouble(fuzName)));

          mass->EstimateUncertainty();

          jointMassFilter->PushBackInput(mass);
          }
        }
      jointMassFilter->Update();
      m_Bel  = jointMassFilter->GetOutput()->GetBelief(m_BeliefHypothesis);
      m_Plau = jointMassFilter->GetOutput()->GetPlausibility(m_PlausibilityHypothesis);

     if (m_Parser->Eval() >= m_CriterionThreshold)
        {
        currentGeometry->SetNodeId(this->GetNextID());
        currentGeometry->SetFieldAsDouble("Belief", m_Bel);
        currentGeometry->SetFieldAsDouble("Plausi", m_Plau);
        this->GetOutput(0)->GetDataTree()->Add(currentGeometry, folder);
        }
      }
    ++itVector;
    }
}


// PrintSelf Method
template <class TVectorData, class TPrecision>
void
VectorDataToDSValidatedVectorDataFilter<TVectorData, TPrecision>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
