/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

  void Update(void) override;

protected:
  /** Constructor */
  PersistentFilterStreamingDecorator();
  /** Destructor */
  ~PersistentFilterStreamingDecorator() override {}
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  void GenerateData(void) override;

  /// Object responsible for streaming
  StreamerPointerType m_Streamer;

  /// Object responsible for computation
  FilterPointerType m_Filter;

private:
  PersistentFilterStreamingDecorator(const Self &) = delete;
  void operator =(const Self&) = delete;

};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPersistentFilterStreamingDecorator.hxx"
#endif

#endif
