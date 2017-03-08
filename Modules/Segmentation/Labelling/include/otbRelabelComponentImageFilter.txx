/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Language:  C++

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef otbRelabelComponentImageFilter_txx
#define otbRelabelComponentImageFilter_txx

#include "otbRelabelComponentImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkNumericTraits.h"
#include "itkProgressReporter.h"
#include <map>

namespace otb
{

template <class TInputImage, class TOutputImage>
RelabelComponentImageFilter<TInputImage, TOutputImage>
::RelabelComponentImageFilter()
{
}


template< class TInputImage, class TOutputImage >
void
RelabelComponentImageFilter< TInputImage, TOutputImage >
::GenerateInputRequestedRegion()
{
  itk::ImageToImageFilter<TInputImage, TOutputImage>::GenerateInputRequestedRegion();
}


} // end namespace otb

#endif
