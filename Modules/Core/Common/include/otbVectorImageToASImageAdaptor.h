/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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


#ifndef otbVectorImageToASImageAdaptor_h
#define otbVectorImageToASImageAdaptor_h

#include "itkImageAdaptor.h"
#include "otbVectorImageToASPixelAccessor.h"

namespace otb
{

/** \class VectorImageToASImageAdaptor
 * \brief Presents an image of pixel type Vector as being and image of
 * FixelArray<unsigned short, 3> type.
 *
 * \ingroup ImageAdaptors
 *
 *
 * \ingroup OTBCommon
 */
template <class TImage>
class ITK_EXPORT VectorImageToASImageAdaptor
    : public itk::ImageAdaptor<TImage, otb::Accessor::VectorImageToASPixelAccessor<typename TImage::PixelType::ValueType>>
{
public:
  /** Standard class typedefs. */
  typedef VectorImageToASImageAdaptor Self;
  typedef itk::ImageAdaptor<TImage, otb::Accessor::VectorImageToASPixelAccessor<typename TImage::PixelType::ValueType>> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(VectorImageToASImageAdaptor, ImageAdaptor);

  /** PixelContainer typedef support. Used to construct a container for
   * the pixel data. */
  typedef typename Superclass::PixelContainer             PixelContainer;
  typedef typename Superclass::PixelContainerPointer      PixelContainerPointer;
  typedef typename Superclass::PixelContainerConstPointer PixelContainerConstPointer;

protected:
  VectorImageToASImageAdaptor()
  {
  }
  ~VectorImageToASImageAdaptor() override
  {
  }

private:
  VectorImageToASImageAdaptor(const Self&) = delete;
  void operator=(const Self&) = delete;
};

} // end namespace otb

#endif
