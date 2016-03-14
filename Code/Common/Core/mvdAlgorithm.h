/*=========================================================================

  Program:   Monteverdi
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See Copyright.txt for details.

  Monteverdi is distributed under the CeCILL licence version 2. See
  Licence_CeCILL_V2-en.txt or
  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt for more details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __mvdAlgorithm_h
#define __mvdAlgorithm_h

//
// Configuration include.
//// Included at first position before any other ones.
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829  //tag=QT4-boost-compatibility
#include "ConfigureMonteverdi.h"
#endif //tag=QT4-boost-compatibility


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QtCore>

//
// System includes (sorted by alphabetic order)
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829  //tag=QT4-boost-compatibility
#include <string>
#endif //tag=QT4-boost-compatibility

//
// ITK includes (sorted by alphabetic order)
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829  //tag=QT4-boost-compatibility
#include "itkFixedArray.h"
#include "itkExceptionObject.h"
#include "itkVariableLengthVector.h"
#endif //tag=QT4-boost-compatibility

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829  //tag=QT4-boost-compatibility
#include "mvdTypes.h"
#endif //tag=QT4-boost-compatibility


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

namespace otb
{

/**
 * Convert an itk::VariableLengthVector< T2 > into a
 * itk::FixedArray< T1, N >.
 *
 * Elements of type T1 are (safely) statically casted into T2.
 *
 * An itk::RangeError exception instance is thrown if lengths/sizes of
 * both the containers are not equal.
 *
 * N.B.: Caller must ensure to that size N of itk::FixedArray< T1, N >
 * to match the variable size of the itk::VariableLengthVector< T1 >.
 */
template< typename T2, unsigned int N, typename T1 >
inline
itk::FixedArray< T2, N >
ToFixedArray( const itk::VariableLengthVector< T1 >& v );

/**
 * Convert an itk::VariableLengthVector< T2 > into a
 * itk::FixedArray< T1, N >.
 *
 * Elements of type T1 are (safely) statically casted into T2.
 *
 * An itk::RangeError exception instance is thrown if lengths/sizes of
 * both the containers are not equal.
 *
 * N.B.: Caller must ensure to that size N of itk::FixedArray< T1, N >
 * to match the variable size of the itk::VariableLengthVector< T1 >.
 */
template< typename T2, unsigned int N, typename T1 >
inline
itk::FixedArray< T2, N >&
ToFixedArray( itk::FixedArray< T2, N >& a,
	      const itk::VariableLengthVector< T1 >& v );

/**
 * Convert an itk::FixedArray< T2, N > into a
 * itk::VariableLengthVector< T1 >.
 *
 * Elements of type T1 are (safely) statically casted into T2.
 *
 * An itk::RangeError exception instance is thrown if lengths/sizes of
 * both the containers are not equal.
 */
template< typename T2, typename T1, unsigned int N >
inline
itk::VariableLengthVector< T2 >
ToVariableLengthVector( const itk::FixedArray< T1, N >& a );

/**
 * Convert an itk::FixedArray< T2, N > into a
 * itk::VariableLengthVector< T1 >.
 *
 * Elements of type T1 are (safely) statically casted into T2.
 *
 * An itk::RangeError exception instance is thrown if lengths/sizes of
 * both the containers are not equal.
 */
template< typename T2, typename T1, unsigned int N >
inline
itk::VariableLengthVector< T2 >&
ToVariableLengthVector( itk::FixedArray< T1, N >& a,
			const itk::VariableLengthVector< T1 >& v );
} // end namespace 'otb'

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
operator >> ( QTextStream& stream,
	      itk::VariableLengthVector< T >& vector );
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
operator >> ( QDataStream& stream,
	      itk::VariableLengthVector< T >& vector );

/**
 * \brief Convert a StringVector object to a QStringList object.
 *
 * Because Qt uses a shallow mecanism to copy containers, the returned
 * QStringList is shallow-copied.
 */
inline
QStringList
ToQStringList( const StringVector& sv );

/**
 * \brief Append the content of a StringVector object to the content
 * of a QStringList object.
 *
 * Because Qt uses a shallow mecanism to copy containers, the returned
 * QStringList is shallow-copied.
 */
inline
QStringList&
AppendToQStringList( QStringList& qsl,
		     const StringVector& sv );

/**
 * \brief Convert and copy an STL std::string to a QString.
 *
 * The 8-bit data is converted to Unicode using the
 * QString::fromAscii() method.
 *
 * \param str The 8-bit STL string to convert.
 *
 * \return The Unicode converted QString.
 */

inline
QString
FromStdString( const std::string& str );

/**
 */
inline
std::string
ToLocalStdString( const QString & );

/**
 */
inline
const char *
ToLocalString( const QString & );

/**
 * \brief Convert and copy a QString to a STL std::string.
 *
 * The Unicode data is converted to 8-bit using the QString::toAscii()
 * method.
 *
 * \param str The Unicode string to convert.
 *
 * \return The 8-bit converted STL std::string.
 */
inline
std::string
ToStdString( const QString& str );

/**
 */
