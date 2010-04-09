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
#ifndef __otbImageToPathFilter_txx
#define __otbImageToPathFilter_txx

#include "otbImageToPathFilter.h"

namespace otb
{
/**
 * Constructor.
 */
template <class TInputImage, class TOutputPath>
ImageToPathFilter<TInputImage, TOutputPath>
::ImageToPathFilter()
{
  this->SetNumberOfRequiredInputs(1);
}
/**
 * Input image setter.
 */
template <class TInputImage, class TOutputPath>
void
ImageToPathFilter<TInputImage, TOutputPath>
::SetInput(const InputImageType * image)
{
  this->ProcessObjectType::SetNthInput(0, const_cast<InputImageType *>(image));
}
/**
 * Input image getter.
 */
template <class TInputImage, class TOutputPath>
const typename ImageToPathFilter<TInputImage, TOutputPath>::InputImageType *
ImageToPathFilter<TInputImage, TOutputPath>
::GetInput(void)
{
  return static_cast<const TInputImage *>(this->ProcessObjectType::GetInput(0));
}
/**
 * PrintSelf Method.
 */
template <class TInputImage, class TOutputPath>
void
ImageToPathFilter<TInputImage, TOutputPath>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // end namespace otb

#endif
