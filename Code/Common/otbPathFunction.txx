/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P. Imbo
  Language  :   C++
  Date      :   22 mars 2006
  Version   :   
  Role      :   Evaluate a function of an image over a specific path
  $Id:$

=========================================================================*/
#ifndef _otbPathFunction_txx
#define _otbPathFunction_txx

#include "otbPathFunction.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputImage,class TInputPath, class TOutput>
PathFunction<TInputImage, TInputPath,TOutput>
::PathFunction()
{
  m_Image = NULL;
  m_StartIndex.Fill(0);
  m_EndIndex.Fill(0);
  m_StartContinuousIndex.Fill(0.0);
  m_EndContinuousIndex.Fill(0.0);
}


/**
 * Standard "PrintSelf" method
 */
template <class TInputImage,class TInputPath, class TOutput>
void
PathFunction<TInputImage,TInputPath, TOutput>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf( os, indent );
  os << indent << "Input: " << m_Image.GetPointer() << std::endl;
  os << indent << "StartIndex: " << m_StartIndex << std::endl;
  os << indent << "EndIndex: " << m_EndIndex << std::endl;
  os << indent << "StartContinuousIndex: " << m_StartContinuousIndex << std::endl;
  os << indent << "EndContinuousIndex: " << m_EndContinuousIndex << std::endl;
}


/**
 * Initialize by setting the input image
 */
template <class TInputImage,class TInputPath, class TOutput>
void
PathFunction<TInputImage, TInputPath, TOutput>
::SetInputImage( const InputImageType * ptr )
{
  // set the input image
  m_Image = ptr;

  if ( ptr )
    {
    typedef typename IndexType::IndexValueType IndexValueType;
    typename InputImageType::SizeType size = ptr->GetBufferedRegion().GetSize();
    m_StartIndex = ptr->GetBufferedRegion().GetIndex();

    for ( unsigned int j = 0; j < ImageDimension; j++ )
      {
      m_EndIndex[j] = m_StartIndex[j] + 
        static_cast<IndexValueType>( size[j] ) - 1;
      m_StartContinuousIndex[j] = static_cast<CoordRepType>( m_StartIndex[j] );
      m_EndContinuousIndex[j]   = static_cast<CoordRepType>( m_EndIndex[j] );
      }

    }
}



} // end namespace otb

#endif

