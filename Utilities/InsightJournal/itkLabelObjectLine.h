/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkLabelObject.h,v $
  Language:  C++
  Date:      $Date: 2005/01/21 20:13:31 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkLabelObjectLine_h
#define __itkLabelObjectLine_h

namespace itk
{

/**
 * LabelObjectLine is the line object used in the LabelObject class
 * to store the line which are part of the object.
 * A line is formed of and index and a length in the dimension 0.
 * It is used in a run-length encoding
 */
template < unsigned int VImageDimension >
class LabelObjectLine
{
public:
  itkStaticConstMacro(ImageDimension, unsigned int, VImageDimension);

  typedef Index< ImageDimension > IndexType;
  typedef unsigned long LengthType;

  LabelObjectLine() {};

  LabelObjectLine( const IndexType & idx, const LengthType & length )
    {
    this->SetIndex( idx );
    this->SetLength( length );
    }

  void SetIndex( const IndexType & idx )
    {
    m_Index = idx;
    }

  IndexType & GetIndex()
    {
    return m_Index;
    }

  const IndexType & GetIndex() const
    {
    return m_Index;
    }

  void SetLength( const LengthType length )
    {
    m_Length = length;
    }

  LengthType & GetLength()
    {
    return m_Length;
    }

  const LengthType & GetLength() const
    {
    return m_Length;
    }

  bool HasIndex( const IndexType idx ) const
    {
    // are we talking about the right line ?
    for( int i=1; i<ImageDimension; i++ )
      {
      if( m_Index[i] != idx[i] )
        {
        return false;
        }
      }
    return ( idx[0] >= m_Index[0] && idx[0] < m_Index[0] + (long)m_Length );
    }

  bool IsNextIndex( const IndexType & idx ) const
    {
    // are we talking about the right line ?
    for( int i=1; i<ImageDimension; i++ )
      {
      if( m_Index[i] != idx[i] )
        {
        return false;
        }
      }
    return idx[0] == m_Index[0] + (long)m_Length;
    }

private:
  IndexType m_Index;
  LengthType m_Length;
};

} // end namespace itk

#endif
