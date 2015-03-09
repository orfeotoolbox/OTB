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
#ifndef __otbPolyLineImageConstIterator_txx
#define __otbPolyLineImageConstIterator_txx

#include "otbPolyLineImageConstIterator.h"
#include "otbMacro.h"

namespace otb
{
/**
 * Constructor
 */
template <class TImage, class TPath>
PolyLineImageConstIterator<TImage, TPath>
::PolyLineImageConstIterator(const ImageType *imagePtr, const PathType * pathPtr)
  : m_InternalImageIterator(const_cast<ImageType *>(imagePtr),
                            imagePtr->GetLargestPossibleRegion().GetIndex(),
                            imagePtr->GetLargestPossibleRegion().GetIndex())
{
  m_Image = imagePtr;
  m_Path = pathPtr;
  m_InternalVertexIterator = m_Path->GetVertexList()->Begin();
  IndexType source, target;
  for (unsigned int i = 0; i < ImageType::ImageDimension; ++i)
    {
    source[i] = static_cast<unsigned int> (vcl_floor(m_InternalVertexIterator.Value()[i] + 0.5));
    }
  ++m_InternalVertexIterator;
  if (m_InternalVertexIterator != m_Path->GetVertexList()->End())
    {

    for (unsigned int i = 0; i < ImageType::ImageDimension; ++i)
      {
      target[i] = static_cast<unsigned int> (vcl_floor(m_InternalVertexIterator.Value()[i] + 0.5));
      }
    }
  else
    {
    target = source;
    }
  m_InternalImageIterator = InternalImageIteratorType(const_cast<ImageType *> (m_Image.GetPointer()), source, target);
}

template <class TImage, class TPath>
typename PolyLineImageConstIterator<TImage, TPath>
::Self&
PolyLineImageConstIterator<TImage, TPath>
::operator = (const Self &it)
  {
  m_Image = it.m_Image;
  m_Path = it.m_Path;
  m_InternalImageIterator = it.m_InternalImageIterator;
  m_InternalVertexIterator = it.m_InternalVertexIterator;
  }

template <class TImage, class TPath>
void
PolyLineImageConstIterator<TImage, TPath>
::GoToBegin(void)
{
  m_InternalVertexIterator = m_Path->GetVertexList()->Begin();
  IndexType source, target;
  for (unsigned int i = 0; i < ImageType::ImageDimension; ++i)
    {
    source[i] = static_cast<unsigned int> (vcl_floor(m_InternalVertexIterator.Value()[i] + 0.5));
    }
  ++m_InternalVertexIterator;
  if (m_InternalVertexIterator != m_Path->GetVertexList()->End())
    {

    for (unsigned int i = 0; i < ImageType::ImageDimension; ++i)
      {
      target[i] = static_cast<unsigned int> (vcl_floor(m_InternalVertexIterator.Value()[i] + 0.5));
      }
    }
  else
    {
    target = source;
    }
  m_InternalImageIterator = InternalImageIteratorType(const_cast<ImageType *> (m_Image.GetPointer()), source, target);
}

template <class TImage, class TPath>
void
PolyLineImageConstIterator<TImage, TPath>
::operator++ ()
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
        source[i] = static_cast<unsigned int> (vcl_floor(m_InternalVertexIterator.Value()[i] + 0.5));
        }
      // otbMsgDebugMacro(<<"Source: "<<source);
      ++m_InternalVertexIterator;
      if (m_InternalVertexIterator != m_Path->GetVertexList()->End())
        {
        IndexType target;
        for (unsigned int i = 0; i < ImageType::ImageDimension; ++i)
          {
          target[i] = static_cast<unsigned int> (vcl_floor(m_InternalVertexIterator.Value()[i] + 0.5));
          }
        // otbMsgDebugMacro(<<"Target: "<<target);
        m_InternalImageIterator = InternalImageIteratorType(const_cast<ImageType *> (m_Image.GetPointer()), source,
                                                            target);
        ++m_InternalImageIterator;
        }
      }
    }
  }

} // End namespace otb
#endif
