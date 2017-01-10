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
#ifndef otbSinclairReciprocalImageFilter_txx
#define otbSinclairReciprocalImageFilter_txx

#include "otbSinclairReciprocalImageFilter.h"
#include "otbMath.h"

namespace otb
{

/**
 * Connect one of the operands for pixel-wise addition
 */
template <class TInputImageHH, class TInputImageHV_VH, class TInputImageVV, class TOutputImage, class TFunction>
void
SinclairReciprocalImageFilter<TInputImageHH, TInputImageHV_VH, TInputImageVV, TOutputImage, TFunction>
::SetInputHH(const TInputImageHH * image)
{
  this->SetInput1(image);
}

/**
 * Connect one of the operands for pixel-wise addition
 */
template <class TInputImageHH, class TInputImageHV_VH, class TInputImageVV, class TOutputImage, class TFunction>
void
SinclairReciprocalImageFilter<TInputImageHH, TInputImageHV_VH, TInputImageVV, TOutputImage, TFunction>
::SetInputHV(const TInputImageHV_VH * image)
{
  this->SetInput2(image);
}

/**
 * Connect one of the operands for pixel-wise addition
 */
template <class TInputImageHH, class TInputImageHV_VH, class TInputImageVV, class TOutputImage, class TFunction>
void
SinclairReciprocalImageFilter<TInputImageHH, TInputImageHV_VH, TInputImageVV, TOutputImage, TFunction>
::SetInputVH(const TInputImageHV_VH * image)
{
  this->SetInput2(image);
}

/**
 * Connect one of the operands for pixel-wise addition
 */
template <class TInputImageHH, class TInputImageHV_VH, class TInputImageVV, class TOutputImage, class TFunction>
void
SinclairReciprocalImageFilter<TInputImageHH, TInputImageHV_VH, TInputImageVV, TOutputImage, TFunction>
::SetInputHV_VH(const TInputImageHV_VH * image)
{
  this->SetInput2(image);
}

/**
 * Connect one of the operands for pixel-wise addition
 */
template <class TInputImageHH, class TInputImageHV_VH, class TInputImageVV, class TOutputImage, class TFunction>
void
SinclairReciprocalImageFilter<TInputImageHH, TInputImageHV_VH, TInputImageVV, TOutputImage, TFunction>
::SetInputVV(const TInputImageVV * image)
{
  this->SetInput3(image);
}

/**
 * Printself
 */
template <class TInputImageHH, class TInputImageHV_VH, class TInputImageVV, class TOutputImage, class TFunction>
void
SinclairReciprocalImageFilter<TInputImageHH, TInputImageHV_VH, TInputImageVV, TOutputImage, TFunction>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}


template <class TInputImageHH, class TInputImageHV_VH, class TInputImageVV, class TOutputImage, class TFunction>
void
SinclairReciprocalImageFilter<TInputImageHH, TInputImageHV_VH, TInputImageVV, TOutputImage, TFunction>
::GenerateOutputInformation()
{
  // Call to the superclass implementation
  Superclass::GenerateOutputInformation();
  typename Superclass::OutputImagePointer     outputPtr = this->GetOutput();

  // initialize the number of channels of the output image
  outputPtr->SetNumberOfComponentsPerPixel( this->GetFunctor().GetNumberOfComponentsPerPixel() );

}

} // end namespace otb

#endif
