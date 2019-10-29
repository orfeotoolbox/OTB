/*
 * Copyright (C) 1999-2011 Insight Software Consortium
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbStreamingHistogramVectorImageFilter_h
#define otbStreamingHistogramVectorImageFilter_h

#include "otbPersistentImageFilter.h"
#include "otbPersistentFilterStreamingDecorator.h"

#include "otbObjectList.h"
#include "itkStatisticsAlgorithm.h"
#include "itkNumericTraits.h"
#include "itkHistogram.h"

namespace otb
{

/** \class PersistentHistogramVectorImageFilter
 * \brief Compute the histogram of a large image using streaming
 *
 *  This filter persists its temporary data. It means that if you Update it n times on n different
 * requested regions, the output histogram will be the histogram of the whole set of n regions.
 *
 * To reset the temporary data, one should call the Reset() function.
 *
 * To get the histogram once the regions have been processed via the pipeline, use the Synthetize() method.
 *
 * \sa PersistentImageFilter
 * \ingroup Streamed
 * \ingroup Multithreaded
 * \ingroup MathematicalStatisticsImageFilters
 *
 *
 * \ingroup OTBStatistics
 */
template <class TInputImage>
class ITK_EXPORT PersistentHistogramVectorImageFilter : public PersistentImageFilter<TInputImage, TInputImage>
{
public:
  /** Standard Self typedef */
  typedef PersistentHistogramVectorImageFilter Self;
  typedef PersistentImageFilter<TInputImage, TInputImage> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(PersistentHistogramVectorImageFilter, PersistentImageFilter);

  /** Image related typedefs. */
  typedef TInputImage                             ImageType;
  typedef typename TInputImage::Pointer           InputImagePointer;
  typedef typename TInputImage::RegionType        RegionType;
  typedef typename TInputImage::SizeType          SizeType;
  typedef typename TInputImage::IndexType         IndexType;
  typedef typename TInputImage::PixelType         PixelType;
  typedef typename TInputImage::InternalPixelType InternalPixelType;

  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);

  /** Image related typedefs. */
  itkStaticConstMacro(ImageDimension, unsigned int, TInputImage::ImageDimension);

  /** Type to use for computations. */
  typedef typename itk::NumericTraits<InternalPixelType>::RealType RealType;
  typedef itk::VariableLengthVector<RealType>                      RealPixelType;

  /** Smart Pointer type to a DataObject. */
  typedef typename itk::DataObject::Pointer                  DataObjectPointer;
  typedef itk::ProcessObject::DataObjectPointerArraySizeType DataObjectPointerArraySizeType;

  /** Types for histogram */
  typedef itk::Statistics::DenseFrequencyContainer2 DFContainerType;

  typedef typename itk::NumericTraits<InternalPixelType>::RealType HistogramMeasurementRealType;

  typedef itk::Statistics::Histogram<HistogramMeasurementRealType, DFContainerType> HistogramType;

  typedef itk::VariableLengthVector<unsigned int> CountVectorType;

  typedef PixelType                                      MeasurementVectorType;
  typedef ObjectList<HistogramType>                      HistogramListType;
  typedef typename HistogramListType::Pointer            HistogramListPointerType;
  typedef typename std::vector<HistogramListPointerType> ArrayHistogramListType;


  /** Set the no data value. These value are ignored in histogram
   *  computation if NoDataFlag is On
   */
  itkSetMacro(NoDataValue, InternalPixelType);

  /** Set the no data value. These value are ignored in histogram
   *  computation if NoDataFlag is On
   */
  itkGetConstReferenceMacro(NoDataValue, InternalPixelType);

  /** Set the NoDataFlag. If set to true, samples with values equal to
   *  m_NoDataValue are ignored.
   */
  itkSetMacro(NoDataFlag, bool);

  /** Get the NoDataFlag. If set to true, samples with values equal to
   *  m_NoDataValue are ignored.
   */
  itkGetMacro(NoDataFlag, bool);

  /** Toggle the NoDataFlag. If set to true, samples with values equal to
   *  m_NoDataValue are ignored.
   */
  itkBooleanMacro(NoDataFlag);

  inline void SetNumberOfBins(unsigned int i, CountVectorType::ValueType size)
  {
    m_Size[i] = size;
  }

  inline void SetNumberOfBins(const CountVectorType& size)
  {
    m_Size = size;
  }

  /** Return the computed histogram list */
  HistogramListType*       GetHistogramListOutput();
  const HistogramListType* GetHistogramListOutput() const;

  /** Get the minimum values for histograms */
  itkGetConstReferenceMacro(HistogramMin, MeasurementVectorType);

  /** Set the minimum values for histograms */
  itkSetMacro(HistogramMin, MeasurementVectorType);

  /** Get the maximum values for histograms */
  itkGetConstReferenceMacro(HistogramMax, MeasurementVectorType);

  /** Set the maximum values for histograms */
  itkSetMacro(HistogramMax, MeasurementVectorType);

  /** Set the subsampling rate */
  itkSetMacro(SubSamplingRate, unsigned int);

  /** Get the subsampling rate */
  itkGetMacro(SubSamplingRate, unsigned int);

  /** Make a DataObject of the correct type to be used as the specified
   * output.
   */
  DataObjectPointer MakeOutput(DataObjectPointerArraySizeType idx) override;
  using Superclass::MakeOutput;

  /** Pass the input through unmodified. Do this by Grafting in the
   *  AllocateOutputs method.
   */
  void AllocateOutputs() override;
  void GenerateOutputInformation() override;
  void Synthetize(void) override;
  void Reset(void) override;

