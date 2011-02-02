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
#ifndef __otbStreamingMinMaxImageFilter2_h
#define __otbStreamingMinMaxImageFilter2_h

#include <vector>
#include "otbPersistentImageFilter.h"
#include "itkNumericTraits.h"
#include "itkSimpleDataObjectDecorator.h"
#include "otbPersistentFilterStreamingDecorator.h"

namespace otb
{

/** \class PersistentMinMaxImageFilter2
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
class ITK_EXPORT PersistentMinMaxImageFilter2 :
  public PersistentImageFilter<TInputImage, TInputImage>
{
public:
  /** Standard Self typedef */
  typedef PersistentMinMaxImageFilter2                 Self;
  typedef PersistentImageFilter<TInputImage, TInputImage> Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(PersistentMinMaxImageFilter2, PersistentImageFilter);

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

  /** Smart Pointer type to a DataObject. */
  typedef typename itk::DataObject::Pointer DataObjectPointer;

  /** Type of DataObjects used for scalar outputs */
  typedef itk::SimpleDataObjectDecorator<PixelType> PixelObjectType;
  typedef itk::SimpleDataObjectDecorator<IndexType> IndexObjectType;

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

  /** Return the computed Minimum. */
  IndexType GetMinimumIndex() const
  {
    return this->GetMinimumIndexOutput()->Get();
  }
  IndexObjectType* GetMinimumIndexOutput();
  const IndexObjectType* GetMinimumIndexOutput() const;

  /** Return the computed Maximum. */
  IndexType GetMaximumIndex() const
  {
    return this->GetMaximumIndexOutput()->Get();
  }
  IndexObjectType* GetMaximumIndexOutput();
  const IndexObjectType* GetMaximumIndexOutput() const;


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
  PersistentMinMaxImageFilter2();
  virtual ~PersistentMinMaxImageFilter2() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Multi-thread version GenerateData. */
  void  ThreadedGenerateData(const RegionType&
                             outputRegionForThread,
                             int threadId);

private:
  PersistentMinMaxImageFilter2(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  std::vector<PixelType> m_ThreadMin;
  std::vector<PixelType> m_ThreadMax;
  std::vector<IndexType> m_ThreadMinIndex;
  std::vector<IndexType> m_ThreadMaxIndex;
}; // end of class PersistentMinMaxImageFilter2


/** \class StreamingMinMaxImageFilter2
 * \brief This class streams the whole input image through the PersistentMinMaxImageFilter2.
 *
 * This way, it allows to compute the first order global statistics of this image. It calls the
 * Reset() method of the PersistentMinMaxImageFilter2 before streaming the image and the
 * Synthetize() method of the PersistentMinMaxImageFilter2 after having streamed the image
 * to compute the statistics. The accessor on the results are wrapping the accessors of the
 * internal PersistentMinMaxImageFilter2.
 *
 * This filter can be used as:
 * \code
 * typedef otb::StreamingMinMaxImageFilter2<ImageType> MinMaxType;
 * MinMaxType::Pointer minmax = MinMaxType::New();
 * minmax->SetInput(reader->GetOutput());
 * minmax->Update();
 * std::cout << minmax-> GetMaximum() << std::endl;
 * std::cout << minmax-> GetMinimum() << std::endl;
 * \endcode
 *
 * \sa PersistentMinMaxImageFilter2
 * \sa PersistentImageFilter
 * \sa PersistentFilterStreamingDecorator
 * \sa StreamingImageVirtualWriter
 * \ingroup Streamed
 * \ingroup Multithreaded
 * \ingroup MathematicalStatisticsImageFilters
 */

template<class TInputImage>
class ITK_EXPORT StreamingMinMaxImageFilter2 :
  public PersistentFilterStreamingDecorator<PersistentMinMaxImageFilter2<TInputImage> >
{
public:
  /** Standard Self typedef */
  typedef StreamingMinMaxImageFilter2 Self;
  typedef PersistentFilterStreamingDecorator
  <PersistentMinMaxImageFilter2<TInputImage> > Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(StreamingMinMaxImageFilter2, PersistentFilterStreamingDecorator);

  typedef typename Superclass::FilterType          StatFilterType;
  typedef typename StatFilterType::PixelType       PixelType;
  typedef typename StatFilterType::IndexType       IndexType;
  typedef typename StatFilterType::PixelObjectType PixelObjectType;
  typedef typename StatFilterType::IndexObjectType IndexObjectType;
  typedef TInputImage                              InputImageType;

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

  /** Return the computed Minimum. */
  IndexType GetMinimumIndex() const
  {
    return this->GetFilter()->GetMinimumIndexOutput()->Get();
  }
  IndexObjectType* GetMinimumIndexOutput()
  {
    return this->GetFilter()->GetMinimumIndexOutput();
  }
  const IndexObjectType* GetMinimumIndexOutput() const
  {
    return this->GetFilter()->GetMinimumIndexOutput();
  }
  /** Return the computed Maximum. */
  IndexType GetMaximumIndex() const
  {
    return this->GetFilter()->GetMaximumIndexOutput()->Get();
  }
  IndexObjectType* GetMaximumIndexOutput()
  {
    return this->GetFilter()->GetMaximumIndexOutput();
  }
  const IndexObjectType* GetMaximumIndexOutput() const
  {
    return this->GetFilter()->GetMaximumIndexOutput();
  }
protected:
  /** Constructor */
  StreamingMinMaxImageFilter2() {}
  /** Destructor */
  virtual ~StreamingMinMaxImageFilter2() {}

private:
  StreamingMinMaxImageFilter2(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStreamingMinMaxImageFilter2.txx"
#endif

#endif
