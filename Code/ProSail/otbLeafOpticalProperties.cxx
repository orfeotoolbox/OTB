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
#ifndef __otbLeafOpticalProperties_cxx
#define __otbLeafOpticalProperties_cxx

#include "otbLeafOpticalProperties.h"


namespace otb
{


/**
 * Constructor
 */

LeafOpticalProperties
::LeafOpticalProperties()
{
   m_Reflectance.clear();
   m_Transmitance.clear();
}



/**PrintSelf method */
void
LeafOpticalProperties
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
   Superclass::PrintSelf(os,indent);
}


} // end namespace otb

#endif
