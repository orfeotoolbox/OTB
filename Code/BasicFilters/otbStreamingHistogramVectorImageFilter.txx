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
#ifndef __otbStreamingHistogramVectorImageFilter_txx
#define __otbStreamingHistogramVectorImageFilter_txx
#include "otbStreamingHistogramVectorImageFilter.h"

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkNumericTraits.h"
#include "itkProgressReporter.h"
#include "otbMacro.h"

namespace otb
{

template<class TInputImage>
PersistentHistogramVectorImageFilter<TInputImage>
::PersistentHistogramVectorImageFilter() :
  m_ThreadHistogramList(),
  m_Size(),
  m_HistogramMin(),
  m_HistogramMax(),
  m_NoDataFlag(false),
  m_NoDataValue(itk::NumericTraits<InternalPixelType>::Zero),
  m_SubSamplingRate(1)
{
  // first output is a copy of the image, DataObject created by
  // superclass
  //
  // allocate the data objects for the outputs which are
  // just decorators around pixel types and histogram list

  m_Size.Fill(255);

  HistogramListPointerType output = static_cast<HistogramListType*>(this->MakeOutput(1).GetPointer());
  this->itk::ProcessObject::SetNthOutput(1, output.GetPointer());
}

template<class TInputImage>
itk::DataObject::Pointer
PersistentHistogramVectorImageFilter<TInputImage>
::MakeOutput(unsigned int output)
{
  itk::DataObject::Pointer ret;
  switch (output)
    {
    case 0:
      ret = static_cast<itk::DataObject*>(TInputImage::New().GetPointer());
      break;
    case 1:
      ret = static_cast<itk::DataObject*>(HistogramListType::New().GetPointer());
      break;
    }
  return ret;
}

template<class TInputImage>
typename PersistentHistogramVectorImageFilter<TInputImage>::HistogramListType*
PersistentHistogramVectorImageFilter<TInputImage>
::GetHistogramListOutput()
{
  return static_cast<HistogramListType*>(this->itk::ProcessObject::GetOutput(1));
}

template<class TInputImage>
const typename PersistentHistogramVectorImageFilter<TInputImage>::HistogramListType*
PersistentHistogramVectorImageFilter<TInputImage>
::GetHistogramListOutput() const
{
  return static_cast<const HistogramListType*>(this->itk::ProcessObject::GetOutput(1));
}

template<class TInputImage>
void
PersistentHistogramVectorImageFilter<TInputImage>
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
PersistentHistogramVectorImageFilter<TInputImage>
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
PersistentHistogramVectorImageFilter<TInputImage>
::Reset()
{
  TInputImage * inputPtr = const_cast<TInputImage *>(this->GetInput());
  inputPtr->UpdateOutputInformation();

  unsigned int numberOfThreads = this->GetNumberOfThreads();
  unsigned int numberOfComponent = inputPtr->GetNumberOfComponentsPerPixel();

// TODO which is the good value ? (false in MVD2)
  bool clipBins = false;

  // if histogram Min and Max have the wrong size : set to default [0, 255]
  if (m_HistogramMin.Size() != numberOfComponent ||
      m_HistogramMax.Size() != numberOfComponent)
    {
    m_HistogramMin.SetSize(numberOfComponent);
    m_HistogramMax.SetSize(numberOfComponent);

    m_HistogramMin.Fill(itk::NumericTraits<InternalPixelType>::Zero);
    m_HistogramMax.Fill(255);
    }

  // Setup output histogram
  HistogramListType* outputHisto = this->GetHistogramListOutput();
  outputHisto->Clear();
  for (unsigned int k=0; k<numberOfComponent; ++k)
    {
    typename HistogramType::MeasurementVectorType bandMin, bandMax;
    bandMin.SetSize(1);
    bandMax.SetSize(1);
    bandMin.Fill(m_HistogramMin[k]);
    bandMax.Fill(m_HistogramMax[k]);

    typename HistogramType::Pointer histogram = HistogramType::New();
    histogram->SetClipBinsAtEnds(clipBins);

    typename HistogramType::SizeType size;
    size.SetSize(1);
    size.Fill( m_Size[ k ] );
    histogram->SetMeasurementVectorSize(1);
    histogram->Initialize( size, bandMin, bandMax );

    outputHisto->PushBack(histogram);
    }
  
  
  // Setup HistogramLists for each thread
  m_ThreadHistogramList.clear();
  for (unsigned int i=0; i<numberOfThreads; ++i)
    {
    HistogramListPointerType histoList = HistogramListType::New();
    histoList->Clear();
    for (unsigned int k=0; k<numberOfComponent; ++k)
      {
      typename HistogramType::MeasurementVectorType bandMin, bandMax;
      bandMin.SetSize(1);
      bandMax.SetSize(1);
      bandMin.Fill(m_HistogramMin[k]);
      bandMax.Fill(m_HistogramMax[k]);
      
      typename HistogramType::Pointer histogram = HistogramType::New();
      histogram->SetClipBinsAtEnds(clipBins);

      typename HistogramType::SizeType size;
      size.SetSize(1);
      size.Fill( m_Size[ k ] );
      histogram->SetMeasurementVectorSize(1);
      histogram->Initialize(size, bandMin, bandMax );

      histoList->PushBack(histogram);
      }
    m_ThreadHistogramList.push_back(histoList);
    }

}

template<class TInputImage>
void
PersistentHistogramVectorImageFilter<TInputImage>
::Synthetize()
{
  HistogramListType* outputHisto = this->GetHistogramListOutput();

  int          numberOfThreads = this->GetNumberOfThreads();
  unsigned int numberOfComponent = this->GetInput()->GetNumberOfComponentsPerPixel();

  // copy histograms to output
  for (int i = 0; i < numberOfThreads; ++i)
    {
    for (unsigned int j = 0; j < numberOfComponent; ++j)
      {
      HistogramType* outHisto = outputHisto->GetNthElement(j);
      HistogramType* threadHisto = m_ThreadHistogramList[i]->GetNthElement(j);

      typename HistogramType::Iterator iterOutput = outHisto->Begin();
      typename HistogramType::Iterator iterThread = threadHisto->Begin();

      while (iterOutput != outHisto->End() && iterThread != threadHisto->End())
        {
        iterOutput.SetFrequency(iterOutput.GetFrequency()+iterThread.GetFrequency());

        ++iterOutput;
        ++iterThread;
        }
      }
    }
}

template<class TInputImage>
void
PersistentHistogramVectorImageFilter<TInputImage>
::ThreadedGenerateData(const RegionType& outputRegionForThread, itk::ThreadIdType threadId)
{
  /**
   * Grab the input
   */
  InputImagePointer inputPtr = const_cast<TInputImage *>(this->GetInput());
  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  typename HistogramType::IndexType   index;

  itk::ImageRegionConstIteratorWithIndex<TInputImage> it(inputPtr, outputRegionForThread);
  it.GoToBegin();

  bool skipSample = false;

  // do the work
  while (!it.IsAtEnd())
    {
    if (m_SubSamplingRate > 1)
      {
      skipSample = false;
      for (unsigned int i=0; i<InputImageDimension; ++i)
        {
        if (it.GetIndex()[i] % m_SubSamplingRate != 0)
          {
          skipSample = true;
          break;
          }
        }
      if (skipSample)
        {
        ++it;
        progress.CompletedPixel();
        continue;
        }
      }

    PixelType vectorValue = it.Get();

       bool skipSampleNoData=false;
       if(m_NoDataFlag)
      {
      unsigned int itComp=0;
         while( itComp < vectorValue.GetSize() )
           {
           if (vectorValue[itComp]==m_NoDataValue)
                {
          skipSampleNoData=true;
          itComp++;
          }
        else
          {
          skipSampleNoData=false;
          break;
          }
           }
      }

    if( !skipSampleNoData )
      {
      for (unsigned int j = 0; j < vectorValue.GetSize(); ++j)
        {
        typename HistogramType::MeasurementVectorType value;
        value.SetSize(1);
        value.Fill(vectorValue[j]);

        m_ThreadHistogramList[threadId]->GetNthElement(j)->GetIndex(value, index);
        if (!m_ThreadHistogramList[threadId]->GetNthElement(j)->IsIndexOutOfBounds(index))
          {
          // if the measurement vector is out of bound then
          // the GetIndex method has returned an index set to the max size of
          // the invalid dimension - even if the hvector is less than the minimum
          // bin value.
          // If the index isn't valid, we don't increase the frequency.
          // See the comments in Histogram->GetIndex() for more info.
          m_ThreadHistogramList[threadId]->GetNthElement(j)->IncreaseFrequencyOfIndex(index, 1);
          }
        }
      }
      
    ++it;
    progress.CompletedPixel();
    }
}

template <class TImage>
void
PersistentHistogramVectorImageFilter<TImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Histogram minimum: " << this->GetHistogramMin() << std::endl;
  os << indent << "Histogram maximum: " << this->GetHistogramMax() << std::endl;
  os << indent << "Number of bins: " << m_Size[0] << std::endl;
  if (m_NoDataFlag)
    {
    os << indent << "Use NoData: true" << std::endl;
    }
  else
    {
    os << indent << "Use NoData: false" << std::endl;
    }
  os << indent << "NoData value: " << this->GetNoDataValue() << std::endl;
}

} // end namespace otb
#endif
