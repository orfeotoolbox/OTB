/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbDataNodeImageFunction_hxx
#define otbDataNodeImageFunction_hxx

#include "otbDataNodeImageFunction.h"

namespace otb
{

/**
 * Constructor
 */
template <class TImage, class TOutput, class TCoordRep, class TPrecision>
DataNodeImageFunction<TImage, TOutput, TCoordRep, TPrecision>
::DataNodeImageFunction()
{
  m_Image = nullptr;
  m_StartIndex.Fill(0);
  m_EndIndex.Fill(0);
  m_StartContinuousIndex.Fill(0.0f);
  m_EndContinuousIndex.Fill(0.0f);
}


/**
 * Standard "PrintSelf" method
 */
template <class TImage, class TOutput, class TCoordRep, class TPrecision>
void
DataNodeImageFunction<TImage, TOutput, TCoordRep, TPrecision>
::PrintSelf(
  std::ostream& os,
  itk::Indent indent) const
{
  Superclass::PrintSelf( os, indent );
  os << indent << "InputImage: " << m_Image.GetPointer() << std::endl;
  os << indent << "StartIndex: " << m_StartIndex << std::endl;
  os << indent << "EndIndex: " << m_EndIndex << std::endl;
  os << indent << "StartContinuousIndex: " << m_StartContinuousIndex << std::endl;
  os << indent << "EndContinuousIndex: " << m_EndContinuousIndex << std::endl;
}


/**
 * Initialize by setting the input image
 */
template <class TImage, class TOutput, class TCoordRep, class TPrecision>
void
DataNodeImageFunction<TImage, TOutput, TCoordRep, TPrecision>
::SetInputImage(
  const InputImageType * ptr )
{
  // set the input image
  m_Image = ptr;

  if ( ptr )
    {
    typename InputImageType::SizeType size = ptr->GetBufferedRegion().GetSize();
    m_StartIndex = ptr->GetBufferedRegion().GetIndex();

    for ( unsigned int j = 0; j < ImageDimension; ++j )
      {
      m_EndIndex[j] = m_StartIndex[j] + static_cast<IndexValueType>( size[j] ) - 1;
      m_StartContinuousIndex[j] = static_cast<CoordRepType>( m_StartIndex[j] - 0.5 );
      m_EndContinuousIndex[j]   = static_cast<CoordRepType>( m_EndIndex[j] + 0.5 );

      //Comment these instructions after migration to ITKv4 where there is a correct
      //usage of centered-pixel coordinates
      //(ITK_USE_CENTERED_PIXEL_COORDINATES_CONSISTENTLY always ON)
      //m_StartContinuousIndex[j] = static_cast<CoordRepType>( m_StartIndex[j] );
      //m_EndContinuousIndex[j]   = static_cast<CoordRepType>( m_EndIndex[j] );
      }
    }
}


} // end namespace itk

#endif
