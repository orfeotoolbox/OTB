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

#ifndef otbStreamingStatisticsMapFromLabelImageFilter_h
#define otbStreamingStatisticsMapFromLabelImageFilter_h

#include "otbPersistentImageFilter.h"
#include "itkNumericTraits.h"
#include "itkArray.h"
#include "itkSimpleDataObjectDecorator.h"
#include "otbPersistentFilterStreamingDecorator.h"


namespace otb
{

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
 * \todo Implement other statistics (min, max, stddev...)
 *
 * \sa StreamingStatisticsMapFromLabelImageFilter
 * \ingroup Streamed
 * \ingroup Multithreaded
 * \ingroup MathematicalStatisticsImageFilters
 *
 * \ingroup OTBStatistics
 */
template<class TInputVectorImage, class TLabelImage>
class ITK_EXPORT PersistentStreamingStatisticsMapFromLabelImageFilter :
  public PersistentImageFilter<TInputVectorImage, TInputVectorImage>
{
public:
  /** Standard Self typedef */
  typedef PersistentStreamingStatisticsMapFromLabelImageFilter               Self;
  typedef PersistentImageFilter<TInputVectorImage, TInputVectorImage> Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(PersistentStreamingStatisticsMapFromLabelImageFilter, PersistentImageFilter);

  /** Image related typedefs. */
  typedef TInputVectorImage                   VectorImageType;
  typedef typename TInputVectorImage::Pointer InputVectorImagePointer;
  typedef TLabelImage                         LabelImageType;
  typedef typename TLabelImage::Pointer       LabelImagePointer;

  typedef typename VectorImageType::PixelType                           VectorPixelType;
  typedef typename LabelImageType::PixelType                            LabelPixelType;
  typedef std::map<LabelPixelType, itk::VariableLengthVector<double> >  MeanValueMapType;
  typedef std::map<LabelPixelType, double>                              LabelPopulationMapType;

  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputVectorImage::ImageDimension);

  /** Image related typedefs. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TInputVectorImage::ImageDimension);

  /** Smart Pointer type to a DataObject. */
  typedef typename itk::DataObject::Pointer DataObjectPointer;
  typedef itk::ProcessObject::DataObjectPointerArraySizeType DataObjectPointerArraySizeType;

  typedef itk::ImageBase<InputImageDimension> ImageBaseType;
  typedef typename ImageBaseType::RegionType InputImageRegionType;

  /** Type of DataObjects used for scalar outputs */
  typedef itk::SimpleDataObjectDecorator<MeanValueMapType>  MeanValueMapObjectType;

  /** Set input label image */
  virtual void SetInputLabelImage( const LabelImageType *image);

  /** Get input label image */
  virtual const LabelImageType * GetInputLabelImage();

  /** Return the computed Mean for each label in the input label image */
  MeanValueMapType GetMeanValueMap() const;

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
  ~PersistentStreamingStatisticsMapFromLabelImageFilter() override {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /** GenerateData. */
  void  GenerateData() override;

private:
  PersistentStreamingStatisticsMapFromLabelImageFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

  MeanValueMapType                       m_RadiometricValueAccumulator;
  LabelPopulationMapType                 m_LabelPopulation;
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
 * StatisticsType::MeanValueMapType meanValueMap = statistics->GetMeanValueMap();
 * StatisticsType::MeanValueMapType::const_iterator end = meanValueMap();
 * for (StatisticsType::MeanValueMapType::const_iterator it = meanValueMap.begin(); it != end; ++it)
 * {
 *       std::cout << "label : " << it->first << " , ";
 *                 << "mean value : " << it->second << std::endl;
 * }
 * \endcode
 *
 * \todo Implement other statistics (min, max, stddev...)
 * \todo Reimplement as a multi-threaded filter
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
class ITK_EXPORT StreamingStatisticsMapFromLabelImageFilter :
  public PersistentFilterStreamingDecorator<PersistentStreamingStatisticsMapFromLabelImageFilter<TInputVectorImage, TLabelImage> >
{
public:
  /** Standard Self typedef */
  typedef StreamingStatisticsMapFromLabelImageFilter Self;
  typedef PersistentFilterStreamingDecorator
  <PersistentStreamingStatisticsMapFromLabelImageFilter<TInputVectorImage, TLabelImage> > Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(StreamingStatisticsMapFromLabelImageFilter, PersistentFilterStreamingDecorator);

  typedef TInputVectorImage                   VectorImageType;
  typedef TLabelImage                         LabelImageType;

  typedef typename Superclass::FilterType::MeanValueMapType          MeanValueMapType;
  typedef typename Superclass::FilterType::MeanValueMapObjectType    MeanValueMapObjectType;

  typedef typename Superclass::FilterType::LabelPopulationMapType    LabelPopulationMapType;

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
  MeanValueMapType GetMeanValueMap() const
  {
    return this->GetFilter()->GetMeanValueMap();
  }

  /** Return the computed number of labeled pixels for each label */
  LabelPopulationMapType GetLabelPopulationMap() const
  {
    return this->GetFilter()->GetLabelPopulationMap();
  }

protected:
  /** Constructor */
  StreamingStatisticsMapFromLabelImageFilter() {}
  /** Destructor */
  ~StreamingStatisticsMapFromLabelImageFilter() override {}

private:
  StreamingStatisticsMapFromLabelImageFilter(const Self &) = delete;
  void operator =(const Self&) = delete;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStreamingStatisticsMapFromLabelImageFilter.hxx"
#endif

#endif
