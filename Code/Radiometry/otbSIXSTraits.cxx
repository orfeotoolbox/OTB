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
#include "otbSIXSTraits.h"

#include "otb_6S.h"
#include "main_6s.h"
#include "otbMacro.h"

namespace otb
{


void
SIXSTraits::ComputeAtmosphericParameters(
        const   double                  SolarZenithalAngle,             /** The Solar zenithal angle */
        const   double                  SolarAzimutalAngle,             /** The Solar azimutal angle */
        const   double                  ViewingZenithalAngle,           /** The Viewing zenithal angle */
        const   double                  ViewingAzimutalAngle,           /** The Viewing azimutal angle */
        const   unsigned int            Month,                          /** The Month */
        const   unsigned int            Day,                            /** The Day (in the month) */
        const   double                  AtmosphericPressure,            /** The Atmospheric pressure */
        const   double                  WaterVaporAmount,               /** The Water vapor amount (Total water vapor content over vertical atmospheric column) */
        const   double                  OzoneAmount,                    /** The Ozone amount (Stratospheric ozone layer content) */
        const   AerosolModelType &      AerosolModel,                   /** The Aerosol model */
        const   double                  AerosolOptical,                 /** The Aerosol optical (radiative impact of aerosol for the reference wavelenght 550-nm) */
                WavelenghtSpectralType& WavelenghtSpectralBand,         /** Wavelenght for the spectral band definition */
                                                                        /** Note : The Max wavelenght spectral band value must be updated ! */
                double &                AtmosphericReflectance,         /** Atmospheric reflectance */     
                double &                AtmosphericSphericalAlbedo,     /** atmospheric spherical albedo */
                double &                TotalGaseousTransmission,       /** Total gaseous transmission */
                double &                DownwardTransmittance,          /** downward transmittance */      
                double &                UpwardTransmittance             /** upward transmittance */
        )
{
// geometrical conditions
        otb_6s_real asol(static_cast<otb_6s_real>(SolarZenithalAngle)); 
        otb_6s_real phi0(static_cast<otb_6s_real>(SolarAzimutalAngle));
        otb_6s_real avis(static_cast<otb_6s_real>(ViewingZenithalAngle));
        otb_6s_real phiv(static_cast<otb_6s_real>(ViewingAzimutalAngle));
        otb_6s_integer month(static_cast<otb_6s_integer>(Month));
        otb_6s_integer jday(static_cast<otb_6s_integer>(Day));
        otb_6s_real pressure(static_cast<otb_6s_real>(AtmosphericPressure));
        otb_6s_real uw(static_cast<otb_6s_real>(WaterVaporAmount));
        otb_6s_real uo3(static_cast<otb_6s_real>(OzoneAmount)); 
// atmospheric model
        otb_6s_integer iaer(static_cast<otb_6s_integer>(AerosolModel));
        otb_6s_real taer55(static_cast<otb_6s_real>(AerosolOptical));
        
 
        // Init output parameters       
        AtmosphericReflectance = 0.;   
        AtmosphericSphericalAlbedo = 0.; 
        TotalGaseousTransmission = 0.;  
        DownwardTransmittance = 0.;    
        UpwardTransmittance = 0.;      

        otb_6s_real wlinf(0.), wlsup(0.);
        otb_6s_real otb_ratm__(0.), sast(0.), tgasm(0.), sdtott(0.), sutott(0.);
        try
        {
                // 6S official Wavelenght Spectral Band step value 
                const double SIXSStepOfWavelenghtSpectralBandValues = .0025;
                // Generate 6s Wavelenght Spectral Band with the offcicial step value
                ComputeWavelenghtSpectralBandValuesFor6S(       SIXSStepOfWavelenghtSpectralBandValues,
                                                                WavelenghtSpectralBand // Update 
                                                                );

                // 6S official tab size Wavelenght Spectral
                const unsigned int S_6S_SIZE=1501;
                // Generate WavelenghtSpectralBand  in 6S compatible buffer s[1501]
                otb_6s_integer iinf = static_cast<otb_6s_integer>((WavelenghtSpectralBand.GetMinSpectralValue() - (double).25) / SIXSStepOfWavelenghtSpectralBandValues + (double)1.5);
                otb_6s_integer isup = static_cast<otb_6s_integer>((WavelenghtSpectralBand.GetMaxSpectralValue() - (double).25) / SIXSStepOfWavelenghtSpectralBandValues + (double)1.5);

                otb_6s_integer cpt=iinf-1;
                otb_6s_real * s(NULL);
                s = new otb_6s_real[S_6S_SIZE];
                memset( s, 0, S_6S_SIZE*sizeof(otb_6s_real) );
                const ValuesVectorType & FilterFunctionValues6S = WavelenghtSpectralBand.GetFilterFunctionValues6S();
                // Set the values of FilterFunctionValues6S in s between [iinf-1;isup]
                for(unsigned int i=0 ; cpt<isup ; i++)
                {
                        s[cpt] = FilterFunctionValues6S[i];
                        cpt++;
                }
                // Call 6s main function
                otbMsgDevMacro(<< "Start call 6S main function ...");
                otb_6s_ssssss_otb_main_function(        &asol, &phi0, &avis, &phiv, &month, &jday, 
                                                        &pressure, &uw, &uo3, 
	                                                &iaer, 
                                                        &taer55, 
                                                        &wlinf, &wlsup, 
                                                        s, 
                                                        &otb_ratm__, 
                                                        &sast, 
                                                        &tgasm, 
                                                        &sdtott, 
                                                        &sutott);
                otbMsgDevMacro(<< "Done call 6S main function!");
                delete [] s;
                s = NULL;
        }
        catch( std::bad_alloc & err )
        {
                itkGenericExceptionMacro( <<"Exception bad_alloc in SIXSTraits class: "<<(char*)err.what());
        }
        catch (...) 
        {
                itkGenericExceptionMacro( <<"Unknown exception in SIXSTraits class (catch(...)");
        }

        // Set outputs parameters       
        AtmosphericReflectance = static_cast<double>(otb_ratm__);   
        AtmosphericSphericalAlbedo = static_cast<double>(sast); 
        TotalGaseousTransmission = static_cast<double>(tgasm);  
        DownwardTransmittance = static_cast<double>(sdtott);    
        UpwardTransmittance = static_cast<double>(sutott);      
}


void
SIXSTraits::ComputeWavelenghtSpectralBandValuesFor6S(
        const   double                          SIXSStepOfWavelenghtSpectralBandValues,
                WavelenghtSpectralType&         WavelenghtSpectralBand
        )
{
        const double L_min = static_cast<double>(WavelenghtSpectralBand.GetMinSpectralValue());
        const double L_max = static_cast<double>(WavelenghtSpectralBand.GetMaxSpectralValue());
        const double L_userStep = static_cast<double>(WavelenghtSpectralBand.GetUserStep());
        const ValuesVectorType & FilterFunctionValues = WavelenghtSpectralBand.GetFilterFunctionValues();
        unsigned int i = 1;
        unsigned int j = 1;
        const double invStep = static_cast<double>(1./L_userStep);
        double value(0.);
        
        // Generate WavelenghtSpectralBand if the step is not the offical 6S step value
        if( vcl_abs(L_userStep-SIXSStepOfWavelenghtSpectralBandValues) > .00000001 ) 
        {
                ValuesVectorType values(1, FilterFunctionValues[0]); //vector size 1 with the value vect[0]
                
	        // Stop the interpolation at the max spectral value.
	        value = i*SIXSStepOfWavelenghtSpectralBandValues;
                while(L_min+value <= L_max )
	        {
	                // Search the User interval that surround the StepOfWavelenghtSpectralBandValues current value.
                        
	                while(j*L_userStep <= value)
	                {
		                j++;
	                }
	                double valueTemp;
	                valueTemp = static_cast<double>(FilterFunctionValues[j-1]) 
	                                + ((static_cast<double>(FilterFunctionValues[j])-static_cast<double>(FilterFunctionValues[j-1]))*invStep)
	                                *(value-L_userStep*(j-1));
	                values.push_back(static_cast<WavelenghtSpectralBandType>(valueTemp));

	                i++;
	        }

	        if (L_min+(i-1)*SIXSStepOfWavelenghtSpectralBandValues != L_max)
	        {
	                values.push_back(0);
	        }
	         // Store this values        
                WavelenghtSpectralBand.SetFilterFunctionValues6S(values);
                // Store the new Max MaxSpectralValue         
                WavelenghtSpectralBand.SetMaxSpectralValue(static_cast<WavelenghtSpectralBandType>(L_min + i*SIXSStepOfWavelenghtSpectralBandValues));

        }
        else
        {
                // Init with copy of FilterFunctionValues input vector values
                WavelenghtSpectralBand.SetFilterFunctionValues6S(FilterFunctionValues);
        }
}

} // namespace otb
