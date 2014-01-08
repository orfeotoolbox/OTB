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
#ifndef __mvdDataStream_h
#define __mvdDataStream_h

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
#include <typeinfo>

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "Core/mvdAlgorithm.h"
#include "Core/mvdSystemError.h"

/*****************************************************************************/
/* PRE-DECLARATION SECTION                                                   */

//
// External classes pre-declaration.

namespace mvd
{
//
// Internal classes pre-declaration.

} // end of namespace 'mvd'.

/**
 */
#define DATA_STREAM_OUT( ostream, T, pointer )                          \
  {                                                                     \
    size_t size = sizeof( T const * );                                  \
                                                                        \
    int count =                                                         \
      ostream.writeRawData(                                             \
        reinterpret_cast< char const * >( &pointer ),                   \
        size                                                            \
      );                                                                \
                                                                        \
    qDebug()                                                            \
      << "Written" << count << "byte(s) as" << typeid( T ).name()       \
      << "address" << pointer << "size" << size;                        \
                                                                        \
    if( static_cast< size_t >( count )!=size )                          \
      throw                                                             \
        mvd::SystemError(                                               \
          mvd::ToStdString(                                             \
            QString( "Error when writing %1 pointer to QDataStream." )  \
            .arg( typeid( T ).name() )                                  \
          )                                                             \
        );                                                              \
  }

/**
 */
#define DATA_STREAM_IN( istream, T, pointer )                           \
  {                                                                     \
    size_t size = sizeof( T * );                                        \
                                                                        \
    int count =                                                         \
      istream.readRawData(                                              \
        reinterpret_cast< char * >( &pointer ), size );                 \
                                                                        \
    qDebug()                                                            \
      << "Read" << count << "byte(s) as" << typeid( T ).name()          \
      << "address" << pointer << "size" << size;                        \
                                                                        \
    if( static_cast< size_t >( count )!=size )                          \
      throw                                                             \
        mvd::SystemError(                                               \
          mvd::ToStdString(                                             \
            QString( "Error when reading %1 pointer from QDataStream." ) \
            .arg( typeid( T ).name() )                                  \
          )                                                             \
        );                                                              \
  }

/*****************************************************************************/
/* FUNCTIONS DECLARATION.                                                    */

/**
 */
QDataStream&
operator << ( QDataStream& out, void const * & pointer );

/**
 */
QDataStream&
operator >>( QDataStream& in, void * & pointer );


/**
 */
#define DATA_STREAM_USE_TEMPLATE_OPERATORS 1

#if DATA_STREAM_USE_TEMPLATE_OPERATORS

/**
 */
template< typename T >
inline
QDataStream&
operator << ( QDataStream& out, T const * & pointer );

/**
 */
template< typename T >
inline
QDataStream&
operator >>( QDataStream& in, T * & pointer );

#endif // DATA_STREAM_USE_TEMPLATE_OPERATORS

namespace mvd
{
} // end namespace 'mvd'.


/*****************************************************************************/
/* INLINE SECTION                                                            */

#if DATA_STREAM_USE_TEMPLATE_OPERATORS

/*****************************************************************************/
template< typename T >
inline
QDataStream&
operator << ( QDataStream& out, T const * & pointer )
{
  qDebug() <<
    "template< T > "
    "QDataStream& operator << ( QDataStream&, T const * & ); "
    "with [ T =" << typeid( T ).name() << "]";

  DATA_STREAM_OUT( out, T, pointer );

  /*
  size_t size = sizeof( T const * );

  int count =
    out.writeRawData(
      reinterpret_cast< char const * >( &pointer ),
      size
    );

  qDebug()
    << "Written" << count << "byte(s) as" << typeid( pointer ).name()
    << "adress" << pointer << "size" << size;

  if( static_cast< size_t >( count )!=size )
    {
    throw mvd::SystemError( "Error when writing T* pointer to QDataStream." );
    }
  */

  return out;
}

/*****************************************************************************/
template< typename T >
inline
QDataStream&
operator >>( QDataStream& in, T * & pointer )
{
  qDebug() <<
    "template< T > "
    "QDataStream& operator >> ( QDataStream&, T * & ); "
    "with [ T =" << typeid( T ).name() << "]";

  DATA_STREAM_IN( in, T, pointer );

  /*
  size_t size = sizeof( T * );

  int count = in.readRawData( reinterpret_cast< char * >( &pointer ), size );

  qDebug()
    << "Read" << count << "byte(s) as" << typeid( pointer ).name()
    << "adress" << pointer << "size" << size;

  if( static_cast< size_t >( count )!=size )
    {
    throw
      mvd::SystemError( "Error when reading T* pointer from QDataStream." );
    }
  */

  return in;
}

#endif // DATA_STREAM_USE_TEMPLATE_OPERATORS

namespace mvd
{

} // end namespace 'mvd'

#endif // __mvdDataStream_h
