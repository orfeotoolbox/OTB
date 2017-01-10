/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef otbImageAndVectorImageOperationFilter_txx
#define otbImageAndVectorImageOperationFilter_txx

#include "otbImageAndVectorImageOperationFilter.h"
#include "itkObjectFactory.h"

namespace otb
{

template <class TInputImage, class TVectorInputImage, class TOutputImage>
ImageAndVectorImageOperationFilter<TInputImage, TVectorInputImage, TOutputImage>
::ImageAndVectorImageOperationFilter()
{
  this->UseAddition();
}

template <class TInputImage, class TVectorInputImage, class TOutputImage>
ImageAndVectorImageOperationFilter<TInputImage, TVectorInputImage, TOutputImage>
::~ImageAndVectorImageOperationFilter()
{
}

template <class TInputImage, class TVectorInputImage, class TOutputImage>
void
ImageAndVectorImageOperationFilter<TInputImage, TVectorInputImage, TOutputImage>
::SetInput(const InputImageType *input)
{
  this->SetNthInput(0, const_cast<InputImageType *>(input));
}

template <class TInputImage, class TVectorInputImage, class TOutputImage>
void
ImageAndVectorImageOperationFilter<TInputImage, TVectorInputImage, TOutputImage>
::SetVectorInput(const VectorInputImageType *input)
{
  this->SetNthInput(1, const_cast<VectorInputImageType *>(input));
}

template <class TInputImage, class TVectorInputImage, class TOutputImage>
const typename ImageAndVectorImageOperationFilter<TInputImage, TVectorInputImage, TOutputImage>::InputImageType *
ImageAndVectorImageOperationFilter<TInputImage, TVectorInputImage, TOutputImage>
::GetInput()
{
  if (this->GetNumberOfInputs() < 1) return 0;

  return dynamic_cast<const InputImageType*>(this->GetInput(0));
}

template <class TInputImage, class TVectorInputImage, class TOutputImage>
const typename ImageAndVectorImageOperationFilter<TInputImage, TVectorInputImage, TOutputImage>::VectorInputImageType *
ImageAndVectorImageOperationFilter<TInputImage, TVectorInputImage, TOutputImage>
::GetVectorInput()
{
  if (this->GetNumberOfInputs() < 2) return ITK_NULLPTR;

  return dynamic_cast<const VectorInputImageType*>(this->itk::ProcessObject::GetInput(1));
}
/**
 *
 */
template <class TInputImage, class TVectorInputImage, class TOutputImage>
void
ImageAndVectorImageOperationFilter<TInputImage, TVectorInputImage, TOutputImage>
::GenerateOutputInformation()
{
  // call the superclass' implementation of this method
  Superclass::GenerateOutputInformation();
  this->GetOutput()->SetNumberOfComponentsPerPixel(this->GetVectorInput()->GetNumberOfComponentsPerPixel());
}

} // end namespace otb

#endif