protected:
  PersistentHistogramVectorImageFilter();
  ~PersistentHistogramVectorImageFilter() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;
  /** Multi-thread version GenerateData. */
  void ThreadedGenerateData(const RegionType& outputRegionForThread, itk::ThreadIdType threadId) override;

private:
  PersistentHistogramVectorImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  ArrayHistogramListType m_ThreadHistogramList;
  CountVectorType        m_Size;
  MeasurementVectorType  m_HistogramMin;
  MeasurementVectorType  m_HistogramMax;
  bool                   m_NoDataFlag;
  InternalPixelType      m_NoDataValue;

  /** Set the subsampling along each direction */
  unsigned int m_SubSamplingRate;

}; // end of class PersistentStatisticsVectorImageFilter

/**===========================================================================*/

/** \class StreamingHistogramVectorImageFilter
 * \brief This class streams the whole input image through the PersistentHistogramVectorImageFilter.
 *
 * This way, it allows computing the min/max of this image. It calls the
 * Reset() method of the PersistentHistogramVectorImageFilter before streaming the image and the
 * Synthetize() method of the PersistentHistogramVectorImageFilter after having streamed the image
 * to compute the statistics. The accessor on the results are wrapping the accessors of the
 * internal PersistentMinMaxImageFilter.
 *
 * \sa PersistentStatisticsVectorImageFilter
 * \sa PersistentImageFilter
 * \sa PersistentFilterStreamingDecorator
 * \sa StreamingImageVirtualWriter
 * \ingroup Streamed
 * \ingroup Multithreaded
 * \ingroup MathematicalStatisticsImageFilters
 *
 * \ingroup OTBStatistics
 */

template <class TInputImage>
class ITK_EXPORT StreamingHistogramVectorImageFilter : public PersistentFilterStreamingDecorator<PersistentHistogramVectorImageFilter<TInputImage>>
{
public:
  /** Standard Self typedef */
  typedef StreamingHistogramVectorImageFilter                                                   Self;
  typedef PersistentFilterStreamingDecorator<PersistentHistogramVectorImageFilter<TInputImage>> Superclass;
  typedef itk::SmartPointer<Self>                                                               Pointer;
  typedef itk::SmartPointer<const Self>                                                         ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(StreamingHistogramVectorImageFilter, PersistentFilterStreamingDecorator);

  typedef TInputImage                     InputImageType;
  typedef typename Superclass::FilterType InternalFilterType;

  /** Types needed for histograms */
  typedef typename InternalFilterType::HistogramType     HistogramType;
  typedef typename InternalFilterType::HistogramListType HistogramListType;

  using Superclass::SetInput;
  void SetInput(InputImageType* input)
  {
    this->GetFilter()->SetInput(input);
  }
  const InputImageType* GetInput()
  {
    return this->GetFilter()->GetInput();
  }

  /** Return the computed histogram */
  HistogramListType* GetHistogramList()
  {
    return this->GetFilter()->GetHistogramListOutput();
  }


protected:
  /** Constructor */
  StreamingHistogramVectorImageFilter(){};
  /** Destructor */
  ~StreamingHistogramVectorImageFilter() override
  {
  }

private:
  StreamingHistogramVectorImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStreamingHistogramVectorImageFilter.hxx"
#endif

#endif
