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
#ifndef _otbStreamingStatisticsImageFilter_txx
#define _otbStreamingStatisticsImageFilter_txx
#include "otbStreamingStatisticsImageFilter.h"

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkNumericTraits.h"
#include "itkProgressReporter.h"

namespace otb {

template<class TInputImage>
StreamingStatisticsImageFilter<TInputImage>
::StreamingStatisticsImageFilter(): m_ThreadSum(1), m_SumOfSquares(1), m_Count(1), m_ThreadMin(1), m_ThreadMax(1)
{
  // first output is a copy of the image, DataObject created by
  // superclass
  //
  // allocate the data objects for the outputs which are
  // just decorators around pixel types
  for (int i=1; i < 3; ++i)
    {
    typename PixelObjectType::Pointer output
      = static_cast<PixelObjectType*>(this->MakeOutput(i).GetPointer());
    this->itk::ProcessObject::SetNthOutput(i, output.GetPointer());
    }
  // allocate the data objects for the outputs which are
  // just decorators around real types
  for (int i=3; i < 7; ++i)
    {
    typename RealObjectType::Pointer output
      = static_cast<RealObjectType*>(this->MakeOutput(i).GetPointer());
    this->itk::ProcessObject::SetNthOutput(i, output.GetPointer());
    }

  this->GetMinimumOutput()->Set( itk::NumericTraits<PixelType>::max() );
  this->GetMaximumOutput()->Set( itk::NumericTraits<PixelType>::NonpositiveMin() );
  this->GetMeanOutput()->Set( itk::NumericTraits<RealType>::max() );
  this->GetSigmaOutput()->Set( itk::NumericTraits<RealType>::max() );
  this->GetVarianceOutput()->Set( itk::NumericTraits<RealType>::max() );
  this->GetSumOutput()->Set( itk::NumericTraits<RealType>::Zero );
  
  // Streaming initialization
  m_BufferMemorySize = 0;
  m_BufferNumberOfLinesDivisions = 0;
  m_NumberOfStreamDivisions = 0;
  // default to AUTOMATIC_NUMBER_OF_DIVISIONS
  m_StreamingMode = SET_AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS;
}


template<class TInputImage>
typename itk::DataObject::Pointer
StreamingStatisticsImageFilter<TInputImage>
::MakeOutput(unsigned int output)
{
  switch (output)
    {
   case 0:
      return static_cast<itk::DataObject*>(TInputImage::New().GetPointer());
      break;
    case 1:
      return static_cast<itk::DataObject*>(PixelObjectType::New().GetPointer());
      break;
    case 2:
      return static_cast<itk::DataObject*>(PixelObjectType::New().GetPointer());
      break;
    case 3:
    case 4:
    case 5:
    case 6:
      return static_cast<itk::DataObject*>(RealObjectType::New().GetPointer());
      break;
    default:
      // might as well make an image
      return static_cast<itk::DataObject*>(TInputImage::New().GetPointer());
      break;
    }
}


template<class TInputImage>
typename StreamingStatisticsImageFilter<TInputImage>::PixelObjectType*
StreamingStatisticsImageFilter<TInputImage>
::GetMinimumOutput()
{
  return static_cast<PixelObjectType*>(this->itk::ProcessObject::GetOutput(1));
}

template<class TInputImage>
const typename StreamingStatisticsImageFilter<TInputImage>::PixelObjectType*
StreamingStatisticsImageFilter<TInputImage>
::GetMinimumOutput() const
{
  return static_cast<const PixelObjectType*>(this->itk::ProcessObject::GetOutput(1));
}


template<class TInputImage>
typename StreamingStatisticsImageFilter<TInputImage>::PixelObjectType*
StreamingStatisticsImageFilter<TInputImage>
::GetMaximumOutput()
{
  return static_cast<PixelObjectType*>(this->itk::ProcessObject::GetOutput(2));
}

template<class TInputImage>
const typename StreamingStatisticsImageFilter<TInputImage>::PixelObjectType*
StreamingStatisticsImageFilter<TInputImage>
::GetMaximumOutput() const
{
  return static_cast<const PixelObjectType*>(this->itk::ProcessObject::GetOutput(2));
}


template<class TInputImage>
typename StreamingStatisticsImageFilter<TInputImage>::RealObjectType*
StreamingStatisticsImageFilter<TInputImage>
::GetMeanOutput()
{
  return static_cast<RealObjectType*>(this->itk::ProcessObject::GetOutput(3));
}

template<class TInputImage>
const typename StreamingStatisticsImageFilter<TInputImage>::RealObjectType*
StreamingStatisticsImageFilter<TInputImage>
::GetMeanOutput() const
{
  return static_cast<const RealObjectType*>(this->itk::ProcessObject::GetOutput(3));
}


template<class TInputImage>
typename StreamingStatisticsImageFilter<TInputImage>::RealObjectType*
StreamingStatisticsImageFilter<TInputImage>
::GetSigmaOutput()
{
  return static_cast<RealObjectType*>(this->itk::ProcessObject::GetOutput(4));
}

template<class TInputImage>
const typename StreamingStatisticsImageFilter<TInputImage>::RealObjectType*
StreamingStatisticsImageFilter<TInputImage>
::GetSigmaOutput() const
{
  return static_cast<const RealObjectType*>(this->itk::ProcessObject::GetOutput(4));
}


template<class TInputImage>
typename StreamingStatisticsImageFilter<TInputImage>::RealObjectType*
StreamingStatisticsImageFilter<TInputImage>
::GetVarianceOutput()
{
  return static_cast<RealObjectType*>(this->itk::ProcessObject::GetOutput(5));
}

template<class TInputImage>
const typename StreamingStatisticsImageFilter<TInputImage>::RealObjectType*
StreamingStatisticsImageFilter<TInputImage>
::GetVarianceOutput() const
{
  return static_cast<const RealObjectType*>(this->itk::ProcessObject::GetOutput(5));
}


template<class TInputImage>
typename StreamingStatisticsImageFilter<TInputImage>::RealObjectType*
StreamingStatisticsImageFilter<TInputImage>
::GetSumOutput()
{
  return static_cast<RealObjectType*>(this->itk::ProcessObject::GetOutput(6));
}

template<class TInputImage>
const typename StreamingStatisticsImageFilter<TInputImage>::RealObjectType*
StreamingStatisticsImageFilter<TInputImage>
::GetSumOutput() const
{
  return static_cast<const RealObjectType*>(this->itk::ProcessObject::GetOutput(6));
}

template<class TInputImage>
void
StreamingStatisticsImageFilter<TInputImage>
::GenerateInputRequestedRegion()
{
  Superclass::GenerateInputRequestedRegion();
  if ( this->GetInput() )
    {
    InputImagePointer image =
      const_cast< typename Superclass::InputImageType * >( this->GetInput() );
    IndexType index = image->GetLargestPossibleRegion().GetIndex();
    SizeType size;
    size.Fill(0);
    RegionType region;
    region.SetSize(size);
    region.SetIndex(index);
    image->SetRequestedRegion(region);
    }
}

template<class TInputImage>
void
StreamingStatisticsImageFilter<TInputImage>
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();
  if ( this->GetInput() )
    {
      this->GetOutput()->CopyInformation(this->GetInput());
      this->GetOutput()->SetLargestPossibleRegion(this->GetInput()->GetLargestPossibleRegion());
      //otbMsgDebugMacro(<<"GenerateOutputInformation() - output largest: "<<this->GetOutput()->GetLargestPossibleRegion());
    }
}

template<class TInputImage>
void
StreamingStatisticsImageFilter<TInputImage>
::AllocateOutputs()
{
  // Pass the input through as the output
  InputImagePointer image =
    const_cast< TInputImage * >( this->GetInput() );
  this->GraftOutput( image );

  // Nothing that needs to be allocated for the remaining outputs
}

template<class TInputImage>
void
StreamingStatisticsImageFilter<TInputImage>
::BeforeThreadedGenerateData()
{
  if(this->GetOutput()->GetRequestedRegion().GetNumberOfPixels()==0)
    {
      this->GetOutput()->SetRequestedRegion(this->GetOutput()->GetLargestPossibleRegion());
    }

  int numberOfThreads = this->GetNumberOfThreads();

  // Resize the thread temporaries
  m_Count.SetSize(numberOfThreads);
  m_SumOfSquares.SetSize(numberOfThreads);
  m_ThreadSum.SetSize(numberOfThreads);
  m_ThreadMin.SetSize(numberOfThreads);
  m_ThreadMax.SetSize(numberOfThreads);
  
  // Initialize the temporaries
  m_Count.Fill(itk::NumericTraits<long>::Zero);
  m_ThreadSum.Fill(itk::NumericTraits<RealType>::Zero);
  m_SumOfSquares.Fill(itk::NumericTraits<RealType>::Zero);
  m_ThreadMin.Fill(itk::NumericTraits<PixelType>::max());
  m_ThreadMax.Fill(itk::NumericTraits<PixelType>::NonpositiveMin());

  //otbMsgDebugMacro(<<"BeforeThreadedGenerateData() - output requested region: "<<this->GetOutput()->GetRequestedRegion());
  otbMsgDebugMacro(<<"Leaving BeforeThreadedGenerateData() - nb threads: "<<numberOfThreads);
}

