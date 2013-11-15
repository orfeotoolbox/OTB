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
#ifndef __otbConvolutionImageFilter_txx
#define __otbConvolutionImageFilter_txx
#include "otbConvolutionImageFilter.h"

#include "itkConstNeighborhoodIterator.h"
#include "itkNeighborhoodInnerProduct.h"
#include "itkImageRegionIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkOffset.h"
#include "itkProgressReporter.h"
#include "itkConstantBoundaryCondition.h"

#include "otbMacro.h"

namespace otb
{

template <class TInputImage, class TOutputImage, class TBoundaryCondition, class TFilterPrecision>
ConvolutionImageFilter<TInputImage, TOutputImage, TBoundaryCondition, TFilterPrecision>
::ConvolutionImageFilter()
{
  m_Radius.Fill(1);
  m_Filter.SetSize(3 * 3);
  m_Filter.Fill(1);
  m_NormalizeFilter = false;
}

template <class TInputImage, class TOutputImage, class TBoundaryCondition, class TFilterPrecision>
void
ConvolutionImageFilter<TInputImage, TOutputImage, TBoundaryCondition, TFilterPrecision>
::GenerateInputRequestedRegion() throw (itk::InvalidRequestedRegionError)
  {
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();

  // get pointers to the input and output
  typename Superclass::InputImagePointer inputPtr =
    const_cast<TInputImage *>(this->GetInput());
  typename Superclass::OutputImagePointer outputPtr = this->GetOutput();

  if (!inputPtr || !outputPtr)
    {
    return;
    }

  // get a copy of the input requested region (should equal the output
  // requested region)
  typename TInputImage::RegionType inputRequestedRegion;
  inputRequestedRegion = inputPtr->GetRequestedRegion();

  // pad the input requested region by the operator radius
  inputRequestedRegion.PadByRadius(m_Radius);
  otbMsgDevMacro(<< "Padding by " << m_Radius);
  otbMsgDevMacro(<< "Region is now " << inputRequestedRegion.GetIndex() << ", " << inputRequestedRegion.GetSize());

  // crop the input requested region at the input's largest possible region
  if (inputRequestedRegion.Crop(inputPtr->GetLargestPossibleRegion()))
    {
    inputPtr->SetRequestedRegion(inputRequestedRegion);
    return;
    }
  else
    {
    // Couldn't crop the region (requested region is outside the largest
    // possible region).  Throw an exception.

    // store what we tried to request (prior to trying to crop)
    inputPtr->SetRequestedRegion(inputRequestedRegion);

    // build an exception
    itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
    e.SetLocation(ITK_LOCATION);
    e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
    e.SetDataObject(inputPtr);
    throw e;
    }
  }

template<class TInputImage, class TOutputImage, class TBoundaryCondition, class TFilterPrecision>
void
ConvolutionImageFilter<TInputImage, TOutputImage, TBoundaryCondition, TFilterPrecision>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                       itk::ThreadIdType threadId)
{
  unsigned int i;

  // Allocate output
  typename OutputImageType::Pointer     output = this->GetOutput();
  typename InputImageType::ConstPointer input  = this->GetInput();

  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  InputRealType sum = itk::NumericTraits<InputRealType>::Zero;
  InputRealType norm = itk::NumericTraits<InputRealType>::Zero;

  InputImageRegionType inputRegionForThread;
  this->CallCopyOutputRegionToInputRegion(inputRegionForThread, outputRegionForThread);

  itk::ConstNeighborhoodIterator<InputImageType, BoundaryConditionType>  inputIt(m_Radius, input, inputRegionForThread);
  itk::ImageRegionIterator<OutputImageType> outputIt(output, outputRegionForThread);

  inputIt.GoToBegin();
  unsigned int neighborhoodSize = inputIt.Size();

  // Compute the norm of the filter
  if (m_NormalizeFilter)
    {
    norm = itk::NumericTraits<InputRealType>::Zero;
    for (i = 0; i < neighborhoodSize; ++i)
      {
      norm += static_cast<InputRealType>(vcl_abs(m_Filter(i)));
      }
    }

  while (!inputIt.IsAtEnd())
    {
    sum = itk::NumericTraits<InputRealType>::Zero;

    for (i = 0; i < neighborhoodSize; ++i)
      {
      sum += static_cast<InputRealType>(inputIt.GetPixel(i) * m_Filter(i));
      }

    // get the mean value
    if (m_NormalizeFilter)
      {
      outputIt.Set(static_cast<OutputPixelType>(sum / double(norm)));
      }
    else
      {
      outputIt.Set(static_cast<OutputPixelType>(sum));
      }

    ++inputIt;
    ++outputIt;
    progress.CompletedPixel();
    }
}

/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TOutput, class TBoundaryCondition, class TFilterPrecision>
void
ConvolutionImageFilter<TInputImage, TOutput, TBoundaryCondition, TFilterPrecision>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Radius: " << m_Radius << std::endl;

}

} // end namespace otb

#endif
