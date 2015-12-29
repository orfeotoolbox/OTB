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
#ifndef __otbStreamingStatisticsImageFilter_h
#define __otbStreamingStatisticsImageFilter_h

#include "otbPersistentImageFilter.h"
#include "itkNumericTraits.h"
#include "itkArray.h"
#include "itkSimpleDataObjectDecorator.h"
#include "otbPersistentFilterStreamingDecorator.h"

namespace otb
{

/** \class PersistentStatisticsImageFilter
 * \brief Compute min. max, variance and mean of an image using the output requested region.
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
 * \ingroup OTBStatistics
 */
template<class TInputImage>
class ITK_EXPORT PersistentStatisticsImageFilter :
  public PersistentImageFilter<TInputImage, TInputImage>
{
public:
  /** Standard Self typedef */
  typedef PersistentStatisticsImageFilter                 Self;
  typedef PersistentImageFilter<TInputImage, TInputImage> Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(PersistentStatisticsImageFilter, PersistentImageFilter);

  /** Image related typedefs. */
  typedef TInputImage                   ImageType;
  typedef typename TInputImage::Pointer InputImagePointer;

  typedef typename TInputImage::RegionType RegionType;
  typedef typename TInputImage::SizeType   SizeType;
  typedef typename TInputImage::IndexType  IndexType;
  typedef typename TInputImage::PixelType  PixelType;

  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);

  /** Image related typedefs. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TInputImage::ImageDimension);

  /** Type to use for computations. */
  typedef typename itk::NumericTraits<PixelType>::RealType RealType;

  /** Smart Pointer type to a DataObject. */
  typedef typename itk::DataObject::Pointer DataObjectPointer;
  typedef itk::ProcessObject::DataObjectPointerArraySizeType DataObjectPointerArraySizeType;

  /** Type of DataObjects used for scalar outputs */
  typedef itk::SimpleDataObjectDecorator<RealType>  RealObjectType;
  typedef itk::SimpleDataObjectDecorator<PixelType> PixelObjectType;

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

  /** Return the computed Mean. */
  RealType GetMean() const
  {
    return this->GetMeanOutput()->Get();
  }
  RealObjectType* GetMeanOutput();
  const RealObjectType* GetMeanOutput() const;

  /** Return the computed Standard Deviation. */
  RealType GetSigma() const
  {
    return this->GetSigmaOutput()->Get();
  }
  RealObjectType* GetSigmaOutput();
  const RealObjectType* GetSigmaOutput() const;

  /** Return the computed Variance. */
  RealType GetVariance() const
  {
    return this->GetVarianceOutput()->Get();
  }
  RealObjectType* GetVarianceOutput();
  const RealObjectType* GetVarianceOutput() const;

  /** Return the compute Sum. */
  RealType GetSum() const
  {
    return this->GetSumOutput()->Get();
  }
  RealObjectType* GetSumOutput();
  const RealObjectType* GetSumOutput() const;

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

  itkSetMacro(IgnoreInfiniteValues, bool);
  itkGetMacro(IgnoreInfiniteValues, bool);

  itkSetMacro(IgnoreUserDefinedValue, bool);
  itkGetMacro(IgnoreUserDefinedValue, bool);

  itkSetMacro(UserIgnoredValue, RealType);
  itkGetMacro(UserIgnoredValue, RealType);

protected:
  PersistentStatisticsImageFilter();
  virtual ~PersistentStatisticsImageFilter() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Multi-thread version GenerateData. */
  void  ThreadedGenerateData(const RegionType&
                             outputRegionForThread,
                             itk::ThreadIdType threadId);

private:
  PersistentStatisticsImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  itk::Array<RealType>       m_ThreadSum;
  itk::Array<RealType>       m_SumOfSquares;
  itk::Array<long>           m_Count;
  itk::Array<PixelType>      m_ThreadMin;
  itk::Array<PixelType>      m_ThreadMax;

  /* Ignored values */
  bool                       m_IgnoreInfiniteValues;
  bool                       m_IgnoreUserDefinedValue;
  RealType                   m_UserIgnoredValue;
  std::vector<unsigned int>  m_IgnoredInfinitePixelCount;
  std::vector<unsigned int>  m_IgnoredUserPixelCount;


}; // end of class PersistentStatisticsImageFilter

/*===========================================================================*/

