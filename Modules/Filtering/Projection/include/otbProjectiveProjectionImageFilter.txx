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
#ifndef __otbProjectiveProjectionImageFilter_txx
#define __otbProjectiveProjectionImageFilter_txx

#include "otbProjectiveProjectionImageFilter.h"

namespace otb
{

template <class TInputImage, class TOutputImage, class TPrecision>
ProjectiveProjectionImageFilter<TInputImage, TOutputImage, TPrecision>
::ProjectiveProjectionImageFilter()
{
}

template <class TInputImage, class TOutputImage, class TPrecision>
void
ProjectiveProjectionImageFilter<TInputImage, TOutputImage, TPrecision>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace

#endif
