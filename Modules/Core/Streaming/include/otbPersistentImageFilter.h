/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbPersistentImageFilter_h
#define otbPersistentImageFilter_h

#include "itkImageToImageFilter.h"

namespace otb
{
/** \class PersistentImageFilter
 *  \brief This filter is the base class for all filter persisting data through multiple
 *   update.
 *   For instance, a filter computing global statistics on an image with streaming
 *   capabilities will have to keep the temporary results for each streamed piece of the
 *   image in order to synthesize the global statistics at the end. This filter is an
 *   itk::ImageToImageFilter, providing two additional methods. The first one, Synthetize(),
 *   allows the user to synthesize temporary data produced by the multiple updates on different
 *   pieces of the image to the global result. The second one, Reset(), allows the user to
 *   reset the temporary data for a new input image for instance.
 *
 *  \note This class contains pure virtual method, and can not be instantiated.
 *
 * \sa StatisticsImageFilter
 * \sa StatisticsVectorImageFilter
 *
 * \ingroup OTBStreaming
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT PersistentImageFilter : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard typedefs */
  typedef PersistentImageFilter Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Creation through object factory macro */
  itkTypeMacro(PersistentImageFilter, ImageToImageFilter);

  /** Template parameters typedefs */
  typedef TInputImage  InputImageType;
  typedef TOutputImage OutputImageType;
  /**
   * Reset the persistent data of the filter.
   */
  virtual void Reset(void) = 0;
  /**
   * Synthesize the persistent data of the filter.
   */
  virtual void Synthetize(void) = 0;

protected:
  /** Constructor */
  PersistentImageFilter()
  {
  }
  /** Destructor */
  ~PersistentImageFilter() override
  {
  }
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override
  {
    Superclass::PrintSelf(os, indent);
  }

private:
  PersistentImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;
};
} // End namespace otb

#endif
