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

#ifndef otbPolyLineImageIterator_h
#define otbPolyLineImageIterator_h

#include "otbPolyLineImageConstIterator.h"

namespace otb
{
/** \class PolyLineImageIterator
 *  \brief Uses the bresenham algorithm to iterate over a polyline on an image.
 *
 *  This iterator encapsulates the itk::LineIterator along with the VectorContainer iterator on
 *  the vertex list of the polyline.
 *
 * \ingroup Iterators
 * \sa PolyLineParametricPath
 * \sa PolyLineParametricPathWithValue
 * \sa LineIterator
 * \sa LineConstIterator
 * \sa PolyLineImageConstIterator
 *
 * \ingroup OTBPath
 */
template <class TImage, class TPath>
class ITK_EXPORT PolyLineImageIterator : public PolyLineImageConstIterator<TImage, TPath>
{
public:
  /** Standard typedefs */
  typedef PolyLineImageIterator Self;
  typedef PolyLineImageConstIterator<TImage, TPath> Superclass;

  itkStaticConstMacro(ImageIteratorDimension, unsigned int, TImage::ImageDimension);

  typedef typename TImage::IndexType             IndexType;
  typedef typename TImage::IndexValueType        IndexValueType;
  typedef typename TImage::OffsetType            OffsetType;
  typedef typename TImage::OffsetValueType       OffsetValueType;
  typedef typename TImage::SizeType              SizeType;
  typedef typename TImage::SizeValueType         SizeValueType;
  typedef typename TImage::RegionType            RegionType;
  typedef typename TImage::SpacingType           SpacingType;
  typedef typename TImage::PointType             PointType;
  typedef TImage                                 ImageType;
  typedef TPath                                  PathType;
  typedef typename PathType::VertexType          VertexType;
  typedef typename PathType::VertexListType      VertexListType;
  typedef typename VertexListType::ConstIterator VertexIteratorType;
  typedef itk::LineIterator<ImageType>           InternalImageIteratorType;
  typedef typename TImage::PixelContainer        PixelContainer;
  typedef typename PixelContainer::Pointer       PixelContainerPointer;
  typedef typename TImage::InternalPixelType     InternalPixelType;
  typedef typename TImage::PixelType             PixelType;
  typedef typename TImage::AccessorType          AccessorType;

  /** Set the pixel value */
  void Set(const PixelType& value)
  {
    this->m_InternalImageIterator.Set(value);
  }
  /** Return a reference to the pixel
   * This method will provide the fastest access to pixel
   * data, but it will NOT support ImageAdaptors. */
  PixelType& Value(void)
  {
    return this->m_InternalImageIterator.Value();
  }
  Self& operator=(const Self& it)
  {
    this->Superclass::operator=(it);
    return *this;
  }

  /** Constructor establishes an iterator to walk along a line */
  PolyLineImageIterator(ImageType* imagePtr, PathType* pathPtr) : Superclass(imagePtr, pathPtr){};
  /** Default Destructor. */
  ~PolyLineImageIterator() override
  {
  }
};
} // End namespace otb

#endif
