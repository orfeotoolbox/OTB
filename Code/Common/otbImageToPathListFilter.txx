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

#ifndef __otbImageToPathListFilter_txx
#define __otbImageToPathListFilter_txx

#include "otbImageToPathListFilter.h"
#include "itkProcessObject.h"

namespace otb
{

/**
 *
 */
template <class TInputImage, class TOutputPath>
ImageToPathListFilter<TInputImage, TOutputPath>
::ImageToPathListFilter()
{
  // Modify superclass default values, can be overridden by subclasses
  this->SetNumberOfRequiredInputs(1);
}

/**
 *
 */
template <class TInputImage, class TOutputPath>
void
ImageToPathListFilter<TInputImage, TOutputPath>
::SetInput(const InputImageType *image)
{
  // We have 1 input:  an image

  // Process object is not const-correct so the const_cast is required here
  this->ProcessObjectType::SetNthInput(0, const_cast<InputImageType *>(image));
}

template <class TInputImage, class TOutputPath>
const typename ImageToPathListFilter<TInputImage, TOutputPath>::InputImageType *
ImageToPathListFilter<TInputImage, TOutputPath>
::GetInput(void)
{

  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }

  return static_cast<const TInputImage *>
           (this->ProcessObjectType::GetInput(0));
}

/**
 *
 */
template <class TInputImage, class TOutputPath>
void
ImageToPathListFilter<TInputImage, TOutputPath>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
