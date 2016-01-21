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

template <typename TIteratorType>
template <typename T1>
MaskedIteratorDecorator<TIteratorType>::MaskedIteratorDecorator(
                        typename MaskType::Pointer mask,
                        typename ImageType::Pointer image,
                        BOOST_FWD_REF(T1) arg1):
                          m_itMask(mask,
                                   boost::forward<T1>(arg1)),
                          m_itImage(image,
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
                          m_itMask(mask,
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
                          m_itMask(mask,
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
  return m_itMask.GetIndex();
}

template <typename TIteratorType>
void MaskedIteratorDecorator<TIteratorType>::GoToBegin()
{
  m_itMask.SetIndex(m_begin);
  m_itImage.SetIndex(m_begin);
}

template <typename TIteratorType>
void MaskedIteratorDecorator<TIteratorType>::GoToEnd()
{
  m_itMask.GoToEnd();
  m_itImage.GoToEnd();
}

template <typename TIteratorType>
bool MaskedIteratorDecorator<TIteratorType>::IsAtBegin() const
{
  return m_itMask.GetIndex() == m_begin || m_itImage.GetIndex() == m_begin;
}

template <typename TIteratorType>
bool MaskedIteratorDecorator<TIteratorType>::IsAtEnd() const
{
  return m_itMask.IsAtEnd() || m_itImage.IsAtEnd();
}

// Wrap the underlying iterators to skip masked pixels
template <typename TIteratorType>
MaskedIteratorDecorator<TIteratorType>& MaskedIteratorDecorator<TIteratorType>::operator++()
{
  do
  {
    ++m_itMask;
    ++m_itImage;
  } while (m_itMask.Value() == 0 && !this->IsAtEnd());
  return *this;
}

// Wrap the underlying iterators to skip masked pixels
template <typename TIteratorType>
MaskedIteratorDecorator<TIteratorType>& MaskedIteratorDecorator<TIteratorType>::operator--()
{
  do
  {
    --m_itMask;
    --m_itImage;
  } while (m_itMask.Value() == 0 && !this->IsAtBegin());
  return *this;
}

template <typename TIteratorType>
void MaskedIteratorDecorator<TIteratorType>::Set(const PixelType& value) const
{
  m_itImage.Set(value);
}

template <typename TIteratorType>
typename MaskedIteratorDecorator<TIteratorType>::PixelType& MaskedIteratorDecorator<TIteratorType>::Value(void) // const ?
{
  return m_itImage.Value();
}

template <typename TIteratorType>
TIteratorType& MaskedIteratorDecorator<TIteratorType>::GetMaskIterator()
{
  return m_itMask;
}

template <typename TIteratorType>
const TIteratorType& MaskedIteratorDecorator<TIteratorType>::GetMaskIterator() const
{
  return m_itMask;
}

template <typename TIteratorType>
TIteratorType& MaskedIteratorDecorator<TIteratorType>::GetImageIterator()
{
  return m_itImage;
}

template <typename TIteratorType>
const TIteratorType& MaskedIteratorDecorator<TIteratorType>::GetImageIterator() const
{
  return m_itImage;
}

// Compute begin iterator position taking into account the mask
template <typename TIteratorType>
void MaskedIteratorDecorator<TIteratorType>::ComputeMaskedBegin()
{
  // Start at the iterator pair actual begin
  m_itMask.GoToBegin();
  m_itImage.GoToEnd();

  // Advance to the first non-masked position, or the end
  while (m_itMask.Value() == 0 && !m_itMask.IsAtEnd() && !m_itImage.IsAtEnd())
  {
    ++m_itMask;
    ++m_itImage;
  }
  m_begin = m_itMask.GetIndex();
}

} // namespace otb

#endif
