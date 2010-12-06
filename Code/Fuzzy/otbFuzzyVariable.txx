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
#ifndef __otbFuzzyVariable_txx
#define __otbFuzzyVariable_txx

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include "otbFuzzyVariable.h"

namespace otb
{

template <unsigned int TNValues, class TPrecision>
FuzzyVariable<TNValues, TPrecision>::FuzzyVariable()
{
}


template <unsigned int TNValues, class TPrecision>
void
FuzzyVariable<TNValues, TPrecision>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb
#endif
