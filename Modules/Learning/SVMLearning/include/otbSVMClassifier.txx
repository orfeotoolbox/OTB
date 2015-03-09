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
#ifndef __otbSVMClassifier_txx
#define __otbSVMClassifier_txx

#include "otbSVMClassifier.h"
#include "otbMacro.h"

namespace otb
{

template<class TSample, class TLabel>
SVMClassifier<TSample, TLabel>
::SVMClassifier()
{
  m_Output = OutputType::New();
  m_HyperplanesDistancesOutput = HyperplanesDistancesListSampleType::New();
}

template<class TSample, class TLabel>
void
SVMClassifier<TSample, TLabel>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

template<class TSample, class TLabel>
void
SVMClassifier<TSample, TLabel>
::Update()
{
  this->GenerateData();
}

template<class TSample, class TLabel>
void
SVMClassifier<TSample, TLabel>
::GenerateData()
{
  if (!m_Model)
    {
    itkExceptionMacro("No model, can not do classification.");
    }

  if (m_Model->GetNumberOfSupportVectors() == 0)
    {
    itkExceptionMacro(<< "SVM model does not contain any support vector, can not perform classification.");
    }

  m_Output->SetSample(this->GetInput());
//  m_Output->Resize(this->GetSample()->Size());
  //FIXME check if this is necessary (work at the decision rule
  //Evaluate() level?)

  unsigned int numberOfClasses = this->GetNumberOfClasses();

  m_Output->SetNumberOfClasses(numberOfClasses);

  this->DoClassification();
}

template<class TSample, class TLabel>
typename SVMClassifier<TSample, TLabel>::OutputType*
SVMClassifier<TSample, TLabel>
::GetOutput()
{
  return m_Output;
}

template<class TSample, class TLabel>
void
SVMClassifier<TSample, TLabel>
::SetOutput(OutputType * output)
{
  m_Output = output;
}

template<class TSample, class TLabel>
typename SVMClassifier<TSample, TLabel>::HyperplanesDistancesListSampleType *
SVMClassifier<TSample, TLabel>
::GetHyperplanesDistancesOutput()
{
  return m_HyperplanesDistancesOutput;
}

template<class TSample, class TLabel>
void
SVMClassifier<TSample, TLabel>
::DoClassification()
{
  itk::TimeProbe probe;

  typename TSample::ConstIterator iter = this->GetInput()->Begin();
  typename TSample::ConstIterator end  = this->GetInput()->End();

  typename OutputType::ConstIterator      iterO = m_Output->Begin();
  typename OutputType::ConstIterator      endO  = m_Output->End();
  typename TSample::MeasurementVectorType measurements;

  // sample Measurement vector size
  int numberOfComponentsPerSample  = iter.GetMeasurementVector().Size();

  // The size of the hyperplane distance vector is the number of hyperplanes
  m_HyperplanesDistancesOutput->SetMeasurementVectorSize(m_Model->GetNumberOfHyperplane());

  otbMsgDevMacro(<< "Starting iterations ");
  while (iter != end && iterO != endO)
    {
    int                                    i = 0;
    typename SVMModelType::MeasurementType modelMeasurement;

    measurements = iter.GetMeasurementVector();
    // otbMsgDevMacro(  << "Loop on components " << svm_type );
    for (i = 0; i < numberOfComponentsPerSample; ++i)
      {
      modelMeasurement.push_back(measurements[i]);
      }

    ClassLabelType classLabel = m_Model->EvaluateLabel(modelMeasurement);
    typename SVMModelType::DistancesVectorType hdistances = m_Model->EvaluateHyperplanesDistances(modelMeasurement);
    HyperplanesDistancesType distances = hdistances;
    m_HyperplanesDistancesOutput->PushBack(distances);
    // Julien: Event if we support larger type for class labels,
    // the AddInstance method wait for an unsigned int, so we cast it here.
    m_Output->AddInstance(static_cast<unsigned int>(classLabel), iterO.GetInstanceIdentifier());

    ++iter;
    ++iterO;
    }
}

} // end of namespace otb

#endif
