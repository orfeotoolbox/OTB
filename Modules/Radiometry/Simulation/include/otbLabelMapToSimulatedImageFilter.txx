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
#ifndef __otbLabelMapToSimulatedImageFilter_txx
#define __otbLabelMapToSimulatedImageFilter_txx

#include "otbLabelMapToSimulatedImageFilter.h"

namespace otb
{

template <class TInputLabelMap, class TSimuStep1, class TSimuStep2, class TOutputImage>
LabelMapToSimulatedImageFilter<TInputLabelMap, TSimuStep1, TSimuStep2, TOutputImage>
::LabelMapToSimulatedImageFilter()
{
   m_NumberOfComponentsPerPixel = 3;
   m_SatRSRFilename="";
   m_PathRoot="";
   m_Mean=0.0;
   m_Variance=1e-8;
}


template <class TInputLabelMap, class TSimuStep1, class TSimuStep2, class TOutputImage>
void
LabelMapToSimulatedImageFilter<TInputLabelMap, TSimuStep1, TSimuStep2, TOutputImage>
::GenerateOutputInformation()
{
   Superclass::GenerateOutputInformation();

   OutputImageType * output = this->GetOutput();
   output->SetNumberOfComponentsPerPixel(m_NumberOfComponentsPerPixel);

}

template <class TInputLabelMap, class TSimuStep1, class TSimuStep2, class TOutputImage>
void
LabelMapToSimulatedImageFilter<TInputLabelMap, TSimuStep1, TSimuStep2, TOutputImage>
::BeforeThreadedGenerateData()
{
   OutputImageType * output = this->GetOutput();
   const InputLabelMapType * input = this->GetInput();

   typename OutputImageType::PixelType pixel;
   pixel.SetSize(m_NumberOfComponentsPerPixel);
   pixel.Fill( input->GetBackgroundValue() );

   output->FillBuffer(pixel);

   Superclass::BeforeThreadedGenerateData();
}

template <class TInputLabelMap, class TSimuStep1, class TSimuStep2, class TOutputImage>
void
LabelMapToSimulatedImageFilter<TInputLabelMap, TSimuStep1, TSimuStep2, TOutputImage>
::ThreadedProcessLabelObject( LabelObjectType * labelObject )
{
  ReduceSpectralResponsePointer reduceSpectralResponse = ReduceSpectralResponseType::New();
  SatelliteRSRPointer satRSR = SatelliteRSRType::New();
  //Compute the spectral response associated to this object.
  SpectralResponsePointer readSpectrum = SpectralResponseType::New();
  bool hasPath = false;
  //Check if the spectrum associated to this object is given by a database.
  for (unsigned int i = 0; i < labelObject->GetNumberOfAttributes(); ++i)
    {
    if (labelObject->GetAvailableAttributes()[i].compare("path") == 0) hasPath = true;
    }
  if (hasPath == true)
    {
    readSpectrum->Load(m_PathRoot + labelObject->GetAttribute("path"), 100);
    reduceSpectralResponse->SetInputSpectralResponse(readSpectrum);
    }
  else //compute the spectrum using ProSail
    {
    LabelToParametersPointer labelToParams = LabelToParametersType::New();
    SimulationStep1Pointer simuStep1 = SimulationStep1Type::New();
    SimulationStep2Pointer simuStep2 = SimulationStep2Type::New();

    //Compute params needed to the spectrum simulator
    labelToParams->SetLabel(labelObject->GetAttribute("area"));
    labelToParams->GenerateData();

    simuStep1->SetInput(labelToParams->GetStep1Parameters());

    simuStep2->SetParameters(labelToParams->GetStep2Parameters());
    simuStep2->SetReflectance(simuStep1->GetReflectance());
    simuStep2->SetTransmittance(simuStep1->GetTransmittance());
    simuStep2->Update();
    reduceSpectralResponse->SetInputSpectralResponse(simuStep2->GetViewingReflectance());

    }
  //compute the satellite response of this spectrum
  satRSR->Clear();
  satRSR->SetNbBands(m_NumberOfComponentsPerPixel);
  satRSR->Load(m_SatRSRFilename);
  reduceSpectralResponse->SetInputSatRSR(satRSR);
  reduceSpectralResponse->CalculateResponse();

  typename OutputImageType::PixelType pixel;
  pixel.SetSize(m_NumberOfComponentsPerPixel);

  //TODO Change with a multithreaded method
  RandomGeneratorPointer randomGen = RandomGeneratorType::GetInstance();

  ConstLineIteratorType lit = ConstLineIteratorType (labelObject);

  while ( !lit.IsAtEnd() )
    {
    IndexType idx = lit.GetLine().GetIndex();
    unsigned long length = lit.GetLine().GetLength();
    for (unsigned int i = 0; i < length; ++i)
      {
      //add gaussian white noise
      for (unsigned int i = 0; i < m_NumberOfComponentsPerPixel; ++i)
        {
        double ran = randomGen->GetNormalVariate(m_Mean, m_Variance);
        pixel[i] = static_cast<InternalPixelType> (reduceSpectralResponse->GetReduceResponse()->GetResponse()[i].second
            + ran);
        }
      this->GetOutput()->SetPixel(idx, pixel);
      idx[0]++;
      }
    ++lit;
    }
}


template <class TInputLabelMap, class TSimuStep1, class TSimuStep2, class TOutputImage>
void
LabelMapToSimulatedImageFilter<TInputLabelMap, TSimuStep1, TSimuStep2, TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
   Superclass::PrintSelf(os, indent);

}

} //end namespace otb

#endif

