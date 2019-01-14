/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 * Copyright (C) 2007-2012 Institut Mines Telecom / Telecom Bretagne
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


#ifndef otbSubsampledImageRegionIterator_h
#define otbSubsampledImageRegionIterator_h

#include "otbSubsampledImageRegionConstIterator.h"

namespace otb {

/** \class SubsampledImageRegionIterator
 * \brief Regular subsample iterator over an image
 *
 * This iterator is a itk::ImageRegionConstIterator that perform a subsampled
 * scan over an image. It runs one pixel over X (in row, line, slice... dimensions),
 * if X is the (integer) value of the SubsampleFactor.
 *
 * It inherits from SubsampledImageRegionConstIterator and can modify the pixel values...
 *
 * \ingroup ImageIterator
 * \sa SubsampledImageRegionConstIterator
 *
 * \ingroup OTBCommon
 */
template <class TImage>
class ITK_EXPORT SubsampledImageRegionIterator
  : public SubsampledImageRegionConstIterator<TImage>
{
public:
  /** Standard typedef. */
  typedef SubsampledImageRegionIterator              Self;
  typedef SubsampledImageRegionConstIterator<TImage> Superclass;

  /** Run-time type information (and related methods). */
  itkTypeMacro(SubsampledImageRegionConstIterator, SubsampledImageRegionConstIterator);

  itkStaticConstMacro(ImageIteratorDimension, unsigned int,
                      Superclass::ImageIteratorDimension);

  // typedef redefinition from superclass
  typedef typename Superclass::IndexType             IndexType;
  typedef typename Superclass::SizeType              SizeType;
  typedef typename Superclass::RegionType            RegionType;
  typedef typename Superclass::OffsetType            OffsetType;
  typedef typename Superclass::ImageType             ImageType;
  typedef typename Superclass::PixelContainer        PixelContainer;
  typedef typename Superclass::PixelContainerPointer PixelContainerPointer;
  typedef typename Superclass::InternalPixelType     InternalPixelType;
  typedef typename Superclass::PixelType             PixelType;
  typedef typename Superclass::AccessorType          AccessorType;
  typedef typename Superclass::IndexValueType        IndexValueType;

  // Constructors
  SubsampledImageRegionIterator()
    : SubsampledImageRegionConstIterator<TImage> () {}

  SubsampledImageRegionIterator (const ImageType * ptr, const RegionType &region)
    : SubsampledImageRegionConstIterator<TImage> (ptr, region) {}

  SubsampledImageRegionIterator(const itk::ImageIterator<TImage> &it)
    : SubsampledImageRegionConstIterator<TImage> (it) {}

  SubsampledImageRegionIterator(const itk::ImageConstIterator<TImage> &it)
    : SubsampledImageRegionConstIterator<TImage> (it) {}

  /** Set the current pixel value */
  void Set(const PixelType& value) const
  {
    this->m_PixelAccessorFunctor.Set(*(const_cast<InternalPixelType *>(
                                         this->m_Buffer + this->m_Offset)), value);
  }

  PixelType& Value(void)
  {
    return *(const_cast<InternalPixelType *>(this->m_Buffer + this->m_Offset));
  }

}; // end of class

} // end of namespace otb

#endif
