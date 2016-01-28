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
#ifndef otbMaskedIteratorDecorator_hxx
#define otbMaskedIteratorDecorator_hxx

#include "otbMaskedIteratorDecorator.h"

namespace otb
{

/*
For documentation on constructor argument forwarding with boost see:
http://www.boost.org/doc/libs/1_60_0/doc/html/move/construct_forwarding.html
*/
template <typename TIteratorType>
template <typename T1>
MaskedIteratorDecorator<TIteratorType>::MaskedIteratorDecorator(
                        typename MaskType::Pointer mask,
                        typename ImageType::Pointer image,
                        BOOST_FWD_REF(T1) arg1):
                          m_ItMask(mask,
                                   boost::forward<T1>(arg1)),
                          m_ItImage(image,
                                    boost::forward<T1>(arg1))
{
  ComputeMaskedBegin();
}

template <typename TIteratorType>
template <typename T1, typename T2>
MaskedIteratorDecorator<TIteratorType>::MaskedIteratorDecorator(
                        typename MaskType::Pointer mask,
                        typename ImageType::Pointer image,
                        BOOST_FWD_REF(T1) arg1,
                        BOOST_FWD_REF(T2) arg2):
                          m_ItMask(mask,
                                   boost::forward<T1>(arg1),
                                   boost::forward<T2>(arg2)),
                          TIteratorType(image,
                                        boost::forward<T1>(arg1),
                                        boost::forward<T2>(arg2))
{
  ComputeMaskedBegin();
}

template <typename TIteratorType>
template <typename T1, typename T2, typename T3>
MaskedIteratorDecorator<TIteratorType>::MaskedIteratorDecorator(
                        typename MaskType::Pointer mask,
                        typename ImageType::Pointer image,
                        BOOST_FWD_REF(T1) arg1,
                        BOOST_FWD_REF(T2) arg2,
                        BOOST_FWD_REF(T3) arg3):
                          m_ItMask(mask,
                                   boost::forward<T1>(arg1),
                                   boost::forward<T2>(arg2),
                                   boost::forward<T3>(arg3)),
                          TIteratorType(image,
                                        boost::forward<T1>(arg1),
                                        boost::forward<T2>(arg2),
                                        boost::forward<T2>(arg3))
{
  ComputeMaskedBegin();
}

template <typename TIteratorType>
typename MaskedIteratorDecorator<TIteratorType>::IndexType MaskedIteratorDecorator<TIteratorType>::GetIndex() const
{
  return m_ItMask.GetIndex();
}

template <typename TIteratorType>
void MaskedIteratorDecorator<TIteratorType>::GoToBegin()
{
  m_ItMask.SetIndex(m_Begin);
  m_ItImage.SetIndex(m_Begin);
}

template <typename TIteratorType>
void MaskedIteratorDecorator<TIteratorType>::GoToEnd()
{
  m_ItMask.GoToEnd();
  m_ItImage.GoToEnd();
}

template <typename TIteratorType>
bool MaskedIteratorDecorator<TIteratorType>::IsAtBegin() const
{
  return m_ItMask.GetIndex() == m_Begin || m_ItImage.GetIndex() == m_Begin;
}

template <typename TIteratorType>
bool MaskedIteratorDecorator<TIteratorType>::IsAtEnd() const
{
  return m_ItMask.IsAtEnd() || m_ItImage.IsAtEnd();
}

// Wrap the underlying iterators to skip masked pixels
template <typename TIteratorType>
MaskedIteratorDecorator<TIteratorType>& MaskedIteratorDecorator<TIteratorType>::operator++()
{
  do
  {
    ++m_ItMask;
    ++m_ItImage;
  } while (m_ItMask.Value() == 0 && !this->IsAtEnd());
  return *this;
}

// Wrap the underlying iterators to skip masked pixels
template <typename TIteratorType>
MaskedIteratorDecorator<TIteratorType>& MaskedIteratorDecorator<TIteratorType>::operator--()
{
  do
  {
    --m_ItMask;
    --m_ItImage;
  } while (m_ItMask.Value() == 0 && !this->IsAtBegin());
  return *this;
}

/*
template <typename TIteratorType>
void MaskedIteratorDecorator<TIteratorType>::Set(const PixelType& value) const
{
  m_ItImage.Set(value);
}
*/

template <typename TIteratorType>
const typename MaskedIteratorDecorator<TIteratorType>::PixelType& MaskedIteratorDecorator<TIteratorType>::Value(void) const
{
  return m_ItImage.Value();
}

template <typename TIteratorType>
TIteratorType& MaskedIteratorDecorator<TIteratorType>::GetMaskIterator()
{
  return m_ItMask;
}

template <typename TIteratorType>
const TIteratorType& MaskedIteratorDecorator<TIteratorType>::GetMaskIterator() const
{
  return m_ItMask;
}

template <typename TIteratorType>
TIteratorType& MaskedIteratorDecorator<TIteratorType>::GetImageIterator()
{
  return m_ItImage;
}

template <typename TIteratorType>
const TIteratorType& MaskedIteratorDecorator<TIteratorType>::GetImageIterator() const
{
  return m_ItImage;
}

// Compute begin iterator position taking into account the mask
template <typename TIteratorType>
void MaskedIteratorDecorator<TIteratorType>::ComputeMaskedBegin()
{
  // We must search for the first index where the image is not masked
  // Start searching at the begining
  m_ItMask.GoToBegin();
  m_ItImage.GoToEnd();

  // Advance to the first non-masked position, or the end
  while (m_ItMask.Value() == 0 && !m_ItMask.IsAtEnd() && !m_ItImage.IsAtEnd())
  {
    ++m_ItMask;
    ++m_ItImage;
  }
  // Remember it so that GoToBegin and IsAtBegin can check it
  this->m_Begin = m_ItMask.GetIndex();
}

} // namespace otb

#endif
