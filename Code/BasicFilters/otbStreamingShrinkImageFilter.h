/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbStreamingShrinkImageFilter_h
#define __otbStreamingShrinkImageFilter_h


#include "otbPersistentImageFilter.h"
#include "otbPersistentFilterStreamingDecorator.h"

#include "otbStreamingManager.h"

namespace otb
{

class ITK_EXPORT StreamingShrinkImageRegionSplitter : public itk::ImageRegionSplitter<2>
{
public:
  /** Standard class typedefs. */
  typedef StreamingShrinkImageRegionSplitter        Self;
  typedef itk::ImageRegionSplitter<2>               Superclass;
  typedef itk::SmartPointer<Self>                   Pointer;
  typedef itk::SmartPointer<const Self>             ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(StreamingShrinkImageRegionSplitter, itk::Object);

  /** Dimension of the image available at compile time. */
  itkStaticConstMacro(ImageDimension, unsigned int, 2);

  /** Dimension of the image available at run time. */
  static unsigned int GetImageDimension()
  {
    return ImageDimension;
  }

  /** Index typedef support. An index is used to access pixel values. */
  typedef itk::Index<ImageDimension>         IndexType;
  typedef IndexType::IndexValueType IndexValueType;

  /** Size typedef support. A size is used to define region bounds. */
  typedef itk::Size<ImageDimension>        SizeType;
  typedef SizeType::SizeValueType SizeValueType;

  /** Region typedef support.   */
  typedef itk::ImageRegion<ImageDimension> RegionType;

  /** How many pieces can the specified region be split? A given region
   *  cannot always be divided into the requested number of pieces.  For
   *  instance, if the numberOfPieces exceeds the number of pixels along
   *  a certain dimensions, then some splits will not be possible.
   */
  virtual unsigned int GetNumberOfSplits(const RegionType& region,
                                         unsigned int requestedNumber);

  /** Get a region definition that represents the ith piece a specified region.
   * The "numberOfPieces" specified should be less than or equal to what
   * GetNumberOfSplits() returns. */
  virtual RegionType GetSplit(unsigned int i, unsigned int numberOfPieces,
                              const RegionType& region);

  itkGetMacro(TileSizeAlignment, unsigned int);
  itkSetMacro(TileSizeAlignment, unsigned int);

  itkGetMacro(TileDimension, unsigned int);

  itkSetMacro(ShrinkFactor, unsigned int);
  itkGetMacro(ShrinkFactor, unsigned int);

protected:
  StreamingShrinkImageRegionSplitter() : m_SplitsPerDimension(0U), m_ShrinkFactor(10) {}
  virtual ~StreamingShrinkImageRegionSplitter() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  StreamingShrinkImageRegionSplitter(const StreamingShrinkImageRegionSplitter &); //purposely not implemented
  void operator =(const StreamingShrinkImageRegionSplitter&); //purposely not implemented

  itk::FixedArray<unsigned int, ImageDimension> m_SplitsPerDimension;
  unsigned int m_TileDimension;
  unsigned int m_TileSizeAlignment;
  unsigned int m_ShrinkFactor;
};


template <class TInputImage>
class ITK_EXPORT StreamingShrinkStreamingManager : public StreamingManager<TInputImage>
{
public:
  /** Standard class typedefs. */
  typedef StreamingShrinkStreamingManager Self;
  typedef StreamingManager<TInputImage>   Superclass;
  typedef itk::SmartPointer<Self>         Pointer;
  typedef itk::SmartPointer<const Self>   ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(StreamingShrinkStreamingManager, StreamingManager);

  typedef TInputImage                            ImageType;
  typedef typename ImageType::Pointer            ImagePointerType;
  typedef typename ImageType::RegionType         RegionType;
  typedef typename RegionType::IndexType         IndexType;
  typedef typename RegionType::SizeType          SizeType;
  typedef typename ImageType::InternalPixelType  PixelType;

  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);

  /** Actually computes the stream divisions, according to the specified streaming mode,
   * eventually using the input parameter to estimate memory consumption */
  virtual void PrepareStreaming(itk::DataObject * input, const RegionType &region);

  void SetShrinkFactor(unsigned int val)
  {
    m_ShrinkFactor = val;
  }

  unsigned int GetShrinkFactor() const
  {
    return m_ShrinkFactor;
  }

protected:
  StreamingShrinkStreamingManager();
  virtual ~StreamingShrinkStreamingManager();

private:
  StreamingShrinkStreamingManager(const StreamingShrinkStreamingManager &); //purposely not implemented
  void operator =(const StreamingShrinkStreamingManager&); //purposely not implemented

  unsigned int m_ShrinkFactor;
};


/** \class PersistentShrinkImageFilter
 * \brief
 *
 * \sa PersistentImageFilter
 * \ingroup Streamed
 * \ingroup Multithreaded
 *
 */
