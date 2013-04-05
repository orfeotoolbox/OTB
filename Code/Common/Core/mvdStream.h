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
#include "mvdAlgorithm.h"
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

/**
 * \brief TODO.
 */
enum StreamTag
{
  STREAM_TAG_SIZE =0,
  STREAM_TAG_VECTOR,
  STREAM_TAG_VARIABLE_LENGTH_VECTOR,
  //
  STREAM_TAG_COUNT
};

/**
 * \brief TODO.
 */
extern
const char* STREAM_TAG_NAMES[ STREAM_TAG_COUNT ];

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
 * \brief Write a text tag followed by a white space to the stream.
 *
 * \param stream Stream to write into.
 * \param name Tag name to write.
 *
 * \return stream instance.
 */
inline
QTextStream&
WriteTag( QTextStream& stream, const QString& name );

/**
 * \brief Read a text tag followed by a whitespace from the stream and
 * check it against an expected value.
 *
 * \param stream Stream to read from.
 * \param tag Text tag which has been read.
 * \param expected The expected tag name. If non-empty, value read
 * from stream is checked agaisnt expected value; an
 * std::runtime_exception is thrown if both values doest not match.
 *
 * \return stream instance.
 */
inline
QTextStream&
ReadTag( QTextStream& stream, QString& tag, const QString& expected =QString() );

/*****************************************************************************/

/**
 * \brief Write an StreamTag followed by a whitespace separator into a
 * QTextStream.
 *
 * \param stream QTextStream into which to write data.
 * \param tag StreamTag data to write.
 *
 * \return stream instance.
 */
inline
QTextStream&
operator << ( QTextStream& stream,
	      StreamTag tag );

/**
 * \brief Read a StreamTag followed by a whitespace from a
 * QTextStream.
 *
 * \param stream QTextStream from which to read data.
 * \param tag StreamTag to data read.
 *
 * \return stream instance.
 */
inline
QTextStream&
operator >> ( QTextStream& stream,
	      StreamTag& tag );

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
inline
QTextStream&
WriteTag( QTextStream& stream, const QString& name )
{
  stream << name << " ";
  CheckStreamStatus( stream );

  return stream;
}

/*******************************************************************************/
inline
QTextStream&
ReadTag( QTextStream& stream, QString& name, const QString& expected )
{
  stream >> name;
  CheckStreamStatus( stream );

  if( !expected.isEmpty() &&
      expected.compare( name, Qt::CaseInsensitive )!=0 )
    {
    throw std::runtime_error(
      ToStdString(
	QCoreApplication::tr(
	  "mvd::Stream",
	  "Mismatching tag '%1'; expected '%2'."
	)
	.arg( name )
	.arg( expected )
      )
    );
    }

  stream.skipWhiteSpace();
  CheckStreamStatus( stream );

  return stream;
}

/*******************************************************************************/
inline
QTextStream&
operator << ( QTextStream& stream,
	      StreamTag tag )
{
  WriteTag( stream, STREAM_TAG_NAMES[ tag ] );

  return stream;
}

/*******************************************************************************/
inline
QTextStream&
operator >> ( QTextStream& stream,
	      StreamTag& tag )
{
  QString name;

  ReadTag( stream, name );

  bool found = false;

  for( int i=0; i<STREAM_TAG_COUNT && !found; ++i )
    {
    if( name.compare( STREAM_TAG_NAMES[ i ], Qt::CaseInsensitive )==0 )
      {
      found = true;
      tag = static_cast< StreamTag >( i );
      }
    }

  if( !found )
    {
    throw std::runtime_error(
      ToStdString(
	QCoreApplication::tr(
	  "mvd::Stream",
	  "Invalid tag name '%1'."
	)
	.arg( name )
      )
    );
    }

  return stream;
}

