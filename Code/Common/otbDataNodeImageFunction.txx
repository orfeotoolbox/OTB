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
#ifndef __otbDataNodeImageFunction_txx
#define __otbDataNodeImageFunction_txx

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
  m_Image = NULL;
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
