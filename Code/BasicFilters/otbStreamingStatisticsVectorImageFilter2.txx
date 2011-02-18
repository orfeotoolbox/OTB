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
#include "otbStreamingStatisticsVectorImageFilter2.h"

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkNumericTraits.h"
#include "itkProgressReporter.h"
#include "otbMacro.h"

namespace otb
{

template<class TInputImage>
PersistentStreamingStatisticsVectorImageFilter2<TInputImage>
::PersistentStreamingStatisticsVectorImageFilter2()
 : m_EnableMean(true),
   m_EnableCorrelation(true),
   m_EnableCovariance(true)
{
  // first output is a copy of the image, DataObject created by
  // superclass
  //
  // allocate the data objects for the outputs which are
  // just decorators around vector/matrix types

  this->itk::ProcessObject::SetNthOutput(1, this->MakeOutput(1).GetPointer());
  this->itk::ProcessObject::SetNthOutput(2, this->MakeOutput(2).GetPointer());
  this->itk::ProcessObject::SetNthOutput(3, this->MakeOutput(3).GetPointer());
}

template<class TInputImage>
itk::DataObject::Pointer
PersistentStreamingStatisticsVectorImageFilter2<TInputImage>
::MakeOutput(unsigned int output)
{
  switch (output)
    {
    case 0:
      return static_cast<itk::DataObject*>(TInputImage::New().GetPointer());
      break;
    case 1:
      return static_cast<itk::DataObject*>(RealPixelObjectType::New().GetPointer());
      break;
    case 2:
    case 3:
      return static_cast<itk::DataObject*>(MatrixObjectType::New().GetPointer());
      break;
    default:
      // might as well make an image
      return static_cast<itk::DataObject*>(TInputImage::New().GetPointer());
      break;
    }
}

template<class TInputImage>
typename PersistentStreamingStatisticsVectorImageFilter2<TInputImage>::RealPixelObjectType*
PersistentStreamingStatisticsVectorImageFilter2<TInputImage>
::GetMeanOutput()
{
  return static_cast<RealPixelObjectType*>(this->itk::ProcessObject::GetOutput(1));
}

template<class TInputImage>
const typename PersistentStreamingStatisticsVectorImageFilter2<TInputImage>::RealPixelObjectType*
PersistentStreamingStatisticsVectorImageFilter2<TInputImage>
::GetMeanOutput() const
{
  return static_cast<const RealPixelObjectType*>(this->itk::ProcessObject::GetOutput(1));
}

template<class TInputImage>
typename PersistentStreamingStatisticsVectorImageFilter2<TInputImage>::MatrixObjectType*
PersistentStreamingStatisticsVectorImageFilter2<TInputImage>
::GetCorrelationOutput()
{
  return static_cast<MatrixObjectType*>(this->itk::ProcessObject::GetOutput(2));
}

template<class TInputImage>
const typename PersistentStreamingStatisticsVectorImageFilter2<TInputImage>::MatrixObjectType*
PersistentStreamingStatisticsVectorImageFilter2<TInputImage>
::GetCorrelationOutput() const
{
  return static_cast<const MatrixObjectType*>(this->itk::ProcessObject::GetOutput(2));
}

template<class TInputImage>
typename PersistentStreamingStatisticsVectorImageFilter2<TInputImage>::MatrixObjectType*
PersistentStreamingStatisticsVectorImageFilter2<TInputImage>
::GetCovarianceOutput()
{
  return static_cast<MatrixObjectType*>(this->itk::ProcessObject::GetOutput(3));
}

template<class TInputImage>
const typename PersistentStreamingStatisticsVectorImageFilter2<TInputImage>::MatrixObjectType*
PersistentStreamingStatisticsVectorImageFilter2<TInputImage>
::GetCovarianceOutput() const
{
  return static_cast<const MatrixObjectType*>(this->itk::ProcessObject::GetOutput(3));
}

template<class TInputImage>
void
PersistentStreamingStatisticsVectorImageFilter2<TInputImage>
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
PersistentStreamingStatisticsVectorImageFilter2<TInputImage>
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
PersistentStreamingStatisticsVectorImageFilter2<TInputImage>
::Reset()
{
  if (m_EnableCovariance)
    {
    m_EnableMean = true;
    }

  TInputImage * inputPtr = const_cast<TInputImage *>(this->GetInput());
  inputPtr->UpdateOutputInformation();

  unsigned int numberOfThreads = this->GetNumberOfThreads();
  unsigned int numberOfComponent = inputPtr->GetNumberOfComponentsPerPixel();

  RealPixelType zeroRealPixel;
  zeroRealPixel.SetSize(numberOfComponent);
  zeroRealPixel.Fill(itk::NumericTraits<RealType>::Zero);
  this->GetMeanOutput()->Set(zeroRealPixel);

  MatrixType zeroMatrix;
  zeroMatrix.SetSize(numberOfComponent, numberOfComponent);
  zeroMatrix.Fill(itk::NumericTraits<RealType>::Zero);
  this->GetCovarianceOutput()->Set(zeroMatrix);
  this->GetCorrelationOutput()->Set(zeroMatrix);

  m_FirstOrderAccumulators.resize(numberOfThreads);
  std::fill(m_FirstOrderAccumulators.begin(), m_FirstOrderAccumulators.end(), zeroRealPixel);

  m_SecondOrderAccumulators.resize(numberOfThreads);
  std::fill(m_SecondOrderAccumulators.begin(), m_SecondOrderAccumulators.end(), zeroMatrix);
}

template<class TInputImage>
void
PersistentStreamingStatisticsVectorImageFilter2<TInputImage>
::Synthetize()
{
  TInputImage * inputPtr = const_cast<TInputImage *>(this->GetInput());
  const unsigned int nbPixels = inputPtr->GetLargestPossibleRegion().GetNumberOfPixels();
  const unsigned int numberOfComponent = inputPtr->GetNumberOfComponentsPerPixel();

  RealPixelType streamFirstOrderAccumulator(numberOfComponent);
  streamFirstOrderAccumulator.Fill(itk::NumericTraits<RealType>::Zero);
  MatrixType    streamSecondOrderAccumulator(numberOfComponent, numberOfComponent);
  streamSecondOrderAccumulator.Fill(itk::NumericTraits<RealType>::Zero);

  const unsigned int numberOfThreads = this->GetNumberOfThreads();
  for (unsigned int i = 0; i < numberOfThreads; ++i)
    {
    if (m_EnableMean)
      streamFirstOrderAccumulator   += m_FirstOrderAccumulators [i];

    if (m_EnableCorrelation || m_EnableCovariance)
      streamSecondOrderAccumulator  += m_SecondOrderAccumulators[i];
    }

  if (m_EnableMean)
    {
    this->GetMeanOutput()->Set(streamFirstOrderAccumulator / nbPixels);

    }

  if (m_EnableCorrelation || m_EnableCovariance)
    {
    MatrixType cor = streamSecondOrderAccumulator / nbPixels;
    this->GetCorrelationOutput()->Set(cor);

    if (m_EnableCovariance)
      {
      const RealPixelType& mean = this->GetMeanOutput()->Get();
      const double regul = static_cast<double>(nbPixels) / (nbPixels - 1);
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
}

template<class TInputImage>
void
PersistentStreamingStatisticsVectorImageFilter2<TInputImage>
::ThreadedGenerateData(const RegionType& outputRegionForThread, int threadId)
{
  // Support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  // Grab the input
  InputImagePointer inputPtr = const_cast<TInputImage *>(this->GetInput());
  RealPixelType& threadFirstOrder  = m_FirstOrderAccumulators [threadId];
  MatrixType&    threadSecondOrder = m_SecondOrderAccumulators[threadId];

  itk::ImageRegionConstIteratorWithIndex<TInputImage> it(inputPtr, outputRegionForThread);

  MatrixType pixelCorr;
  for (it.GoToBegin(); !it.IsAtEnd(); ++it, progress.CompletedPixel())
    {
    PixelType vectorValue = it.Get();

    if (m_EnableMean)
      {
      threadFirstOrder += vectorValue;
      }

    if (m_EnableCorrelation || m_EnableCovariance)
      {
      for (unsigned int r = 0; r < threadSecondOrder.Rows(); ++r)
        {
        for (unsigned int c = 0; c < threadSecondOrder.Cols(); ++c)
          {
          threadSecondOrder(r,c) += vectorValue[r] * vectorValue[c];
          }
        }
      }
    }
}

template <class TImage>
void
PersistentStreamingStatisticsVectorImageFilter2<TImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Mean: "        << this->GetMeanOutput()->Get()        << std::endl;
  os << indent << "Covariance: "  << this->GetCovarianceOutput()->Get()  << std::endl;
  os << indent << "Correlation: " << this->GetCorrelationOutput()->Get() << std::endl;
}

} // end namespace otb
#endif
