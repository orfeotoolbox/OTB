/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Language:  C++

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef otbLabelMapToLabelImageFilter_txx
#define otbLabelMapToLabelImageFilter_txx

#include "otbLabelMapToLabelImageFilter.h"
#include "itkNumericTraits.h"
#include "itkProgressReporter.h"
#include "itkImageRegionConstIteratorWithIndex.h"

namespace otb {

template <class TInputImage, class TOutputImage>
LabelMapToLabelImageFilter<TInputImage, TOutputImage>
::LabelMapToLabelImageFilter()
{
}

template<class TInputImage, class TOutputImage>
void
LabelMapToLabelImageFilter<TInputImage, TOutputImage>
::GenerateInputRequestedRegion()
{
  itk::ImageToImageFilter<TInputImage, TOutputImage>::GenerateInputRequestedRegion();
}

}// end namespace otb
#endif
