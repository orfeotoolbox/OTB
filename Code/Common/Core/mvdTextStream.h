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
  STREAM_TAG_ARRAY = 0,
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
 * \param enqueueWS Enqueue a whitespace following the tag name.
 *
 * \return stream instance.
 */
inline
QTextStream&
WriteStreamTag( QTextStream& stream,
		const QString& name,
		bool enqueueWS =true );

/**
 * \brief Read a text tag followed by a whitespace from the stream and
 * check it against an expected value.
 *
 * \param stream Stream to read from.
 * \param tag Text tag which has been read.
 * \param expected The expected tag name. If non-empty, value read
 * from stream is checked agaisnt expected value; an
 * std::runtime_exception is thrown if both values doest not match.
 * \param skipWS Skip whitespace following tag name.
 *
 * \return stream instance.
 */
inline
QTextStream&
ReadStreamTag( QTextStream& stream,
	       QString& tag,
	       const QString& expected =QString(),
	       bool skipWS =true );

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
 * \brief Write an std::vector< T, std::allocator< T > > into a QTextStream.
 *
 * \param stream QTextStream into which to write data.
 * \param vector std::vector< T, std::allocator< T > > data to write.
 *
 * \return stream instance.
 */
template< typename T >
inline
QTextStream&
operator << ( QTextStream& stream,
	      const std::vector< T, std::allocator< T > >& vector );

/**
 * \brief
 */
template<>
inline
QTextStream&
operator << < double >( QTextStream& stream,
			const std::vector< double >& vector );

/**
 * \brief
 */
template<>
inline
QTextStream&
operator << < float >( QTextStream& stream,
		       const std::vector< float >& vector );

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
 * \brief Write an itk::Array< T > into a QTextStream.
 *
 * \param stream QTextStream into which to write data.
 * \param array itk::Array< T > data to write.
 *
 * \return stream instance.
 */
template< typename T >
inline
QTextStream&
operator << ( QTextStream& stream,
	      const itk::Array< T >& array );

/**
 * \brief Read an itk::Array< T > from a QTextStream.
 *
 * \param stream QTextStream from which to read data.
 * \param array itk::Array< T > container to read data into.
 *
 * \return stream instance.
 */
template< typename T >
inline
QTextStream&
operator >> ( QTextStream& stream,
	      itk::Array< T >& array );

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
 */
template<>
inline
QTextStream&
operator << ( QTextStream& stream,
	      const itk::VariableLengthVector< float >& vector );

/**
 */
template<>
inline
QTextStream&
operator << ( QTextStream& stream,
	      const itk::VariableLengthVector< double >& vector );

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
 * \brief Write an itk::statistics::Histogram< T, FC > into a QTextStream.
 *
 * \param stream QTextStream into which to write data.
 * \param histogram itk::statistics::Histogram< T, FC > data to write.
 *
 * \return stream instance.
 */
template< typename T, typename FC >
inline
QTextStream&
operator << ( QTextStream& stream,
	      const itk::Statistics::Histogram< T, FC >& histogram );

/**
 * \brief Read an itk::statistics::Histogram< T, FC > from a QTextStream.
 *
 * \param stream QTextStream from which to read data.
 * \param histogram itk::statistics::Histogram< T, FC > container to
 * read data into.
 *
 * \return stream instance.
 */
template< typename T, typename FC >
inline
QTextStream&
operator >> ( QTextStream& stream,
	      itk::Statistics::Histogram< T, FC >& histogram );

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
QTextStream&
WriteStreamTag( QTextStream& stream,
		const QString& name,
		bool queueWhiteSpace )
{
  stream << name;

  if( queueWhiteSpace )
    stream << " ";

  CheckStreamStatus( stream );

  return stream;
}

/*******************************************************************************/
inline
QTextStream&
ReadStreamTag( QTextStream& stream,
	       QString& name,
	       const QString& expected,
	       bool skipWhiteSpace )
{
  stream >> name;

  if( skipWhiteSpace )
    stream >> ws;

  CheckStreamStatus( stream );

  if( !expected.isEmpty() &&
      expected.compare( name, Qt::CaseInsensitive )!=0 )
    {
    throw std::runtime_error(
      ToStdString(
	QCoreApplication::translate(
	  "mvd::TextStream",
	  "Read tag '%1' does not match expected tag '%2'."
	)
	.arg( name )
	.arg( expected )
      )
    );
    }

  return stream;
}

