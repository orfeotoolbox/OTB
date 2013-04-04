/*=========================================================================

  Program:   Monteverdi2
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See Copyright.txt for details.

  Monteverdi2 is distributed under the CeCILL licence version 2. See
  Licence_CeCILL_V2-en.txt or
  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt for more details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __mvdStream_h
#define __mvdStream_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi2.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QtCore>

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)
#include "itkHistogram.h"
#include "itkSize.h"

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdSystemError.h"
#include "mvdTypes.h"


/*****************************************************************************/
/* PRE-DECLARATION SECTION                                                   */

//
// External classes pre-declaration.
namespace
{
}

namespace mvd
{
//
// Internal classes pre-declaration.
} // end of namespace 'mvd'.


/*****************************************************************************/
/* FUNCTIONS DECLARATION.                                                    */


/*****************************************************************************/

namespace mvd
{

/**
 * \brief Check status Qt stream and throw SystemError exception if
 * status is not Ok.
 */
inline
void
CheckStreamStatus( QTextStream& stream );

/**
 * \brief Check status Qt stream and throw SystemError exception if
 * status is not Ok.
 */
inline
void
CheckStreamStatus( QDataStream& stream );

/*****************************************************************************/

/**
 * \brief Write an std::vector< T, Alloc > into a QTextStream.
 *
 * \param stream QTextStream into which to write data.
 * \param vector std::vector< T, Alloc > data to write.
 *
 * \return stream instance.
 */
template< typename T, typename Alloc >
inline
QTextStream&
operator << ( QTextStream& stream,
	      const std::vector< T, Alloc >& vector );

/**
 * \brief Read an std::vector< T, Alloc > from a QTextStream.
 *
 * \param stream QTextStream from which to read data.
 * \param vector std::vector< T, Alloc > container to read data into.
 *
 * \return stream instance.
 */
template< typename T, typename Alloc >
inline
QTextStream&
operator >> ( QTextStream& stream,
	      std::vector< T, Alloc >& vector );

/*****************************************************************************/

/**
 * \brief Write an itk::Size< T > into a QTextStream.
 *
 * \param stream QTextStream into which to write data.
 * \param size itk::Size< T > data to write.
 *
 * \return stream instance.
 */
template< unsigned int N >
inline
QTextStream&
operator << ( QTextStream& stream,
	      const itk::Size< N >& size );

/**
 * \brief Read an itk::Size< T > from a QTextStream.
 *
 * \param stream QTextStream from which to read data.
 * \param size itk::Size< T > container to read data into.
 *
 * \return stream instance.
 */
template< unsigned int N >
inline
QTextStream&
operator >> ( QTextStream& stream,
	      itk::Size< N >& size );

/*****************************************************************************/

/**
 * \brief Write an itk::VariableLengthVector< T > into a QTextStream.
 *
 * \param stream QTextStream into which to write data.
 * \param vector itk::VariableLengthVector< T > data to write.
 *
 * \return stream instance.
 */
template< typename T >
inline
QTextStream&
operator << ( QTextStream& stream,
	      const itk::VariableLengthVector< T >& vector );

/**
 * \brief Read an itk::VariableLengthVector< T > from a QTextStream.
 *
 * \param stream QTextStream from which to read data.
 * \param vector itk::VariableLengthVector< T > container to read data into.
 *
 * \return stream instance.
 */
template< typename T >
inline
QTextStream&
operator >> ( QTextStream& stream,
	      itk::VariableLengthVector< T >& vector );

/*****************************************************************************/

/**
 * \brief Write an itk::statistics::Histogram< T, N, FX > into a QTextStream.
 *
 * \param stream QTextStream into which to write data.
 * \param histogram itk::statistics::Histogram< T, N, FC > data to write.
 *
 * \return stream instance.
 */
template< typename T, unsigned int N, typename FC >
inline
QTextStream&
operator << ( QTextStream& stream,
	      const itk::Statistics::Histogram< T, N, FC >& histogram );

/**
 * \brief Read an itk::statistics::Histogram< T, N, FC > from a QTextStream.
 *
 * \param stream QTextStream from which to read data.
 * \param histogram itk::statistics::Histogram< T, N, FC > container to
 * read data into.
 *
 * \return stream instance.
 */
template< typename T, unsigned int N, typename FC >
inline
QTextStream&
operator >> ( QTextStream& stream,
	      itk::Statistics::Histogram< T, N, FC >& histogram );

} // end namespace 'mvd'.


