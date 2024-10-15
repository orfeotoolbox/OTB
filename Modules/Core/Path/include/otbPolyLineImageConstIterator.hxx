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

#ifndef otbPolyLineImageConstIterator_hxx
#define otbPolyLineImageConstIterator_hxx

#include "otbPolyLineImageConstIterator.h"
#include "otbMacro.h"

namespace otb
{
/**
 * Constructor
 */
template <class TImage, class TPath>
PolyLineImageConstIterator<TImage, TPath>::PolyLineImageConstIterator(const ImageType* imagePtr, const PathType* pathPtr)
  : m_InternalImageIterator(const_cast<ImageType*>(imagePtr), imagePtr->GetLargestPossibleRegion().GetIndex(), imagePtr->GetLargestPossibleRegion().GetIndex())
{
  m_Image                  = imagePtr;
  m_Path                   = pathPtr;
  m_InternalVertexIterator = m_Path->GetVertexList()->Begin();
  IndexType source, target;
  for (unsigned int i = 0; i < ImageType::ImageDimension; ++i)
  {
    source[i] = static_cast<unsigned int>(std::floor(m_InternalVertexIterator.Value()[i] + 0.5));
  }
  ++m_InternalVertexIterator;
  if (m_InternalVertexIterator != m_Path->GetVertexList()->End())
  {

    for (unsigned int i = 0; i < ImageType::ImageDimension; ++i)
    {
      target[i] = static_cast<unsigned int>(std::floor(m_InternalVertexIterator.Value()[i] + 0.5));
    }
  }
  else
  {
    target = source;
  }
  m_InternalImageIterator = InternalImageIteratorType(const_cast<ImageType*>(m_Image.GetPointer()), source, target);
}

template <class TImage, class TPath>
typename PolyLineImageConstIterator<TImage, TPath>::Self& PolyLineImageConstIterator<TImage, TPath>::operator=(const Self& it)
{
  m_Image                  = it.m_Image;
  m_Path                   = it.m_Path;
  m_InternalImageIterator  = it.m_InternalImageIterator;
  m_InternalVertexIterator = it.m_InternalVertexIterator;
}

template <class TImage, class TPath>
void PolyLineImageConstIterator<TImage, TPath>::GoToBegin(void)
{
  m_InternalVertexIterator = m_Path->GetVertexList()->Begin();
  IndexType source, target;
  for (unsigned int i = 0; i < ImageType::ImageDimension; ++i)
  {
    source[i] = static_cast<unsigned int>(std::floor(m_InternalVertexIterator.Value()[i] + 0.5));
  }
  ++m_InternalVertexIterator;
  if (m_InternalVertexIterator != m_Path->GetVertexList()->End())
  {

    for (unsigned int i = 0; i < ImageType::ImageDimension; ++i)
    {
      target[i] = static_cast<unsigned int>(std::floor(m_InternalVertexIterator.Value()[i] + 0.5));
    }
  }
  else
  {
    target = source;
  }
  m_InternalImageIterator = InternalImageIteratorType(const_cast<ImageType*>(m_Image.GetPointer()), source, target);
}

template <class TImage, class TPath>
void PolyLineImageConstIterator<TImage, TPath>::operator++()
{
  //  otbMsgDebugMacro(<<this->GetIndex());
  ++m_InternalImageIterator;
  if (m_InternalImageIterator.IsAtEnd())
  {
    if (m_InternalVertexIterator != m_Path->GetVertexList()->End())
    {
      IndexType source;
      for (unsigned int i = 0; i < ImageType::ImageDimension; ++i)
      {
        source[i] = static_cast<unsigned int>(std::floor(m_InternalVertexIterator.Value()[i] + 0.5));
      }
      // otbMsgDebugMacro(<<"Source: "<<source);
      ++m_InternalVertexIterator;
      if (m_InternalVertexIterator != m_Path->GetVertexList()->End())
      {
        IndexType target;
        for (unsigned int i = 0; i < ImageType::ImageDimension; ++i)
        {
          target[i] = static_cast<unsigned int>(std::floor(m_InternalVertexIterator.Value()[i] + 0.5));
        }
        // otbMsgDebugMacro(<<"Target: "<<target);
        m_InternalImageIterator = InternalImageIteratorType(const_cast<ImageType*>(m_Image.GetPointer()), source, target);
        ++m_InternalImageIterator;
      }
    }
  }
}

} // End namespace otb
#endif