inline
const char*
ToString( const QString& str );


/**
 * \return a std::string form various types.
 */
inline
std::string
ToStdString( unsigned int val );

inline
std::string
ToStdString( const SpacingType & spacing);

inline
std::string
ToStdString( const SizeType & size);

inline
std::string
ToStdString( const PointType & point);

template< typename T>
std::string
ToStdString( const std::vector<T> & vec);

/**
 */
template< typename T >
inline
QString
ToQString( const T& val );

/**
 */
template<>
inline
QString
ToQString< float >( const float& );

/**
 */
template<>
inline
QString
ToQString< double >( const double& );

/**
 */
inline
QString
ToHumanReadableSize( qint64 fize, bool isInBits =true );

} // end namespace 'mvd'.

/*****************************************************************************/

namespace mvd
{
/**
 * \brief Test condition on all elements in range.
 *
 * \param first First element of range to test.
 * \param last Upper (external) boundary of range to test.
 * \param pred Predicate testing element in range.
 *
 * \return true if predicate is true on all elements in the range
 * [first, last[ or if range is empty.
 */
template< typename TInputIterator, typename TUnaryPredicate >
inline
bool
AllOf( TInputIterator first, TInputIterator last, TUnaryPredicate pred );

/**
 * \brief Test if any of element in range fulfills condition.
 *
 * \param first First element of range to test.
 * \param last Upper (external) boundary of range to test.
 * \param pred Predicate testing element in range.
 *
 * \return true if predicated is true for, at least, one element in range
 * [first, last[. If range is empty, this function returns false.
 */
template< typename TInputIterator, typename TUnaryPredicate >
inline
bool
AnyOf( TInputIterator first, TInputIterator last, TUnaryPredicate pred );

/**
 * \brief Test if no element in range fulfills condition.
 *
 * \param first First element of range to test.
 * \param last Upper (external) boundary of range to test.
 * \param pred Predicate testing element in range.
 *
 * \return true if predicated is false for all elements in range [first, last[. If range is empty, this function returns true.
 */
template< typename TInputIterator, typename TUnaryPredicate >
inline
bool
NoneOf( TInputIterator first, TInputIterator last, TUnaryPredicate pred );

} // end namespace 'mvd'.

/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace otb
{
/*******************************************************************************/
template< typename T2, unsigned int N, typename T1 >
inline
itk::FixedArray< T2, N >
ToFixedArray( const itk::VariableLengthVector< T1 >& v )
{
  assert( v.Size()==N );

  throw itk::RangeError( __FILE__, __LINE__ );

  itk::FixedArray< T2, N > a;

  for( unsigned int i=0; i<N; ++i )
    a[ i ] = static_cast< T2 >( v[ i ] );

  return a;
}

/*******************************************************************************/
template< typename T2, unsigned int N, typename T1 >
inline
itk::FixedArray< T2, N >&
ToFixedArray( itk::FixedArray< T2, N >& a,
	      const itk::VariableLengthVector< T1 >& v )
{
  assert( v.Size()==N && v.Size()==a.Size() );

  throw itk::RangeError( __FILE__, __LINE__ );

  for( unsigned int i=0; i<N; ++i )
    a[ i ] = static_cast< T2>( v[ i ] );

  return a;
}

/*******************************************************************************/
template< typename T2, typename T1, unsigned int N >
inline
itk::VariableLengthVector< T2 >
ToVariableLengthVector( const itk::FixedArray< T1, N >& a )
{
  assert( a.Size()==N );

  throw itk::RangeError( __FILE__, __LINE__ );

  itk::VariableLengthVector< T2 > v;

  v.Reserve( N );

  for( unsigned int i=0; i<N; ++i )
    v[ i ] = static_cast< T2 >( a[ i ] );

  return v;
}

/*******************************************************************************/
template< typename T2, typename T1, unsigned int N >
inline
itk::VariableLengthVector< T2 >&
ToVariableLengthVector( itk::VariableLengthVector< T2 >& v,
			const itk::FixedArray< T1, N >& a )
{
  assert( a.Size()==N );

  throw itk::RangeError( __FILE__, __LINE__ );

  v.Reserve( N );

  for( unsigned int i=0; i<N; ++i )
    v[ i ] = static_cast< T2 >( a[ i ] );

  return v;
}

} // end namespace 'otb'.

