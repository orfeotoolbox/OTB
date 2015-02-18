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
#ifndef __otbSVMMarginSampler_txx
#define __otbSVMMarginSampler_txx

#include "otbSVMMarginSampler.h"
#include "otbMacro.h"

namespace otb
{

template< class TSample, class TModel >
SVMMarginSampler< TSample, TModel >
::SVMMarginSampler()
{
  m_NumberOfCandidates = 10;
}

template< class TSample, class TModel >
void
SVMMarginSampler< TSample, TModel >
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

template< class TSample, class TModel >
void
SVMMarginSampler< TSample, TModel >
::GenerateData()
{
  if(!m_Model)
    {
    itkExceptionMacro("No model, can not do classification.");
    }

  if(m_Model->GetNumberOfSupportVectors() == 0)
    {
    itkExceptionMacro(<<"SVM model does not contain any support vector, can not perform margin sampling.");
    }

  OutputType* output = const_cast<OutputType*>(this->GetOutput());
  output->SetSample(this->GetInput());

  this->DoMarginSampling();
}

template< class TSample, class TModel >
void
SVMMarginSampler< TSample, TModel >
::DoMarginSampling()
{
  IndexAndDistanceVectorType idDistVector;
  OutputType* output = const_cast<OutputType*>(this->GetOutput());

  typename TSample::ConstIterator iter = this->GetInput()->Begin();
  typename TSample::ConstIterator end  = this->GetInput()->End();

  typename OutputType::ConstIterator iterO = output->Begin();
  typename OutputType::ConstIterator endO  = output->End();
  typename TSample::MeasurementVectorType measurements;


  int numberOfComponentsPerSample  = iter.GetMeasurementVector().Size();

  otbMsgDevMacro(  << "Starting iterations " );
  while (iter != end && iterO != endO)
  {
    int i = 0;
    typename SVMModelType::MeasurementType     modelMeasurement;

    measurements = iter.GetMeasurementVector();
    // otbMsgDevMacro(  << "Loop on components " << svm_type );
    for (i=0; i<numberOfComponentsPerSample; ++i)
    {
    modelMeasurement.push_back(measurements[i]);
    }

    // Get distances to the hyperplanes
    DistancesVectorType hdistances = m_Model->EvaluateHyperplanesDistances(modelMeasurement);
    double minDistance = vcl_abs(hdistances[0]);

    // Compute th min distances
    for(unsigned int i = 1; i<hdistances.Size(); ++i)
      {
      if(vcl_abs(hdistances[i])<minDistance)
        {
        minDistance = vcl_abs(hdistances[i]);
        }
      }
    // Keep index and min distance
    IndexAndDistanceType value(iter.GetInstanceIdentifier(), minDistance);
    idDistVector.push_back(value);

    ++iter;
    ++iterO;
  }

  // Sort index by increasing distances
  sort(idDistVector.begin(), idDistVector.end(), &Compare);

  // Display the first 10 values
  otbMsgDevMacro( <<" Margin Sampling: " );

  // Clear previous margin samples
  m_MarginSamples.clear();

  for(unsigned int i = 0; i<m_NumberOfCandidates && i<idDistVector.size(); ++i)
    {
    otbMsgDevMacro( "Sample "<<idDistVector[i].first<<" (distance= "<<idDistVector[i].second<<")" )
    m_MarginSamples.push_back(idDistVector[i].first);
    }

  // m_Output->AddInstance(static_cast<unsigned int>(classLabel), iterO.GetInstanceIdentifier());

}

} // end of namespace otb

#endif
