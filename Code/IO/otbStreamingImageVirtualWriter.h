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
#ifndef __otbStreamingImageVirtualWriter_h
#define __otbStreamingImageVirtualWriter_h

#include "itkMacro.h"
#include "itkImageToImageFilter.h"
#include "otbStreamingManager.h"

namespace otb
{

/** \class StreamingImageVirtualWriter
 *  \brief This class acts like a StreamingImageFileWriter, but without actually
 *  writing data to the disk.
 *
 *  This allows to stream the whole image through persistent filters such as
 *  PersitentStatisticsImageFilter in order to get the global statistics of an image.
 *
 *  This filter is not intended to be used with classic ImageToImageFilter, though it
 *  will not generate any error.
 *
 *  This filter proposes the same streaming setup methods than the StreamingImageFileWriter.
 *
 *  It is used in the PersistentFilterStreamingDecorator helper class to propose an easy
 *  way to stream an image through a persistent filter.
 *
 * \sa PersistentImageFilter
 * \sa PersistentStatisticsImageFilter
 * \sa PersistentImageStreamingDecorator.
 */
template <class TInputImage, class TStreamingManager = StreamingManager<TInputImage> >
class ITK_EXPORT StreamingImageVirtualWriter : public itk::ImageToImageFilter<TInputImage, TInputImage>
{
public:
  /** Standard class typedefs. */
  typedef StreamingImageVirtualWriter                       Self;
  typedef itk::ImageToImageFilter<TInputImage, TInputImage> Superclass;
  typedef itk::SmartPointer<Self>                           Pointer;
  typedef itk::SmartPointer<const Self>                     ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(StreamingImageVirtualWriter, itk::ImageToImageFilter);

  /** Some typedefs for the input and output. */
  typedef TInputImage                         InputImageType;
  typedef typename InputImageType::Pointer    InputImagePointer;
  typedef typename InputImageType::RegionType InputImageRegionType;
  typedef typename InputImageType::PixelType  InputImagePixelType;

  /** Streaming traits helper typedef */
  typedef TStreamingManager                      StreamingManagerType;
  typedef typename StreamingManagerType::Pointer StreamingManagerPointerType;

  /** Dimension of input image. */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      InputImageType::ImageDimension);

  StreamingManagerType* GetStreamingManager(void)
    {
    return m_StreamingManager;
    }

  void SetStreamingManager(StreamingManagerType* streamingManager)
    {
    m_StreamingManager = streamingManager;
    }

protected:
  StreamingImageVirtualWriter();

  virtual ~StreamingImageVirtualWriter();

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  virtual void GenerateData(void);

  virtual void GenerateInputRequestedRegion(void);

private:
  StreamingImageVirtualWriter(const StreamingImageVirtualWriter &); //purposely not implemented
  void operator =(const StreamingImageVirtualWriter&); //purposely not implemented

  void ObserveSourceFilterProgress(itk::Object* object, const itk::EventObject & event )
  {
    if (typeid(event) != typeid(itk::ProgressEvent))
      {
      return;
      }

    itk::ProcessObject* processObject = dynamic_cast<itk::ProcessObject*>(object);
    if (processObject)
      m_DivisionProgress = processObject->GetProgress();

    this->UpdateFilterProgress();
  }

  void UpdateFilterProgress()
  {
    this->UpdateProgress( (m_DivisionProgress + m_CurrentDivision) / m_NumberOfDivisions );
  }

  unsigned int m_NumberOfDivisions;
  unsigned int m_CurrentDivision;
  float m_DivisionProgress;

  StreamingManagerPointerType m_StreamingManager;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStreamingImageVirtualWriter.txx"
#endif

#endif
