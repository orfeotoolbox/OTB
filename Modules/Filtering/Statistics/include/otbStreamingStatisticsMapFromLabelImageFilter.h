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
#ifndef __otbStreamingStatisticsMapFromLabelImageFilter_h
#define __otbStreamingStatisticsMapFromLabelImageFilter_h

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
  virtual DataObjectPointer MakeOutput(DataObjectPointerArraySizeType idx);
  using Superclass::MakeOutput;

  /** Pass the input through unmodified. Do this by Grafting in the
   *  AllocateOutputs method.
   */
  void AllocateOutputs();

  virtual void GenerateOutputInformation();

  void Synthetize(void);

  void Reset(void);

  /** Due to heterogeneous input template GenerateInputRequestedRegion must be reimplemented using explicit cast **/
  /** This new implementation is inspired by the one of itk::ImageToImageFilter **/
  void GenerateInputRequestedRegion();

protected:
  PersistentStreamingStatisticsMapFromLabelImageFilter();
  virtual ~PersistentStreamingStatisticsMapFromLabelImageFilter() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** GenerateData. */
  void  GenerateData();

private:
  PersistentStreamingStatisticsMapFromLabelImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

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
  virtual ~StreamingStatisticsMapFromLabelImageFilter() {}

private:
  StreamingStatisticsMapFromLabelImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStreamingStatisticsMapFromLabelImageFilter.txx"
#endif

#endif
