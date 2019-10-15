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

#ifndef otbStreamingStatisticsMapFromLabelImageFilter_h
#define otbStreamingStatisticsMapFromLabelImageFilter_h

#include "otbPersistentImageFilter.h"
#include "itkNumericTraits.h"
#include "itkArray.h"
#include "itkSimpleDataObjectDecorator.h"
#include "otbPersistentFilterStreamingDecorator.h"
#include <unordered_map>

namespace otb
{

/** \class StatisticsMapAccumulator
 * \brief Holds statistics for each label of a label image
 *
 * Intended to store and update the following statistics:
 * -count
 * -sum of values
 * -sum of squared values
 * -min
 * -max
 *
 * TODO:
 * -Better architecture?
 * -Enrich with other statistics?
 * -Move this class in a dedicated source to enable its use by other otbStatistics stuff?
 *
 * \ingroup OTBStatistics
 */
template <class TRealVectorPixelType>
class StatisticsAccumulator
{
public:
  typedef typename TRealVectorPixelType::ValueType  RealValueType;
  typedef uint64_t                                  PixelCountType;
  typedef itk::VariableLengthVector<PixelCountType> PixelCountVectorType;

  // Constructor (default)
  StatisticsAccumulator() : m_Count(), m_NoDataValue(), m_UseNoDataValue()
  {
  }

  // Constructor (initialize the accumulator with the given pixel)
  StatisticsAccumulator(RealValueType noDataValue, bool useNoDataValue, const TRealVectorPixelType& pixel)
    : m_NoDataValue(noDataValue), m_Count(1), m_UseNoDataValue(useNoDataValue)
  {
    m_Count = 1;
    m_Sum   = pixel;
    m_Min   = pixel;
    m_Max   = pixel;
    m_SqSum = pixel;
    m_BandCount.SetSize(pixel.GetSize());
    for (unsigned int band = 0; band < m_SqSum.GetSize(); band++)
    {
      auto val = pixel[band];
      if (!m_UseNoDataValue || val != m_NoDataValue)
      {
        m_BandCount[band] = 1;
        m_SqSum[band] *= m_SqSum[band];
      }
      else
      {
        m_BandCount[band] = 0;
      }
    }
  }

  // Function update (pixel)
  void Update(const TRealVectorPixelType& pixel)
  {
    m_Count++;
    const unsigned int nBands = pixel.GetSize();
    for (unsigned int band = 0; band < nBands; band++)
    {
      const RealValueType value   = pixel[band];
      const RealValueType sqValue = value * value;

      if(!m_UseNoDataValue || value != m_NoDataValue)
        {
        UpdateValues(1, value, sqValue, value, value, m_BandCount[band], m_Sum[band], m_SqSum[band], m_Min[band],
                   m_Max[band]);
        }
    }
  }

  // Function update (self)
  void Update(const StatisticsAccumulator& other)
  {
    m_Count += other.m_Count;
    const unsigned int nBands = other.m_Sum.GetSize();
    for (unsigned int band = 0; band < nBands; band++)
    {
      UpdateValues(other.m_BandCount[band], other.m_Sum[band], other.m_SqSum[band], other.m_Min[band], other.m_Max[band], m_BandCount[band], m_Sum[band],
                   m_SqSum[band], m_Min[band], m_Max[band]);
    }
  }

