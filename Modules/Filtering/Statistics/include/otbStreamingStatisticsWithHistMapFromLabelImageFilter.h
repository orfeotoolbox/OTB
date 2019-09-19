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

#ifndef otbStreamingStatisticsWithHistMapFromLabelImageFilter_h
#define otbStreamingStatisticsWithHistMapFromLabelImageFilter_h

#include "otbPersistentImageFilter.h"
#include "itkNumericTraits.h"
#include "itkArray.h"
#include "itkSimpleDataObjectDecorator.h"
#include "otbPersistentFilterStreamingDecorator.h"
#include "otbStreamingStatisticsAccumulators.h"

namespace otb
{

/** \class PersistentStreamingStatisticsWithHistMapFromLabelImageFilter
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
 * \sa StreamingStatisticsWithHistMapFromLabelImageFilter
 * \ingroup Streamed
 * \ingroup Multithreaded
 * \ingroup MathematicalStatisticsImageFilters
 *
 * \ingroup OTBStatistics
 */
template<class TInputVectorImage, class TLabelImage>
class ITK_EXPORT PersistentStreamingStatisticsWithHistMapFromLabelImageFilter :
public PersistentStreamingStatisticsMapFromLabelImageFilter<TInputVectorImage, TLabelImage>
{
public:
  /** Standard Self typedef */
  typedef PersistentStreamingStatisticsWithHistMapFromLabelImageFilter  Self;
  typedef PersistentStreamingStatisticsMapFromLabelImageFilter<TInputVectorImage, TLabelImage> Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(PersistentStreamingStatisticsWithHistMapFromLabelImageFilter, PersistentStreamingStatisticsMapFromLabelImageFilter);

  /** Image related typedefs. */
  typedef TInputVectorImage                   VectorImageType;
  typedef typename TInputVectorImage::Pointer InputVectorImagePointer;
  typedef TLabelImage                         LabelImageType;
  typedef typename TLabelImage::Pointer       LabelImagePointer;

  typedef typename VectorImageType::RegionType                          RegionType;
  typedef typename VectorImageType::PixelType                           VectorPixelType;
  typedef typename VectorImageType::PixelType::ValueType                VectorPixelValueType;
  typedef typename LabelImageType::PixelType                            LabelPixelType;
  typedef itk::VariableLengthVector<double>                             RealVectorPixelType;
  typedef StatisticsAccumulator<RealVectorPixelType>                    AccumulatorType;
  typedef std::unordered_map<LabelPixelType, AccumulatorType >          AccumulatorMapType;
  typedef std::vector<AccumulatorMapType>                               AccumulatorMapCollectionType;
  typedef std::unordered_map<LabelPixelType, RealVectorPixelType >      PixelValueMapType;
  typedef std::unordered_map<LabelPixelType, double>                    LabelPopulationMapType;
//  typedef std::unordered_map<double,std::unordered_map<LabelPixelType,
//        std::unordered_map<double, double>>>                            HistogramMap;

  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputVectorImage::ImageDimension);

  /** Image related typedefs. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TInputVectorImage::ImageDimension);

  itkGetMacro(NoDataValue, VectorPixelValueType);
  itkSetMacro(NoDataValue, VectorPixelValueType);
  itkGetMacro(UseNoDataValue, bool);
  itkSetMacro(UseNoDataValue, bool);

  /** Smart Pointer type to a DataObject. */
  typedef typename itk::DataObject::Pointer DataObjectPointer;
  typedef itk::ProcessObject::DataObjectPointerArraySizeType DataObjectPointerArraySizeType;

  typedef itk::ImageBase<InputImageDimension> ImageBaseType;
  typedef typename ImageBaseType::RegionType InputImageRegionType;

  /** Type of DataObjects used for scalar outputs */
  typedef itk::SimpleDataObjectDecorator<PixelValueMapType>  PixelValueMapObjectType;

  /** Set input label image */
  virtual void SetInputLabelImage( const LabelImageType *image);

