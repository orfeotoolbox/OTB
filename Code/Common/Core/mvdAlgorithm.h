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
#ifndef __mvdAlgorithm_h
#define __mvdAlgorithm_h

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
#include <string>

//
// ITK includes (sorted by alphabetic order)
#include "itkFixedArray.h"
#include "itkExceptionObject.h"
#include "itkVariableLengthVector.h"

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
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
 * Convert a StringVector object to a QStringList object.
 *
 * Because Qt uses a shallow mecanism to copy containers, the returned
 * QStringList is shallow-copied.
 */
inline
QStringList
ToQStringList( const StringVector& sv );

/**
 * Append the content of a StringVector object to the content of a
 * QStringList object.
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
 * return a std::string form various types
 *
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

} // end namespace 'mvd'


#endif // __mvdAlgorithm_h