  // Accessors
  itkGetMacro(BandCount, PixelCountVectorType);
  itkGetMacro(Sum, TRealVectorPixelType);
  itkGetMacro(SqSum, TRealVectorPixelType);
  itkGetMacro(Min, TRealVectorPixelType);
  itkGetMacro(Max, TRealVectorPixelType);
  itkGetMacro(Count, double);

private:
  void UpdateValues(PixelCountType otherCount, RealValueType otherSum, RealValueType otherSqSum, RealValueType otherMin, RealValueType otherMax,
                    PixelCountType& count, RealValueType& sum, RealValueType& sqSum, RealValueType& min, RealValueType& max)
  {
    count += otherCount;
    sum += otherSum;
    sqSum += otherSqSum;
    if (otherMin < min)
      min = otherMin;
    if (otherMax > max)
      max = otherMax;
  }

protected:
  PixelCountVectorType m_BandCount;
  TRealVectorPixelType m_Sum;
  TRealVectorPixelType m_SqSum;
  TRealVectorPixelType m_Min;
  TRealVectorPixelType m_Max;
  RealValueType        m_NoDataValue;
  PixelCountType       m_Count;
  bool                 m_UseNoDataValue;
};

/** \class PersistentStreamingStatisticsMapFromLabelImageFilter
 * \brief Computes mean radiometric value for each label of a label image, based on a support VectorImage
 *
 * This filter persists its temporary data. It means that if you Update it n times on n different
 * requested regions, the output statistics will be the statitics of the whole set of n regions.
 *
 * To reset the temporary data, one should call the Reset() function.
 *
 * To get the statistics once the regions have been processed via the pipeline, use the Synthetize() method.
 *
 *
 * \sa StreamingStatisticsMapFromLabelImageFilter
 * \ingroup Streamed
 * \ingroup Multithreaded
 * \ingroup MathematicalStatisticsImageFilters
 *
 * \ingroup OTBStatistics
 */
template <class TInputVectorImage, class TLabelImage>
class ITK_EXPORT PersistentStreamingStatisticsMapFromLabelImageFilter : public PersistentImageFilter<TInputVectorImage, TInputVectorImage>
{
public:
  /** Standard Self typedef */
  typedef PersistentStreamingStatisticsMapFromLabelImageFilter Self;
  typedef PersistentImageFilter<TInputVectorImage, TInputVectorImage> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(PersistentStreamingStatisticsMapFromLabelImageFilter, PersistentImageFilter);

  /** Image related typedefs. */
  typedef TInputVectorImage                   VectorImageType;
  typedef typename TInputVectorImage::Pointer InputVectorImagePointer;
  typedef TLabelImage                         LabelImageType;
  typedef typename TLabelImage::Pointer       LabelImagePointer;

  typedef typename VectorImageType::RegionType           RegionType;
  typedef typename VectorImageType::PixelType            VectorPixelType;
  typedef typename VectorImageType::PixelType::ValueType VectorPixelValueType;
  typedef typename LabelImageType::PixelType             LabelPixelType;
  typedef itk::VariableLengthVector<double>              RealVectorPixelType;
  typedef StatisticsAccumulator<RealVectorPixelType>     AccumulatorType;
  typedef std::unordered_map<LabelPixelType, AccumulatorType> AccumulatorMapType;
  typedef std::vector<AccumulatorMapType> AccumulatorMapCollectionType;
  typedef std::unordered_map<LabelPixelType, RealVectorPixelType> PixelValueMapType;
  typedef std::unordered_map<LabelPixelType, double>              LabelPopulationMapType;

  itkStaticConstMacro(InputImageDimension, unsigned int, TInputVectorImage::ImageDimension);

  /** Image related typedefs. */
  itkStaticConstMacro(ImageDimension, unsigned int, TInputVectorImage::ImageDimension);

  itkGetMacro(NoDataValue, VectorPixelValueType);
  itkSetMacro(NoDataValue, VectorPixelValueType);
  itkGetMacro(UseNoDataValue, bool);
  itkSetMacro(UseNoDataValue, bool);

  /** Smart Pointer type to a DataObject. */
  typedef typename itk::DataObject::Pointer                  DataObjectPointer;
  typedef itk::ProcessObject::DataObjectPointerArraySizeType DataObjectPointerArraySizeType;

  typedef itk::ImageBase<InputImageDimension> ImageBaseType;
  typedef typename ImageBaseType::RegionType  InputImageRegionType;

  /** Type of DataObjects used for scalar outputs */
  typedef itk::SimpleDataObjectDecorator<PixelValueMapType> PixelValueMapObjectType;

