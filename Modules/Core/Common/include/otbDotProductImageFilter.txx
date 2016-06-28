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
#ifndef otbDotProductImageFilter_txx
#define otbDotProductImageFilter_txx

#include "otbDotProductImageFilter.h"

namespace otb
{

/**
 *
 */
template <class TInputImage, class TOutputImage>
DotProductImageFilter<TInputImage, TOutputImage>
::DotProductImageFilter()
{
}

template <class TInputImage, class TOutputImage>
void
DotProductImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace

#endif
