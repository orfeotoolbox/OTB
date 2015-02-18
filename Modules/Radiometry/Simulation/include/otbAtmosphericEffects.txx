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
#ifndef __otbAtmosphericEffects_txx
#define __otbAtmosphericEffects_txx

#include "otbAtmosphericEffects.h"
#include "otbSurfaceReflectanceToReflectanceFilter.h"

namespace otb
{

template <class TSpectralResponse , class TRSR>
AtmosphericEffects<TSpectralResponse , TRSR>
::AtmosphericEffects() :
m_IsSetAtmosphericRadiativeTerms(false)
  {
    m_InputSpectralResponse = InputSpectralResponseType::New();
    m_CorrectedSpectralResponse = InputSpectralResponseType::New();
    m_InputSatRSR = InputRSRType::New();
  }
/*
template <class TSpectralResponse , class TRSR>
void
AtmosphericEffects<TSpectralResponse , TRSR>
::LoadFilterFunctionAtmosphericCorrectionParameters(double step)
{
  //LMoad the filter function from the RSR
  for (unsigned int i=0; i<this->m_InputSatRSR->GetNbBands(); ++i)
  {
    m_DataAtmosphericCorrectionParameters->SetWavelenghtSpectralBandWithIndex(i, this->m_InputSatRSR->GetRSR()[i]->GetFilterFunctionValues (step));
  }
}
*/
template <class TSpectralResponse , class TRSR>
void
AtmosphericEffects<TSpectralResponse , TRSR>
::Process(/*const unsigned int numBand*/)
{
  /*typedef otb::AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms
      AtmosphericCorrectionParametersTo6SRadiativeTermsType;

  AtmosphericCorrectionParametersTo6SRadiativeTermsType::Pointer  filterAtmosphericCorrectionParametersTo6SRadiativeTerms = AtmosphericCorrectionParametersTo6SRadiativeTermsType::New();

    //Set filter function to get the RSR Sat!!!//TODO
  typedef otb::FilterFunctionValues FilterFunctionValuesType;
  typedef FilterFunctionValuesType::ValuesVectorType ValuesVectorType;
  ValuesVectorType valuesVector;
  const double step = 0.0025;
  for (unsigned int i=0; i < this->m_InputSatRSR->GetNbBands(); ++i)
  {
    //Foreach band
    valuesVector.clear();
    PairType inter;
    inter = (this->m_InputSatRSR->GetRSR())[i]->GetInterval();
    otbGenericMsgDebugMacro(<<"inter " << inter.first << " " << inter.second);

    PairType pair;
    pair.first=(inter.first+inter.second)/2.0;
    pair.second=0.;
    m_CorrectedSpectralResponse->GetResponse().push_back(pair);

    for ( double j=inter.first; j <= inter.second; j+=step)
    {
      valuesVector.push_back( (*this->m_InputSatRSR->GetRSR()[i])(j) );
    }

    FilterFunctionValuesType::Pointer functionValues = FilterFunctionValuesType::New();
    functionValues->SetFilterFunctionValues(valuesVector);
    functionValues->SetMinSpectralValue(inter.first);
    functionValues->SetMaxSpectralValue(inter.second - step);

  //TODO Is it the effecient method Problem in 6STraits
    functionValues->SetUserStep( step );

    m_DataAtmosphericCorrectionParameters->SetWavelengthSpectralBandWithIndex(i, functionValues);
  }

  filterAtmosphericCorrectionParametersTo6SRadiativeTerms->SetInput( m_DataAtmosphericCorrectionParameters );
  filterAtmosphericCorrectionParametersTo6SRadiativeTerms->Update();

  otbGenericMsgDebugMacro(<<"6S radiative term performed");

  typedef typename InputSpectralResponseType::ImageType       ImageType;

  typedef SurfaceReflectanceToReflectanceFilter<ImageType, ImageType>              SurfaceReflectanceToReflectanceFilterType;
  typename SurfaceReflectanceToReflectanceFilterType::Pointer filterSRToR = SurfaceReflectanceToReflectanceFilterType::New();

  filterSRToR->SetAtmosphericRadiativeTerms( filterAtmosphericCorrectionParametersTo6SRadiativeTerms->GetOutput() );

  typename ImageType::Pointer image = ImageType::New();
  this->m_InputSpectralResponse->GetImage(image);
  filterSRToR->SetInput( image );
  filterSRToR->Update();


  this->m_CorrectedSpectralResponse->SetFromImage( filterSRToR->GetOutput() ); */

  /*if (m_IsSetAtmosphericRadiativeTerms)
  {*/
    typedef typename InputSpectralResponseType::ImageType       ImageType;

    typedef SurfaceReflectanceToReflectanceFilter<ImageType, ImageType>              SurfaceReflectanceToReflectanceFilterType;
    typename SurfaceReflectanceToReflectanceFilterType::Pointer filterSRToR = SurfaceReflectanceToReflectanceFilterType::New();

    filterSRToR->SetAtmosphericRadiativeTerms( m_AtmosphericRadiativeTerms );

    typename ImageType::Pointer image = ImageType::New();
    this->m_InputSpectralResponse->GetImage(image);
    filterSRToR->SetInput( image );
    filterSRToR->Update();

    this->m_CorrectedSpectralResponse->SetFromImage( filterSRToR->GetOutput() );
  /*}
  else
    itkExceptionMacro(<< "Atmospheric radiative terms must be provided before updating the CSR (Corrected Spectral Response)"); */

}
} // end namespace otb

#endif