  /** Set input label image */
  virtual void SetInputLabelImage(const LabelImageType* image);

  /** Get input label image */
  virtual const LabelImageType* GetInputLabelImage();

  /** Return the computed Mean for each label in the input label image */
  PixelValueMapType GetMeanValueMap() const;

  /** Return the computed Standard Deviation for each label in the input label image */
  PixelValueMapType GetStandardDeviationValueMap() const;

  /** Return the computed Min for each label in the input label image */
  PixelValueMapType GetMinValueMap() const;

  /** Return the computed Max for each label in the input label image */
  PixelValueMapType GetMaxValueMap() const;

  /** Return the computed number of labeled pixels for each label in the input label image */
  LabelPopulationMapType GetLabelPopulationMap() const;

  /** Make a DataObject of the correct type to be used as the specified
   * output. */
  DataObjectPointer MakeOutput(DataObjectPointerArraySizeType idx) override;
  using Superclass::MakeOutput;

  /** Pass the input through unmodified. Do this by Grafting in the
   *  AllocateOutputs method.
   */
  void AllocateOutputs() override;

  void GenerateOutputInformation() override;

  void Synthetize(void) override;

  void Reset(void) override;

  /** Due to heterogeneous input template GenerateInputRequestedRegion must be reimplemented using explicit cast **/
  /** This new implementation is inspired by the one of itk::ImageToImageFilter **/
  void GenerateInputRequestedRegion() override;

protected:
  PersistentStreamingStatisticsMapFromLabelImageFilter();
  ~PersistentStreamingStatisticsMapFromLabelImageFilter() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  void ThreadedGenerateData(const RegionType& outputRegionForThread, itk::ThreadIdType threadId) override;

private:
  PersistentStreamingStatisticsMapFromLabelImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  VectorPixelValueType m_NoDataValue;
  bool                 m_UseNoDataValue;

  AccumulatorMapCollectionType m_AccumulatorMaps;

  PixelValueMapType m_MeanRadiometricValue;
  PixelValueMapType m_StDevRadiometricValue;
  PixelValueMapType m_MinRadiometricValue;
  PixelValueMapType m_MaxRadiometricValue;

