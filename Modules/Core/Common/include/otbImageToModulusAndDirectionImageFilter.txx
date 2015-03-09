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
#ifndef __otbImageToModulusAndDirectionImageFilter_txx
#define __otbImageToModulusAndDirectionImageFilter_txx

#include "otbImageToModulusAndDirectionImageFilter.h"
#include "itkProcessObject.h"

namespace otb
{

/**
 *
 */
template <class TInputImage, class TOutputImage, class TOutputImageDirection>
ImageToModulusAndDirectionImageFilter<TInputImage, TOutputImage, TOutputImageDirection>::
ImageToModulusAndDirectionImageFilter()
{
  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredOutputs(2);
  this->SetNumberOfRequiredOutputs(2);

  this->SetNthOutput(0, OutputImageType::New());
  this->SetNthOutput(1, OutputImageDirectionType::New());
}

/** Return the const output image modulus */
template <class TInputImage, class TOutputImage, class TOutputImageDirection>
const typename ImageToModulusAndDirectionImageFilter<TInputImage, TOutputImage,
    TOutputImageDirection>::OutputImageType *
ImageToModulusAndDirectionImageFilter<TInputImage, TOutputImage, TOutputImageDirection>::
GetOutput() const
{
  if (this->GetNumberOfOutputs() < 1)
    {
    return 0;
    }
  return static_cast<const OutputImageType *>
           (this->itk::ProcessObject::GetOutput(0));
}
/** Return the output image modulus */
template <class TInputImage, class TOutputImage, class TOutputImageDirection>
typename ImageToModulusAndDirectionImageFilter<TInputImage, TOutputImage, TOutputImageDirection>::OutputImageType *
ImageToModulusAndDirectionImageFilter<TInputImage, TOutputImage, TOutputImageDirection>::
GetOutput()
{
  if (this->GetNumberOfOutputs() < 1)
    {
    return 0;
    }
  return static_cast<OutputImageType *>
           (this->itk::ProcessObject::GetOutput(0));
}

/** Return the const output image direction */
template <class TInputImage, class TOutputImage, class TOutputImageDirection>
const typename ImageToModulusAndDirectionImageFilter<TInputImage, TOutputImage,
    TOutputImageDirection>::OutputImageDirectionType *
ImageToModulusAndDirectionImageFilter<TInputImage, TOutputImage, TOutputImageDirection>::
GetOutputDirection() const
{
  if (this->GetNumberOfOutputs() < 2)
    {
    return 0;
    }
  return static_cast<const OutputImageDirectionType *>
           (this->itk::ProcessObject::GetOutput(1));
}

/** Return the output image direction */
template <class TInputImage, class TOutputImage, class TOutputImageDirection>
typename ImageToModulusAndDirectionImageFilter<TInputImage, TOutputImage,
    TOutputImageDirection>::OutputImageDirectionType *
ImageToModulusAndDirectionImageFilter<TInputImage, TOutputImage, TOutputImageDirection>::
GetOutputDirection()
{
  if (this->GetNumberOfOutputs() < 2)
    {
    return 0;
    }
  return static_cast<OutputImageDirectionType *>
           (this->itk::ProcessObject::GetOutput(1));
}

/**
 * Standard "GenerateInputRequestedRegion" method
 */
template <class TInputImage, class TOutputImage, class TOutputImageDirection>
void
ImageToModulusAndDirectionImageFilter<TInputImage, TOutputImage, TOutputImageDirection>
::GenerateInputRequestedRegion()
{
  Superclass::GenerateInputRequestedRegion();
}

/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TOutputImage, class TOutputImageDirection>
void
ImageToModulusAndDirectionImageFilter<TInputImage, TOutputImage, TOutputImageDirection>::
PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

}

} // end namespace otb

#endif
