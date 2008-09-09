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
#ifndef _otbMeanShiftImageFilter_txx
#define _otbMeanShiftImageFilter_txx

#include "otbMeanShiftImageFilter.h"


namespace otb
{
  template <class TInputImage,class TOutputImage,class TPrecision>
  unsigned int 
  MeanShiftImageFilter<TInputImage,TOutputImage,TPrecision>
  ::GetNumberOfComponentsPerPixel()
  {
    return 1;
  }

  template <class TInputImage,class TOutputImage,class TPrecision>
  void
  MeanShiftImageFilter<TInputImage,TOutputImage,TPrecision>
  ::InitValue(PrecisionPixelType & value, const unsigned int& nbComponents)
  {
    value = 0;
  }

  template <class TInputImage,class TOutputImage,class TPrecision>
  double
  MeanShiftImageFilter<TInputImage,TOutputImage,TPrecision>
  ::SquaredNorm(const PrecisionPixelType & value)
  {   
    return value * value;
  }

  template <class TInputImage,class TOutputImage,class TPrecision>
  void
  MeanShiftImageFilter<TInputImage,TOutputImage,TPrecision>
  ::PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os,indent);
  }

} // end namespace otb

#endif
