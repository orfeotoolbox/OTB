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
#ifndef otbImageListToRCC8GraphFilter_txx
#define otbImageListToRCC8GraphFilter_txx

#include "otbImageListToRCC8GraphFilter.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TOutputGraph>
ImageListToRCC8GraphFilter<TInputImage, TOutputGraph>
::ImageListToRCC8GraphFilter()
{
  this->SetNumberOfRequiredInputs(1);
}
/**
 * Input Connection
 * \param image The input image.
 */
template <class TInputImage, class TOutputGraph>
void
ImageListToRCC8GraphFilter<TInputImage, TOutputGraph>
::SetInput(const InputImageListType *imageList)
{
  // A single input image
  this->itk::ProcessObject::SetNthInput(0, const_cast<InputImageListType*>(imageList));
}
/**
 * Input image retrieval
 * \return The input image.
 */
template <class TInputImage, class TOutputGraph>
typename ImageListToRCC8GraphFilter<TInputImage, TOutputGraph>::InputImageListType *
ImageListToRCC8GraphFilter<TInputImage, TOutputGraph>
::GetInput(void)
{
  // If there is no input
  if (this->GetNumberOfInputs() < 1)
    {
    // exit
    return 0;
    }
  // else return the first input
  return static_cast<InputImageListType *>
           (this->itk::ProcessObject::GetInput(0));
}
/**
 * PrintSelf Method
 */
template <class TInputImage, class TOutputGraph>
void
ImageListToRCC8GraphFilter<TInputImage, TOutputGraph>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif
