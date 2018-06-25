/*
 * Copyright (C) 1999-2011 Insight Software Consortium
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbStreamingStatisticsImageFilter_hxx
#define otbStreamingStatisticsImageFilter_hxx
#include "otbStreamingStatisticsImageFilter.h"

#include "itkImageRegionIterator.h"
#include "itkProgressReporter.h"
#include "otbMacro.h"

namespace otb
{

template<class TInputImage>
PersistentStatisticsImageFilter<TInputImage>
::PersistentStatisticsImageFilter()
 : m_ThreadSum(1),
   m_SumOfSquares(1),
   m_Count(1),
   m_ThreadMin(1),
   m_ThreadMax(1),
   m_IgnoreInfiniteValues(true),
   m_IgnoreUserDefinedValue(false)
{
  // first output is a copy of the image, DataObject created by
  // superclass
  //
  // allocate the data objects for the outputs which are
  // just decorators around pixel types
  for (int i = 1; i < 3; ++i)
    {
    typename PixelObjectType::Pointer output
      = static_cast<PixelObjectType*>(this->MakeOutput(i).GetPointer());
    this->itk::ProcessObject::SetNthOutput(i, output.GetPointer());
    }
  // allocate the data objects for the outputs which are
  // just decorators around real types
  for (int i = 3; i < 7; ++i)
    {
    typename RealObjectType::Pointer output
      = static_cast<RealObjectType*>(this->MakeOutput(i).GetPointer());
    this->itk::ProcessObject::SetNthOutput(i, output.GetPointer());
    }

  this->GetMinimumOutput()->Set(itk::NumericTraits<PixelType>::max());
  this->GetMaximumOutput()->Set(itk::NumericTraits<PixelType>::NonpositiveMin());
  this->GetMeanOutput()->Set(itk::NumericTraits<RealType>::max());
  this->GetSigmaOutput()->Set(itk::NumericTraits<RealType>::max());
  this->GetVarianceOutput()->Set(itk::NumericTraits<RealType>::max());
  this->GetSumOutput()->Set(itk::NumericTraits<RealType>::Zero);

  // Initiate the infinite ignored pixel counters
  m_IgnoredInfinitePixelCount= std::vector<unsigned int>(this->GetNumberOfThreads(), 0);
  m_IgnoredUserPixelCount= std::vector<unsigned int>(this->GetNumberOfThreads(), 0);

  this->Reset();
}

template<class TInputImage>
typename itk::DataObject::Pointer
PersistentStatisticsImageFilter<TInputImage>
::MakeOutput(DataObjectPointerArraySizeType output)
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
typename PersistentStatisticsImageFilter<TInputImage>::PixelObjectType*
PersistentStatisticsImageFilter<TInputImage>
::GetMinimumOutput()
{
  return static_cast<PixelObjectType*>(this->itk::ProcessObject::GetOutput(1));
}

template<class TInputImage>
const typename PersistentStatisticsImageFilter<TInputImage>::PixelObjectType*
PersistentStatisticsImageFilter<TInputImage>
::GetMinimumOutput() const
{
  return static_cast<const PixelObjectType*>(this->itk::ProcessObject::GetOutput(1));
}

template<class TInputImage>
typename PersistentStatisticsImageFilter<TInputImage>::PixelObjectType*
PersistentStatisticsImageFilter<TInputImage>
::GetMaximumOutput()
{
  return static_cast<PixelObjectType*>(this->itk::ProcessObject::GetOutput(2));
}

template<class TInputImage>
const typename PersistentStatisticsImageFilter<TInputImage>::PixelObjectType*
PersistentStatisticsImageFilter<TInputImage>
::GetMaximumOutput() const
{
  return static_cast<const PixelObjectType*>(this->itk::ProcessObject::GetOutput(2));
}

template<class TInputImage>
typename PersistentStatisticsImageFilter<TInputImage>::RealObjectType*
PersistentStatisticsImageFilter<TInputImage>
::GetMeanOutput()
{
  return static_cast<RealObjectType*>(this->itk::ProcessObject::GetOutput(3));
}

template<class TInputImage>
const typename PersistentStatisticsImageFilter<TInputImage>::RealObjectType*
PersistentStatisticsImageFilter<TInputImage>
::GetMeanOutput() const
{
  return static_cast<const RealObjectType*>(this->itk::ProcessObject::GetOutput(3));
}

template<class TInputImage>
typename PersistentStatisticsImageFilter<TInputImage>::RealObjectType*
PersistentStatisticsImageFilter<TInputImage>
::GetSigmaOutput()
{
  return static_cast<RealObjectType*>(this->itk::ProcessObject::GetOutput(4));
}

template<class TInputImage>
const typename PersistentStatisticsImageFilter<TInputImage>::RealObjectType*
PersistentStatisticsImageFilter<TInputImage>
::GetSigmaOutput() const
{
  return static_cast<const RealObjectType*>(this->itk::ProcessObject::GetOutput(4));
}

template<class TInputImage>
typename PersistentStatisticsImageFilter<TInputImage>::RealObjectType*
PersistentStatisticsImageFilter<TInputImage>
::GetVarianceOutput()
{
  return static_cast<RealObjectType*>(this->itk::ProcessObject::GetOutput(5));
}

template<class TInputImage>
const typename PersistentStatisticsImageFilter<TInputImage>::RealObjectType*
PersistentStatisticsImageFilter<TInputImage>
::GetVarianceOutput() const
{
  return static_cast<const RealObjectType*>(this->itk::ProcessObject::GetOutput(5));
}

template<class TInputImage>
typename PersistentStatisticsImageFilter<TInputImage>::RealObjectType*
PersistentStatisticsImageFilter<TInputImage>
::GetSumOutput()
{
  return static_cast<RealObjectType*>(this->itk::ProcessObject::GetOutput(6));
}

template<class TInputImage>
const typename PersistentStatisticsImageFilter<TInputImage>::RealObjectType*
PersistentStatisticsImageFilter<TInputImage>
::GetSumOutput() const
{
  return static_cast<const RealObjectType*>(this->itk::ProcessObject::GetOutput(6));
}
template<class TInputImage>
void
PersistentStatisticsImageFilter<TInputImage>
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();
  if (this->GetInput())
    {
    this->GetOutput()->CopyInformation(this->GetInput());
    this->GetOutput()->SetLargestPossibleRegion(this->GetInput()->GetLargestPossibleRegion());

    if (this->GetOutput()->GetRequestedRegion().GetNumberOfPixels() == 0)
      {
      this->GetOutput()->SetRequestedRegion(this->GetOutput()->GetLargestPossibleRegion());
      }
    }
}
template<class TInputImage>
void
PersistentStatisticsImageFilter<TInputImage>
::AllocateOutputs()
{
  // This is commented to prevent the streaming of the whole image for the first stream strip
  // It shall not cause any problem because the output image of this filter is not intended to be used.
  //InputImagePointer image = const_cast< TInputImage * >( this->GetInput() );
  //this->GraftOutput( image );
  // Nothing that needs to be allocated for the remaining outputs
}

template<class TInputImage>
void
PersistentStatisticsImageFilter<TInputImage>
::Synthetize()
{
  int      i;
  long     count;
  RealType sumOfSquares;

  int numberOfThreads = this->GetNumberOfThreads();

  PixelType minimum;
  PixelType maximum;
  RealType  mean = itk::NumericTraits<RealType>::Zero;
  RealType  sigma = itk::NumericTraits<RealType>::Zero;
  RealType  variance = itk::NumericTraits<RealType>::Zero;
  RealType  sum;

  sum = sumOfSquares = itk::NumericTraits<RealType>::Zero;
  count = 0;

  // Find the min/max over all threads and accumulate count, sum and
  // sum of squares
  minimum = itk::NumericTraits<PixelType>::max();
  maximum = itk::NumericTraits<PixelType>::NonpositiveMin();
  for (i = 0; i < numberOfThreads; ++i)
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
  if (count > 0)
    {
    // compute statistics
    mean = sum / static_cast<RealType>(count);

    if (count > 1)
      {
      // unbiased estimate
      variance = (sumOfSquares - (sum * sum / static_cast<RealType>(count)))
             / static_cast<RealType>(count - 1);
      sigma = vcl_sqrt(variance);
      }
    }
  else
    {
    itkWarningMacro(<<"No pixel found to compute statistics!");
    }

  // Set the outputs
  this->GetMinimumOutput()->Set(minimum);
  this->GetMaximumOutput()->Set(maximum);
  this->GetMeanOutput()->Set(mean);
  this->GetSigmaOutput()->Set(sigma);
  this->GetVarianceOutput()->Set(variance);
  this->GetSumOutput()->Set(sum);
}

template<class TInputImage>
void
PersistentStatisticsImageFilter<TInputImage>
::Reset()
{
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
  if (m_IgnoreInfiniteValues)
    {
    m_IgnoredInfinitePixelCount= std::vector<unsigned int>(numberOfThreads, 0);
    }

  if (m_IgnoreUserDefinedValue)
    {
    m_IgnoredUserPixelCount= std::vector<unsigned int>(this->GetNumberOfThreads(), 0);
    }
}

template<class TInputImage>
void
PersistentStatisticsImageFilter<TInputImage>
::ThreadedGenerateData(const RegionType& outputRegionForThread,
                       itk::ThreadIdType threadId)
{
  /**
   * Grab the input
   */
  InputImagePointer inputPtr =  const_cast<TInputImage *>(this->GetInput(0));
  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  RealType  realValue;
  PixelType value;

  itk::ImageRegionConstIterator<TInputImage> it(inputPtr, outputRegionForThread);

  it.GoToBegin();
  // do the work
  while (!it.IsAtEnd())
    {
    value = it.Get();
    realValue = static_cast<RealType>(value);
    if (m_IgnoreInfiniteValues && !(vnl_math_isfinite(realValue)))
      {
      m_IgnoredInfinitePixelCount[threadId] ++;
      }
    else
      {
      if (m_IgnoreUserDefinedValue && (value == m_UserIgnoredValue))
        {
        m_IgnoredUserPixelCount[threadId] ++;
        }
      else
        {
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
        }
      }
    ++it;
    progress.CompletedPixel();
    }
}
template <class TImage>
void
PersistentStatisticsImageFilter<TImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Minimum: "
     << static_cast<typename itk::NumericTraits<PixelType>::PrintType>(this->GetMinimum()) << std::endl;
  os << indent << "Maximum: "
     << static_cast<typename itk::NumericTraits<PixelType>::PrintType>(this->GetMaximum()) << std::endl;
  os << indent << "Sum: "      << this->GetSum() << std::endl;
  os << indent << "Mean: "     << this->GetMean() << std::endl;
  os << indent << "Sigma: "    << this->GetSigma() << std::endl;
  os << indent << "Variance: " << this->GetVariance() << std::endl;
}
} // end namespace otb
#endif
