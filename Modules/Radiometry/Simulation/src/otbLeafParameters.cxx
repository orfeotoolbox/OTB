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

#include "otbLeafParameters.h"


namespace otb
{


/**
 * Constructor
 */

LeafParameters
::LeafParameters()
{
   m_Cab=30.0;
   m_Car=10.0;
   m_CBrown=0.0;
   m_Cw=0.015;
   m_Cm=0.009;
   m_N=1.2;
}

/**
 * Destructor
 */
LeafParameters
::~LeafParameters()
{}

/**PrintSelf method */
void
LeafParameters
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
   Superclass::PrintSelf(os, indent);
   os << "Chlorophyll content       : " << m_Cab << std::endl;
   os << "Carotenoid content        : " << m_Car << std::endl;
   os << "Brown pigment content     : " << m_CBrown << std::endl;
   os << "EWT                       : " << m_Cw << std::endl;
   os << "LMA                       : " << m_Cm << std::endl;
   os << "structure coef            : " << m_N << std::endl;
}


} // end namespace otb


