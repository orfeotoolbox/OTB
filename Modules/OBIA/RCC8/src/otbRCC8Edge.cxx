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

#include "otbRCC8Edge.h"

namespace otb
{
/**
 * Constructor
 */
RCC8Edge::RCC8Edge()
{
  m_Value = OTB_RCC8_DC;
}
/**
 * PrintSelf method
 */
void
RCC8Edge::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "RCC8 Value: " << m_Value << std::endl;
}
} // end namespace otb
