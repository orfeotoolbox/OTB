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

namespace otb
{

template <class TInputImage, class TOutputImage, class TBoundaryCondition>
ConvolutionImageFilter<TInputImage, TOutputImage, TBoundaryCondition>
::ConvolutionImageFilter()
{
  typedef itk::ZeroFluxNeumannBoundaryCondition<InputImageType> DefaultBoundaryConditionType;
  m_Radius.Fill(1);
  m_Filter.SetSize(3*3);
  m_Filter.Fill(1);
  m_NormalizeFilter = false;
}

template <class TInputImage, class TOutputImage, class TBoundaryCondition>
void
ConvolutionImageFilter<TInputImage, TOutputImage, TBoundaryCondition>
::GenerateInputRequestedRegion() throw (itk::InvalidRequestedRegionError)
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();

  // get pointers to the input and output
  typename Superclass::InputImagePointer inputPtr =
    const_cast< TInputImage * >( this->GetInput() );
  typename Superclass::OutputImagePointer outputPtr = this->GetOutput();

  if ( !inputPtr || !outputPtr )
    {
    return;
    }

  // get a copy of the input requested region (should equal the output
  // requested region)
  typename TInputImage::RegionType inputRequestedRegion;
  inputRequestedRegion = inputPtr->GetRequestedRegion();

  // pad the input requested region by the operator radius
  inputRequestedRegion.PadByRadius( m_Radius );

  // crop the input requested region at the input's largest possible region
  if ( inputRequestedRegion.Crop(inputPtr->GetLargestPossibleRegion()) )
    {
    inputPtr->SetRequestedRegion( inputRequestedRegion );
    return;
    }
  else
    {
    // Couldn't crop the region (requested region is outside the largest
    // possible region).  Throw an exception.

    // store what we tried to request (prior to trying to crop)
    inputPtr->SetRequestedRegion( inputRequestedRegion );

    // build an exception
    itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
    e.SetLocation(ITK_LOCATION);
    e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
    e.SetDataObject(inputPtr);
    throw e;
    }
}


template< class TInputImage, class TOutputImage, class TBoundaryCondition>
void
ConvolutionImageFilter< TInputImage, TOutputImage, TBoundaryCondition>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                       int threadId)
{

  BoundaryConditionType nbc;
  unsigned int i;
  itk::ConstNeighborhoodIterator<InputImageType> bit;
  itk::ImageRegionIterator<OutputImageType> it;

  // Allocate output
  typename OutputImageType::Pointer output = this->GetOutput();
  typename  InputImageType::ConstPointer input  = this->GetInput();

  // Find the data-set boundary "faces"
  typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType>::FaceListType faceList;
  itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType> bC;
  faceList = bC(input, outputRegionForThread, m_Radius);

  typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType>::FaceListType::iterator fit;

  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  InputRealType sum;
  InputRealType norm;

  // Process each of the boundary faces.  These are N-d regions which border
  // the edge of the buffer.
  for (fit=faceList.begin(); fit != faceList.end(); ++fit)
  {
    bit = itk::ConstNeighborhoodIterator<InputImageType>(m_Radius,
        input, *fit);

    it = itk::ImageRegionIterator<OutputImageType>(output, *fit);
    bit.OverrideBoundaryCondition(&nbc);
    bit.GoToBegin();
    unsigned int neighborhoodSize = bit.Size();

    while ( ! bit.IsAtEnd() )
    {
      sum = itk::NumericTraits<InputRealType>::Zero;
      norm = itk::NumericTraits<InputRealType>::Zero;
      for (i = 0; i < neighborhoodSize; ++i)
      {
        sum += static_cast<InputRealType>( bit.GetPixel(i)*m_Filter(i) );
        norm += static_cast<InputRealType>( m_Filter(i) );
      }

      // get the mean value
      if (m_NormalizeFilter)
      {
        it.Set( static_cast<OutputPixelType>(sum / double(norm)) );
      }
      else
      {
        it.Set( static_cast<OutputPixelType>(sum));
      }

      ++bit;
      ++it;
      progress.CompletedPixel();
    }
  }
}

/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TOutput, class TBoundaryCondition>
void
ConvolutionImageFilter<TInputImage, TOutput, TBoundaryCondition>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf( os, indent );
  os << indent << "Radius: " << m_Radius << std::endl;

}

} // end namespace otb

#endif