/*******************************************************************************/
inline
QTextStream&
operator << ( QTextStream& stream,
	      StreamTag tag )
{
  WriteStreamTag( stream, STREAM_TAG_NAMES[ tag ] );

  return stream;
}

/*******************************************************************************/
inline
QTextStream&
operator >> ( QTextStream& stream,
	      StreamTag& tag )
{
  QString name;

  ReadStreamTag( stream, name );

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
	QCoreApplication::translate(
	  "mvd::TextStream",
	  "Invalid tag name '%1'."
	)
	.arg( name )
      )
    );
    }

  return stream;
}

/*******************************************************************************/
template< typename T >
inline
QTextStream&
operator << ( QTextStream& stream,
	      const std::vector< T, std::allocator< T > >& vector )
{
  typedef std::vector< T, std::allocator< T > > Vector;

  stream << STREAM_TAG_VECTOR << vector.size();

  for( typename Vector::const_iterator it( vector.begin() );
       it!=vector.end();
       ++it )
    {
    stream << " " << *it;
    }

  return stream;
}

/*******************************************************************************/
template<>
inline
QTextStream&
operator << < float >( QTextStream& stream,
		       const std::vector< float >& vector )
{
  typedef std::vector< float, std::allocator< float > > Vector;

  stream << STREAM_TAG_VECTOR << vector.size();

  for( Vector::const_iterator it( vector.begin() );
       it!=vector.end();
       ++it )
    {
    stream << " " << ToQString( *it );
    }

  return stream;
}

