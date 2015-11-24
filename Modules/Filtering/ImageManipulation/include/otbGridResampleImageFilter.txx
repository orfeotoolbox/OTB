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
#ifndef __otbGridResampleImageFilter_txx
#define __otbGridResampleImageFilter_txx

#include "otbGridResampleImageFilter.h"

#include "otbStreamingTraits.h"

#include "itkNumericTraits.h"
#include "itkProgressReporter.h"
#include "itkImageRegionIteratorWithIndex.h"

namespace otb
{
  
template <typename TInputImage, typename TOutputImage,
          typename TInterpolatorPrecision>
GridResampleImageFilter<TInputImage, TOutputImage, TInterpolatorPrecision>
::GridResampleImageFilter()
  : m_OutputStartIndex(),
    m_OutputSize(),
    m_OutputOrigin(),
    m_OutputSpacing(),
    m_EdgePaddingValue(),
    m_CheckOutputBounds(true),
    m_Interpolator()
{
  // Set linear interpolator as default
  m_Interpolator = dynamic_cast<InterpolatorType *>(DefaultInterpolatorType::New().GetPointer());

  // Initialize EdgePaddingValue
  m_EdgePaddingValue = itk::NumericTraits<OutputPixelType>::ZeroValue(m_EdgePaddingValue);
}


/** Import output parameters from a given image */
template <typename TInputImage, typename TOutputImage,
          typename TInterpolatorPrecision>
void
GridResampleImageFilter<TInputImage, TOutputImage, TInterpolatorPrecision>
::SetOutputParametersFromImage(const ImageBaseType * image)
{
  this->SetOutputOrigin ( image->GetOrigin() );
  this->SetOutputSpacing ( image->GetSpacing() );
  this->SetOutputStartIndex ( image->GetLargestPossibleRegion().GetIndex() );
  this->SetOutputSize ( image->GetLargestPossibleRegion().GetSize() );
}

template <typename TInputImage, typename TOutputImage,
          typename TInterpolatorPrecision>
void
GridResampleImageFilter<TInputImage, TOutputImage, TInterpolatorPrecision>
::GenerateOutputInformation()
{
   // call the superclass' implementation of this method
  Superclass::GenerateOutputInformation();

  // get pointers to the input and output
  OutputImageType *outputPtr = this->GetOutput();
  if ( !outputPtr )
    {
    return;
    }

  // Fill output image data
  typename TOutputImage::RegionType outputLargestPossibleRegion;
  outputLargestPossibleRegion.SetSize(m_OutputSize);
  outputLargestPossibleRegion.SetIndex(m_OutputStartIndex);
  
  outputPtr->SetLargestPossibleRegion(outputLargestPossibleRegion);
  outputPtr->SetSpacing(m_OutputSpacing);
  outputPtr->SetOrigin(m_OutputOrigin);

  // TODO: Report no data value here
}

template <typename TInputImage, typename TOutputImage,
          typename TInterpolatorPrecision>
void
GridResampleImageFilter<TInputImage, TOutputImage, TInterpolatorPrecision>
::GenerateInputRequestedRegion()
{
  // call the superclass's implementation of this method
  Superclass::GenerateInputRequestedRegion();

  // Check for input
  if ( !this->GetInput() )
    {
    return;
    }

  // get pointers to the input and output
  typename TInputImage::Pointer inputPtr  =
    const_cast< TInputImage * >( this->GetInput() );

  // check for output
  OutputImageType *outputPtr = this->GetOutput();
  if ( !outputPtr )
    {
    return;
    }

  typename TOutputImage::RegionType outputRequestedRegion = outputPtr->GetRequestedRegion();

  IndexType outULIndex,outLRIndex, inULIndex, inLRIndex;

  // Get output image requested region corners as Index
  outULIndex = outputRequestedRegion.GetIndex();
  outLRIndex = outULIndex+outputRequestedRegion.GetSize();
  
  // Transform to physiscal points
  PointType outULPoint,outLRPoint;
  outputPtr->TransformIndexToPhysicalPoint(outULIndex,outULPoint);
  outputPtr->TransformIndexToPhysicalPoint(outLRIndex,outLRPoint);

  // Transform to input image Index
  inputPtr->TransformPhysicalPointToIndex(outULPoint,inULIndex);
  inputPtr->TransformPhysicalPointToIndex(outLRPoint,inLRIndex);

  SizeType inSize;
  
  // Reorder index properly and compute size
  for(unsigned int dim = 0; dim < ImageDimension;++dim)
    {
    if(inULIndex[dim] > inLRIndex[dim])
      {
      // swap
      typename IndexType::IndexValueType tmp(inULIndex[dim]);
      inULIndex[dim]=inLRIndex[dim];
      inLRIndex[dim]=tmp;
      }

    inSize[dim] = static_cast<typename SizeType::SizeValueType>(inLRIndex[dim]-inULIndex[dim]);
    
    }

  // Build the input requested region
  typename TInputImage::RegionType inputRequestedRegion;
  inputRequestedRegion.SetIndex(inULIndex);
  inputRequestedRegion.SetSize(inSize);


  // Compute the padding due to the interpolator
  unsigned int interpolatorRadius =
      StreamingTraits<typename Superclass::InputImageType>::CalculateNeededRadiusForInterpolator(this->GetInterpolator());
  inputRequestedRegion.PadByRadius(interpolatorRadius);

  // crop the input requested region at the input's largest possible region
  if (inputRequestedRegion.Crop(inputPtr->GetLargestPossibleRegion()))
    {
    inputPtr->SetRequestedRegion(inputRequestedRegion);
    }
  else
    {
    
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


template <typename TInputImage, typename TOutputImage,
          typename TInterpolatorPrecision>
void
GridResampleImageFilter<TInputImage, TOutputImage, TInterpolatorPrecision>
::BeforeThreadedGenerateData()
{
  if ( !m_Interpolator )
    {
    itkExceptionMacro(<< "Interpolator not set");
    }

  // Connect input image to interpolator
  m_Interpolator->SetInputImage( this->GetInput() );


  unsigned int nComponents
    = itk::DefaultConvertPixelTraits<OutputPixelType>::GetNumberOfComponents(
      m_EdgePaddingValue );

  if (nComponents == 0)
    {

    // Build a default value of the correct number of components
    OutputPixelComponentType zeroComponent
      = itk::NumericTraits<OutputPixelComponentType>::ZeroValue( zeroComponent );
    
    nComponents = this->GetInput()->GetNumberOfComponentsPerPixel();
    
    itk::NumericTraits<OutputPixelType>::SetLength(m_EdgePaddingValue, nComponents );
    for (unsigned int n=0; n<nComponents; n++)
      {
      OutputPixelConvertType::SetNthComponent( n, m_EdgePaddingValue,
                                               zeroComponent );
      }
    }

}

template <typename TInputImage, typename TOutputImage,
          typename TInterpolatorPrecision>
void
GridResampleImageFilter<TInputImage, TOutputImage, TInterpolatorPrecision>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId)
{
  // Get the output pointers
  OutputImageType *outputPtr = this->GetOutput();

  // Get this input pointers
  const InputImageType *inputPtr = this->GetInput();

  // Min/max values of the output pixel type AND these values
  // represented as the output type of the interpolator
  const OutputPixelComponentType minValue =  itk::NumericTraits< OutputPixelComponentType >::NonpositiveMin();
  const OutputPixelComponentType maxValue =  itk::NumericTraits< OutputPixelComponentType >::max();

  const InterpolatorComponentType minOutputValue = static_cast< InterpolatorComponentType >( minValue );
  const InterpolatorComponentType maxOutputValue = static_cast< InterpolatorComponentType >( maxValue );

  // Iterator on the output region for current thread
  itk::ImageRegionIteratorWithIndex<OutputImageType> outIt(outputPtr, outputRegionForThread);

  // Support for progress methods/callbacks
  itk::ProgressReporter progress( this,
                             threadId,
                             outputRegionForThread.GetNumberOfPixels() );

  // Temporary variables for loop
  PointType outPoint;
  ContinuousInputIndexType inCIndex;
  InterpolatorOutputType interpolatorValue;
  OutputPixelType outputValue;

  
  // Iterate through the output region
  for(outIt.GoToBegin();!outIt.IsAtEnd();++outIt)
    {
    // Map output index to input continuous index
    outputPtr->TransformIndexToPhysicalPoint(outIt.GetIndex(),outPoint);
    inputPtr->TransformPhysicalPointToContinuousIndex(outPoint,inCIndex);

    // If inside buffer
    if(m_Interpolator->IsInsideBuffer(inCIndex))
      {
      // Interpolate
      interpolatorValue = m_Interpolator->EvaluateAtContinuousIndex(inCIndex);

      // Cast and check bounds
      outputValue = this->CastPixelWithBoundsChecking(interpolatorValue,minOutputValue,maxOutputValue);

      // Set output value
      outIt.Set(outputValue);
      }
    else
      {
      // Else output is EdgePaddingValue
      outIt.Set(m_EdgePaddingValue);
      }

    // Report progress
    progress.CompletedPixel();
    }

}

template <typename TInputImage, typename TOutputImage,
          typename TInterpolatorPrecision>
void
GridResampleImageFilter<TInputImage, TOutputImage, TInterpolatorPrecision>
::AfterThreadedGenerateData()
{
  // Disconnect input image from the interpolator
  m_Interpolator->SetInputImage(ITK_NULLPTR);
}

/**
 * Cast from interpolator output to pixel type
 */
template< typename TInputImage, typename TOutputImage,
          typename TInterpolatorPrecision >
typename GridResampleImageFilter< TInputImage, TOutputImage, TInterpolatorPrecision >
::OutputPixelType
GridResampleImageFilter< TInputImage, TOutputImage, TInterpolatorPrecision >
::CastPixelWithBoundsChecking(const InterpolatorOutputType value,
                              const InterpolatorComponentType minComponent,
                              const InterpolatorComponentType maxComponent ) const
{
  // Method imported from itk::ResampleImageFilter
  const unsigned int nComponents = InterpolatorConvertType::GetNumberOfComponents(value);
  OutputPixelType outputValue;

  itk::NumericTraits<OutputPixelType>::SetLength( outputValue, nComponents );

  for (unsigned int n=0; n<nComponents; n++)
    {
    InterpolatorComponentType component = InterpolatorConvertType::GetNthComponent( n, value );

    if ( m_CheckOutputBounds && component < minComponent )
      {
      OutputPixelConvertType::SetNthComponent( n, outputValue, static_cast<OutputPixelComponentType>( minComponent ) );
      }
    else if ( m_CheckOutputBounds && component > maxComponent )
      {
      OutputPixelConvertType::SetNthComponent( n, outputValue, static_cast<OutputPixelComponentType>( maxComponent ) );
      }
    else
      {
      OutputPixelConvertType::SetNthComponent(n, outputValue,
                                        static_cast<OutputPixelComponentType>( component ) );
      }
    }

  return outputValue;
}



template <typename TInputImage, typename TOutputImage,
          typename TInterpolatorPrecision>
itk::ModifiedTimeType
GridResampleImageFilter< TInputImage, TOutputImage, TInterpolatorPrecision >
::GetMTime(void) const
{
  itk::ModifiedTimeType latestTime = itk::Object::GetMTime();

  if ( m_Interpolator )
    {
    if ( latestTime < m_Interpolator->GetMTime() )
      {
      latestTime = m_Interpolator->GetMTime();
      }
    }

  return latestTime;
}


template <typename TInputImage, typename TOutputImage,
          typename TInterpolatorPrecision>
void
GridResampleImageFilter<TInputImage, TOutputImage, TInterpolatorPrecision>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "EdgePaddingValue: "
     << static_cast< typename itk::NumericTraits< OutputPixelType >::PrintType >
  ( m_EdgePaddingValue )
     << std::endl;
  os << indent << "OutputStartIndex: " << m_OutputStartIndex << std::endl;
  os << indent << "OutputSize: " << m_OutputSize << std::endl;
  os << indent << "OutputOrigin: " << m_OutputOrigin << std::endl;
  os << indent << "OutputSpacing: " << m_OutputSpacing << std::endl;
  os << indent << "Interpolator: " << m_Interpolator.GetPointer() << std::endl;
  os << indent << "CheckOutputBounds: " << ( m_CheckOutputBounds ? "On" : "Off" )
     << std::endl;
}



} // namespace otb


#endif
