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
#ifndef __otbPolyLineImageIterator_h
#define __otbPolyLineImageIterator_h

#include "itkImage.h"
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
 */
template <class TImage, class TPath>
class ITK_EXPORT PolyLineImageIterator
  : public PolyLineImageConstIterator<TImage, TPath>
{
public:
  /** Standard typedefs */
  typedef PolyLineImageIterator                     Self;
  typedef PolyLineImageConstIterator<TImage, TPath> Superclass;

  itkStaticConstMacro(ImageIteratorDimension, unsigned int,
                      TImage::ImageDimension);

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

  /** Run-time type information (and related methods). */
  itkTypeMacro(PolyLineImageIterator, PolyLineImageConstIterator);

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
  Self & operator =(const Self& it)
  {
    this->Superclass::operator =(it);
    return *this;
  }

  /** Constructor establishes an iterator to walk along a line */
  PolyLineImageIterator(ImageType * imagePtr, PathType * pathPtr)
    : Superclass(imagePtr, pathPtr) {};
  /** Default Destructor. */
  virtual ~PolyLineImageIterator() {}
};
} // End namespace otb

#endif