/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace mvd
{

/*******************************************************************************/
inline
void
CheckStreamStatus( QTextStream& stream )
{
  if( stream.status()!=QTextStream::Ok )
    throw SystemError();
}

/*******************************************************************************/
inline
void
CheckStreamStatus( QDataStream& stream )
{
  if( stream.status()!=QDataStream::Ok )
    throw SystemError();
}

/*******************************************************************************/
template< typename T, typename Alloc >
inline
QTextStream&
operator << ( QTextStream& stream,
	      const std::vector< T, Alloc >& vector )
{
  typedef std::vector< T, Alloc > Vector;

  stream << vector.size();
  CheckStreamStatus( stream );

  for( typename Vector::const_iterator it( vector.begin() );
       it!=vector.end();
       ++it )
    {
    stream << " " << *it;
    CheckStreamStatus( stream );
    }

  return stream;
}

/*******************************************************************************/
template< typename T, typename Alloc >
inline
QTextStream&
operator >> ( QTextStream& stream,
	      std::vector< T, Alloc >& vector )
{
  typedef std::vector< T, Alloc > Vector;

  typename Vector::size_type size = 0;

  stream >> size;
  CheckStreamStatus( stream );

  vector.resize( size );

  for( typename Vector::iterator it( vector.begin() );
       it!=vector.end();
       ++it )
    {
    stream.skipWhiteSpace();
    CheckStreamStatus( stream );

    stream >> *it;
    CheckStreamStatus( stream );
    }

  return stream;
}

/*****************************************************************************/
template< unsigned int N >
inline
QTextStream&
operator << ( QTextStream& stream,
	      const itk::Size< N >& size )
{
  typedef itk::Size< N > Size;

  stream << Size::GetSizeDimension();
  CheckStreamStatus( stream );

  for( CountType i=0; i<Size::GetSizeDimension(); ++i )
    {
    stream << " " << size[ i ];
    CheckStreamStatus( stream );
    }

  return stream;
}

/*******************************************************************************/
template< unsigned int N >
inline
QTextStream&
operator >> ( QTextStream& stream,
	      itk::Size< N >& size )
{
  typedef itk::Size< N > Size;

  CountType dimension = 0;

  stream >> dimension;
  CheckStreamStatus( stream );

  if( dimension!=Size::GetSizeDimension() )
    {
    throw std::length_error(
      ToStdString(
	QCoreApplication::tr(
	  "mvd::Stream",
	  "Input dimension '%1' does not match storage size dimension '%2'."
	)
	.arg( dimension )
	.arg( Size::GetSizeDimension() )
      )
    );
    }

  for( CountType i=0; i<Size::GetSizeDimension(); ++i )
    {
    stream.skipWhiteSpace();
    CheckStreamStatus( stream );

    stream >> size[ i ];
    CheckStreamStatus( stream );
    }

  return stream;
}

/*******************************************************************************/
template< typename T >
inline
QTextStream&
operator << ( QTextStream& stream,
	      const itk::VariableLengthVector< T >& vector )
{
  typedef typename itk::VariableLengthVector< T > Vector;

  // Write number of elements.
  stream << vector.Size();
  CheckStreamStatus( stream );

  // Write each element prefixed by a delimitting whitespace.
  for( typename Vector::ElementIdentifier i=0;
       i<vector.Size();
       ++i )
    {
    stream << " " << vector[ i ];
    CheckStreamStatus( stream );
    }

  return stream;
}

/*******************************************************************************/
template< typename T >
inline
QTextStream&
operator >> ( QTextStream& stream,
	      itk::VariableLengthVector< T >& vector )
{
  typedef typename itk::VariableLengthVector< T > Vector;

  // Read number of elements.
  typename Vector::ElementIdentifier size = 0;
  stream >> size;
  CheckStreamStatus( stream );

  // Allocates vector elements.
  vector.SetSize( size );

  // Read each element prefixed by a delimitting whitespace.
  for( typename Vector::ElementIdentifier i=0;
       i<vector.Size();
       ++i )
    {
    stream.skipWhiteSpace();
    CheckStreamStatus( stream );
    
    stream >> vector[ i ];
    CheckStreamStatus( stream );
    }

  return stream;
}

/*******************************************************************************/
template< typename T >
inline
QDataStream&
operator << ( QDataStream& stream,
	      const itk::VariableLengthVector< T >& vector )
{
  typedef typename itk::VariableLengthVector< T > Vector;

  // Write number of elements.
  stream << vector.Size();
  CheckStreamStatus( stream );

  // Write each element prefixed by a delimitting whitespace.
  for( typename Vector::ElementIdentifier i=0;
       i<vector.Size();
       ++i )
    {
    stream << vector[ i ];
    CheckStreamStatus( stream );
    }

  return stream;
}

/*******************************************************************************/
template< typename T >
inline
QDataStream&
operator >> ( QDataStream& stream,
	      itk::VariableLengthVector< T >& vector )
{
  typedef typename itk::VariableLengthVector< T > Vector;

  // Read number of elements.
  typename Vector::ElementIdentifier size = 0;
  stream >> size;
  CheckStreamStatus( stream );

  // Allocates vector elements.
  vector.SetSize( size );

  // Read each element prefixed by a delimitting whitespace.
  for( typename Vector::ElementIdentifier i=0;
       i<vector.Size();
       ++i )
    {
    stream >> vector[ i ];
    CheckStreamStatus( stream );
    }

  return stream;
}

/*******************************************************************************/
template< typename T, unsigned int N, typename FC >
inline
QTextStream&
operator << ( QTextStream& stream,
	      const itk::Statistics::Histogram< T, N, FC >& histogram )
{
  typedef typename itk::Statistics::Histogram< T, N, FC > Histogram;

  stream << histogram.GetSize();
  CheckStreamStatus( stream );

  stream << histogram.GetMins();
  CheckStreamStatus( stream );

  stream << histogram.GetMaxs();
  CheckStreamStatus( stream );

  for( typename Histogram::ConstIterator it( histogram.Begin() );
       it!=histogram.End();
       ++it )
    {
    }

  return stream;
}

/*******************************************************************************/
template< typename T, unsigned int N, typename FC >
inline
QTextStream&
operator >> ( QTextStream& stream,
	      itk::Statistics::Histogram< T, N, FC >& histogram )
{
  return stream;
}

} // end namespace 'mvd'

#endif // __mvdStream_h
