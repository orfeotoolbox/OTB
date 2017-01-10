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
#ifndef otbRAndGAndNIRIndexImageFilter_txx
#define otbRAndGAndNIRIndexImageFilter_txx

#include "otbRAndGAndNIRIndexImageFilter.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputImageR, class TInputImageG, class TInputImageNIR,
    class TOutputImage, class TFunction>
RAndGAndNIRIndexImageFilter<TInputImageR, TInputImageG, TInputImageNIR, TOutputImage, TFunction>
::RAndGAndNIRIndexImageFilter()
{
}

/**
 * Connect one of the operands for pixel-wise addition
 */
template <class TInputImageR, class TInputImageG, class TInputImageNIR,
    class TOutputImage, class TFunction>
void
RAndGAndNIRIndexImageFilter<TInputImageR, TInputImageG, TInputImageNIR, TOutputImage, TFunction>
::SetInputR(const TInputImageR * image)
{
  // Process object is not const-correct so the const casting is required.
  this->SetInput1(image);
}

/**
 * Connect one of the operands for pixel-wise addition
 */
template <class TInputImageR, class TInputImageG, class TInputImageNIR,
    class TOutputImage, class TFunction>
void
RAndGAndNIRIndexImageFilter<TInputImageR, TInputImageG, TInputImageNIR, TOutputImage, TFunction>
::SetInputG(const TInputImageG * image)
{
  this->SetInput2(image);
}

/**
 * Connect one of the operands for pixel-wise addition
 */
template <class TInputImageR, class TInputImageG, class TInputImageNIR,
    class TOutputImage, class TFunction>
void
RAndGAndNIRIndexImageFilter<TInputImageR, TInputImageG, TInputImageNIR, TOutputImage, TFunction>
::SetInputNIR(const TInputImageNIR * image)
{
  this->SetInput3(image);
}

template <class TInputImageR, class TInputImageG, class TInputImageNIR, class TOutputImage, class TFunction>
void
RAndGAndNIRIndexImageFilter<TInputImageR, TInputImageG, TInputImageNIR, TOutputImage, TFunction>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

}

#endif
