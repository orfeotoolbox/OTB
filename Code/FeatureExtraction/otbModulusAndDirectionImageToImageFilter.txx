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
#ifndef __otbModulusAndDirectionImageToImageFilter_txx
#define __otbModulusAndDirectionImageToImageFilter_txx

#include "otbModulusAndDirectionImageToImageFilter.h"
#include "itkProcessObject.h"

namespace otb
{

/**
 *
 */
template <class TInputImage, class TOutputImage, class TOutputImageDirection>
ModulusAndDirectionImageToImageFilter<TInputImage, TOutputImage, TOutputImageDirection>::
ModulusAndDirectionImageToImageFilter()
{
  this->SetNumberOfRequiredInputs(2);
  this->SetNumberOfRequiredInputs(2);
  this->SetNumberOfRequiredOutputs(1);
  this->SetNumberOfRequiredOutputs(1);

  this->SetNthOutput(0, OutputImageType::New());
}

template <class TInputImage, class TInputImageDirection, class TOutputImage>
void
ModulusAndDirectionImageToImageFilter<TInputImage, TInputImageDirection, TOutputImage>::
SetInput(const InputImageType *input)
{
  // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(0,
                                        const_cast<InputImageType *>(input));
}

template <class TInputImage, class TInputImageDirection, class TOutputImage>
void
ModulusAndDirectionImageToImageFilter<TInputImage, TInputImageDirection, TOutputImage>::
SetInputDirection(const InputImageDirectionType *direction)
{
  // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(1,
                                        const_cast<InputImageDirectionType *>(direction));
}

/** Return the input image modulus */
template <class TInputImage, class TInputImageDirection, class TOutputImage>
const typename ModulusAndDirectionImageToImageFilter<TInputImage, TInputImageDirection, TOutputImage>::InputImageType *
ModulusAndDirectionImageToImageFilter<TInputImage, TInputImageDirection, TOutputImage>::
GetInput(void)
{
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }

  return static_cast<const TInputImage *>
           (this->itk::ProcessObject::GetInput(0));
}

/** Return the intput image direction */
template <class TInputImage, class TInputImageDirection, class TOutputImage>
const typename ModulusAndDirectionImageToImageFilter<TInputImage, TInputImageDirection,
    TOutputImage>::InputImageDirectionType *
ModulusAndDirectionImageToImageFilter<TInputImage, TInputImageDirection, TOutputImage>::
GetInputDirection(void)
{
  if (this->GetNumberOfInputs() < 2)
    {
    return 0;
    }

  return static_cast<const TInputImageDirection *>
           (this->itk::ProcessObject::GetInput(1));

}

/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TInputImageDirection, class TOutputImage>
void
ModulusAndDirectionImageToImageFilter<TInputImage, TInputImageDirection, TOutputImage>::
PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

}

} // end namespace otb

#endif
