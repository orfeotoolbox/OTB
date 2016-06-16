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
#ifndef otbImageListToImageListFilter_txx
#define otbImageListToImageListFilter_txx

#include "otbImageListToImageListFilter.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TOutputImage>
ImageListToImageListFilter<TInputImage, TOutputImage>
::ImageListToImageListFilter()
{
  this->SetNumberOfRequiredInputs(1);
}
/**
 * Input Connection
 * \param imageList The input image list.
 */
template <class TInputImage, class TOutputImage>
void
ImageListToImageListFilter<TInputImage, TOutputImage>
::SetInput(const InputImageListType *imageList)
{
  // A single input image
  this->itk::ProcessObject::SetNthInput(0, const_cast<InputImageListType*>(imageList));
}
/**
 * Input image retrieval
 * \return The input image.
 */
template <class TInputImage, class TOutputImage>
typename ImageListToImageListFilter<TInputImage, TOutputImage>::InputImageListType *
ImageListToImageListFilter<TInputImage, TOutputImage>
::GetInput(void)
{
  // If there is no input
  if (this->GetNumberOfInputs() < 1)
    {
    // exit
    return ITK_NULLPTR;
    }
  // else return the first input
  return static_cast<InputImageListType *>
           (this->itk::ProcessObject::GetInput(0));
}
/**
 * PrintSelf Method
 */
template <class TInputImage, class TOutputImage>
void
ImageListToImageListFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif
