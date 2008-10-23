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
#ifndef __otbPointSetWithTransformToDeformationFieldGenerator_txx
#define __otbPointSetWithTransformToDeformationFieldGenerator_txx

#include "otbPointSetWithTransformToDeformationFieldGenerator.h"

#include "otbMacro.h"

namespace otb
{
/**
 * Constructor
 */
template <class TPointSet, class TDeformationField>
PointSetWithTransformToDeformationFieldGenerator<TPointSet, TDeformationField>
::PointSetWithTransformToDeformationFieldGenerator()
{
 m_Transform = 0;    // has to be provided by the user
}
/**
 * PrintSelf Method
 */
template <class TPointSet, class TDeformationField>
void
PointSetWithTransformToDeformationFieldGenerator<TPointSet, TDeformationField>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif
