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
#ifndef otbPersistentFilterStreamingDecorator_h
#define otbPersistentFilterStreamingDecorator_h

#include "otbStreamingImageVirtualWriter.h"
#include "itkProcessObject.h"

namespace otb
{
/** \class PersistentFilterStreamingDecorator
 *  \brief This filter link a persistent filter with a StreamingImageVirtualWriter.
 *
 *  The StreamingVirtualWriter will break the input image into pieces and stream each
 *  piece through the persistent filter. That way, the persistent filter will computes
 *  its data on the whole image, but never loads the whole of it, and eventually processes
 *  each piece with multiple threads. Before the streaming of the whole image is triggered,
 *  the Reset() method of the persistent filter is called to clear the temporary data it might
 *  contain. After the streaming, the Synthetize() method is called to synthetize the
 *  temporary data. One can access the persistent filter via the GetFilter() method, and
 * StreamingVirtualWriter via the GetStreamer() method.
 *
 * \sa StreamingStatisticsImageFilter
 * \sa StreamingStatisticsVectorImageFilter
 *
 * \ingroup OTBStreaming
 */
template <class TFilter>
class ITK_EXPORT PersistentFilterStreamingDecorator
  : public itk::ProcessObject
{
public:
  /** Standard typedefs */
  typedef PersistentFilterStreamingDecorator Self;
  typedef itk::ProcessObject                 Superclass;
  typedef itk::SmartPointer<Self>            Pointer;
  typedef itk::SmartPointer<const Self>      ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(PersistentFilterStreamingDecorator, ProcessObject);

  /** Template parameters typedefs */
  typedef TFilter                             FilterType;
  typedef typename FilterType::Pointer        FilterPointerType;
  typedef typename FilterType::InputImageType ImageType;

  typedef StreamingImageVirtualWriter<ImageType> StreamerType;
  typedef typename StreamerType::Pointer         StreamerPointerType;

  itkSetObjectMacro(Filter, FilterType);
  itkGetObjectMacro(Filter, FilterType);
  itkGetConstObjectMacro(Filter, FilterType);
  itkGetObjectMacro(Streamer, StreamerType);

  void Update(void) ITK_OVERRIDE;

protected:
  /** Constructor */
  PersistentFilterStreamingDecorator();
  /** Destructor */
  ~PersistentFilterStreamingDecorator() ITK_OVERRIDE {}
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;

  void GenerateData(void) ITK_OVERRIDE;

  /// Object responsible for streaming
  StreamerPointerType m_Streamer;

  /// Object responsible for computation
  FilterPointerType m_Filter;

private:
  PersistentFilterStreamingDecorator(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPersistentFilterStreamingDecorator.txx"
#endif

#endif
