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

#include "otbAtmosphericCorrectionParameters.h"

namespace otb 
{ 
  /***********************      FilterFunctionValues **************************/
  const FilterFunctionValues::WavelenghtSpectralBandType FilterFunctionValues::m_StepOfWavelenghtSpectralBandValues = 2.5;
  FilterFunctionValues
  ::FilterFunctionValues()
  { 
     m_MinSpectralValue = 0;
     m_MaxSpectralValue = 0;
     m_FilterFunctionValues.empty();
  }
  
  /** Check the correpondance between the vector value size and the interval number between min and max.*/ 
  /** return true if the vector step is not at 2.5 and the vector has been changed. */
/*
  bool 
  FilterFunctionValues
  :: SetParameters(const WavelenghtSpectralBandType L_min, const WavelenghtSpectralBandType L_max, const WavelenghtSpectralBandType step, const ValuesVectorType & vect)
  {
    m_MinSpectralValue = L_min;
    m_UserStep = step;

    bool hasChangedVector = false;

    if(m_UserStep != m_StepOfWavelenghtSpectralBandValues) 
      {
	ValuesVectorType values(1, vect[0]);
	unsigned int i = 1;
	unsigned int j = 1;
	double invStep = 1/m_UserStep;
	// Stop the interpolation at the max spectral value.
	while(m_MinSpectralValue+i*m_StepOfWavelenghtSpectralBandValues <= L_max )
	  {
	    // Search the User interval that surround the StepOfWavelenghtSpectralBandValues current value.
	    while(j*m_UserStep <= i*m_StepOfWavelenghtSpectralBandValues)
	      {
		j++;
	      }
	    double valueTemp;
	    valueTemp = static_cast<double>(vect[j-1]) 
	      + ((static_cast<double>(vect[j])-static_cast<double>(vect[j-1]))*invStep)
	      *(i*static_cast<double>(m_StepOfWavelenghtSpectralBandValues)-static_cast<double>(m_UserStep)*(j-1));
	    values.push_back(static_cast<WavelenghtSpectralBandType>(valueTemp));

	    i++;
	  }

	if (m_MinSpectralValue+(i-1)*m_StepOfWavelenghtSpectralBandValues != L_max)
	  {
	    values.push_back(0);
	  }
	m_FilterFunctionValues = values;
	m_MaxSpectralValue = m_MinSpectralValue + i*m_StepOfWavelenghtSpectralBandValues;
      }
    else
      {
	m_MaxSpectralValue = L_max;
	m_FilterFunctionValues = vect;
	hasChangedVector = true;
      }
    return hasChangedVector;
  }
*/
  /**PrintSelf method */
  void 
  FilterFunctionValues
  ::PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os,indent);
    os << indent << "Minimum spectral value: " << m_MinSpectralValue << std::endl;
    os << indent << "Maximum spectral value: " << m_MaxSpectralValue << std::endl;
    os << indent << "User Step between each wavelenght spectral band values: " << m_UserStep << std::endl;
    os << indent << "Filter function Vector Values: " << std::endl;
    for (unsigned int i=0; i<m_FilterFunctionValues.size(); i++)
      {
	os << indent << m_FilterFunctionValues[i] <<std::endl;
      }
    os << indent << "6S Step between each wavelenght spectral band values: " << m_StepOfWavelenghtSpectralBandValues << std::endl;
  }


  /***********************      AtmosphericCorrectionParameters **************************/
  /**
   * Constructor
   */
  
  AtmosphericCorrectionParameters
  ::AtmosphericCorrectionParameters()
  { 
    m_AerosolModel = CONTINENTAL;
  }
  
  /**PrintSelf method */
  void 
  AtmosphericCorrectionParameters
  ::PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os,indent);
    os << indent << "Solar zenithal angle: " << m_SolarZenithalAngle << std::endl;
    os << indent << "Solar azimutal angle: " << m_SolarAzimutalAngle << std::endl;
    os << indent << "Viewing zenithal angle: " << m_ViewingZenithalAngle << std::endl;
    os << indent << "Viewing azimutal angle: " << m_ViewingAzimutalAngle << std::endl;
    os << indent << "Month: " << m_Month << std::endl;
    os << indent << "Day: " << m_Day << std::endl;
    os << indent << "Atmospheric pressure: " << m_AtmosphericPressure << std::endl;
    os << indent << "Water vapor amount: " << m_WaterVaporAmount << std::endl;
    os << indent << "Ozone amount: " << m_OzoneAmount << std::endl;
    os << indent << "Aerosol model: " << m_AerosolModel << std::endl;
    os << indent << "Aerosol optical : " << m_AerosolOptical << std::endl;

    // Function values print :
    os << indent << "Filter function Values: " << std::endl;
    for (unsigned int i=0; i<m_WavelenghtSpectralBand.size(); i++)
      {
	os << indent << "Channel : "<< i+1 <<" : " << std::endl;
	os << indent << m_WavelenghtSpectralBand[i]<< std::endl;
      }
    }
} // end namespace otb

