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
#ifndef __otbBijectionCoherencyFilter_txx
#define __otbBijectionCoherencyFilter_txx

#include "otbBijectionCoherencyFilter.h"

#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkImageRegionIterator.h"

namespace otb
{

template <class TDisparityImage, class TOutputImage>
BijectionCoherencyFilter<TDisparityImage,TOutputImage>
::BijectionCoherencyFilter()
{
  // Set the number of inputs (1 moving image by default -> 3 inputs)
  this->SetNumberOfInputs(4);
  this->SetNumberOfRequiredInputs(1);
  this->m_Tolerance = 1.;

  // Set the outputs
  this->SetNumberOfOutputs(1);
  this->SetNthOutput(0,TOutputImage::New());
}

template <class TDisparityImage, class TOutputImage>
void
BijectionCoherencyFilter<TDisparityImage,TOutputImage>
::SetDirectHorizontalDisparityMapInput( const TDisparityImage * hmap )
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(0, const_cast<TDisparityImage *>( hmap ));
}

template <class TDisparityImage, class TOutputImage>
void
BijectionCoherencyFilter<TDisparityImage,TOutputImage>
::SetDirectVerticalDisparityMapInput( const TDisparityImage * vmap )
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(1, const_cast<TDisparityImage *>( vmap ));
}

template <class TDisparityImage, class TOutputImage>
void
BijectionCoherencyFilter<TDisparityImage,TOutputImage>
::SetReverseHorizontalDisparityMapInput( const TDisparityImage * hmap )
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(2, const_cast<TDisparityImage *>( hmap ));
}

template <class TDisparityImage, class TOutputImage>
void
BijectionCoherencyFilter<TDisparityImage,TOutputImage>
::SetReverseVerticalDisparityMapInput( const TDisparityImage * vmap )
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(3, const_cast<TDisparityImage *>( vmap ));
}

template <class TDisparityImage, class TOutputImage>
const TDisparityImage *
BijectionCoherencyFilter<TDisparityImage,TOutputImage>
::GetDirectHorizontalDisparityMapInput() const
{
  if (this->GetNumberOfInputs()<1)
    {
    return 0;
    }
  return static_cast<const TDisparityImage *>(this->itk::ProcessObject::GetInput(0));
}

template <class TDisparityImage, class TOutputImage>
const TDisparityImage *
BijectionCoherencyFilter<TDisparityImage,TOutputImage>
::GetDirectVerticalDisparityMapInput() const
{
  if (this->GetNumberOfInputs()<2)
    {
    return 0;
    }
  return static_cast<const TDisparityImage *>(this->itk::ProcessObject::GetInput(1));
}

template <class TDisparityImage, class TOutputImage>
const TDisparityImage *
BijectionCoherencyFilter<TDisparityImage,TOutputImage>
::GetReverseHorizontalDisparityMapInput() const
{
  if (this->GetNumberOfInputs()<3)
    {
    return 0;
    }
  return static_cast<const TDisparityImage *>(this->itk::ProcessObject::GetInput(2));
}

template <class TDisparityImage, class TOutputImage>
const TDisparityImage *
BijectionCoherencyFilter<TDisparityImage,TOutputImage>
::GetReverseVerticalDisparityMapInput() const
{
  if (this->GetNumberOfInputs()<4)
    {
    return 0;
    }
  return static_cast<const TDisparityImage *>(this->itk::ProcessObject::GetInput(3));
}

template <class TDisparityImage, class TOutputImage>
void
BijectionCoherencyFilter<TDisparityImage,TOutputImage>
::GenerateOutputInformation()
{
  this->Superclass::GenerateOutputInformation();
  
  // check that both direct and reverse disparity maps are present
  const TDisparityImage * directHmap = this->GetDirectHorizontalDisparityMapInput();
  const TDisparityImage * reverseHmap = this->GetReverseHorizontalDisparityMapInput();
  
  if (!directHmap)
    {
    itkExceptionMacro(<<"Direct horizontal disparity map is missing");
    }
  
  if (!reverseHmap)
    {
    itkExceptionMacro(<<"Reverse horizontal disparity map is missing");
    }
  
  // TODO : check sizes
  
}

template <class TDisparityImage, class TOutputImage>
void
BijectionCoherencyFilter<TDisparityImage,TOutputImage>
::GenerateInputRequestedRegion()
{
  this->Superclass::GenerateInputRequestedRegion();
  
  OutputRegionType requested = this->GetOutput()->GetRequestedRegion();
  InputRegionType  largest = this->GetDirectHorizontalDisparityMap()->GetLargestPossibleRegion();
  InputRegionType  inputRequested;
  this->CallCopyOutputRegionToInputRegion(inputRequested,requested);
  
  // TODO : the requested region on Reverse dispmap must be shifted with an estimate of the min/max disparities values
  // HOW CAN I DO THAT ???
  
  SizeType radius;
  radius.Fill(static_cast<unsigned int>(vcl_ceil(this->m_Tolerance)));
  
  inputRequested.PadByRadius(radius);
  
  inputRequested.Crop(largest);
  
  TDisparityImage * directHmap = const_cast<TDisparityImage *>(this->GetDirectHorizontalDisparityMapInput());
  TDisparityImage * directVmap = const_cast<TDisparityImage *>(this->GetDirectVerticalDisparityMapInput());
  
  
}


}

#endif
