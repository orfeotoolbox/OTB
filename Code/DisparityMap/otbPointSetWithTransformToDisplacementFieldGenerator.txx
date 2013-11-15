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
#ifndef __otbPointSetWithTransformToDisplacementFieldGenerator_txx
#define __otbPointSetWithTransformToDisplacementFieldGenerator_txx

#include "otbPointSetWithTransformToDisplacementFieldGenerator.h"

#include "otbMacro.h"

namespace otb
{
/**
 * Constructor
 */
template <class TPointSet, class TDisplacementField>
PointSetWithTransformToDisplacementFieldGenerator<TPointSet, TDisplacementField>
::PointSetWithTransformToDisplacementFieldGenerator()
{
  m_Transform = 0;    // has to be provided by the user
}
/**
 * PrintSelf Method
 */
template <class TPointSet, class TDisplacementField>
void
PointSetWithTransformToDisplacementFieldGenerator<TPointSet, TDisplacementField>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif
