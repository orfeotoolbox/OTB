/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkLabelMapToLabelImageFilter.txx, v $
  Language:  C++
  Date:      $Date: 2009-05-16 22:19:31 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbLabelMapToLabelImageFilter_txx
#define __otbLabelMapToLabelImageFilter_txx

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
