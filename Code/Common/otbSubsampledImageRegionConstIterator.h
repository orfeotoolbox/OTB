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
 * \ingroup ImageIterator
 * \sa StationaryFilterBank
 */
template < class TImage >
class ITK_EXPORT SubsampledImageRegionConstIterator 
	: public itk::ImageRegionConstIterator< TImage >
{
public:
  /** Standard typedef. */
  typedef SubsampledImageRegionConstIterator	Self;
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
  typedef typename Superclass::RegionType   RegionType;


  /** Image typedef support. While this was already typdef'ed in the superclass
   * it needs to be redone here for this subclass to compile properly with gcc. */
  typedef typename Superclass::ImageType ImageType;

  /** PixelContainer typedef support. Used to refer to the container for
   * the pixel data. While this was already typdef'ed in the superclass
   * it needs to be redone here for this subclass to compile properly with gcc. */
  typedef typename Superclass::PixelContainer         PixelContainer;
  typedef typename Superclass::PixelContainerPointer  PixelContainerPointer;
  
  /** Internal Pixel Type */
  typedef typename Superclass::InternalPixelType   InternalPixelType;

  /** External Pixel Type */
  typedef typename Superclass::PixelType   PixelType;

  /**  Accessor type that convert data between internal and external
   *  representations. */
  typedef typename Superclass::AccessorType     AccessorType;

  /** Index value used for pixel location */
  typedef typename Superclass::IndexValueType IndexValueType;

  /** Default constructor. Needed since we provide a cast constructor. */
  SubsampledImageRegionConstIterator() : itk::ImageRegionConstIterator<TImage> ()
  {
    m_SubsampleFactor = 1;
    m_SubSampledEndOffset = this->m_EndOffset;

    const IndexType& startIndex = this->m_Region.GetIndex();
    const SizeType& size = this->m_Region.GetSize();

    for ( unsigned int i = 0; i < ImageIteratorDimension; i++ )
    {
      m_LastUsableIndex[i] = startIndex[i] 
        + static_cast<IndexValueType>( m_SubsampleFactor * ((size[i]-1) / m_SubsampleFactor) );
    }
  }

  /** Constructor establishes an iterator to walk a particular image and a
   * particular region of that image. */
  SubsampledImageRegionConstIterator ( const ImageType *ptr,
                                        const RegionType &region)
    : itk::ImageRegionConstIterator< TImage > ( ptr, region )
  {
    m_SubsampleFactor = 1;
    m_SubSampledEndOffset = this->m_EndOffset;

    const IndexType& startIndex = this->m_Region.GetIndex();
    const SizeType& size = this->m_Region.GetSize();

    for ( unsigned int i = 0; i < ImageIteratorDimension; i++ )
    {
      m_LastUsableIndex[i] = startIndex[i] 
        + static_cast<IndexValueType>( m_SubsampleFactor * ((size[i]-1) / m_SubsampleFactor) );
    }
  }

  /** Constructor that can be used to cast from an ImageIterator to an
   * SubsampledImageRegionConstIterator. Many routines return an ImageIterator
   * but for a particular task, you may want an SubsampledImageRegionConstIterator.  
   * Rather than provide overloaded APIs that return different types of Iterators, itk
   * returns ImageIterators and uses constructors to cast from an
   * ImageIterator to a SubsampledImageRegionConstIterator. */
  SubsampledImageRegionConstIterator( const itk::ImageIterator<TImage> &it )
    : itk::ImageRegionConstIterator< TImage >( it )
  {
    m_SubsampleFactor = 1;
    m_SubSampledEndOffset = this->m_EndOffset;

    const IndexType& startIndex = this->m_Region.GetIndex();
    const SizeType& size = this->m_Region.GetSize();

    for ( unsigned int i = 0; i < ImageIteratorDimension; i++ )
    {
      m_LastUsableIndex[i] = startIndex[i] 
        + static_cast<IndexValueType>( m_SubsampleFactor * ((size[i]-1) / m_SubsampleFactor) );
    }
  }

  /** Constructor that can be used to cast from an ImageConstIterator to an
   * SubsampledImageRegionConstIterator. Many routines return an ImageIterator 
   * but for a particular task, you may want an SubsampledImageRegionConstIterator.  
   * Rather than provide overloaded APIs that return different types of Iterators, itk
   * returns ImageIterators and uses constructors to cast from an
   * ImageIterator to a SubsampledImageRegionConstIterator. */
  SubsampledImageRegionConstIterator( const itk::ImageConstIterator<TImage> &it)
    : itk::ImageRegionConstIterator< TImage >( it )
  {
    m_SubsampleFactor = 1;

    const IndexType& startIndex = this->m_Region.GetIndex();
    const SizeType& size = this->m_Region.GetSize();

    for ( unsigned int i = 0; i < ImageIteratorDimension; i++ )
    {
      m_LastUsableIndex[i] = startIndex[i] 
        + static_cast<IndexValueType>( m_SubsampleFactor * ((size[i]-1) / m_SubsampleFactor) );
    }

  }

  /** Set / Get the subsample factor */
  void SetSubsampleFactor ( unsigned int factor )
  {
    this->m_SubsampleFactor = factor;

    // Evaluate the last possible pixel.
    const IndexType& startIndex = this->m_Region.GetIndex();
    const SizeType& size = this->m_Region.GetSize();

    for ( unsigned int i = 0; i < ImageIteratorDimension; i++ )
    {
      m_LastUsableIndex[i] = startIndex[i]
        + static_cast<IndexValueType>( m_SubsampleFactor * ( (size[i]-1) / m_SubsampleFactor ) );
    }

    m_SubSampledEndOffset = this->m_Image->ComputeOffset( m_LastUsableIndex ) + 1;

    otbGenericMsgDebugMacro(<<"m_EndOffset=" << this->m_EndOffset);
    otbGenericMsgDebugMacro(<<"m_SubSampledEndOffset="<< m_SubSampledEndOffset);

    for ( unsigned int i = 0; i < ImageIteratorDimension; i++ )
    {
      otbGenericMsgDebugMacro(<<"m_LastUsableIndex[" << i << "] = " << m_LastUsableIndex[i]);
    }

}

  unsigned int GetSubsampleFactor () const
  {
    return this->m_SubsampleFactor;
  }

  /** Move an iterator to the beginning of the region. "Begin" is
  * defined as the first pixel in the region. */
  void GoToBegin()
  {
    Superclass::GoToBegin();

    const SizeType& size = this->m_Region.GetSize();

    this->m_SpanBeginOffset = this->m_Offset;
    this->m_SpanEndOffset = this->m_Offset 
                            + static_cast<IndexValueType>( m_SubsampleFactor * ((size[0]-1) / m_SubsampleFactor) )
                            + 1;
  }

  /** Move an iterator to the end of the region. "End" is defined as
  * one pixel past the last pixel of the region. */
  void GoToEnd()
  {
    Superclass::GoToEnd();
    this->m_Offset = m_SubSampledEndOffset - 1;
    this->m_SpanEndOffset = this->m_Offset + 1;
    this->m_SpanBeginOffset = this->m_Offset - m_LastUsableIndex[0];
  }

  /** Is the iterator at the beginning of the region? 
   * "Begin" is defined here as before the first pixel 
   * of the region. 
   * On the contrary to Superclasses, now you cannot get 
   * *it even if IsAtBegin() is true!!!
   */
  bool IsAtBegin(void) const
  {
    return ( this->m_Offset <= this->m_BeginOffset );
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
    return ( this->m_Offset >= m_SubSampledEndOffset );
  }

  /** Set the index. No bounds checking is performed. This is overridden
   * from the parent because we have an extra ivar.
   * \sa GetIndex */
  void SetIndex(const IndexType &ind)
  {
    Superclass::SetIndex( ind );
  }

  /** Increment (prefix) the fastest moving dimension of the iterator's index.
   * This operator will constrain the iterator within the region (i.e. the
   * iterator will automatically wrap from the end of the row of the region
   * to the beginning of the next row of the region) up until the iterator
   * tries to moves past the last pixel of the region.  Here, the iterator
   * will be set to be one pixel past the end of the region.
   * \sa operator++(int) */
  Self & operator++()
  {
    // On the contrary to itk::ImageRegionConstIterator, m_Offset to
    // not incremented before the test
    if( this->m_Offset + m_SubsampleFactor >= this->m_SpanEndOffset )
    {
      this->Increment();
    }
    else
    {
      // Now, the increment is performed
      this->m_Offset += m_SubsampleFactor;
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
  Self & operator--()
  {
    // On the contrary to itk::ImageRegionConstIterator, m_Offset
    // is not decremented here (it may become negative!)
    if ( this->m_Offset < this->m_SpanBeginOffset + m_SubsampleFactor ) 
    {
      this->Decrement();
    }
    else
    {
      // Now we can
      this->m_Offset -= m_SubsampleFactor;
    }
    return *this;
  }

  /** In order to help copy into a new Image, give the new region parameters
   */
  RegionType GetNewRegion () const
  {
    IndexType startIndex = this->m_Region.GetIndex();
    SizeType size = this->m_Region.GetSize();

    for ( unsigned int i = 0; i < ImageIteratorDimension; i++ )
    {
      startIndex[i] /= m_SubsampleFactor;
      --size[i];
      size[i] /= m_SubsampleFactor;
      ++size[i];
    }

    RegionType newRegion;
    newRegion.SetIndex( startIndex );
    newRegion.SetSize( size );

    for ( unsigned int i = 0; i < ImageIteratorDimension; i++ )
    {
      otbGenericMsgDebugMacro(<<"NewRegionIndex[" << i << "] = " << startIndex[i]);
      otbGenericMsgDebugMacro(<<"NewRegionSize [" << i << "] = " << size[i]);
    }

    return newRegion;
  }

protected:
  unsigned int m_SubsampleFactor;
  unsigned long m_SubSampledEndOffset;
  IndexType m_LastUsableIndex;

private:
  void Increment(); // jump in a direction other than the fastest moving
  void Decrement(); // go back in a direction other than the fastest moving
};


} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSubsampledImageRegionConstIterator.txx"
#endif

#endif

