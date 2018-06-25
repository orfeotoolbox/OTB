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

#ifndef otbStreamingMinMaxVectorImageFilter_h
#define otbStreamingMinMaxVectorImageFilter_h

#include "otbPersistentImageFilter.h"
#include "otbPersistentFilterStreamingDecorator.h"
#include "itkSimpleDataObjectDecorator.h"
#include "itkImageRegionSplitter.h"
#include "itkVariableSizeMatrix.h"
#include "itkVariableLengthVector.h"

namespace otb
{

/** \class PersistentMinMaxVectorImageFilter
 * \brief Compute min. max of a large image using streaming
 *
 *  This filter persists its temporary data. It means that if you Update it n times on n different
 * requested regions, the output statistics will be the statitics of the whole set of n regions.
 *
 * To reset the temporary data, one should call the Reset() function.
 *
 * To get the statistics once the regions have been processed via the pipeline, use the Synthetize() method.
 *
 * \sa PersistentImageFilter
 * \ingroup Streamed
 * \ingroup Multithreaded
 * \ingroup MathematicalStatisticsImageFilters
 *
 *
 * \ingroup OTBStatistics
 */
template<class TInputImage>
class ITK_EXPORT PersistentMinMaxVectorImageFilter :
  public PersistentImageFilter<TInputImage, TInputImage>
{
public:
  /** Standard Self typedef */
  typedef PersistentMinMaxVectorImageFilter               Self;
  typedef PersistentImageFilter<TInputImage, TInputImage> Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(PersistentMinMaxVectorImageFilter, PersistentImageFilter);

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
  typedef typename itk::DataObject::Pointer DataObjectPointer;
  typedef itk::ProcessObject::DataObjectPointerArraySizeType DataObjectPointerArraySizeType;

  /** Type of DataObjects used for scalar outputs */
  typedef typename itk::VariableSizeMatrix<RealType>    MatrixType;
  typedef typename std::vector<MatrixType>              ArrayMatrixType;
  typedef typename itk::Array<long>                     ArrayLongPixelType;
  typedef typename std::vector<RealPixelType>           ArrayRealPixelType;
  typedef typename std::vector<PixelType>               ArrayPixelType;
  typedef itk::SimpleDataObjectDecorator<RealPixelType> RealPixelObjectType;
  typedef itk::SimpleDataObjectDecorator<PixelType>     PixelObjectType;
  typedef itk::SimpleDataObjectDecorator<MatrixType>    MatrixObjectType;

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


  /** Return the computed Minimum. */
  PixelType GetMinimum() const
  {
    return this->GetMinimumOutput()->Get();
  }
  PixelObjectType* GetMinimumOutput();
  const PixelObjectType* GetMinimumOutput() const;

  /** Return the computed Maximum. */
  PixelType GetMaximum() const
  {
    return this->GetMaximumOutput()->Get();
  }
  PixelObjectType* GetMaximumOutput();
  const PixelObjectType* GetMaximumOutput() const;

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
  PersistentMinMaxVectorImageFilter();
  ~PersistentMinMaxVectorImageFilter() override {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;
  /** Multi-thread version GenerateData. */
  void  ThreadedGenerateData(const RegionType& outputRegionForThread, itk::ThreadIdType threadId) override;

private:
  PersistentMinMaxVectorImageFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

  ArrayPixelType     m_ThreadMin;
  ArrayPixelType     m_ThreadMax;
  bool               m_NoDataFlag;
  InternalPixelType  m_NoDataValue;

}; // end of class PersistentStatisticsVectorImageFilter

/**===========================================================================*/

/** \class StreamingMinMaxVectorImageFilter
 * \brief This class streams the whole input image through the PersistentMinMaxVectorImageFilter.
 *
 * This way, it allows computing the min/max of this image. It calls the
 * Reset() method of the PersistentMinMaxVectorImageFilter before streaming the image and the
 * Synthetize() method of the PersistentMinMaxVectorImageFilter after having streamed the image
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

template<class TInputImage>
class ITK_EXPORT StreamingMinMaxVectorImageFilter :
  public PersistentFilterStreamingDecorator<PersistentMinMaxVectorImageFilter<TInputImage> >
{
public:
  /** Standard Self typedef */
  typedef StreamingMinMaxVectorImageFilter   Self;
  typedef PersistentFilterStreamingDecorator
  <PersistentMinMaxVectorImageFilter<TInputImage> > Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(StreamingMinMaxVectorImageFilter, PersistentFilterStreamingDecorator);

  typedef TInputImage                                 InputImageType;
  typedef typename Superclass::FilterType             StatFilterType;
  typedef typename StatFilterType::PixelType          PixelType;
  typedef typename StatFilterType::RealType           RealType;
  typedef typename StatFilterType::RealPixelType      RealPixelType;
  typedef typename StatFilterType::MatrixType         MatrixType;
  typedef typename StatFilterType::ArrayMatrixType    ArrayMatrixType;
  typedef typename StatFilterType::ArrayLongPixelType ArrayLongPixelType;
  typedef typename StatFilterType::ArrayRealPixelType ArrayRealPixelType;
  typedef typename StatFilterType::ArrayPixelType     ArrayPixelType;

  /** Type of DataObjects used for scalar outputs */
  typedef typename StatFilterType::RealPixelObjectType RealPixelObjectType;
  typedef typename StatFilterType::PixelObjectType     PixelObjectType;
  typedef typename StatFilterType::MatrixObjectType    MatrixObjectType;

  using Superclass::SetInput;
  void SetInput(InputImageType * input)
  {
    this->GetFilter()->SetInput(input);
  }
  const InputImageType * GetInput()
  {
    return this->GetFilter()->GetInput();
  }

  /** Return the computed Minimum. */
  PixelType GetMinimum() const
  {
    return this->GetFilter()->GetMinimumOutput()->Get();
  }
  PixelObjectType* GetMinimumOutput()
  {
    return this->GetFilter()->GetMinimumOutput();
  }
  const PixelObjectType* GetMinimumOutput() const
  {
    return this->GetFilter()->GetMinimumOutput();
  }

  /** Return the computed Maximum. */
  PixelType GetMaximum() const
  {
    return this->GetFilter()->GetMaximumOutput()->Get();
  }
  PixelObjectType* GetMaximumOutput()
  {
    return this->GetFilter()->GetMaximumOutput();
  }
  const PixelObjectType* GetMaximumOutput() const
  {
    return this->GetFilter()->GetMaximumOutput();
  }

protected:
  /** Constructor */
  StreamingMinMaxVectorImageFilter() {};
  /** Destructor */
  ~StreamingMinMaxVectorImageFilter() override {}

private:
  StreamingMinMaxVectorImageFilter(const Self &) = delete;
  void operator =(const Self&) = delete;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStreamingMinMaxVectorImageFilter.hxx"
#endif

#endif