/*******************************************************************************/
template< typename T, typename Alloc >
inline
QTextStream&
operator << ( QTextStream& stream,
	      const std::vector< T, Alloc >& vector )
{
  typedef std::vector< T, Alloc > Vector;

  stream << STREAM_TAG_VECTOR << vector.size();
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

  QString type;
  ReadTag( stream, type, STREAM_TAG_NAMES[ STREAM_TAG_VECTOR ] );

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

  stream << STREAM_TAG_SIZE << Size::GetSizeDimension();
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

  QString type;
  ReadTag( stream, type, STREAM_TAG_NAMES[ STREAM_TAG_SIZE ] );

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
  stream << STREAM_TAG_VARIABLE_LENGTH_VECTOR << vector.Size();
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

  QString type;
  ReadTag( stream, type, STREAM_TAG_NAMES[ STREAM_TAG_VECTOR ] );

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
/*
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
*/

/*******************************************************************************/
/*
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
*/

/*******************************************************************************/
template< typename T, unsigned int N, typename FC >
inline
QTextStream&
operator << ( QTextStream& stream,
	      const itk::Statistics::Histogram< T, N, FC >& histogram )
{
  typedef typename itk::Statistics::Histogram< T, N, FC > Histogram;


  stream << "HISTOGRAM" << endl;
  CheckStreamStatus( stream );


  WriteTag( stream, "MEASUREMENT" );
  stream << histogram.GetSize() << endl;
  CheckStreamStatus( stream );

#if 0
  WriteTag( stream, "MINS" );
  stream << histogram.GetMins() << endl;
  CheckStreamStatus( stream );


  WriteTag( stream, "MAXS" );
  stream << histogram.GetMaxs() << endl;
  CheckStreamStatus( stream );
#endif


  typename Histogram::SizeType size( histogram.GetSize() );

  for( unsigned int dim=0; dim<N; ++dim )
    {
    for( unsigned long bin=0; bin<size[ dim ]; ++bin )
      {
      stream
	<< dim << " "
	<< bin << " "
	<< histogram.GetMins()[ dim ][ bin ] << " "
	<< histogram.GetMaxs()[ dim ][ bin ] << " "
	<< histogram.GetFrequency( bin, dim ) << endl;

      CheckStreamStatus( stream );
      }
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
  typedef typename itk::Statistics::Histogram< T, N, FC > Histogram;

  QString string;

  //
  // Histogram section.
  ReadTag( stream, string, "HISTOGRAM" );

  //
  // Measurement-size section.
  ReadTag( stream, string, "MEASUREMENT" );
  typename Histogram::SizeType size;
  stream >> size;

  histogram.Initialize( size );

#if 0
  //
  // Bin mins.
  ReadTag( stream, string, "MINS" );
  typename Histogram::BinMinContainerType mins;
  stream >> mins;

  int dim = -1;
  int bin = -1;

  for( dim=0;
       dim<mins.size();
       ++dim )
    {
    for( bin=0;
	 bin<mins[ dim ].size();
	 ++bin )
      {
      histogram.SetBinMin( dim, bin, mins[ dim ][ bin ] );
      }
    }

  //
  // Bin maxes.
  ReadTag( stream, string, "MAXS" );
  typename Histogram::BinMaxContainerType maxs;
  stream >> maxs;

  for( dim=0;
       dim<maxs.size();
       ++dim )
    {
    for( bin=0;
	 bin<maxs[ dim ].size();
	 ++bin )
      {
      histogram.SetBinMax( dim, bin, maxs[ dim ][ bin ] );
      }
    }
#endif

  //
  // Bins.
  for( unsigned int dim=0; dim<N; ++dim )
    {
    for( unsigned long bin=0; bin<size[ dim ]; ++bin )
      {
      // Dimension.
      unsigned int d = 0;

      stream >> d;
      CheckStreamStatus( stream );
      if( d!=dim )
	{
	throw std::runtime_error(
	  ToStdString(
	    QCoreApplication::tr(
	      "mvd::Stream",
	      "Mismatching dimension '%1'; expected '%1'."
	    )
	    .arg( d )
	    .arg( dim )
	  )
	);
	}

      stream.skipWhiteSpace();
      CheckStreamStatus( stream );

      // Bin.
      unsigned long b = 0;

      stream >> b;
      CheckStreamStatus( stream );
      if( b!=bin )
	{
	throw std::runtime_error(
	  ToStdString(
	    QCoreApplication::tr(
	      "mvd::Stream",
	      "Mismatching bin '%1'; expected '%2'."
	    )
	    .arg( b )
	    .arg( bin )
	  )
	);
	}

      stream.skipWhiteSpace();
      CheckStreamStatus( stream );

      // Min.
      typename Histogram::BinMinContainerType::value_type::value_type min;

      stream >> min;
      CheckStreamStatus( min );

      histogram.SetBinMin( dim, bin, min );

      // Max.
      typename Histogram::BinMaxContainerType::value_type::value_type max;

      stream >> max;
      CheckStreamStatus( max );

      histogram.SetBinMax( dim, bin, max );

      // Frequency.
      typename Histogram::FequencyType freq( 0 );

      stream >> freq;
      CheckStreamStatus( stream );

      histogram.SetFrequency( b, d, freq );
      }
    }

  return stream;
}

} // end namespace 'mvd'

#endif // __mvdStream_h
