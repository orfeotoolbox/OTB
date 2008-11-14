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
#ifndef __otbMeanShiftVectorImageFilter_txx
#define __otbMeanShiftVectorImageFilter_txx

#include "otbMeanShiftVectorImageFilter.h"

#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkImageRegionIterator.h"
#include "itkOffset.h"
#include "itkProgressReporter.h"

#include "otbMacro.h"

namespace otb
{
  template <class TInputImage,class TOutputImage, class TClusterImage,class TPrecision>
  unsigned int 
  MeanShiftVectorImageFilter<TInputImage,TOutputImage,TClusterImage,TPrecision>
  ::GetNumberOfComponentsPerPixel()
  {
    return this->GetInput()->GetNumberOfComponentsPerPixel();
  }

  template <class TInputImage,class TOutputImage, class TClusterImage,class TPrecision>
  void
  MeanShiftVectorImageFilter<TInputImage,TOutputImage,TClusterImage,TPrecision>
  ::InitValue(PrecisionPixelType & value, const unsigned int& nbComponents)
  {
    value.SetSize(nbComponents);
    value.Fill(0);
  }

  template <class TInputImage,class TOutputImage, class TClusterImage,class TPrecision>
  double
  MeanShiftVectorImageFilter<TInputImage,TOutputImage,TClusterImage,TPrecision>
  ::SquaredNorm(const PrecisionPixelType & value)
  {
    return value.GetSquaredNorm();
  }

  template <class TInputImage,class TOutputImage, class TClusterImage,class TPrecision>
  const typename  MeanShiftVectorImageFilter<TInputImage,TOutputImage,TClusterImage,TPrecision>
  ::PrecisionPixelType 
  MeanShiftVectorImageFilter<TInputImage,TOutputImage,TClusterImage,TPrecision>
  ::CastInputPixelToPrecisionPixel(const InputPixelType & pixel)
  {
    // Inplicit cast in construction by copy, nothing to do here
    return pixel;
  }

  template <class TInputImage,class TOutputImage, class TClusterImage,class TPrecision>
  const typename  MeanShiftVectorImageFilter<TInputImage,TOutputImage,TClusterImage,TPrecision>
  ::OutputPixelType 
  MeanShiftVectorImageFilter<TInputImage,TOutputImage,TClusterImage,TPrecision>
  ::CastPrecisionPixelToOutputPixel(const PrecisionPixelType & pixel)
  {   
    // Inplicit cast in construction by copy, nothing to do here
    return pixel;
  }

  template <class TInputImage,class TOutputImage, class TClusterImage,class TPrecision>
  void
  MeanShiftVectorImageFilter<TInputImage,TOutputImage,TClusterImage,TPrecision>
  ::PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os,indent);
  }
} // end namespace otb

#endif
