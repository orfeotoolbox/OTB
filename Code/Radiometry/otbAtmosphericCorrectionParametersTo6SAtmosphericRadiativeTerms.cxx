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
#include "otbAtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms.h"
#include "otbSIXSTraits.h"

namespace otb
{
/** 
 * Constructor.
 */

AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms
::AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms()
{
  this->ProcessObject::SetNumberOfRequiredInputs(1);
  this->ProcessObject::SetNumberOfRequiredOutputs(1);
  // Create the output. We use static_cast<> here because we know the default
  // output must be of type TOutputPointSet
  AtmosphericRadiativeTermsPointer output
    = static_cast<AtmosphericRadiativeTermsType*>(this->MakeOutput(0).GetPointer()); 

  this->ProcessObject::SetNthOutput( 0, output.GetPointer() );

}
/**
 *
 */
AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms::DataObjectPointer
AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms
::MakeOutput(unsigned int)
{
  return static_cast<itk::DataObject*>(AtmosphericRadiativeTermsType::New().GetPointer());
}

/**
 * 
 */
void
AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms
::GraftOutput(itk::DataObject *graft)
{
  this->GraftNthOutput(0, graft);
}


/**
 * 
 */
void
AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms
::GraftNthOutput(unsigned int idx, itk::DataObject *graft)
{
  if ( idx >= this->GetNumberOfOutputs() )
    {
    itkExceptionMacro(<<"Requested to graft output " << idx << 
        " but this filter only has " << this->GetNumberOfOutputs() << " Outputs.");
    }  

  if ( !graft )
    {
    itkExceptionMacro(<<"Requested to graft output that is a NULL pointer" );
    }

  itk::DataObject * output = this->GetOutput( idx );

  // Call Graft on the PointSet in order to copy meta-information, and containers.
  output->Graft( graft );
}

/**
 * Get the output data
 * \return The data produced.
 */

AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms::AtmosphericRadiativeTermsType *
AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms
::GetOutput(void)
{
  if(this->GetNumberOfOutputs()<1)
    {
      return 0;
    }
  return static_cast<AtmosphericRadiativeTermsType *> (this->ProcessObject::GetOutput(0));
}

/**
 *
 */
AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms::AtmosphericRadiativeTermsType *
AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms
::GetOutput(unsigned int idx)
{
  return static_cast<AtmosphericRadiativeTermsType*>
    (this->itk::ProcessObject::GetOutput(idx));
}


void
AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms
::SetInput(const AtmosphericCorrectionParametersType *object)
{
  // A single input image
  this->itk::ProcessObject::SetNthInput(0,const_cast<AtmosphericCorrectionParametersType*>(object));
}

AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms::AtmosphericCorrectionParametersType *
AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms
::GetInput(void)
{
  // If there is no input 
  if (this->GetNumberOfInputs()!=1)
    {
    // exit
    return 0;
    }
  // else return the first input
  return static_cast<AtmosphericCorrectionParametersType * >
    (this->itk::ProcessObject::GetInput(0) );
}


void
AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms
::GenerateData()
{

        AtmosphericCorrectionParametersPointer input = this->GetInput();
        AtmosphericRadiativeTermsPointer output = this->GetOutput();
        
        output->GetValues().clear();
        typedef AtmosphericCorrectionParameters::WavelenghtSpectralBandVectorType WavelenghtSpectralBandVectorType;
        WavelenghtSpectralBandVectorType WavelenghtSpectralBandVector = input->GetWavelenghtSpectralBand();
        unsigned int NbBand = WavelenghtSpectralBandVector.size();

        double atmosphericReflectance(0.);
        double atmosphericSphericalAlbedo(0.);
        double totalGaseousTransmission(0.);
        double downwardTransmittance(0.);
        double upwardTransmittance(0.);
                
        for(unsigned int i=0 ; i<NbBand ; i++)
        {
	  atmosphericReflectance = 0.;
                atmosphericSphericalAlbedo = 0.;
                totalGaseousTransmission = 0.;
                downwardTransmittance = 0.;
                upwardTransmittance = 0.;
                SIXSTraits::ComputeAtmosphericParameters(
                        input->GetSolarZenithalAngle(),                 /** The Solar zenithal angle */
                        input->GetSolarAzimutalAngle(),                 /** The Solar azimutal angle */
                        input->GetViewingZenithalAngle(),               /** The Viewing zenithal angle */
                        input->GetViewingAzimutalAngle(),               /** The Viewing azimutal angle */
                        input->GetMonth(),                              /** The Month */
                        input->GetDay(),                                /** The Day (in the month) */
                        input->GetAtmosphericPressure(),                /** The Atmospheric pressure */
                        input->GetWaterVaporAmount(),                   /** The Water vapor amount (Total water vapor content over vertical atmospheric column) */
                        input->GetOzoneAmount(),                        /** The Ozone amount (Stratospheric ozone layer content) */
                        input->GetAerosolModel(),                       /** The Aerosol model */
                        input->GetAerosolOptical(),                     /** The Aerosol optical (radiative impact of aerosol for the reference wavelenght 550-nm) */
                        input->GetWavelenghtSpectralBand()[i],          /** Wavelenght for the spectral band definition */
                                                                        /** Note : The Max wavelenght spectral band value must be updated ! */
                        atmosphericReflectance,                         /** Atmospheric reflectance */     
                        atmosphericSphericalAlbedo,                     /** atmospheric spherical albedo */
                        totalGaseousTransmission,                       /** Total gaseous transmission */
                        downwardTransmittance,                          /** downward transmittance */      
                        upwardTransmittance );                          /** upward transmittance */
                
		/*
		  atmRadiativeTerm = New();
		  atmRadiativeTerm->SetAtmosphericReflectances(AtmosphericReflectance);
		  atmRadiativeTerm->SetAtmosphericSphericalAlbedo(AtmosphericSphericalAlbedo);
		  atmRadiativeTerm->SetTotalGaseousTransmission(TotalGaseousTransmission);
		  atmRadiativeTerm->SetDownwardTransmittance(DownwardTransmittance);
		  atmRadiativeTerm->SetUpwardTransmittance(UpwardTransmittance); 
		  //output->Get...().push_back(atmRadiativeTerm);
		  output->SetValueByIndex(i, atmRadiativeTerm);
		*/
		output->SetIntrinsicAtmosphericReflectances(i, atmosphericReflectance);
		output->SetSphericalAlbedos(i, atmosphericSphericalAlbedo);
		output->SetTotalGaseousTransmissions(i, totalGaseousTransmission);
		output->SetDownwardTransmittances(i, downwardTransmittance);
		output->SetUpwardTransmittances(i, upwardTransmittance);

        }
        

}
        

/**
 * PrintSelf Method
 */
void
AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

}// end namespace otb
