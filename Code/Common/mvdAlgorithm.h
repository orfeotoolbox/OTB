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
    qsl.append( QString::fromStdString( *it  ) );
    }

  return qsl;
}

/*******************************************************************************/

} // end namespace 'mvd'

#endif // __mvdAlgorithm_h
