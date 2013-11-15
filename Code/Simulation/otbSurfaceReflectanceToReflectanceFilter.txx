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
#ifndef __otbSurfaceReflectanceToReflectanceFilter_txx
#define __otbSurfaceReflectanceToReflectanceFilter_txx

#include "otbSurfaceReflectanceToReflectanceFilter.h"
#include "otbOpticalImageMetadataInterfaceFactory.h"
#include "otbOpticalImageMetadataInterface.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputImage, class TOutputImage>
SurfaceReflectanceToReflectanceFilter<TInputImage, TOutputImage>
::SurfaceReflectanceToReflectanceFilter()
{
  m_AtmosphericRadiativeTerms = AtmosphericRadiativeTerms::New();
  m_CorrectionParameters      = AtmosphericCorrectionParameters::New();
  m_IsSetAtmosphericRadiativeTerms = false;
  m_AeronetFileName = "";
  m_FilterFunctionValuesFileName = "";
  m_FilterFunctionCoef.clear();
  m_UseGenerateParameters = true;
}


template <class TInputImage, class TOutputImage>
void
SurfaceReflectanceToReflectanceFilter<TInputImage, TOutputImage>
::UpdateAtmosphericRadiativeTerms()
{
  MetaDataDictionaryType dict = this->GetInput()->GetMetaDataDictionary();

  OpticalImageMetadataInterface::Pointer imageMetadataInterface = OpticalImageMetadataInterfaceFactory::CreateIMI(dict);

  if ((m_CorrectionParameters->GetDay() == 0))
    {
      m_CorrectionParameters->SetDay(imageMetadataInterface->GetDay());
    }

  if ((m_CorrectionParameters->GetMonth() == 0))
    {
      m_CorrectionParameters->SetMonth(imageMetadataInterface->GetMonth());
    }

  if ((m_CorrectionParameters->GetSolarZenithalAngle() == 361.))
    {
      m_CorrectionParameters->SetSolarZenithalAngle(90. - imageMetadataInterface->GetSunElevation());
    }

  if ((m_CorrectionParameters->GetSolarAzimutalAngle() == 361.))
    {
      m_CorrectionParameters->SetSolarAzimutalAngle(imageMetadataInterface->GetSunAzimuth());
    }

  if ((m_CorrectionParameters->GetViewingZenithalAngle() == 361.))
    {
      m_CorrectionParameters->SetViewingZenithalAngle(90. - imageMetadataInterface->GetSatElevation());
    }

  if ((m_CorrectionParameters->GetViewingAzimutalAngle() == 361.))
    {
      m_CorrectionParameters->SetViewingAzimutalAngle(imageMetadataInterface->GetSatAzimuth());
    }

  if(m_AeronetFileName != "")
    m_CorrectionParameters->UpdateAeronetData( m_AeronetFileName,
                                          imageMetadataInterface->GetYear(),
                                          imageMetadataInterface->GetHour(),
                                          imageMetadataInterface->GetMinute() );

  // load fiter function values
  if(m_FilterFunctionValuesFileName != "")
    {
      m_CorrectionParameters->LoadFilterFunctionValue( m_FilterFunctionValuesFileName );
   }
  // the user has set the filter function values
  else
    {
      if( m_FilterFunctionCoef.size() != this->GetInput()->GetNumberOfComponentsPerPixel() )
       {
         itkExceptionMacro(<<"Filter Function and image channels mismatch.");
       }
      for(unsigned int i=0; i<this->GetInput()->GetNumberOfComponentsPerPixel(); ++i)
       {
         FilterFunctionValuesType::Pointer functionValues = FilterFunctionValuesType::New();
         functionValues->SetFilterFunctionValues(m_FilterFunctionCoef[i]);
         functionValues->SetMinSpectralValue(imageMetadataInterface->GetFirstWavelengths()[i]);
         functionValues->SetMaxSpectralValue(imageMetadataInterface->GetLastWavelengths()[i]);

         m_CorrectionParameters->SetWavelengthSpectralBandWithIndex(i, functionValues);
       }
    }

  Parameters2RadiativeTermsPointerType param2Terms = Parameters2RadiativeTermsType::New();

  param2Terms->SetInput(m_CorrectionParameters);
  param2Terms->Update();
  m_AtmosphericRadiativeTerms = param2Terms->GetOutput();
}


template <class TInputImage, class TOutputImage>
void
SurfaceReflectanceToReflectanceFilter<TInputImage, TOutputImage>
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();
  if(m_UseGenerateParameters)
    this->GenerateParameters();
}

template <class TInputImage, class TOutputImage>
void
SurfaceReflectanceToReflectanceFilter<TInputImage, TOutputImage>
::UpdateFunctors()
{
  //this->GetFunctor().clear();

//    for (unsigned int i = 0; i<this->GetInput()->GetNumberOfComponentsPerPixel(); ++i)
   for (unsigned int i = 0; i<m_AtmosphericRadiativeTerms->GetWavelengthSpectralBand().size(); ++i)
   {
      double coef;
      double res;
      coef = static_cast<double>(m_AtmosphericRadiativeTerms->GetTotalGaseousTransmission(i)
                                 * m_AtmosphericRadiativeTerms->GetDownwardTransmittance(i)
                                 * m_AtmosphericRadiativeTerms->GetUpwardTransmittance(i)     );
      //coef = 1. / coef;
//       res = -m_AtmosphericRadiativeTerms->GetIntrinsicAtmosphericReflectance(i) * coef;
      res = m_AtmosphericRadiativeTerms->GetIntrinsicAtmosphericReflectance(i);
      FunctorType functor;
      functor.SetCoefficient(coef);
      functor.SetResidu(res);
      functor.SetSphericalAlbedo(static_cast<double>(m_AtmosphericRadiativeTerms->GetSphericalAlbedo(i)));

      this->GetFunctorVector().push_back(functor);
//       this->SetFunctor(functor);
   }
}


template <class TInputImage, class TOutputImage>
void
SurfaceReflectanceToReflectanceFilter<TInputImage, TOutputImage>
::GenerateParameters()
{
  if(m_IsSetAtmosphericRadiativeTerms==false)
    {
      this->UpdateAtmosphericRadiativeTerms();
    }

  this->UpdateFunctors();
}

}

#endif