/*******************************************************************************/
template<>
inline
QTextStream&
operator << < double >( QTextStream& stream,
			const std::vector< double >& vector )
{
  typedef std::vector< double, std::allocator< double > > Vector;

  stream << STREAM_TAG_VECTOR << vector.size();

  for( Vector::const_iterator it( vector.begin() );
       it!=vector.end();
       ++it )
    {
    stream << " " << ToQString( *it );
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
  ReadStreamTag( stream, type, STREAM_TAG_NAMES[ STREAM_TAG_VECTOR ] );

  typename Vector::size_type size = 0;

  stream >> size;

  vector.resize( size );

  for( typename Vector::iterator it( vector.begin() );
       it!=vector.end();
       ++it )
    {
    stream >> ws >> *it;
    }

  return stream;
}

/*****************************************************************************/
template< typename T >
inline
QTextStream&
operator << ( QTextStream& stream,
	      const itk::Array< T >& array )
{

  stream << STREAM_TAG_ARRAY << array.GetSize();
  CheckStreamStatus( stream );

  for( CountType i=0; i<array.GetSize(); ++i )
    {
    stream << " " << array[ i ];
    CheckStreamStatus( stream );
    }

  return stream;
}

/*******************************************************************************/
template< typename T >
inline
QTextStream&
operator >> ( QTextStream& stream,
	      itk::Array< T >& array )
{

  CountType dimension = 0;

  QString type;
  ReadStreamTag( stream, type, STREAM_TAG_NAMES[ STREAM_TAG_ARRAY ] );

  stream >> dimension;

  if( dimension!=array.GetSize() )
    {
    throw std::length_error(
      ToStdString(
	QCoreApplication::translate(
	  "mvd::TextStream",
	  "Array dimension (%1) does not match "
	  "expected dimension (%2)."
	)
	.arg( dimension )
	.arg( array.GetSize() )
      )
    );
    }

  for( CountType i=0; i<array.GetSize(); ++i )
    {
    stream >> ws >> array[ i ];
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
template<>
inline
QTextStream&
operator << ( QTextStream& stream,
	      const itk::VariableLengthVector< double >& vector )
{
  typedef itk::VariableLengthVector< double > Vector;

  // Write number of elements.
  stream << STREAM_TAG_VARIABLE_LENGTH_VECTOR << vector.Size();
  CheckStreamStatus( stream );

  // Write each element prefixed by a delimitting whitespace.
  for( Vector::ElementIdentifier i=0;
       i<vector.Size();
       ++i )
    {
    stream << " " << ToQString( vector[ i ] );
    CheckStreamStatus( stream );
    }

  return stream;
}

/*******************************************************************************/
template<>
inline
QTextStream&
operator << ( QTextStream& stream,
	      const itk::VariableLengthVector< float >& vector )
{
  typedef itk::VariableLengthVector< float > Vector;

  // Write number of elements.
  stream << STREAM_TAG_VARIABLE_LENGTH_VECTOR << vector.Size();
  CheckStreamStatus( stream );

  // Write each element prefixed by a delimitting whitespace.
  for( Vector::ElementIdentifier i=0;
       i<vector.Size();
       ++i )
    {
    stream << " " << ToQString( vector[ i ] );
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
  ReadStreamTag( stream, type, STREAM_TAG_NAMES[ STREAM_TAG_VARIABLE_LENGTH_VECTOR ] );

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
    stream >> ws >> vector[ i ];
    CheckStreamStatus( stream );
    }

  return stream;
}

/*******************************************************************************/
template< typename T, typename FC >
inline
QTextStream&
operator << ( QTextStream& stream,
	      const itk::Statistics::Histogram< T, FC >& histogram )
{
  typedef typename itk::Statistics::Histogram< T, FC > Histogram;

  WriteStreamTag( stream, "HISTOGRAM", false );
  stream << endl;

  WriteStreamTag( stream, "MEASUREMENT" );
  stream << histogram.GetSize() << endl;

#if 1
  WriteStreamTag( stream, "MINS" );
  stream << histogram.GetMins() << endl;

  WriteStreamTag( stream, "MAXS" );
  stream << histogram.GetMaxs() << endl;
#endif


#if 0
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

#else
  CountType size = histogram.Size();

  WriteStreamTag( stream, "SAMPLES" );
  stream << size << endl;
  CheckStreamStatus( stream );

  CountType count = 0;

  for( typename Histogram::ConstIterator it( histogram.Begin() );
       it!=histogram.End();
       ++it, ++count )
    {
    stream
      << it.GetInstanceIdentifier() << " "
      << ToQString( it.GetFrequency() ) << endl;

    CheckStreamStatus( stream );
    }

  WriteStreamTag( stream, "COUNT" );
  stream << count << endl;
  CheckStreamStatus( stream );

  if( count!=size )
    {
    throw std::runtime_error(
      ToStdString(
	QCoreApplication::translate(
	  "mvd::TextStream",
	  "Histogram sample count (%1) does not match "
	  "written sample count (%2)."
	)
	.arg( size )
	.arg( count )
      )
    );
    }
#endif

  return stream;
}

/*******************************************************************************/
template< typename T, typename FC >
inline
QTextStream&
operator >> ( QTextStream& stream,
	      itk::Statistics::Histogram< T, FC >& histogram )
{
  typedef itk::Statistics::Histogram< T, FC > Histogram;

  QString string;
  unsigned int N = histogram.GetMeasurementVectorSize();


  //
  // Histogram section.
  ReadStreamTag( stream, string, "HISTOGRAM" );

  //
  // Measurement-size section.
  ReadStreamTag( stream, string, "MEASUREMENT" );
  typename Histogram::SizeType size;
  size.SetSize(N);

  stream >> size;

  // Dimension of size is checked in operator >> ( QTextStream&,
  // itk::Size< T > ).

  histogram.Initialize( size );

#if 1
  //
  // Bin mins.
  ReadStreamTag( stream, string, "MINS" );
  typename Histogram::BinMinContainerType mins;
  stream >> mins;

  //
  // Bin maxes.
  ReadStreamTag( stream, string, "MAXS" );
  typename Histogram::BinMaxContainerType maxs;
  stream >> maxs;

  if( mins.size()!=N )
    {
    throw std::runtime_error(
      ToStdString(
	QCoreApplication::translate(
	  "mvd::TextStream",
	  "Histogram mins dimension (%1) does not match "
	  "expected dimension (%2)."
	)
	.arg( mins.size() )
	.arg( N )
      )
    );
    }

  if( maxs.size()!=N )
    {
    throw std::runtime_error(
      ToStdString(
	QCoreApplication::translate(
	  "mvd::TextStream",
	  "Histogram maxs dimension (%1) does not match "
	  "expected dimension (%2)."
	)
	.arg( maxs.size() )
	.arg( N )
      )
    );
    }

  //
  // Initialize histogram mins & maxs within single pass.
  for( typename Histogram::BinMaxContainerType::size_type dim=0;
       dim<maxs.size();
       ++dim )
    {
    for( typename Histogram::BinMaxContainerType::value_type::size_type bin=0;
	 bin<maxs[ dim ].size();
	 ++bin )
      {
      histogram.SetBinMin( dim, bin, mins[ dim ][ bin ] );
      histogram.SetBinMax( dim, bin, maxs[ dim ][ bin ] );
      }
    }
#endif

#if 0
  //
  // Bins.
  for( unsigned int dim=0; dim<N; ++dim )
    {
    for( unsigned long bin=0; bin<size[ dim ]; ++bin )
      {
      // Dimension.
      unsigned int d = 0;

      stream >> d >> ws;
      CheckStreamStatus( stream );
      if( d!=dim )
	{
	throw std::runtime_error(
	  ToStdString(
	    QCoreApplication::translate(
	      "mvd::TextStream",
	      "Histogram dimension (%1) does not match "
	      "expected dimension (%2)."
	    )
	    .arg( d )
	    .arg( dim )
	  )
	);
	}

      // Bin.
      unsigned long b = 0;

      stream >> b >> ws;
      CheckStreamStatus( stream );
      if( b!=bin )
	{
	throw std::runtime_error(
	  ToStdString(
	    QCoreApplication::translate(
	      "mvd::TextStream",
	      "Histogram bin count (%1) does not match "
	      "expected bin count (%2)."
	    )
	    .arg( b )
	    .arg( bin )
	  )
	);
	}

      // Min.
      typename Histogram::BinMinContainerType::value_type::value_type min;

      stream >> min >> ws;
      CheckStreamStatus( stream );

      histogram.SetBinMin( dim, bin, min );

      // Max.
      typename Histogram::BinMaxContainerType::value_type::value_type max;

      stream >> max >> ws;
      CheckStreamStatus( stream );

      histogram.SetBinMax( dim, bin, max );

      // Frequency.
      typename Histogram::AbsoluteFrequencyType freq( 0 );

      stream >> freq;
      CheckStreamStatus( stream );

      typename Histogram::MeasurementType measurement(
	histogram.GetMeasurement( b, d )
      );

      typename Histogram::IndexType index( histogram.GetIndex( measurement ) );

      histogram.SetFrequency( index, freq );
      }
    }

#else
  CountType samples = 0;

  ReadStreamTag( stream, string, "SAMPLES" );
  stream >> samples;
  CheckStreamStatus( stream );

  CountType space = histogram.Size();

  if( samples!=space )
    {
    throw std::runtime_error(
      ToStdString(
	QCoreApplication::translate(
	  "mvd::TextStream",
	  "Histogram sample count (%1) does not match "
	  "expected sample count (%2)."
	)
	.arg( samples )
	.arg( space )
      )
    );
    }

  CountType i = 0;
  for( ; i<samples; ++i )
    {
    typename Histogram::InstanceIdentifier id( 0 );
    typename Histogram::AbsoluteFrequencyType freq( 0 );

    stream >> id >> ws >> freq;
    CheckStreamStatus( stream );

    histogram.SetFrequency( id, freq );
    }

  CountType count = 0;

  ReadStreamTag( stream, string, "COUNT" );
  stream >> count;
  CheckStreamStatus( stream );

  if( count!=i )
    {
    throw std::runtime_error(
      ToStdString(
	QCoreApplication::translate(
	  "mvd::TextStream",
	  "Read sample count (histogram) (%1) does not match "
	  "written sample count (%2)."
	)
	.arg( i )
	.arg( count )
      )
    );
    }

#endif

  return stream;
}

} // end namespace 'mvd'

#endif // __mvdStream_h
