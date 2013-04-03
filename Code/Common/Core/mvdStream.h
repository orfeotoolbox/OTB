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
 * \brief Write an itk::VariableLengthVector< T > into a QDataStream.
 *
 * \param stream QDataStream into which to write data.
 * \param vector itk::VariableLengthVector< T > data to write.
 *
 * \return stream instance.
 */
template< typename T >
inline
QDataStream&
operator << ( QDataStream& stream,
	      const itk::VariableLengthVector< T >& vector );

/**
 * \brief Read an itk::VariableLengthVector< T > from a QDataStream.
 *
 * \param stream QDataStream from which to read data.
 * \param vector itk::VariableLengthVector< T > container to read data into.
 *
 * \return stream instance.
 */
template< typename T >
inline
QDataStream&
operator >> ( QDataStream& stream,
	      itk::VariableLengthVector< T >& vector );

/*****************************************************************************/

/**
 * \brief Write an itk::statistics::Histogram< T, N, FX > into a QTextStream.
 *
 * \param stream QTextStream into which to write data.
 * \param vector itk::statistics::Histogram< T, N, FC > data to write.
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
 * \param vector itk::statistics::Histogram< T, N, FC > container to
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
template< typename T >
inline
QTextStream&
operator << ( QTextStream& stream,
	      const itk::VariableLengthVector< T >& vector )
{
  typedef typename itk::VariableLengthVector< T > Vector;

  // Write number of elements.
  stream << vector.Size();
  if( stream.status()!=QTextStream::Ok )
    throw SystemError();

  // Write each element prefixed by a delimitting whitespace.
  for( typename Vector::ElementIdentifier i=0;
       i<vector.Size();
       ++i )
    {
    stream << " " << vector[ i ];
    if( stream.status()!=QTextStream::Ok )
      throw SystemError();
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
  if( stream.status()!=QTextStream::Ok )
    throw SystemError();

  // Allocates vector elements.
  vector.SetSize( size );

  // Read each element prefixed by a delimitting whitespace.
  for( typename Vector::ElementIdentifier i=0;
       i<vector.Size();
       ++i )
    {
    stream.skipWhiteSpace();
    if( stream.status()!=QTextStream::Ok )
      throw SystemError();
    
    stream >> vector[ i ];
    if( stream.status()!=QTextStream::Ok )
      throw SystemError();
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
  if( stream.status()!=QDataStream::Ok )
    throw SystemError();

  // Write each element prefixed by a delimitting whitespace.
  for( typename Vector::ElementIdentifier i=0;
       i<vector.Size();
       ++i )
    {
    stream << vector[ i ];
    if( stream.status()!=QDataStream::Ok )
      throw SystemError();
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
  if( stream.status()!=QDataStream::Ok )
    throw SystemError();

  // Allocates vector elements.
  vector.SetSize( size );

  // Read each element prefixed by a delimitting whitespace.
  for( typename Vector::ElementIdentifier i=0;
       i<vector.Size();
       ++i )
    {
    stream >> vector[ i ];
    if( stream.status()!=QDataStream::Ok )
      throw SystemError();
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
