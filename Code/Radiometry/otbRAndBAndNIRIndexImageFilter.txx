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
#ifndef __otbRAndBAndNIRIndexImageFilter_txx
#define __otbRAndBAndNIRIndexImageFilter_txx

#include "otbRAndBAndNIRIndexImageFilter.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputImageR, class TInputImageB, class TInputImageNIR,
    class TOutputImage, class TFunction>
RAndBAndNIRIndexImageFilter<TInputImageR, TInputImageB, TInputImageNIR, TOutputImage, TFunction>
::RAndBAndNIRIndexImageFilter()
{
}

/**
 * Connect one of the operands for pixel-wise addition
 */
template <class TInputImageR, class TInputImageB, class TInputImageNIR,
    class TOutputImage, class TFunction>
void
RAndBAndNIRIndexImageFilter<TInputImageR, TInputImageB, TInputImageNIR, TOutputImage, TFunction>
::SetInputR(const TInputImageR * image)
{
  // Process object is not const-correct so the const casting is required.
  this->SetInput1(image);
}

/**
 * Connect one of the operands for pixel-wise addition
 */
template <class TInputImageR, class TInputImageB, class TInputImageNIR,
    class TOutputImage, class TFunction>
void
RAndBAndNIRIndexImageFilter<TInputImageR, TInputImageB, TInputImageNIR, TOutputImage, TFunction>
::SetInputB(const TInputImageB * image)
{
  this->SetInput2(image);
}

/**
 * Connect one of the operands for pixel-wise addition
 */
template <class TInputImageR, class TInputImageB, class TInputImageNIR,
    class TOutputImage, class TFunction>
void
RAndBAndNIRIndexImageFilter<TInputImageR, TInputImageB, TInputImageNIR, TOutputImage, TFunction>
::SetInputNIR(const TInputImageNIR * image)
{
  this->SetInput3(image);
}

template <class TInputImageR, class TInputImageB, class TInputImageNIR, class TOutputImage, class TFunction>
void
RAndBAndNIRIndexImageFilter<TInputImageR, TInputImageB, TInputImageNIR, TOutputImage, TFunction>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

}

#endif
