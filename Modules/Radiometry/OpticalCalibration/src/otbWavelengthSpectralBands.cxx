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

#include "otbWavelengthSpectralBands.h"
#include <fstream>

namespace otb
{
/***********************      WavelengthSpectralBands **************************/
WavelengthSpectralBands
::WavelengthSpectralBands()
{
  m_WavelengthSpectralBandsList = InternalWavelengthSpectralBandListType::New();
  m_NbBands = 0;
}

/**PrintSelf method */
void
WavelengthSpectralBands
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  for (unsigned int i = 0; i < m_NbBands; ++i)
    {
    os << indent << "Filter function values " << i << ":" << std::endl;
    os << indent << m_WavelengthSpectralBandsList->GetNthElement(i) << std::endl;
    }
}

} // end namespace otb
