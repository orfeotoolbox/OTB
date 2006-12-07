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
#ifndef _otbImageMultiSegmentationToRCC8GraphFilter_txx
#define _otbImageMultiSegmentationToRCC8GraphFilter_txx

#include "otbImageMultiSegmentationToRCC8GraphFilter.h"
#include "itkMinimumMaximumImageFilter.h"
#include "otbImageToImageRCC8Calculator.h"

namespace otb
{
/** 
 * Constructor.
 */
template <class TInputImage, class TOutputGraph>
ImageMultiSegmentationToRCC8GraphFilter<TInputImage, TOutputGraph>
::ImageMultiSegmentationToRCC8GraphFilter()
{}
/**
 * Destructor.
 */
template <class TInputImage, class TOutputGraph>
ImageMultiSegmentationToRCC8GraphFilter<TInputImage, TOutputGraph>
::~ImageMultiSegmentationToRCC8GraphFilter()
{}
/**
 * Main computation method.
 */
template <class TInputImage, class TOutputGraph>
void
ImageMultiSegmentationToRCC8GraphFilter<TInputImage, TOutputGraph>
::GenerateData()
{}
template <class TInputImage, class TOutputGraph>
void
ImageMultiSegmentationToRCC8GraphFilter<TInputImage, TOutputGraph>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // end namespace otb
#endif

