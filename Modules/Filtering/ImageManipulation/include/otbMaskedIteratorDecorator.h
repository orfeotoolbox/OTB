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
#ifndef otbMaskedIteratorDecorator_h
#define otbMaskedIteratorDecorator_h

#include <boost/move/utility_core.hpp>

namespace otb
{

/** \class MaskedIteratorDecorator
 * \brief Decorate an iterator to ignore masked pixels
 *
 * An iterator that dispatches its operations on both an image and a mask using
 * two iterators of the same type. It skips over pixels where the value of the
 * mask is zero. There is a limitation that the image and the mask must both be
 * of the same type.
 *
 * \ingroup ImageIterator
 * \ingroup OTBCommon
 */
template <typename TIteratorType>
class MaskedIteratorDecorator
{
public:
  typedef MaskedIteratorDecorator<TIteratorType> Self;
  typedef typename TIteratorType::ImageType      MaskType;
  typedef typename TIteratorType::ImageType      ImageType;
  typedef typename ImageType::IndexType          IndexType;
  typedef typename ImageType::PixelType          PixelType;

  // Run-time type information
  itkTypeMacroNoParent(MaskedIteratorDecorator);

  // Constructors forward arguments to both iterators
  template <typename T1>
  MaskedIteratorDecorator(typename MaskType::Pointer mask,
                          typename ImageType::Pointer image,
                          BOOST_FWD_REF(T1) arg1);

  template <typename T1, typename T2>
  MaskedIteratorDecorator(typename MaskType::Pointer mask,
                          typename ImageType::Pointer image,
                          BOOST_FWD_REF(T1) arg1,
                          BOOST_FWD_REF(T2) arg2);

  template <typename T1, typename T2, typename T3>
  MaskedIteratorDecorator(typename MaskType::Pointer mask,
                          typename ImageType::Pointer image,
                          BOOST_FWD_REF(T1) arg1,
                          BOOST_FWD_REF(T2) arg2,
                          BOOST_FWD_REF(T3) arg3);

  /** Current iterator index. Wraps the underlying iterator GetIndex() */
  IndexType GetIndex() const;

  /** Move to the first valid iteration position. Note that this is different
   * from the underlying iterators GoToBegin because their first pixels might be
   * masked */
  void GoToBegin();

  /** Move one pixel past the last valid iteration index */
  void GoToEnd();

  /** True iff the iterator is at the beginning */
  bool IsAtBegin() const;

  /** True iff the iterator is at the end */
  bool IsAtEnd() const;

  /** Advance to the next valid iteration position. That is the next non masked
   * position of the image iterator */
  Self& operator++();

  /** Move to the previous valid iteration position. That is the previous non
   * masked position of the image iterator */
  Self & operator--();

  /** Set the image pixel value */
  void Set(const PixelType& value) const;

  /** Get the image pixel value */
  PixelType& Value(void); // const?

  // Can't wrap all methods because iterator interface is open ended
  // So provide access to inner iterators:

  /** Underlying mask iterator accessor */
  TIteratorType&       GetMaskIterator();

  /** Underlying mask iterator const accessor */
  const TIteratorType& GetMaskIterator() const;

  /** Underlying image iterator accessor */
  TIteratorType&       GetImageIterator();

  /** Underlying image iterator const accessor */
  const TIteratorType& GetImageIterator() const;

private:
  /** Private method to compute the begin iterator position taking into account
  the mask. Called by the constructor. */
  void ComputeMaskedBegin();

private:
  // Inner iterators on the image and mask
  TIteratorType m_ItMask;
  TIteratorType m_ItImage;

  // Unmasked bounds
  IndexType m_Begin;
};

}

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMaskedIteratorDecorator.hxx"
#endif

#endif