namespace mvd
{

/*******************************************************************************/
inline
QStringList
ToQStringList( const StringVector& sv )
{
  QStringList qsl;
  return AppendToQStringList( qsl, sv );
}

/*******************************************************************************/
inline
QStringList&
AppendToQStringList( QStringList& qsl, const StringVector& sv )
{
  for( StringVector::const_iterator it( sv.begin() );
       it!=sv.end();
       ++ it )
    {
    qsl.append( QString(it->c_str()) );
    }

  return qsl;
}

/*******************************************************************************/
inline
std::string
ToLocalStdString( const QString & str )
{
  return std::string( str.toLocal8Bit().constData() );
}

/*******************************************************************************/
inline
const char *
ToLocalString( const QString & str )
{
  return str.toLocal8Bit().constData();
}

/*******************************************************************************/
inline
QString
FromStdString( const std::string& str )
{
  return QString( str.c_str() );
}

/*******************************************************************************/
inline
std::string
ToStdString( const QString& str )
{
  return std::string( str.toAscii().constData() );
}

/*******************************************************************************/
inline
const char*
ToString( const QString& str )
{
  return str.toAscii().constData();
}

/*******************************************************************************/
inline
std::string
ToStdString( unsigned int val )
{
  std::ostringstream oss;
  oss << val;
  return oss.str();
}

/*******************************************************************************/
inline
std::string
ToStdString( const SpacingType & spacing)
{
  std::ostringstream oss;
  oss << spacing[0] << " , " << spacing[1];
  return oss.str();
}

/*******************************************************************************/
inline
std::string
ToStdString( const SizeType & size)
{
  std::ostringstream oss;
  oss << size[0] << " , " << size[1];
  return oss.str();
}

/*******************************************************************************/
inline
std::string
ToStdString( const PointType & point)
{
  std::ostringstream oss;
  oss << point[0] << " , " << point[1];
  return oss.str();
}

/*******************************************************************************/
template< typename T>
inline
std::string
ToStdString( const std::vector<T> & vec)
{
  std::ostringstream oss;
  typename std::vector<T>::const_iterator it = vec.begin();
  while( it != vec.end() )
    {
    oss<<*it<< " ";
    ++it;
    }
  
  return oss.str();
}

/*******************************************************************************/
template< typename T >
inline
QString
ToQString( const T& val )
{
  return QString( "%1" ).arg( val );
}

/*****************************************************************************/
template<>
inline
QString
ToQString< float >( const float& val )
{
  assert( !std::isnan( val ) );

  QString valf;

  // See IEEE-754
  // Ref. http://en.wikipedia.org/wiki/Single-precision_floating-point_format
  valf.sprintf( "%.9g", val );

#if 0
  std::cout
    << "ToString< float >(" << val << "): "
    << ToStdString( valf )
    << "\t"
    << valf.toFloat()
    << std::endl;
#endif

  if( valf.toFloat()!=val )
    throw std::runtime_error(
      ToStdString(
	QCoreApplication::translate(
	  "mvd::DatasetDescritor",
	  "Accuracy loss when converting float (%1) to string."
	)
	.arg( valf )
      )
    );

  return valf;
}

/*****************************************************************************/
template<>
inline
QString
ToQString< double >( const double& val )
{
  assert( !std::isnan( val ) );

  QString vald;

  // See IEEE-754
  // Ref. http://en.wikipedia.org/wiki/Double_precision
  vald.sprintf( "%.17g", val );

#if 0
  std::cout
    << "ToString< double >(" << val << "): "
    << ToStdString( vald )
    << "\t"
    << vald.toDouble()
    << std::endl;
#endif

  if( vald.toDouble()!=val )
    throw std::runtime_error(
      ToStdString(
	QCoreApplication::translate(
	  "mvd::DatasetDescriptor",
	  "Accuracy loss when converting double (%1) to string."
	)
	.arg( vald )
      )
    );

  return vald;
}

} // end namespace 'mvd'

namespace mvd
{

/*******************************************************************************/
template< typename TInputIterator, typename TUnaryPredicate >
inline
bool
AllOf( TInputIterator first, TInputIterator last, TUnaryPredicate pred )
{
  while( first!=last )
    {
    if( !pred( *first ) )
      return false;

    ++first;
    }

  return true;
}

/*******************************************************************************/
template< typename TInputIterator, typename TUnaryPredicate >
inline
bool
AnyOf( TInputIterator first, TInputIterator last, TUnaryPredicate pred )
{
  while( first!=last )
    {
    if( pred( *first ) )
      return true;

    ++first;
    }

  return false;
}

/*******************************************************************************/
template< typename TInputIterator, typename TUnaryPredicate >
inline
bool
NoneOf( TInputIterator first, TInputIterator last, TUnaryPredicate pred )
{
  while( first!=last )
    {
    if( pred( *first ) )
      return false;

    ++first;
    }

  return true;
}

/*******************************************************************************/
inline
QString
ToHumanReadableSize( qint64 size, bool isInBits )
{
  double thousand =
    isInBits ? 1024.0 : 1000.0;

  double remainder = static_cast< double >( size );

  if( size < thousand )
    return QString::number( size );

  remainder /= thousand;

  if( remainder < thousand )
    return QString("%1 KiB" ).arg( QString::number( remainder, 'g', 3 ) );

  remainder /= thousand;

  if( remainder < thousand )
    return QString("%1 MiB" ).arg( QString::number( remainder, 'g', 3 ) );

  remainder /= thousand;

  if( remainder < thousand )
    return QString("%1 GiB" ).arg( QString::number( remainder, 'g', 3 ) );

  remainder /= thousand;

  return QString("%1 TiB" ).arg( QString::number( remainder, 'g', 3 ) );
}

} // end namespace 'mvd'


#endif // __mvdAlgorithm_h
