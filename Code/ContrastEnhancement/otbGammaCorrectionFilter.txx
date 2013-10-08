/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbGammaCorrectionFilter_txx
#define __otbGammaCorrectionFilter_txx


#include "otbGammaCorrectionFilter.h"

namespace otb
{

template <class TInput, class TOutput, class TFunctor>
void
GammaCorrectionFilter<TInput, TOutput, TFunctor>
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();

}

template <class TInput, class TOutput, class TFunctor>
void
GammaCorrectionFilter<TInput,TOutput, TFunctor>
::BeforeThreadedGenerateData()
{

}

} // end namespace otb
#endif
