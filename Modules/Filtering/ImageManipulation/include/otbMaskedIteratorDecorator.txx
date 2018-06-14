/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbMaskedIteratorDecorator_hxx
#define otbMaskedIteratorDecorator_hxx

#include "otbMaskedIteratorDecorator.h"
#include "itkMacro.h"

namespace otb
{

template <typename TIteratorType, typename TMaskIteratorType>
MaskedIteratorDecorator<TIteratorType,TMaskIteratorType>
::MaskedIteratorDecorator( MaskType* mask, 
                           ImageType* image,
                          const RegionType& region)
{
  m_ItImage = TIteratorType(image,region);
  if (mask == nullptr)
    {
    m_UseMask = false;
    }
  else
    {
    if (image->GetLargestPossibleRegion() != mask->GetLargestPossibleRegion())
      {
      itkGenericExceptionMacro("Input image and mask have different largest regions : mask is discarded!");
      }
    else
      {
      m_UseMask = true;
      m_ItMask = TMaskIteratorType(mask,region);
      m_StartMask = TMaskIteratorType(mask,region);
      m_StartImage = TIteratorType(image,region);
      }
    }
}

template <typename TIteratorType, typename TMaskIteratorType>
typename MaskedIteratorDecorator<TIteratorType,TMaskIteratorType>::IndexType
MaskedIteratorDecorator<TIteratorType,TMaskIteratorType>
::GetIndex() const
{
  return m_ItImage.GetIndex();
}

template <typename TIteratorType, typename TMaskIteratorType>
void
MaskedIteratorDecorator<TIteratorType,TMaskIteratorType>
::GoToBegin()
{
  if (m_UseMask)
    {
    this->ComputeMaskedBegin();
    }
  else
    {
    m_ItImage.GoToBegin();
    }
}

template <typename TIteratorType, typename TMaskIteratorType>
void
MaskedIteratorDecorator<TIteratorType,TMaskIteratorType>
::GoToEnd()
{
  if (m_UseMask)
    {
    // make sure masked begin is computed as next calls might be operator--() 
    // and IsAtBegin() (for a reverse iteration)
    this->ComputeMaskedBegin();
    m_ItMask.GoToEnd();
    }
  m_ItImage.GoToEnd();
}

template <typename TIteratorType, typename TMaskIteratorType>
bool
MaskedIteratorDecorator<TIteratorType,TMaskIteratorType>
::IsAtBegin() const
{
  if (m_UseMask)
    {
    return m_ItMask == m_StartMask || m_ItImage == m_StartImage;
    }
  return m_ItImage.IsAtBegin();
}

template <typename TIteratorType, typename TMaskIteratorType>
bool
MaskedIteratorDecorator<TIteratorType,TMaskIteratorType>
::IsAtEnd() const
{
  if (m_UseMask)
    {
    return m_ItMask.IsAtEnd() || m_ItImage.IsAtEnd();
    }
  return m_ItImage.IsAtEnd();
}

// Wrap the underlying iterators to skip masked pixels
template <typename TIteratorType, typename TMaskIteratorType>
MaskedIteratorDecorator<TIteratorType,TMaskIteratorType>&
MaskedIteratorDecorator<TIteratorType,TMaskIteratorType>
::operator++()
{
  if (m_UseMask)
    {
    do
      {
      ++m_ItMask;
      ++m_ItImage;
      } while (m_ItMask.Value() == 0 && !this->IsAtEnd());
    }
  else
    {
    ++m_ItImage;
    }
  return *this;
}

// Wrap the underlying iterators to skip masked pixels
template <typename TIteratorType, typename TMaskIteratorType>
MaskedIteratorDecorator<TIteratorType,TMaskIteratorType>&
MaskedIteratorDecorator<TIteratorType,TMaskIteratorType>
::operator--()
{
  if (m_UseMask)
    {
    do
      {
      --m_ItMask;
      --m_ItImage;
      } while (m_ItMask.Value() == 0 && !this->IsAtBegin());
    }
  else
    {
    --m_ItImage;
    }
  return *this;
}

/*
template <typename TIteratorType, typename TMaskIteratorType>
void MaskedIteratorDecorator<TIteratorType,TMaskIteratorType>::Set(const PixelType& value) const
{
  m_ItImage.Set(value);
}
*/

template <typename TIteratorType, typename TMaskIteratorType>
const typename MaskedIteratorDecorator<TIteratorType,TMaskIteratorType>::PixelType&
MaskedIteratorDecorator<TIteratorType,TMaskIteratorType>
::Value(void) const
{
  return m_ItImage.Value();
}

template <typename TIteratorType, typename TMaskIteratorType>
TMaskIteratorType&
MaskedIteratorDecorator<TIteratorType,TMaskIteratorType>
::GetMaskIterator()
{
  return m_ItMask;
}

template <typename TIteratorType, typename TMaskIteratorType>
const TMaskIteratorType&
MaskedIteratorDecorator<TIteratorType,TMaskIteratorType>
::GetMaskIterator() const
{
  return m_ItMask;
}

template <typename TIteratorType, typename TMaskIteratorType>
TIteratorType&
MaskedIteratorDecorator<TIteratorType,TMaskIteratorType>
::GetImageIterator()
{
  return m_ItImage;
}

template <typename TIteratorType, typename TMaskIteratorType>
const TIteratorType&
MaskedIteratorDecorator<TIteratorType,TMaskIteratorType>
::GetImageIterator() const
{
  return m_ItImage;
}

template <typename TIteratorType, typename TMaskIteratorType>
const bool&
MaskedIteratorDecorator<TIteratorType,TMaskIteratorType>
::HasMask() const
{
  return m_UseMask;
}

// Compute begin iterator position taking into account the mask
template <typename TIteratorType, typename TMaskIteratorType>
void
MaskedIteratorDecorator<TIteratorType,TMaskIteratorType>
::ComputeMaskedBegin()
{
  // We must search for the first index where the image is not masked
  // Start searching at the beginning
  m_ItMask.GoToBegin();
  m_ItImage.GoToBegin();

  // Advance to the first non-masked position, or the end
  while (m_ItMask.Value() == 0 && !m_ItMask.IsAtEnd() && !m_ItImage.IsAtEnd())
  {
    ++m_ItMask;
    ++m_ItImage;
  }
  m_StartMask.SetIndex(m_ItMask.GetIndex());
  m_StartImage.SetIndex(m_ItImage.GetIndex());
}

} // namespace otb

#endif
