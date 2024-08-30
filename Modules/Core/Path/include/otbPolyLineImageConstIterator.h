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

#ifndef otbPolyLineImageConstIterator_h
#define otbPolyLineImageConstIterator_h

#include "itkIndex.h"
#include "itkImage.h"
#include "itkLineIterator.h"

namespace otb
{
/** \class PolyLineImageConstIterator
 *  \brief Uses the bresenham algorithm to iterate over a polyline on an image (const version).
 *
 *  This iterator encapsulates the itk::LineIterator along with the VectorContainer iterator on
 *  the vertex list of the polyline.
 *
 * \ingroup Iterators
 * \sa PolyLineParametricPath
 * \sa PolyLineParametricPathWithValue
 * \sa LineIterator
 * \sa LineConstIterator
 * \sa PolyLineImageIterator
 *
 * \ingroup OTBPath
 */
template <class TImage, class TPath>
class ITK_EXPORT PolyLineImageConstIterator
{
public:
  /** Standard typedefs */
  typedef PolyLineImageConstIterator Self;

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

  /** Get the dimension (size) of the index. */
  static unsigned int GetImageIteratorDimension()
  {
    return TImage::ImageDimension;
  }
  /** Get the index */
  const IndexType GetIndex()
  {
    return m_InternalImageIterator.GetIndex();
  }
  /** Get the pixel value */
  const PixelType Get(void) const
  {
    return m_InternalImageIterator.Get();
  }
  /** Is the iterator at the end of the line? */
  bool IsAtEnd()
  {
    return (m_InternalVertexIterator == m_Path->GetVertexList()->End()) && m_InternalImageIterator.IsAtEnd();
  }
  /** Move an iterator to the beginning of the line. */
  void GoToBegin();
  /** Walk forward along the line to the next index in the image. */
  void operator++();
  /** operator= is provided to make sure the handle to the image is properly
   * reference counted. */
  Self& operator=(const Self& it);
  /** Constructor establishes an iterator to walk along a line */
  PolyLineImageConstIterator(const ImageType* imagePtr, const PathType* pathPtr);
  /** Default Destructor. */
  virtual ~PolyLineImageConstIterator()
  {
  }

protected: // made protected so other iterators can access
  /** Smart pointer to the source image. */
  typename ImageType::ConstWeakPointer m_Image;

  /** Smart pointer to the path */
  typename PathType::ConstPointer m_Path;
  InternalImageIteratorType       m_InternalImageIterator;
  VertexIteratorType              m_InternalVertexIterator;
};

} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPolyLineImageConstIterator.hxx"
#endif

#endif