template<class TInputImage, class TOutputImage = TInputImage>
class ITK_EXPORT PersistentShrinkImageFilter :
  public PersistentImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard Self typedef */
  typedef PersistentShrinkImageFilter                      Self;
  typedef PersistentImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                          Pointer;
  typedef itk::SmartPointer<const Self>                    ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(PersistentShrinkImageFilter, PersistentImageFilter);

  /** Image related typedefs. */
  typedef TInputImage                             InputImageType;
  typedef typename TInputImage::Pointer           InputImagePointer;
  typedef typename TInputImage::RegionType        RegionType;
  typedef typename TInputImage::SizeType          SizeType;
  typedef typename TInputImage::IndexType         IndexType;
  typedef typename TInputImage::PixelType         PixelType;

  /** Image related typedefs. */
  typedef TOutputImage                             OutputImageType;
  typedef typename TOutputImage::Pointer           OutputImagePointer;

  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);

  /** Smart Pointer type to a DataObject. */
  typedef typename itk::DataObject::Pointer DataObjectPointer;

  OutputImageType * GetShrinkedOutput()
  {
    return m_ShrinkedOutput;
  }

  virtual void Synthetize(void);

  virtual void Reset(void);

  itkSetMacro(ShrinkFactor, unsigned int);
  itkGetMacro(ShrinkFactor, unsigned int);

protected:
  PersistentShrinkImageFilter();

  virtual ~PersistentShrinkImageFilter();

  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

  void  BeforeThreadedGenerateData();

  /** Multi-thread version GenerateData. */
  void  ThreadedGenerateData(const RegionType& outputRegionForThread, itk::ThreadIdType threadId);

  void  AfterThreadedGenerateData();

  /** Pass the input through unmodified. Do this by Grafting in the
   *  AllocateOutputs method.
   */
  virtual void AllocateOutputs();

  virtual void GenerateOutputInformation();


private:
  PersistentShrinkImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /* the output shrinked image */
  OutputImagePointer m_ShrinkedOutput;

  /** The shrink factor */
  unsigned int m_ShrinkFactor;
}; // end of class PersistentStatisticsVectorImageFilter


/** \class StreamingShrinkImageFilter
 * \brief Generates a quicklook of the input image
 *
 * This filter computes a subsampled version of the input image with streaming capabilities
 *
 * The subsampling ration is set with SetShrinkFactor
 *
 * \sa PersistentImageFilter
 * \ingroup Streamed
 * \ingroup Multithreaded
 *
 */
template<class TInputImage, class TOutputImage = TInputImage>
class ITK_EXPORT StreamingShrinkImageFilter :
  public PersistentFilterStreamingDecorator< PersistentShrinkImageFilter<TInputImage, TOutputImage> >
{
public:
  /** Standard Self typedef */
  typedef StreamingShrinkImageFilter                Self;
  typedef PersistentFilterStreamingDecorator
    <PersistentShrinkImageFilter<TInputImage, TOutputImage> >  Superclass;
  typedef itk::SmartPointer<Self>                   Pointer;
  typedef itk::SmartPointer<const Self>             ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(StreamingShrinkImageFilter, PersistentFilterStreamingDecorator);

  typedef TInputImage                                 InputImageType;
  typedef TOutputImage                                OutputImageType;
  typedef typename Superclass::FilterType             PersistentFilterType;

  typedef StreamingShrinkStreamingManager<InputImageType>       StreamingShrinkStreamingManagerType;
  typedef typename StreamingShrinkStreamingManagerType::Pointer StreamingShrinkStreamingManagerPointerType;

  void SetInput(InputImageType * input)
  {
    this->GetFilter()->SetInput(input);
  }

  const InputImageType * GetInput()
  {
    return this->GetFilter()->GetInput();
  }

  OutputImageType * GetOutput()
  {
    return this->GetFilter()->GetShrinkedOutput();
  }

  otbSetObjectMemberMacro(Filter, ShrinkFactor, unsigned int);
  otbGetObjectMemberMacro(Filter, ShrinkFactor, unsigned int);

  virtual void Update(void)
  {
    m_StreamingManager->SetShrinkFactor( this->GetFilter()->GetShrinkFactor() );
    Superclass::Update();
  }

protected:
  /** Constructor */
  StreamingShrinkImageFilter()
  {
    // Use a specific StreamingManager implementation
    m_StreamingManager = StreamingShrinkStreamingManagerType::New();
    this->GetStreamer()->SetStreamingManager( m_StreamingManager );
  }

  /** Destructor */
  virtual ~StreamingShrinkImageFilter() {}

private:
  StreamingShrinkImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  StreamingShrinkStreamingManagerPointerType m_StreamingManager;
};

} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStreamingShrinkImageFilter.txx"
#endif

#endif
