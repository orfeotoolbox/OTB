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

#include "otbMacro.h"
#include "itkImageToImageFilter.h"
#include "itkDataObject.h"

#include "otbPersistentImageFilter.h"
#include "otbPersistentFilterStreamingDecorator.h"

#include "itkTimeProbe.h"

namespace otb
{


/** \class PersistentShrinkImageFilter
 * \brief
 *
 * \sa PersistentImageFilter
 * \ingroup Streamed
 * \ingroup Multithreaded
 *
 */
template<class TInputImage, class TOutputImage>
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
  void  ThreadedGenerateData(const RegionType& outputRegionForThread, int threadId);

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

  itk::TimeProbe m_Chrono;

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
template<class TInputImage, class TOutputImage>
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

protected:
  /** Constructor */
  StreamingShrinkImageFilter() {}
  /** Destructor */
  virtual ~StreamingShrinkImageFilter() {}

private:
  StreamingShrinkImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};

} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStreamingShrinkImageFilter.txx"
#endif

#endif