/** \class StreamingStatisticsImageFilter
 * \brief This class streams the whole input image through the PersistentStatisticsImageFilter.
 *
 * This way, it allows computing the first order global statistics of this image. It calls the
 * Reset() method of the PersistentStatisticsImageFilter before streaming the image and the
 * Synthetize() method of the PersistentStatisticsImageFilter after having streamed the image
 * to compute the statistics. The accessor on the results are wrapping the accessors of the
 * internal PersistentStatisticsImageFilter.
 * By default infinite values are ignored, use IgnoreInfiniteValues accessor to consider
 * infinite values in the computation.
 *
 * This filter can be used as:
 * \code
 * typedef otb::StreamingStatisticsImageFilter<ImageType> StatisticsType;
 * StatisticsType::Pointer statistics = StatisticsType::New();
 * statistics->SetInput(reader->GetOutput());
 * statistics->Update();
 * std::cout << statistics-> GetMaximum() << std::endl;
 * std::cout << statistics-> GetMinimum() << std::endl;
 * \endcode
 *
 * \sa PersistentStatisticsImageFilter
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
class ITK_EXPORT StreamingStatisticsImageFilter :
  public PersistentFilterStreamingDecorator<PersistentStatisticsImageFilter<TInputImage> >
{
public:
  /** Standard Self typedef */
  typedef StreamingStatisticsImageFilter Self;
  typedef PersistentFilterStreamingDecorator
  <PersistentStatisticsImageFilter<TInputImage> > Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(StreamingStatisticsImageFilter, PersistentFilterStreamingDecorator);

  typedef typename Superclass::FilterType    StatFilterType;
  typedef typename StatFilterType::PixelType PixelType;
  typedef typename StatFilterType::RealType  RealType;
  typedef TInputImage                        InputImageType;

  /** Type of DataObjects used for scalar outputs */
  typedef itk::SimpleDataObjectDecorator<RealType>  RealObjectType;
  typedef itk::SimpleDataObjectDecorator<PixelType> PixelObjectType;

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
  /** Return the computed Mean. */
  RealType GetMean() const
  {
    return this->GetFilter()->GetMeanOutput()->Get();
  }
  RealObjectType* GetMeanOutput()
  {
    return this->GetFilter()->GetMeanOutput();
  }
  const RealObjectType* GetMeanOutput() const
  {
    return this->GetFilter()->GetMeanOutput();
  }

  /** Return the computed Standard Deviation. */
  RealType GetSigma() const
  {
    return this->GetSigmaOutput()->Get();
  }
  RealObjectType* GetSigmaOutput()
  {
    return this->GetFilter()->GetSigmaOutput();
  }
  const RealObjectType* GetSigmaOutput() const
  {
    return this->GetFilter()->GetSigmaOutput();
  }

  /** Return the computed Variance. */
  RealType GetVariance() const
  {
    return this->GetFilter()->GetVarianceOutput()->Get();
  }
  RealObjectType* GetVarianceOutput()
  {
    return this->GetFilter()->GetVarianceOutput();
  }
  const RealObjectType* GetVarianceOutput() const
  {
    return this->GetFilter()->GetVarianceOutput();
  }

  /** Return the compute Sum. */
  RealType GetSum() const
  {
    return this->GetFilter()->GetSumOutput()->Get();
  }
  RealObjectType* GetSumOutput()
  {
    return this->GetFilter()->GetSumOutput();
  }
  const RealObjectType* GetSumOutput() const
  {
    return this->GetFilter()->GetSumOutput();
  }

  otbSetObjectMemberMacro(Filter, IgnoreInfiniteValues, bool);
  otbGetObjectMemberMacro(Filter, IgnoreInfiniteValues, bool);

  otbSetObjectMemberMacro(Filter, IgnoreUserDefinedValue, bool);
  otbGetObjectMemberMacro(Filter, IgnoreUserDefinedValue, bool);

  otbSetObjectMemberMacro(Filter, UserIgnoredValue, RealType);
  otbGetObjectMemberMacro(Filter, UserIgnoredValue, RealType);

protected:
  /** Constructor */
  StreamingStatisticsImageFilter() {};
  /** Destructor */
  virtual ~StreamingStatisticsImageFilter() {}

private:
  StreamingStatisticsImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStreamingStatisticsImageFilter.txx"
#endif

#endif