template<class TInputImage>
void
StreamingStatisticsImageFilter<TInputImage>
::AfterThreadedGenerateData()
{
  //otbMsgDebugMacro(<<"Entering AfterThreadedGenerateData()");
  int i;
  long count;
  RealType sumOfSquares;
    
  int numberOfThreads = this->GetNumberOfThreads();

  PixelType minimum;
  PixelType maximum;
  RealType  mean;
  RealType  sigma;
  RealType  variance;
  RealType  sum;
  
  sum = sumOfSquares = itk::NumericTraits<RealType>::Zero;
  count = 0;

  // Find the min/max over all threads and accumulate count, sum and
  // sum of squares
  minimum = itk::NumericTraits<PixelType>::max();
  maximum = itk::NumericTraits<PixelType>::NonpositiveMin();
  for( i = 0; i < numberOfThreads; i++)
    {
    count += m_Count[i];
    sum += m_ThreadSum[i];
    sumOfSquares += m_SumOfSquares[i];

    if (m_ThreadMin[i] < minimum)
      {
      minimum = m_ThreadMin[i];
      }
    if (m_ThreadMax[i] > maximum)
      {
      maximum = m_ThreadMax[i];
      }
    }
  // compute statistics
  mean = sum / static_cast<RealType>(count);

  // unbiased estimate
  variance = (sumOfSquares - (sum*sum / static_cast<RealType>(count)))
    / (static_cast<RealType>(count) - 1);
  sigma = vcl_sqrt(variance);

  // Set the outputs
  this->GetMinimumOutput()->Set( minimum );
  this->GetMaximumOutput()->Set( maximum );
  this->GetMeanOutput()->Set( mean );
  this->GetSigmaOutput()->Set( sigma );
  this->GetVarianceOutput()->Set( variance );
  this->GetSumOutput()->Set( sum );
  //otbMsgDebugMacro(<<"Leaving AfterThreadedGenerateData()");
}

template<class TInputImage>
void
StreamingStatisticsImageFilter<TInputImage>
::ThreadedGenerateData(const RegionType& outputRegionForThread,
                       int threadId) 
{
  //otbMsgDebugMacro(<<"Entering ThreadedGenerateData()");
  /**
   * Grab the input
   */
  InputImagePointer inputPtr = 
    const_cast< TInputImage * >( this->GetInput(0) );

  
  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  // Here we will divide the input region into pieces to process one afther the other.
  unsigned int numDivisions = static_cast<unsigned int>(StreamingTraitsType
    ::CalculateNumberOfStreamDivisions(this->GetInput(),
				       outputRegionForThread,
				       m_StreamingMode,
				       m_NumberOfStreamDivisions,
				       m_BufferMemorySize,
				       m_BufferNumberOfLinesDivisions)); 
  
  SplitterPointer regionSplitter = SplitterType::New();
  unsigned int numDivisionsFromSplitter = regionSplitter->GetNumberOfSplits(outputRegionForThread, numDivisions);
  if (numDivisionsFromSplitter < numDivisions)
    {
      numDivisions = numDivisionsFromSplitter;
    }
  otbMsgDebugMacro(<<"ThreadedGenerateData() - thread "<<threadId <<" - nb of divisions: "<<numDivisions);
  RealType realValue;
  PixelType value;

  /**
   * Loop over the number of pieces, execute the upstream pipeline on each
   * piece, and copy the results into the output image.
   */
  unsigned int piece;
  for (piece = 0;
       piece < numDivisions && !this->GetAbortGenerateData();
       piece++)
    {
      //otbMsgDebugMacro(<<"ThreadedGenerateData() - processing piece: "<<piece<<"/"<<numDivisions);
      RegionType streamRegion = regionSplitter->GetSplit(piece,numDivisions,outputRegionForThread);
      //otbMsgDebugMacro(<<"ThreadedGenerateData() - piece region: "<<streamRegion);
      
      inputPtr->SetRequestedRegion(streamRegion);
      inputPtr->PropagateRequestedRegion();
      inputPtr->UpdateOutputData();
      
      
      
      itk::ImageRegionConstIterator<TInputImage> it (inputPtr, streamRegion);
      
      
      // do the work
      while (!it.IsAtEnd())
	{
	  value = it.Get();
	  realValue = static_cast<RealType>( value );
	  if (value < m_ThreadMin[threadId])
	    {
	      m_ThreadMin[threadId] = value;
	    }
	  if (value > m_ThreadMax[threadId])
	    {
	      m_ThreadMax[threadId] = value;
	    }
	  
	  m_ThreadSum[threadId] += realValue;
	  m_SumOfSquares[threadId] += (realValue * realValue);
	  m_Count[threadId]++;
	  ++it;
	  progress.CompletedPixel();
	}
    }
  //otbMsgDebugMacro(<<"Leaving ThreadedGenerateData()");
}

template <class TImage>
void 
StreamingStatisticsImageFilter<TImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "Minimum: "
     << static_cast<typename itk::NumericTraits<PixelType>::PrintType>(this->GetMinimum()) << std::endl;
  os << indent << "Maximum: "
     << static_cast<typename itk::NumericTraits<PixelType>::PrintType>(this->GetMaximum()) << std::endl;
  os << indent << "Sum: "      << this->GetSum() << std::endl;
  os << indent << "Mean: "     << this->GetMean() << std::endl;
  os << indent << "Sigma: "    << this->GetSigma() << std::endl;
  os << indent << "Variance: " << this->GetVariance() << std::endl;
}


}// end namespace otb
#endif
