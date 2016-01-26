/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Mines-Telecom. All rights reserved.
  See IMTCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbSubsampledImageRegionConstIterator_h
#define __otbSubsampledImageRegionConstIterator_h

#include "itkImageRegionConstIterator.h"

namespace otb {

/** SubsampledImageRegionConstIterator
 * \brief Regular subsample iterator over an image
 *
 * This iterator is a itk::ImageRegionConstIterator that perform a subsampled
 * scan over an image. It runs one pixel over X (in row, line, slice... dimensions),
 * if X is the (integer) value of the SubsampleFactor.
 *
 * Specific value of the subsample factor may be given for each dimension.
 *
 * \ingroup ImageIterator
 * \sa WaveletFilterBank
 */
template <class TImage>
class ITK_EXPORT SubsampledImageRegionConstIterator
  : public itk::ImageRegionConstIterator<TImage>
{
public:
  /** Standard typedef. */
  typedef SubsampledImageRegionConstIterator    Self;
  typedef itk::ImageRegionConstIterator<TImage> Superclass;

  /** Run-time type information (and related methods). */
  itkTypeMacro(SubsampledImageRegionConstIterator, ImageRegionConstIterator);

  /** Dimension of the image the iterator walks.  This constant is needed so
   * functions that are templated over image iterator type (as opposed to
   * being templated over pixel type and dimension) can have compile time
   * access to the dimension of the image that the iterator walks. */
  itkStaticConstMacro(ImageIteratorDimension, unsigned int,
                      Superclass::ImageIteratorDimension);

  /** Index typedef support. While this was already typdef'ed in the superclass
   * it needs to be redone here for this subclass to compile properly with gcc. */
  typedef typename Superclass::IndexType IndexType;

  /** Size typedef support. While this was already typdef'ed in the superclass
   * it needs to be redone here for this subclass to compile properly with gcc. */
  typedef typename Superclass::SizeType SizeType;

  /** Region typedef support. */
  typedef typename Superclass::RegionType RegionType;

  /** Image typedef support. While this was already typdef'ed in the superclass
   * it needs to be redone here for this subclass to compile properly with gcc. */
  typedef typename Superclass::ImageType ImageType;

  /** Offset typedef support. This explicit redefinition allows Setting/Getting
   * the location of the iterator. */
  //typedef typename Superclass::OffsetType OffsetType;
  typedef unsigned long OffsetType;

  /** PixelContainer typedef support. Used to refer to the container for
   * the pixel data. While this was already typdef'ed in the superclass
   * it needs to be redone here for this subclass to compile properly with gcc. */
  typedef typename Superclass::PixelContainer        PixelContainer;
  typedef typename Superclass::PixelContainerPointer PixelContainerPointer;

  /** Internal Pixel Type */
  typedef typename Superclass::InternalPixelType InternalPixelType;

  /** External Pixel Type */
  typedef typename Superclass::PixelType PixelType;

  /**  Accessor type that convert data between internal and external
   *  representations. */
  typedef typename Superclass::AccessorType AccessorType;

  /** Index value used for pixel location */
  //typedef typename Superclass::IndexValueType IndexValueType;
  typedef typename IndexType::IndexValueType    IndexValueType;

  /** Default constructor. Needed since we provide a cast constructor. */
  SubsampledImageRegionConstIterator();

  /** Constructor establishes an iterator to walk a particular image and a
   * particular region of that image. */
  SubsampledImageRegionConstIterator (const ImageType * ptr, const RegionType &region);

  /** Constructor that can be used to cast from an ImageIterator to an
   * SubsampledImageRegionConstIterator. Many routines return an ImageIterator
   * but for a particular task, you may want an SubsampledImageRegionConstIterator.
   * Rather than provide overloaded APIs that return different types of Iterators, itk
   * returns ImageIterators and uses constructors to cast from an
   * ImageIterator to a SubsampledImageRegionConstIterator. */
  SubsampledImageRegionConstIterator(const itk::ImageIterator<TImage> &it);

  /** Constructor that can be used to cast from an ImageConstIterator to an
   * SubsampledImageRegionConstIterator. Many routines return an ImageIterator
   * but for a particular task, you may want an SubsampledImageRegionConstIterator.
   * Rather than provide overloaded APIs that return different types of Iterators, itk
   * returns ImageIterators and uses constructors to cast from an
   * ImageIterator to a SubsampledImageRegionConstIterator. */
  SubsampledImageRegionConstIterator(const itk::ImageConstIterator<TImage> &it);

  /** Set an isotropic subsampling factor */
  void SetSubsampleFactor (IndexValueType factor);

  /** Set / Get the subsample factor */
  void SetSubsampleFactor(const IndexType& factor);

  const IndexType& GetSubsampleFactor() const
  {
    return this->m_SubsampleFactor;
  }

  /** Move an iterator to the beginning of the region. "Begin" is
  * defined as the first pixel in the region. */
  void GoToBegin();

  /** Move an iterator to the end of the region. "End" is defined as
  * one pixel past the last pixel of the region. */
  void GoToEnd();

  /** Is the iterator at the beginning of the region?
   * "Begin" is defined here as before the first pixel
   * of the region.
   * On the contrary to Superclasses, now you cannot get
   * *it even if IsAtBegin() is true!!!
   */
  bool IsAtBegin(void) const
  {
    return (this->m_Offset <= m_SubSampledBeginOffset);
  }

  /** Is the iterator at the end of the region?
   * "End" is defined as past the last candidate pixel
   * of the region (under the subsample point of view).
   *
   * For instance, if in 1D, the Size is 10 with a subsample
   * factor of 4, the last candidate offset would be 8.
   */
  bool IsAtEnd(void) const
  {
    return (this->m_Offset >= m_SubSampledEndOffset);
  }

  /** Set the index.
   * It is moved to the next available (usable) index.
   * \sa GetIndex */
  void SetIndex(const IndexType& ind);

  /** Get the Index. */
  IndexType GetIndex() const
  {
    return Superclass::GetIndex();
  }

  /** Increment (prefix) the fastest moving dimension of the iterator's index.
   * This operator will constrain the iterator within the region (i.e. the
   * iterator will automatically wrap from the end of the row of the region
   * to the beginning of the next row of the region) up until the iterator
   * tries to moves past the last pixel of the region.  Here, the iterator
   * will be set to be one pixel past the end of the region.
   * \sa operator++(int) */
  Self & operator ++()
  {
    // On the contrary to itk::ImageRegionConstIterator, m_Offset to
    // not incremented before the test
    if (this->m_Offset + m_SubsampleFactor[0] >= this->m_SpanEndOffset)
      {
      this->Increment();
      }
    else
      {
      // Now, the increment is performed
      this->m_Offset += m_SubsampleFactor[0];
      }
    return *this;
  }

  /** Decrement (prefix) the fastest moving dimension of the iterator's index.
   * This operator will constrain the iterator within the region (i.e. the
   * iterator will automatically wrap from the beginning of the row of the region
   * to the end of the next row of the region) up until the iterator
   * tries to moves past the first pixel of the region.  Here, the iterator
   * will be set to be one pixel past the beginning of the region.
   * \sa operator--(int) */
  Self & operator --()
  {
    // On the contrary to itk::ImageRegionConstIterator, m_Offset
    // is not decremented here (it may become negative!)
    if (this->m_Offset < this->m_SpanBeginOffset + m_SubsampleFactor[0])
      {
      this->Decrement();
      }
    else
      {
      // Now we can
      this->m_Offset -= m_SubsampleFactor[0];
      }
    return *this;
  }

  /** This iterator give the possibility to Set/Get the current location if scanning.
   * No Bound checking is performed when setting the position.
   */
  void SetOffset(const OffsetType& offset);
  OffsetType GetOffset() const
  {
    return this->m_Offset;
  }

  /** GenerateOutputInformation.
   * In order to help copy into a new Image, give the new region parameters
   */
  RegionType GenerateOutputInformation() const;

protected:
  IndexType     m_SubsampleFactor;
  unsigned long m_SubSampledBeginOffset;
  //unsigned long m_SubSampledReverseEndOffset;
  long          m_SubSampledEndOffset;
  IndexType     m_FirstUsableIndex;
  IndexType     m_LastUsableIndex;

private:
  void Increment(); // jump in a direction other than the fastest moving
  void Decrement(); // go back in a direction other than the fastest moving
};

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSubsampledImageRegionConstIterator.txx"
#endif

#endif