  /** Get input label image */
  virtual const LabelImageType * GetInputLabelImage();

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

//  /** Return the computed histogram */
//  HistogramMap GetHistogramMap() const;

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
  PersistentStreamingStatisticsWithHistMapFromLabelImageFilter();
  ~PersistentStreamingStatisticsWithHistMapFromLabelImageFilter() override {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  void ThreadedGenerateData(const RegionType& outputRegionForThread, itk::ThreadIdType threadId ) override;

private:
  PersistentStreamingStatisticsWithHistMapFromLabelImageFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

  VectorPixelValueType                   m_NoDataValue;
  bool                                   m_UseNoDataValue;

  AccumulatorMapCollectionType           m_AccumulatorMaps;

  PixelValueMapType                      m_MeanRadiometricValue;
  PixelValueMapType                      m_StDevRadiometricValue;
  PixelValueMapType                      m_MinRadiometricValue;
  PixelValueMapType                      m_MaxRadiometricValue;

  LabelPopulationMapType                 m_LabelPopulation;
//  HistogramMap                         m_Histogram;

}; // end of class PersistentStreamingStatisticsWithHistMapFromLabelImageFilter


/*===========================================================================*/

/** \class StreamingStatisticsWithHistMapFromLabelImageFilter
 * \brief Computes mean radiometric value for each label of a label image, based on a support VectorImage
 *
 * Currently the class only computes the mean value.
 *
 * This class streams the whole input image through the PersistentStreamingStatisticsWithHistMapFromLabelImageFilter.
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
 * typedef otb::StreamingStatisticsWithHistMapFromLabelImageFilter<ImageType> StatisticsType;
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

template<class TInputVectorImage, class TLabelImage>
class ITK_EXPORT StreamingStatisticsWithHistMapFromLabelImageFilter :
public PersistentFilterStreamingDecorator<PersistentStreamingStatisticsWithHistMapFromLabelImageFilter<TInputVectorImage, TLabelImage> >
{
public:
  /** Standard Self typedef */
  typedef StreamingStatisticsWithHistMapFromLabelImageFilter Self;
  typedef PersistentFilterStreamingDecorator
      <PersistentStreamingStatisticsWithHistMapFromLabelImageFilter<TInputVectorImage, TLabelImage> > Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(StreamingStatisticsWithHistMapFromLabelImageFilter, PersistentFilterStreamingDecorator);

  typedef TInputVectorImage                   VectorImageType;
  typedef TLabelImage                         LabelImageType;

  typedef typename VectorImageType::PixelType                        VectorPixelType;
  typedef typename VectorImageType::PixelType::ValueType             VectorPixelValueType;

  typedef typename Superclass::FilterType::PixelValueMapType         PixelValueMapType;
  typedef typename Superclass::FilterType::PixelValueMapObjectType   PixelValueMapObjectType;

  typedef typename Superclass::FilterType::LabelPopulationMapType    LabelPopulationMapType;
//  typedef typename Superclass::FilterType::HistogramMap            HistogramMap;

  /** Set input multispectral image */
  using Superclass::SetInput;
  void SetInput(const VectorImageType * input)
  {
    this->GetFilter()->SetInput(input);
  }

  /** Get input multispectral image */
  const VectorImageType * GetInput()
  {
    return this->GetFilter()->GetInput();
  }

  /** Set input label image (monoband) */
  void SetInputLabelImage(const LabelImageType * input)
  {
    this->GetFilter()->SetInputLabelImage(input);
  }

  /** Get input label image (monoband) */
  const LabelImageType * GetInputLabelImage()
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

//  /** Return the computed number of labeled pixels for each label */
//  HistogramMap GetHistogramMap() const
//  {
//    return this->GetFilter()->GetHistogramMap();
//  }

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
  StreamingStatisticsWithHistMapFromLabelImageFilter() {}
  /** Destructor */
  ~StreamingStatisticsWithHistMapFromLabelImageFilter() override {}

private:
  StreamingStatisticsWithHistMapFromLabelImageFilter(const Self &) = delete;
  void operator =(const Self&) = delete;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStreamingStatisticsWithHistMapFromLabelImageFilter.hxx"
#endif

#endif
