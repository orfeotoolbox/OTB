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
#ifndef _otbMeanShiftVectorImageFilter_txx
#define _otbMeanShiftVectorImageFilter_txx

#include "otbMeanShiftVectorImageFilter.h"

#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkImageRegionIterator.h"
#include "itkOffset.h"
#include "itkProgressReporter.h"

#include "otbMacro.h"

namespace otb
{
  template <class TInputImage,class TOutputImage,class TPrecision>
  unsigned int 
  MeanShiftVectorImageFilter<TInputImage,TOutputImage,TPrecision>
  ::GetNumberOfComponentsPerPixel()
  {
    return this->GetInput()->GetNumberOfComponentsPerPixel();
  }

  template <class TInputImage,class TOutputImage,class TPrecision>
  void
  MeanShiftVectorImageFilter<TInputImage,TOutputImage,TPrecision>
  ::InitValue(PrecisionPixelType & value, const unsigned int& nbComponents)
  {
    value.SetSize(nbComponents);
    value.Fill(0);
  }

  template <class TInputImage,class TOutputImage,class TPrecision>
  double
  MeanShiftVectorImageFilter<TInputImage,TOutputImage,TPrecision>
  ::SquaredNorm(const PrecisionPixelType & value)
  {
    return value.GetSquaredNorm();
  }

  template <class TInputImage,class TOutputImage,class TPrecision>
  const typename  MeanShiftVectorImageFilter<TInputImage,TOutputImage,TPrecision>
  ::PrecisionPixelType 
  MeanShiftVectorImageFilter<TInputImage,TOutputImage,TPrecision>
  ::CastInputPixelToPrecisionPixel(const InputPixelType & pixel)
  {
    // Inplicit cast in construction by copy, nothing to do here
    return pixel;
  }

  template <class TInputImage,class TOutputImage,class TPrecision>
  const typename  MeanShiftVectorImageFilter<TInputImage,TOutputImage,TPrecision>
  ::OutputPixelType 
  MeanShiftVectorImageFilter<TInputImage,TOutputImage,TPrecision>
  ::CastPrecisionPixelToOutputPixel(const PrecisionPixelType & pixel)
  {   
    // Inplicit cast in construction by copy, nothing to do here
    return pixel;
  }

  template <class TInputImage,class TOutputImage,class TPrecision>
  void
  MeanShiftVectorImageFilter<TInputImage,TOutputImage,TPrecision>
  ::PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os,indent);
  }
} // end namespace otb

#endif
