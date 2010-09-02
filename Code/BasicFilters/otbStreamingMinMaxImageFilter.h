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
#ifndef __otbStreamingMinMaxImageFilter_h
#define __otbStreamingMinMaxImageFilter_h

#include "otbPersistentImageFilter.h"
#include "itkNumericTraits.h"
#include "itkArray.h"
#include "itkSimpleDataObjectDecorator.h"
#include "otbPersistentFilterStreamingDecorator.h"

namespace otb
{

/** \class PersistentMinMaxImageFilter
 * \brief Compute min. max of an image using the output requested region.
 *
 *  This filter persists its temporary data. It means that if you Update it n times on n different
 * requested regions, the output statistics will be the statitics of the whole set of n regions.
 *
 * To reset the temporary data, one should call the Reset() function.
 *
 * To get the min/max once the regions have been processed via the pipeline, use the Synthetize() method.
 *
 * \sa PersistentImageFilter
 * \ingroup Streamed
 * \ingroup Multithreaded
 * \ingroup MathematicalStatisticsImageFilters
 */
template<class TInputImage>
class ITK_EXPORT PersistentMinMaxImageFilter :
  public PersistentImageFilter<TInputImage, TInputImage>
{
public:
  /** Standard Self typedef */
  typedef PersistentMinMaxImageFilter                 Self;
  typedef PersistentImageFilter<TInputImage, TInputImage> Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(PersistentMinMaxImageFilter, PersistentImageFilter);

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

  /** Make a DataObject of the correct type to be used as the specified
   * output. */
  virtual DataObjectPointer MakeOutput(unsigned int idx);

  /** Pass the input through unmodified. Do this by Grafting in the
   *  AllocateOutputs method.
   */
  void AllocateOutputs();
  virtual void GenerateOutputInformation();
  void Synthetize(void);
  void Reset(void);

protected:
  PersistentMinMaxImageFilter();
  virtual ~PersistentMinMaxImageFilter() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Multi-thread version GenerateData. */
  void  ThreadedGenerateData(const RegionType&
                             outputRegionForThread,
                             int threadId);

private:
  PersistentMinMaxImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  itk::Array<PixelType> m_ThreadMin;
  itk::Array<PixelType> m_ThreadMax;
}; // end of class PersistentMinMaxImageFilter

/*===========================================================================*/

/** \class StreamingMinMaxImageFilter
 * \brief This class streams the whole input image through the PersistentMinMaxImageFilter.
 *
 * This way, it allows to compute the first order global statistics of this image. It calls the
 * Reset() method of the PersistentMinMaxImageFilter before streaming the image and the
 * Synthetize() method of the PersistentMinMaxImageFilter after having streamed the image
 * to compute the statistics. The accessor on the results are wrapping the accessors of the
 * internal PersistentMinMaxImageFilter.
 *
 * This filter can be used as:
 * \code
 * typedef otb::StreamingMinMaxImageFilter<ImageType> MinMaxType;
 * MinMaxType::Pointer minmax = MinMaxType::New();
 * minmax->SetInput(reader->GetOutput());
 * minmax->Update();
 * std::cout << minmax-> GetMaximum() << std::endl;
 * std::cout << minmax-> GetMinimum() << std::endl;
 * \endcode
 *
 * \sa PersistentMinMaxImageFilter
 * \sa PersistentImageFilter
 * \sa PersistentFilterStreamingDecorator
 * \sa StreamingImageVirtualWriter
 * \ingroup Streamed
 * \ingroup Multithreaded
 * \ingroup MathematicalStatisticsImageFilters
 */

template<class TInputImage>
class ITK_EXPORT StreamingMinMaxImageFilter :
  public PersistentFilterStreamingDecorator<PersistentMinMaxImageFilter<TInputImage> >
{
public:
  /** Standard Self typedef */
  typedef StreamingMinMaxImageFilter Self;
  typedef PersistentFilterStreamingDecorator
  <PersistentMinMaxImageFilter<TInputImage> > Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(StreamingMinMaxImageFilter, PersistentFilterStreamingDecorator);

  typedef typename Superclass::FilterType    StatFilterType;
  typedef typename StatFilterType::PixelType PixelType;
  typedef typename StatFilterType::RealType  RealType;
  typedef TInputImage                        InputImageType;

  /** Type of DataObjects used for scalar outputs */
  typedef itk::SimpleDataObjectDecorator<RealType>  RealObjectType;
  typedef itk::SimpleDataObjectDecorator<PixelType> PixelObjectType;

  void SetInput(InputImageType * input)
  {
    this->GetFilter()->SetInput(input);
  }

  /** Return the computed Minimum. */
  PixelType GetMinimum() const
  {
    return this->GetFilter()->GetMinimumOutput()->Get();
  }
  PixelObjectType* GetMinimumOutput()
  {
    return this->GetFilter()->MinimumOutput();
  }
  const PixelObjectType* GetMinimumOutput() const
  {
    return this->GetFilter()->MinimumOutput();
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
  StreamingMinMaxImageFilter() {};
  /** Destructor */
  virtual ~StreamingMinMaxImageFilter() {}

private:
  StreamingMinMaxImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStreamingMinMaxImageFilter.txx"
#endif

#endif