  LabelPopulationMapType m_LabelPopulation;

}; // end of class PersistentStreamingStatisticsMapFromLabelImageFilter


/*===========================================================================*/

/** \class StreamingStatisticsMapFromLabelImageFilter
 * \brief Computes mean radiometric value for each label of a label image, based on a support VectorImage
 *
 * Currently the class only computes the mean value.
 *
 * This class streams the whole input image through the PersistentStreamingStatisticsMapFromLabelImageFilter.
 *
 * This way, it allows computing the first order global statistics of this image.
 * It calls the Reset() method of the PersistentStatisticsImageFilter before streaming
 * the image and the Synthetize() method of the PersistentStatisticsImageFilter
 * after having streamed the image to compute the statistics.
 * The accessor on the results are wrapping the accessors of the
 * internal PersistentStatisticsImageFilter.
 *
 * This filter can be used as:
 * \code
 * typedef otb::StreamingStatisticsMapFromLabelImageFilter<ImageType> StatisticsType;
 * StatisticsType::Pointer statistics = StatisticsType::New();
 * statistics->SetInput(reader->GetOutput());
 * statistics->Update();
 * StatisticsType::PixelValueMapType meanValueMap = statistics->GetMeanValueMap();
 * StatisticsType::PixelValueMapType::const_iterator end = meanValueMap();
 * for (StatisticsType::PixelValueMapType::const_iterator it = meanValueMap.begin(); it != end; ++it)
 * {
 *       std::cout << "label : " << it->first << " , ";
 *                 << "mean value : " << it->second << std::endl;
 * }
 * \endcode
 *
 *
 * \sa PersistentStatisticsImageFilter
 * \sa PersistentImageFilter
 * \sa PersistentFilterStreamingDecorator
 * \sa StreamingImageVirtualWriter
 *
 * \ingroup Streamed
 * \ingroup Multithreaded
 * \ingroup MathematicalStatisticsImageFilters
 *
 * \ingroup OTBStatistics
 */

template <class TInputVectorImage, class TLabelImage>
class ITK_EXPORT StreamingStatisticsMapFromLabelImageFilter
    : public PersistentFilterStreamingDecorator<PersistentStreamingStatisticsMapFromLabelImageFilter<TInputVectorImage, TLabelImage>>
{
public:
  /** Standard Self typedef */
  typedef StreamingStatisticsMapFromLabelImageFilter Self;
  typedef PersistentFilterStreamingDecorator<PersistentStreamingStatisticsMapFromLabelImageFilter<TInputVectorImage, TLabelImage>> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(StreamingStatisticsMapFromLabelImageFilter, PersistentFilterStreamingDecorator);

  typedef TInputVectorImage VectorImageType;
  typedef TLabelImage       LabelImageType;

  typedef typename VectorImageType::PixelType            VectorPixelType;
  typedef typename VectorImageType::PixelType::ValueType VectorPixelValueType;

  typedef typename Superclass::FilterType::PixelValueMapType       PixelValueMapType;
  typedef typename Superclass::FilterType::PixelValueMapObjectType PixelValueMapObjectType;

  typedef typename Superclass::FilterType::LabelPopulationMapType LabelPopulationMapType;

  /** Set input multispectral image */
  using Superclass::SetInput;
  void SetInput(const VectorImageType* input)
  {
    this->GetFilter()->SetInput(input);
  }

  /** Get input multispectral image */
  const VectorImageType* GetInput()
  {
    return this->GetFilter()->GetInput();
  }

  /** Set input label image (monoband) */
  void SetInputLabelImage(const LabelImageType* input)
  {
    this->GetFilter()->SetInputLabelImage(input);
  }

  /** Get input label image (monoband) */
  const LabelImageType* GetInputLabelImage()
  {
    return this->GetFilter()->GetInputLabelImage();
  }

  /** Return the computed Mean for each label */
  PixelValueMapType GetMeanValueMap() const
  {
    return this->GetFilter()->GetMeanValueMap();
  }

  /** Return the computed Standard Deviation for each label */
  PixelValueMapType GetStandardDeviationValueMap() const
  {
    return this->GetFilter()->GetStandardDeviationValueMap();
  }

  /** Return the computed Min for each label */
  PixelValueMapType GetMinValueMap() const
  {
    return this->GetFilter()->GetMinValueMap();
  }

  /** Return the computed Max for each label */
  PixelValueMapType GetMaxValueMap() const
  {
    return this->GetFilter()->GetMaxValueMap();
  }

  /** Return the computed number of labeled pixels for each label */
  LabelPopulationMapType GetLabelPopulationMap() const
  {
    return this->GetFilter()->GetLabelPopulationMap();
  }

  /** Set the no data value */
  void SetNoDataValue(VectorPixelValueType value)
  {
    this->GetFilter()->SetNoDataValue(value);
  }

  /** Return the no data value */
  VectorPixelValueType GetNoDataValue() const
  {
    return this->GetFilter()->GetNoDataValue();
  }

  /** Configure whether no data pixels ignored, treating each band independently */
  void SetUseNoDataValue(bool useNoDataValue)
  {
    this->GetFilter()->SetUseNoDataValue(useNoDataValue);
  }

  /** Return whether no data pixels are ignored */
  bool GetUseNoDataValue() const
  {
    return this->GetFilter()->GetUseNoDataValue();
  }

protected:
  /** Constructor */
  StreamingStatisticsMapFromLabelImageFilter()
  {
  }
  /** Destructor */
  ~StreamingStatisticsMapFromLabelImageFilter() override
  {
  }

private:
  StreamingStatisticsMapFromLabelImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStreamingStatisticsMapFromLabelImageFilter.hxx"
#endif

#endif
