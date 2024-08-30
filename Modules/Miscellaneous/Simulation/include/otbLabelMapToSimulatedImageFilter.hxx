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

#ifndef otbLabelMapToSimulatedImageFilter_hxx
#define otbLabelMapToSimulatedImageFilter_hxx

#include "otbLabelMapToSimulatedImageFilter.h"

namespace otb
{

template <class TInputLabelMap, class TSimuStep1, class TSimuStep2, class TOutputImage>
LabelMapToSimulatedImageFilter<TInputLabelMap, TSimuStep1, TSimuStep2, TOutputImage>::LabelMapToSimulatedImageFilter()
{
  m_NumberOfComponentsPerPixel = 3;
  m_SatRSRFilename             = "";
  m_PathRoot                   = "";
  m_Mean                       = 0.0;
  m_Variance                   = 1e-8;
}


template <class TInputLabelMap, class TSimuStep1, class TSimuStep2, class TOutputImage>
void LabelMapToSimulatedImageFilter<TInputLabelMap, TSimuStep1, TSimuStep2, TOutputImage>::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();

  OutputImageType* output = this->GetOutput();
  output->SetNumberOfComponentsPerPixel(m_NumberOfComponentsPerPixel);
}

template <class TInputLabelMap, class TSimuStep1, class TSimuStep2, class TOutputImage>
void LabelMapToSimulatedImageFilter<TInputLabelMap, TSimuStep1, TSimuStep2, TOutputImage>::BeforeThreadedGenerateData()
{
  OutputImageType*         output = this->GetOutput();
  const InputLabelMapType* input  = this->GetInput();

  typename OutputImageType::PixelType pixel;
  pixel.SetSize(m_NumberOfComponentsPerPixel);
  pixel.Fill(input->GetBackgroundValue());

  output->FillBuffer(pixel);

  Superclass::BeforeThreadedGenerateData();
}

template <class TInputLabelMap, class TSimuStep1, class TSimuStep2, class TOutputImage>
void LabelMapToSimulatedImageFilter<TInputLabelMap, TSimuStep1, TSimuStep2, TOutputImage>::ThreadedProcessLabelObject(LabelObjectType* labelObject)
{
  ReduceSpectralResponsePointer reduceSpectralResponse = ReduceSpectralResponseType::New();
  SatelliteRSRPointer           satRSR                 = SatelliteRSRType::New();
  // Compute the spectral response associated to this object.
  SpectralResponsePointer readSpectrum = SpectralResponseType::New();
  bool                    hasPath      = false;
  // Check if the spectrum associated to this object is given by a database.
  for (unsigned int i = 0; i < labelObject->GetNumberOfAttributes(); ++i)
  {
    if (labelObject->GetAvailableAttributes()[i].compare("path") == 0)
      hasPath = true;
  }
  if (hasPath == true)
  {
    readSpectrum->Load(m_PathRoot + labelObject->GetAttribute("path"), 100);
    reduceSpectralResponse->SetInputSpectralResponse(readSpectrum);
  }
  else // compute the spectrum using ProSail
  {
    LabelToParametersPointer labelToParams = LabelToParametersType::New();
    SimulationStep1Pointer   simuStep1     = SimulationStep1Type::New();
    SimulationStep2Pointer   simuStep2     = SimulationStep2Type::New();

    // Compute params needed to the spectrum simulator
    labelToParams->SetLabel(labelObject->GetAttribute("area"));
    labelToParams->GenerateData();

    simuStep1->SetInput(labelToParams->GetStep1Parameters());

    simuStep2->SetParameters(labelToParams->GetStep2Parameters());
    simuStep2->SetReflectance(simuStep1->GetReflectance());
    simuStep2->SetTransmittance(simuStep1->GetTransmittance());
    simuStep2->Update();
    reduceSpectralResponse->SetInputSpectralResponse(simuStep2->GetViewingReflectance());
  }
  // compute the satellite response of this spectrum
  satRSR->Clear();
  satRSR->SetNbBands(m_NumberOfComponentsPerPixel);
  satRSR->Load(m_SatRSRFilename);
  reduceSpectralResponse->SetInputSatRSR(satRSR);
  reduceSpectralResponse->CalculateResponse();

  typename OutputImageType::PixelType pixel;
  pixel.SetSize(m_NumberOfComponentsPerPixel);

  // TODO Change with a multithreaded method
  RandomGeneratorPointer randomGen = RandomGeneratorType::GetInstance();

  ConstLineIteratorType lit = ConstLineIteratorType(labelObject);

  while (!lit.IsAtEnd())
  {
    IndexType     idx    = lit.GetLine().GetIndex();
    unsigned long length = lit.GetLine().GetLength();
    for (unsigned int i = 0; i < length; ++i)
    {
      // add gaussian white noise
      for (unsigned int j = 0; j < m_NumberOfComponentsPerPixel; ++j)
      {
        double ran = randomGen->GetNormalVariate(m_Mean, m_Variance);
        pixel[j]   = static_cast<InternalPixelType>(reduceSpectralResponse->GetReduceResponse()->GetResponse()[j].second + ran);
      }
      this->GetOutput()->SetPixel(idx, pixel);
      idx[0]++;
    }
    ++lit;
  }
}


template <class TInputLabelMap, class TSimuStep1, class TSimuStep2, class TOutputImage>
void LabelMapToSimulatedImageFilter<TInputLabelMap, TSimuStep1, TSimuStep2, TOutputImage>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
