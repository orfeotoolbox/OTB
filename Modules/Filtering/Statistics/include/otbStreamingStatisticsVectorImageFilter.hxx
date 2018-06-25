/*
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

#ifndef otbStreamingStatisticsVectorImageFilter_hxx
#define otbStreamingStatisticsVectorImageFilter_hxx
#include "otbStreamingStatisticsVectorImageFilter.h"

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkProgressReporter.h"
#include "otbMacro.h"

namespace otb
{

template<class TInputImage, class TPrecision>
PersistentStreamingStatisticsVectorImageFilter<TInputImage, TPrecision>
::PersistentStreamingStatisticsVectorImageFilter()
 : m_EnableMinMax(true),
   m_EnableFirstOrderStats(true),
   m_EnableSecondOrderStats(true),
   m_UseUnbiasedEstimator(true),
   m_IgnoreInfiniteValues(true),
   m_IgnoreUserDefinedValue(false),
   m_UserIgnoredValue(itk::NumericTraits<InternalPixelType>::Zero)
{
  // first output is a copy of the image, DataObject created by
  // superclass

  // allocate the data objects for the outputs which are
  // just decorators around vector/matrix types
  for (unsigned int i = 1; i < 11; ++i)
    {
    this->itk::ProcessObject::SetNthOutput(i, this->MakeOutput(i).GetPointer());
    }
  // Initiate ignored pixel counters
  m_IgnoredInfinitePixelCount= std::vector<unsigned int>(this->GetNumberOfThreads(), 0);
  m_IgnoredUserPixelCount= std::vector<unsigned int>(this->GetNumberOfThreads(), 0);
}

template<class TInputImage, class TPrecision>
itk::DataObject::Pointer
PersistentStreamingStatisticsVectorImageFilter<TInputImage, TPrecision>
::MakeOutput(DataObjectPointerArraySizeType output)
{
  switch (output)
    {
    case 0:
      return static_cast<itk::DataObject*>(TInputImage::New().GetPointer());
      break;
    case 1:
    case 2:
      // min/max
      return static_cast<itk::DataObject*>(PixelObjectType::New().GetPointer());
      break;
    case 3:
    case 4:
      // mean / sum
      return static_cast<itk::DataObject*>(RealPixelObjectType::New().GetPointer());
      break;
    case 5:
    case 6:
      // covariance / correlation
      return static_cast<itk::DataObject*>(MatrixObjectType::New().GetPointer());
      break;
    case 7:
    case 8:
    case 9:
      // component mean, component covariance, component correlation
      return static_cast<itk::DataObject*>(RealObjectType::New().GetPointer());
      break;
    case 10:
      // relevant pixel
      return static_cast<itk::DataObject*>(CountObjectType::New().GetPointer());
    default:
      // might as well make an image
      return static_cast<itk::DataObject*>(TInputImage::New().GetPointer());
      break;
    }
}

template<class TInputImage, class TPrecision>
typename PersistentStreamingStatisticsVectorImageFilter<TInputImage, TPrecision>::CountObjectType*
PersistentStreamingStatisticsVectorImageFilter<TInputImage, TPrecision>
::GetNbRelevantPixelsOutput()
{
  return static_cast<CountObjectType*>(this->itk::ProcessObject::GetOutput(10));
}

template<class TInputImage, class TPrecision>
const typename PersistentStreamingStatisticsVectorImageFilter<TInputImage, TPrecision>::CountObjectType*
PersistentStreamingStatisticsVectorImageFilter<TInputImage, TPrecision>
::GetNbRelevantPixelsOutput() const
{
  return static_cast<const CountObjectType*>(this->itk::ProcessObject::GetOutput(10));
}


template<class TInputImage, class TPrecision>
typename PersistentStreamingStatisticsVectorImageFilter<TInputImage, TPrecision>::PixelObjectType*
PersistentStreamingStatisticsVectorImageFilter<TInputImage, TPrecision>
::GetMinimumOutput()
{
  return static_cast<PixelObjectType*>(this->itk::ProcessObject::GetOutput(1));
}

template<class TInputImage, class TPrecision>
const typename PersistentStreamingStatisticsVectorImageFilter<TInputImage, TPrecision>::PixelObjectType*
PersistentStreamingStatisticsVectorImageFilter<TInputImage, TPrecision>
::GetMinimumOutput() const
{
  return static_cast<const PixelObjectType*>(this->itk::ProcessObject::GetOutput(1));
}

template<class TInputImage, class TPrecision>
typename PersistentStreamingStatisticsVectorImageFilter<TInputImage, TPrecision>::PixelObjectType*
PersistentStreamingStatisticsVectorImageFilter<TInputImage, TPrecision>
::GetMaximumOutput()
{
  return static_cast<PixelObjectType*>(this->itk::ProcessObject::GetOutput(2));
}

template<class TInputImage, class TPrecision>
const typename PersistentStreamingStatisticsVectorImageFilter<TInputImage, TPrecision>::PixelObjectType*
PersistentStreamingStatisticsVectorImageFilter<TInputImage, TPrecision>
::GetMaximumOutput() const
{
  return static_cast<const PixelObjectType*>(this->itk::ProcessObject::GetOutput(2));
}

template<class TInputImage, class TPrecision>
typename PersistentStreamingStatisticsVectorImageFilter<TInputImage, TPrecision>::RealObjectType*
PersistentStreamingStatisticsVectorImageFilter<TInputImage, TPrecision>
::GetComponentMeanOutput()
{
  return static_cast<RealObjectType*>(this->itk::ProcessObject::GetOutput(7));
}

template<class TInputImage, class TPrecision>
const typename PersistentStreamingStatisticsVectorImageFilter<TInputImage, TPrecision>::RealObjectType*
PersistentStreamingStatisticsVectorImageFilter<TInputImage, TPrecision>
::GetComponentMeanOutput() const
{
  return static_cast<const RealObjectType*>(this->itk::ProcessObject::GetOutput(7));
}

template<class TInputImage, class TPrecision>
typename PersistentStreamingStatisticsVectorImageFilter<TInputImage, TPrecision>::RealObjectType*
PersistentStreamingStatisticsVectorImageFilter<TInputImage, TPrecision>
::GetComponentCorrelationOutput()
{
  return static_cast<RealObjectType*>(this->itk::ProcessObject::GetOutput(8));
}

template<class TInputImage, class TPrecision>
const typename PersistentStreamingStatisticsVectorImageFilter<TInputImage, TPrecision>::RealObjectType*
PersistentStreamingStatisticsVectorImageFilter<TInputImage, TPrecision>
::GetComponentCorrelationOutput() const
{
  return static_cast<const RealObjectType*>(this->itk::ProcessObject::GetOutput(8));
}


template<class TInputImage, class TPrecision>
typename PersistentStreamingStatisticsVectorImageFilter<TInputImage, TPrecision>::RealObjectType*
PersistentStreamingStatisticsVectorImageFilter<TInputImage, TPrecision>
::GetComponentCovarianceOutput()
{
  return static_cast<RealObjectType*>(this->itk::ProcessObject::GetOutput(9));
}

template<class TInputImage, class TPrecision>
const typename PersistentStreamingStatisticsVectorImageFilter<TInputImage, TPrecision>::RealObjectType*
PersistentStreamingStatisticsVectorImageFilter<TInputImage, TPrecision>
::GetComponentCovarianceOutput() const
{
  return static_cast<const RealObjectType*>(this->itk::ProcessObject::GetOutput(9));
}

template<class TInputImage, class TPrecision>
typename PersistentStreamingStatisticsVectorImageFilter<TInputImage, TPrecision>::RealPixelObjectType*
PersistentStreamingStatisticsVectorImageFilter<TInputImage, TPrecision>
::GetMeanOutput()
{
  return static_cast<RealPixelObjectType*>(this->itk::ProcessObject::GetOutput(3));
}

template<class TInputImage, class TPrecision>
const typename PersistentStreamingStatisticsVectorImageFilter<TInputImage, TPrecision>::RealPixelObjectType*
PersistentStreamingStatisticsVectorImageFilter<TInputImage, TPrecision>
::GetMeanOutput() const
{
  return static_cast<const RealPixelObjectType*>(this->itk::ProcessObject::GetOutput(3));
}

template<class TInputImage, class TPrecision>
typename PersistentStreamingStatisticsVectorImageFilter<TInputImage, TPrecision>::RealPixelObjectType*
PersistentStreamingStatisticsVectorImageFilter<TInputImage, TPrecision>
::GetSumOutput()
{
  return static_cast<RealPixelObjectType*>(this->itk::ProcessObject::GetOutput(4));
}

template<class TInputImage, class TPrecision>
const typename PersistentStreamingStatisticsVectorImageFilter<TInputImage, TPrecision>::RealPixelObjectType*
PersistentStreamingStatisticsVectorImageFilter<TInputImage, TPrecision>
::GetSumOutput() const
{
  return static_cast<const RealPixelObjectType*>(this->itk::ProcessObject::GetOutput(4));
}

template<class TInputImage, class TPrecision>
typename PersistentStreamingStatisticsVectorImageFilter<TInputImage, TPrecision>::MatrixObjectType*
PersistentStreamingStatisticsVectorImageFilter<TInputImage, TPrecision>
::GetCorrelationOutput()
{
  return static_cast<MatrixObjectType*>(this->itk::ProcessObject::GetOutput(5));
}

template<class TInputImage, class TPrecision>
const typename PersistentStreamingStatisticsVectorImageFilter<TInputImage, TPrecision>::MatrixObjectType*
PersistentStreamingStatisticsVectorImageFilter<TInputImage, TPrecision>
::GetCorrelationOutput() const
{
  return static_cast<const MatrixObjectType*>(this->itk::ProcessObject::GetOutput(5));
}

template<class TInputImage, class TPrecision>
typename PersistentStreamingStatisticsVectorImageFilter<TInputImage, TPrecision>::MatrixObjectType*
PersistentStreamingStatisticsVectorImageFilter<TInputImage, TPrecision>
::GetCovarianceOutput()
{
  return static_cast<MatrixObjectType*>(this->itk::ProcessObject::GetOutput(6));
}

template<class TInputImage, class TPrecision>
const typename PersistentStreamingStatisticsVectorImageFilter<TInputImage, TPrecision>::MatrixObjectType*
PersistentStreamingStatisticsVectorImageFilter<TInputImage, TPrecision>
::GetCovarianceOutput() const
{
  return static_cast<const MatrixObjectType*>(this->itk::ProcessObject::GetOutput(6));
}

template<class TInputImage, class TPrecision>
void
PersistentStreamingStatisticsVectorImageFilter<TInputImage, TPrecision>
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

template<class TInputImage, class TPrecision>
void
PersistentStreamingStatisticsVectorImageFilter<TInputImage, TPrecision>
::AllocateOutputs()
{
  // This is commented to prevent the streaming of the whole image for the first stream strip
  // It shall not cause any problem because the output image of this filter is not intended to be used.
  //InputImagePointer image = const_cast< TInputImage * >( this->GetInput() );
  //this->GraftOutput( image );
  // Nothing that needs to be allocated for the remaining outputs
}

template<class TInputImage, class TPrecision>
void
PersistentStreamingStatisticsVectorImageFilter<TInputImage, TPrecision>
::Reset()
{
  TInputImage * inputPtr = const_cast<TInputImage *>(this->GetInput());
  inputPtr->UpdateOutputInformation();

  unsigned int numberOfThreads = this->GetNumberOfThreads();
  unsigned int numberOfComponent = inputPtr->GetNumberOfComponentsPerPixel();

  if (m_EnableMinMax)
    {
    PixelType tempPixel;
    tempPixel.SetSize(numberOfComponent);

    tempPixel.Fill(itk::NumericTraits<InternalPixelType>::max());
    this->GetMinimumOutput()->Set(tempPixel);

    tempPixel.Fill(itk::NumericTraits<InternalPixelType>::NonpositiveMin());
    this->GetMaximumOutput()->Set(tempPixel);

    PixelType tempTemporiesPixel;
    tempTemporiesPixel.SetSize(numberOfComponent);
    tempTemporiesPixel.Fill(itk::NumericTraits<InternalPixelType>::max());
    m_ThreadMin = std::vector<PixelType>(numberOfThreads, tempTemporiesPixel);

    tempTemporiesPixel.Fill(itk::NumericTraits<InternalPixelType>::NonpositiveMin());
    m_ThreadMax = std::vector<PixelType>(numberOfThreads, tempTemporiesPixel);
    }

  if (m_EnableSecondOrderStats)
    {
    m_EnableFirstOrderStats = true;
    }

  if (m_EnableFirstOrderStats)
    {
    RealPixelType zeroRealPixel;
    zeroRealPixel.SetSize(numberOfComponent);
    zeroRealPixel.Fill(itk::NumericTraits<PrecisionType>::ZeroValue());
    this->GetMeanOutput()->Set(zeroRealPixel);
    this->GetSumOutput()->Set(zeroRealPixel);
    m_ThreadFirstOrderAccumulators.resize(numberOfThreads);
    std::fill(m_ThreadFirstOrderAccumulators.begin(), m_ThreadFirstOrderAccumulators.end(), zeroRealPixel);

    RealType zeroReal = itk::NumericTraits<RealType>::ZeroValue();
    m_ThreadFirstOrderComponentAccumulators.resize(numberOfThreads);
    std::fill(m_ThreadFirstOrderComponentAccumulators.begin(), m_ThreadFirstOrderComponentAccumulators.end(), zeroReal);

    }

  if (m_EnableSecondOrderStats)
    {
    MatrixType zeroMatrix;
    zeroMatrix.SetSize(numberOfComponent, numberOfComponent);
    zeroMatrix.Fill(itk::NumericTraits<PrecisionType>::Zero);
    this->GetCovarianceOutput()->Set(zeroMatrix);
    this->GetCorrelationOutput()->Set(zeroMatrix);

    m_ThreadSecondOrderAccumulators.resize(numberOfThreads);
    std::fill(m_ThreadSecondOrderAccumulators.begin(), m_ThreadSecondOrderAccumulators.end(), zeroMatrix);

    RealType zeroReal = itk::NumericTraits<RealType>::ZeroValue();
    m_ThreadSecondOrderComponentAccumulators.resize(numberOfThreads);
    std::fill(m_ThreadSecondOrderComponentAccumulators.begin(), m_ThreadSecondOrderComponentAccumulators.end(), zeroReal);
    }

  if (m_IgnoreInfiniteValues)
    {
      m_IgnoredInfinitePixelCount= std::vector<unsigned int>(numberOfThreads, 0);
    }

  if (m_IgnoreUserDefinedValue)
    {
    m_IgnoredUserPixelCount= std::vector<unsigned int>(this->GetNumberOfThreads(), 0);
    }
}

template<class TInputImage, class TPrecision>
void
PersistentStreamingStatisticsVectorImageFilter<TInputImage, TPrecision>
::Synthetize()
{
  TInputImage * inputPtr = const_cast<TInputImage *>(this->GetInput());
  const unsigned int nbPixels = inputPtr->GetLargestPossibleRegion().GetNumberOfPixels();
  const unsigned int numberOfComponent = inputPtr->GetNumberOfComponentsPerPixel();

  PixelType minimum;
  minimum.SetSize(numberOfComponent);
  minimum.Fill(itk::NumericTraits<InternalPixelType>::max());
  PixelType maximum;
  maximum.SetSize(numberOfComponent);
  maximum.Fill(itk::NumericTraits<InternalPixelType>::NonpositiveMin());

  RealPixelType streamFirstOrderAccumulator(numberOfComponent);
  streamFirstOrderAccumulator.Fill(itk::NumericTraits<PrecisionType>::Zero);
  MatrixType    streamSecondOrderAccumulator(numberOfComponent, numberOfComponent);
  streamSecondOrderAccumulator.Fill(itk::NumericTraits<PrecisionType>::Zero);

  RealType streamFirstOrderComponentAccumulator = itk::NumericTraits<RealType>::Zero;
  RealType streamSecondOrderComponentAccumulator = itk::NumericTraits<RealType>::Zero;

  unsigned int ignoredInfinitePixelCount = 0;
  unsigned int ignoredUserPixelCount = 0;

  // Accumulate results from all threads
  const itk::ThreadIdType numberOfThreads = this->GetNumberOfThreads();
  for (itk::ThreadIdType threadId = 0; threadId < numberOfThreads; ++threadId)
    {
    if (m_EnableMinMax)
      {
      const PixelType& threadMin  = m_ThreadMin [threadId];
      const PixelType& threadMax  = m_ThreadMax [threadId];

      for (unsigned int j = 0; j < numberOfComponent; ++j)
        {
        if (threadMin[j] < minimum[j])
          {
          minimum[j] = threadMin[j];
          }
        if (threadMax[j] > maximum[j])
          {
          maximum[j] = threadMax[j];
          }
        }
      }

    if (m_EnableFirstOrderStats)
      {
      streamFirstOrderAccumulator += m_ThreadFirstOrderAccumulators[threadId];
      streamFirstOrderComponentAccumulator += m_ThreadFirstOrderComponentAccumulators[threadId];
      }

    if (m_EnableSecondOrderStats)
      {
      streamSecondOrderAccumulator += m_ThreadSecondOrderAccumulators[threadId];
      streamSecondOrderComponentAccumulator += m_ThreadSecondOrderComponentAccumulators[threadId];
      }
    // Ignored Infinite Pixels
    ignoredInfinitePixelCount += m_IgnoredInfinitePixelCount[threadId];
    // Ignored Pixels
    ignoredUserPixelCount += m_IgnoredUserPixelCount[threadId];
    }

  // There cannot be more ignored pixels than read pixels.
  assert( nbPixels >= ignoredInfinitePixelCount + ignoredUserPixelCount );
  if( nbPixels < ignoredInfinitePixelCount + ignoredUserPixelCount )
    {
    itkExceptionMacro(
      "nbPixels < ignoredInfinitePixelCount + ignoredUserPixelCount"
    );
    }

  unsigned int nbRelevantPixel =
    nbPixels - (ignoredInfinitePixelCount + ignoredUserPixelCount);

  CountType nbRelevantPixels(numberOfComponent);
  nbRelevantPixels.Fill(nbRelevantPixel);

  this->GetNbRelevantPixelsOutput()->Set(nbRelevantPixels);

  if( nbRelevantPixel==0 )
    {
    itkExceptionMacro(
      "Statistics cannot be calculated with zero relevant pixels."
    );
    }

  // Final calculations
  if (m_EnableMinMax)
    {
    this->GetMinimumOutput()->Set(minimum);
    this->GetMaximumOutput()->Set(maximum);
    }

  if (m_EnableFirstOrderStats)
    {
    this->GetComponentMeanOutput()->Set(streamFirstOrderComponentAccumulator / (nbRelevantPixel * numberOfComponent));

    this->GetMeanOutput()->Set(streamFirstOrderAccumulator / nbRelevantPixel);
    this->GetSumOutput()->Set(streamFirstOrderAccumulator);
    }

  if (m_EnableSecondOrderStats)
    {
    MatrixType cor = streamSecondOrderAccumulator / nbRelevantPixel;
    this->GetCorrelationOutput()->Set(cor);

    const RealPixelType& mean = this->GetMeanOutput()->Get();

    double regul = 1.0;
    double regulComponent = 1.0;

    if( m_UseUnbiasedEstimator && nbRelevantPixel>1 )
      {
      regul =
       static_cast< double >( nbRelevantPixel ) /
       ( static_cast< double >( nbRelevantPixel ) - 1.0 );
      }

    if( m_UseUnbiasedEstimator && (nbRelevantPixel * numberOfComponent) > 1 )
      {
      regulComponent =
        static_cast< double >(nbRelevantPixel * numberOfComponent) /
       ( static_cast< double >(nbRelevantPixel * numberOfComponent) - 1.0 );
      }

    MatrixType cov  = cor;
    for (unsigned int r = 0; r < numberOfComponent; ++r)
      {
      for (unsigned int c = 0; c < numberOfComponent; ++c)
        {
        cov(r, c) = regul * (cov(r, c) - mean[r] * mean[c]);
        }
      }
    this->GetCovarianceOutput()->Set(cov);

    this->GetComponentMeanOutput()->Set(streamFirstOrderComponentAccumulator / (nbRelevantPixel * numberOfComponent));
    this->GetComponentCorrelationOutput()->Set(streamSecondOrderComponentAccumulator / (nbRelevantPixel * numberOfComponent));
    this->GetComponentCovarianceOutput()->Set(
        regulComponent * (this->GetComponentCorrelation()
           - (this->GetComponentMean() * this->GetComponentMean())));
    }
}

template<class TInputImage, class TPrecision>
void
PersistentStreamingStatisticsVectorImageFilter<TInputImage, TPrecision>
::ThreadedGenerateData(const RegionType& outputRegionForThread, itk::ThreadIdType threadId)
 {
  // Support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  // Grab the input
  InputImagePointer inputPtr = const_cast<TInputImage *>(this->GetInput());
  PixelType& threadMin  = m_ThreadMin [threadId];
  PixelType& threadMax  = m_ThreadMax [threadId];


  itk::ImageRegionConstIteratorWithIndex<TInputImage> it(inputPtr, outputRegionForThread);

  for (it.GoToBegin(); !it.IsAtEnd(); ++it, progress.CompletedPixel())
    {
    const PixelType& vectorValue = it.Get();

    float finiteProbe = 0.;
    bool userProbe = m_IgnoreUserDefinedValue;
    for (unsigned int j = 0; j < vectorValue.GetSize(); ++j)
      {
      finiteProbe += (float)(vectorValue[j]);
      userProbe = userProbe && (vectorValue[j] == m_UserIgnoredValue);
      }

    if (m_IgnoreInfiniteValues && !(vnl_math_isfinite(finiteProbe)))
      {
      m_IgnoredInfinitePixelCount[threadId] ++;
      }
    else
      {
      if (userProbe)
        {
        m_IgnoredUserPixelCount[threadId] ++;
        }
      else
        {
        if (m_EnableMinMax)
          {
          for (unsigned int j = 0; j < vectorValue.GetSize(); ++j)
            {
            if (vectorValue[j] < threadMin[j])
              {
              threadMin[j] = vectorValue[j];
              }
            if (vectorValue[j] > threadMax[j])
              {
              threadMax[j] = vectorValue[j];
              }
            }
          }

        if (m_EnableFirstOrderStats)
          {
          RealPixelType& threadFirstOrder  = m_ThreadFirstOrderAccumulators [threadId];
          RealType& threadFirstOrderComponent  = m_ThreadFirstOrderComponentAccumulators [threadId];

          threadFirstOrder += vectorValue;

          for (unsigned int i = 0; i < vectorValue.GetSize(); ++i)
            {
            threadFirstOrderComponent += vectorValue[i];
            }
          }

        if (m_EnableSecondOrderStats)
          {
          MatrixType&    threadSecondOrder = m_ThreadSecondOrderAccumulators[threadId];
          RealType& threadSecondOrderComponent = m_ThreadSecondOrderComponentAccumulators[threadId];

          for (unsigned int r = 0; r < threadSecondOrder.Rows(); ++r)
            {
            for (unsigned int c = 0; c < threadSecondOrder.Cols(); ++c)
              {
              threadSecondOrder(r, c) += static_cast<PrecisionType>(vectorValue[r]) * static_cast<PrecisionType>(vectorValue[c]);
              }
            }
          threadSecondOrderComponent += vectorValue.GetSquaredNorm();
          }
        }
      }
    }

 }

template <class TImage, class TPrecision>
void
PersistentStreamingStatisticsVectorImageFilter<TImage, TPrecision>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Min: "         << this->GetMinimumOutput()->Get()     << std::endl;
  os << indent << "Max: "         << this->GetMaximumOutput()->Get()     << std::endl;
  os << indent << "Mean: "        << this->GetMeanOutput()->Get()        << std::endl;
  os << indent << "Covariance: "  << this->GetCovarianceOutput()->Get()  << std::endl;
  os << indent << "Correlation: " << this->GetCorrelationOutput()->Get() << std::endl;
  os << indent << "Relevant pixel: "        << this->GetNbRelevantPixelsOutput()->Get()     << std::endl;
  os << indent << "Component Mean: "        << this->GetComponentMeanOutput()->Get()        << std::endl;
  os << indent << "Component Covariance: "  << this->GetComponentCovarianceOutput()->Get()  << std::endl;
  os << indent << "Component Correlation: " << this->GetComponentCorrelationOutput()->Get() << std::endl;
  os << indent << "UseUnbiasedEstimator: "  << (this->m_UseUnbiasedEstimator ? "true" : "false")  << std::endl;
}

} // end namespace otb
#endif
