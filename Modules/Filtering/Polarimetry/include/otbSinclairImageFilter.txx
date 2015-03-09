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
#ifndef __otbSinclairImageFilter_txx
#define __otbSinclairImageFilter_txx

#include "otbSinclairImageFilter.h"
#include "otbMath.h"

namespace otb
{


/**
 * Connect one of the operands for pixel-wise addition
 */
template <class TInputImageHH, class TInputImageHV, class TInputImageVH, class TInputImageVV, class TOutputImage,
    class TFunction>
void
SinclairImageFilter<TInputImageHH, TInputImageHV, TInputImageVH, TInputImageVV, TOutputImage, TFunction>
::SetInputHH(const TInputImageHH * image)
{
  this->SetInput1(image);
}

/**
 * Connect one of the operands for pixel-wise addition
 */
template <class TInputImageHH, class TInputImageHV, class TInputImageVH, class TInputImageVV, class TOutputImage,
    class TFunction>
void
SinclairImageFilter<TInputImageHH, TInputImageHV, TInputImageVH, TInputImageVV, TOutputImage, TFunction>
::SetInputHV(const TInputImageHV * image)
{
  this->SetInput2(image);
}

/**
 * Connect one of the operands for pixel-wise addition
 */
template <class TInputImageHH, class TInputImageHV, class TInputImageVH, class TInputImageVV, class TOutputImage,
    class TFunction>
void
SinclairImageFilter<TInputImageHH, TInputImageHV, TInputImageVH, TInputImageVV, TOutputImage, TFunction>
::SetInputVH(const TInputImageVH * image)
{
  this->SetInput3(image);
}

/**
 * Connect one of the operands for pixel-wise addition
 */
template <class TInputImageHH, class TInputImageHV, class TInputImageVH, class TInputImageVV, class TOutputImage,
    class TFunction>
void
SinclairImageFilter<TInputImageHH, TInputImageHV, TInputImageVH, TInputImageVV, TOutputImage, TFunction>
::SetInputVV(const TInputImageVV * image)
{
  this->SetInput4(image);
}

/**
 * Printself
 */
template <class TInputImageHH, class TInputImageHV, class TInputImageVH, class TInputImageVV, class TOutputImage,
    class TFunction>
void
SinclairImageFilter<TInputImageHH, TInputImageHV, TInputImageVH, TInputImageVV, TOutputImage, TFunction>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}


template <class TInputImageHH, class TInputImageHV, class TInputImageVH, class TInputImageVV, class TOutputImage,
    class TFunction>
void
SinclairImageFilter<TInputImageHH, TInputImageHV, TInputImageVH, TInputImageVV, TOutputImage, TFunction>
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
