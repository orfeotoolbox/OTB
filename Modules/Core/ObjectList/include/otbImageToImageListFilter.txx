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
#ifndef __otbImageToImageListFilter_txx
#define __otbImageToImageListFilter_txx

#include "otbImageToImageListFilter.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TOutputImage>
ImageToImageListFilter<TInputImage, TOutputImage>
::ImageToImageListFilter()
{
  this->SetNumberOfRequiredInputs(1);
}
/**
 * Input Connection
 * \param image The input image.
 */
template <class TInputImage, class TOutputImage>
void
ImageToImageListFilter<TInputImage, TOutputImage>
::SetInput(const InputImageType *image)
{
  // A single input image
  this->itk::ProcessObject::SetNthInput(0, const_cast<InputImageType*>(image));
}
/**
 * Input image retrieval
 * \return The input image.
 */
template <class TInputImage, class TOutputImage>
typename ImageToImageListFilter<TInputImage, TOutputImage>::InputImageType *
ImageToImageListFilter<TInputImage, TOutputImage>
::GetInput(void)
{
  // If there is no input
  if (this->GetNumberOfInputs() < 1)
    {
    // exit
    return 0;
    }
  // else return the first input
  return static_cast<TInputImage *>
           (this->itk::ProcessObject::GetInput(0));
}
/**
 * PrintSelf Method
 */
template <class TInputImage, class TOutputImage>
void
ImageToImageListFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif
