/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom / Telecom Bretagne. All rights reserved.
  See ITCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbSubsampledImageRegionIterator_h
#define __otbSubsampledImageRegionIterator_h

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
