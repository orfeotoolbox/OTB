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
  this->m_MinHDisp = -5;
  this->m_MaxHDisp = 5;
  this->m_MinVDisp = -5;
  this->m_MaxVDisp = 5;

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
  
  const TDisparityImage * directVmap = this->GetDirectVerticalDisparityMapInput();
  const TDisparityImage * reverseVmap = this->GetReverseVerticalDisparityMapInput();
  
  if (!directHmap)
    {
    itkExceptionMacro(<<"Direct horizontal disparity map is missing");
    }
  
  if (!reverseHmap)
    {
    itkExceptionMacro(<<"Reverse horizontal disparity map is missing");
    }
  
  if (directVmap && directVmap->GetLargestPossibleRegion() != directHmap->GetLargestPossibleRegion())
    {
    itkExceptionMacro(<<"Horizontal and vertical direct disparity maps have different sizes.");
    }
  
  if (reverseVmap && reverseVmap->GetLargestPossibleRegion() != reverseHmap->GetLargestPossibleRegion())
    {
    itkExceptionMacro(<<"Horizontal and vertical reverse disparity maps have different sizes.");
    }
  
  if (this->m_MinHDisp > this->m_MaxHDisp)
    {
    itkExceptionMacro(<<"Wrong horizontal exploration values");
    }
  if (this->m_MinVDisp > this->m_MaxVDisp)
    {
    itkExceptionMacro(<<"Wrong horizontal exploration values");
    }
}

template <class TDisparityImage, class TOutputImage>
void
BijectionCoherencyFilter<TDisparityImage,TOutputImage>
::GenerateInputRequestedRegion()
{
  this->Superclass::GenerateInputRequestedRegion();
  
  OutputRegionType requested = this->GetOutput()->GetRequestedRegion();
  InputRegionType  directLargest = this->GetDirectHorizontalDisparityMap()->GetLargestPossibleRegion();
  InputRegionType  directRequested;
  InputRegionType  reverseLargest = this->GetReverseHorizontalDisparityMap()->GetLargestPossibleRegion();
  InputRegionType  reverseRequested;
  
  this->CallCopyOutputRegionToInputRegion(requested,directRequested);
  
  reverseRequested.SetIndex(0,requested.GetIndex(0) + this->m_MinHDisp);
  reverseRequested.SetIndex(1,requested.GetIndex(1) + this->m_MinVDisp);
  reverseRequested.SetSize(0,requested.GetSize(0) + this->m_MaxHDisp - this->m_MinHDisp);
  reverseRequested.SetSize(1,requested.GetSize(1) + this->m_MaxVDisp - this->m_MinVDisp);
  
  reverseRequested.Crop(reverseLargest);
  
  TDisparityImage * directHmap = const_cast<TDisparityImage *>(this->GetDirectHorizontalDisparityMapInput());
  TDisparityImage * directVmap = const_cast<TDisparityImage *>(this->GetDirectVerticalDisparityMapInput());
  TDisparityImage * reverseHmap = const_cast<TDisparityImage *>(this->GetDirectHorizontalDisparityMapInput());
  TDisparityImage * reverseVmap = const_cast<TDisparityImage *>(this->GetDirectVerticalDisparityMapInput());
  
  directHmap->SetRequestedRegion(directRequested);
  if (directVmap) directVmap->SetRequestedRegion(directRequested);
  
  reverseHmap->SetRequestedRegion(reverseRequested);
  if (reverseVmap) reverseVmap->SetRequestedRegion(reverseRequested);
}

template <class TDisparityImage, class TOutputImage>
void
BijectionCoherencyFilter<TDisparityImage,TOutputImage>
::ThreadedGenerateData(const RegionType & outputRegionForThread, int threadId)
{
  const TDisparityImage * directHmap = this->GetDirectHorizontalDisparityMapInput();
  const TDisparityImage * directVmap = this->GetDirectVerticalDisparityMapInput();
  const TDisparityImage * reverseHmap = this->GetDirectHorizontalDisparityMapInput();
  const TDisparityImage * reverseVmap = this->GetDirectVerticalDisparityMapInput();
  
  TOutputImage * output = this->GetOutput();
  
  InputRegionType buffered = reverseHmap->GetBufferedRegion();
  
  typedef itk::ImageRegionIterator<TOutputImage> MaskIteratorType;
  MaskIteratorType outIter = MaskIteratorType(output,outputRegionForThread);
  
  typedef itk::ImageRegionConstIteratorWithIndex<TDisparityImage> DispIteratorType;
  DispIteratorType directHorizIter = DispIteratorType(directHmap,outputRegionForThread);
  
  DispIteratorType directVertiIter;
  if (directVmap)
    {
    directVertiIter = DispIteratorType(directVmap,outputRegionForThread);
    directVertiIter.GoToBegin();
    }
  
  outIter.GoToBegin();
  directHorizIter.GoToBegin();
  
  while (!outIter.IsAtEnd())
  {
    IndexType startIndex = directHorizIter.GetIndex();
    itk::ContinuousIndex<double,2> tmpIndex(startIndex);
    
    tmpIndex[0] += directHorizIter.Get();
    if (directVmap) tmpIndex[1] += directVertiIter.Get();
    
    // Interpolate in reverse disparity map
    IndexType ul,ur,ll,lr;
    ul[0] = static_cast<long>(vcl_floor(tmpIndex[0]));
    ul[1] = static_cast<long>(vcl_floor(tmpIndex[1]));
    if (ul[0]<buffered.GetIndex()[0]) ul[0]=buffered.GetIndex()[0];
    if (ul[1]<buffered.GetIndex()[1]) ul[1]=buffered.GetIndex()[1];
    if (ul[0]>(buffered.GetIndex()[0]+buffered.GetSize()[0]-1)) ul[0]=(buffered.GetIndex()[0]+buffered.GetSize()[0]-1);
    if (ul[1]>(buffered.GetIndex()[1]+buffered.GetSize()[1]-1)) ul[1]=(buffered.GetIndex()[1]+buffered.GetSize()[1]-1);
  
    ur = ul;
    ur[0] += 1;
    ll = ul;
    ll[1] += 1;
    lr = ul;
    lr[0] += 1;
    lr[1] += 1;
    
    double rx = tmpIndex[0] - static_cast<double>(ul[0]);
    double ry = tmpIndex[1] - static_cast<double>(ul[1]);
    
    itk::ContinuousIndex<double,2> backIndex(tmpIndex);
    backIndex[0] += (1. - ry) * ((1. - rx) * reverseHmap->GetPixel(ul) + rx * reverseHmap->GetPixel(ur)) +
                            ry * ((1. - rx) * reverseHmap->GetPixel(ll) + rx * reverseHmap->GetPixel(lr));
    if (reverseVmap)
      {
      backIndex[1] += (1. - ry) * ((1. - rx) * reverseVmap->GetPixel(ul) + rx * reverseVmap->GetPixel(ur)) +
                          ry * ((1. - rx) * reverseVmap->GetPixel(ll) + rx * reverseVmap->GetPixel(lr));
      }
    
    if (vcl_abs(backIndex[0] - static_cast<double>(startIndex[0]))< this->m_Tolerance &&
        vcl_abs(backIndex[1] - static_cast<double>(startIndex[1]))< this->m_Tolerance)
      {
      outIter.Set(255);
      }
    else
      {
      outIter.Set(0);
      }
    
    ++outIter;
    ++directHorizIter;
    if (directVmap) ++directVertiIter;
  }
  
  
}

}

#endif
