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

#include "otbFilterFunctionValues.h"

#include <fstream>

namespace otb
{
/***********************      FilterFunctionValues **************************/
FilterFunctionValues
::FilterFunctionValues()
{
  m_MinSpectralValue = 0.3;
  m_MaxSpectralValue = 1;
  // Fill with 1. Size 3 for 6S interpolation
  m_FilterFunctionValues = ValuesVectorType(280, 1.);
  m_UserStep = 0.0025;
}

FilterFunctionValues::WavelengthSpectralBandType
FilterFunctionValues
::GetCenterSpectralValue() const
{
  // The computation is done by taking the weighted average of the
  // filter. The computation is done each time the value is requested.
  // It is anticipated that this method won't be called much and that
  // the cost will be negligible compared to the rest of the processing
  WavelengthSpectralBandType total = 0;
  for (unsigned int i = 0; i < m_FilterFunctionValues6S.size(); ++i)
    {
    total += m_FilterFunctionValues6S[i];
    }
  unsigned int centerIndex = 0;
  WavelengthSpectralBandType total2 = 0;
  for (centerIndex = 0; centerIndex < m_FilterFunctionValues6S.size(); ++centerIndex)
    {
    total2 += m_FilterFunctionValues6S[centerIndex];
    if (total2 > total / 2) break;
    }
  return m_MinSpectralValue + m_UserStep * centerIndex;
}

/**PrintSelf method */
void
FilterFunctionValues
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  os << indent << "Minimum spectral value       : " << m_MinSpectralValue << std::endl;
  os << indent << "Maximum spectral value       : " << m_MaxSpectralValue << std::endl;
  os << indent << "Wavelength spectral band step: " << m_UserStep << std::endl;
  os << indent << "Filter function values: " << std::endl;

  os << std::scientific;
  for (unsigned int i = 0; i < m_FilterFunctionValues.size(); ++i)
    {
       if ( i%6 == 0 && i >= 6)
       {
          os << std::endl;
       }
       os << indent << m_FilterFunctionValues[i];
    }
  os << std::endl;
  //Default notation for floating point value can be selected by
  //calling str.unsetf(ios_base::floatfield)
  os.unsetf(std::ios_base::floatfield);
  // os << indent << "6S Filter function values: " << std::endl;
  // for (unsigned int i = 0; i < m_FilterFunctionValues6S.size(); ++i)
  //   {
  //   os << indent << m_FilterFunctionValues6S[i] << std::endl;
  //   }
}

} // end namespace otb
