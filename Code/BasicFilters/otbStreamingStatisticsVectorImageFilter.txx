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
#ifndef __otbStreamingStatisticsVectorImageFilter_txx
#define __otbStreamingStatisticsVectorImageFilter_txx
#include "otbStreamingStatisticsVectorImageFilter.h"

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkNumericTraits.h"
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
   m_UseUnbiasedEstimator(true)
{
  // first output is a copy of the image, DataObject created by
  // superclass

  // allocate the data objects for the outputs which are
  // just decorators around vector/matrix types
  for (unsigned int i = 1; i < 7; ++i)
    {
    this->itk::ProcessObject::SetNthOutput(i, this->MakeOutput(i).GetPointer());
    }
}

template<class TInputImage, class TPrecision>
itk::DataObject::Pointer
PersistentStreamingStatisticsVectorImageFilter<TInputImage, TPrecision>
::MakeOutput(unsigned int output)
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
    default:
      // might as well make an image
      return static_cast<itk::DataObject*>(TInputImage::New().GetPointer());
      break;
    }
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
    zeroRealPixel.Fill(itk::NumericTraits<PrecisionType>::Zero);
    this->GetMeanOutput()->Set(zeroRealPixel);
    this->GetSumOutput()->Set(zeroRealPixel);

    m_ThreadFirstOrderAccumulators.resize(numberOfThreads);
    std::fill(m_ThreadFirstOrderAccumulators.begin(), m_ThreadFirstOrderAccumulators.end(), zeroRealPixel);
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

  // Accumulate results from all threads
  const unsigned int numberOfThreads = this->GetNumberOfThreads();
  for (unsigned int threadId = 0; threadId < numberOfThreads; ++threadId)
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
      streamFirstOrderAccumulator   += m_ThreadFirstOrderAccumulators [threadId];

    if (m_EnableSecondOrderStats)
      streamSecondOrderAccumulator  += m_ThreadSecondOrderAccumulators[threadId];
    }

  // Final calculations
  if (m_EnableMinMax)
    {
    this->GetMinimumOutput()->Set(minimum);
    this->GetMaximumOutput()->Set(maximum);
    }

  if (m_EnableFirstOrderStats)
    {
    this->GetMeanOutput()->Set(streamFirstOrderAccumulator / nbPixels);
    this->GetSumOutput()->Set(streamFirstOrderAccumulator);
    }

  if (m_EnableSecondOrderStats)
    {
    MatrixType cor = streamSecondOrderAccumulator / nbPixels;
    this->GetCorrelationOutput()->Set(cor);

    const RealPixelType& mean = this->GetMeanOutput()->Get();
    double regul = static_cast<double>(nbPixels) / (nbPixels - 1);

    if (!m_UseUnbiasedEstimator)
      {
        regul = 1;
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
    }
}

template<class TInputImage, class TPrecision>
void
PersistentStreamingStatisticsVectorImageFilter<TInputImage, TPrecision>
::ThreadedGenerateData(const RegionType& outputRegionForThread, int threadId)
{
  // Support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  // Grab the input
  InputImagePointer inputPtr = const_cast<TInputImage *>(this->GetInput());
  PixelType& threadMin  = m_ThreadMin [threadId];
  PixelType& threadMax  = m_ThreadMax [threadId];
  RealPixelType& threadFirstOrder  = m_ThreadFirstOrderAccumulators [threadId];
  MatrixType&    threadSecondOrder = m_ThreadSecondOrderAccumulators[threadId];

  itk::ImageRegionConstIteratorWithIndex<TInputImage> it(inputPtr, outputRegionForThread);

  for (it.GoToBegin(); !it.IsAtEnd(); ++it, progress.CompletedPixel())
    {
    const PixelType& vectorValue = it.Get();

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
      threadFirstOrder += vectorValue;
      }

    if (m_EnableSecondOrderStats)
      {
      for (unsigned int r = 0; r < threadSecondOrder.Rows(); ++r)
        {
        for (unsigned int c = 0; c < threadSecondOrder.Cols(); ++c)
          {
          threadSecondOrder(r, c) += vectorValue[r] * vectorValue[c];
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
}

} // end namespace otb
#endif
