/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef mvdDataStream_h
#define mvdDataStream_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi.h"


#include "OTBMonteverdiCoreExport.h"

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
#include "mvdAlgorithm.h"
#include "mvdSystemError.h"

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
    if( false )                                                         \
      qDebug()                                                          \
        << "Written" << count << "byte(s) as" << typeid( T ).name()     \
        << "address" << pointer << "size" << size;                      \
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
    if( false )                                                         \
      qDebug()                                                          \
        << "Read" << count << "byte(s) as" << typeid( T ).name()        \
        << "address" << pointer << "size" << size;                      \
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
  /*
  qDebug() <<
    "template< T > "
    "QDataStream& operator << ( QDataStream&, T const * & ); "
    "with [ T =" << typeid( T ).name() << "]";
  */

  DATA_STREAM_OUT( out, T, pointer );

  return out;
}

/*****************************************************************************/
template< typename T >
inline
QDataStream&
operator >>( QDataStream& in, T * & pointer )
{
  /*
  qDebug() <<
    "template< T > "
    "QDataStream& operator >> ( QDataStream&, T * & ); "
    "with [ T =" << typeid( T ).name() << "]";
  */

  DATA_STREAM_IN( in, T, pointer );

  return in;
}

#endif // DATA_STREAM_USE_TEMPLATE_OPERATORS

namespace mvd
{

} // end namespace 'mvd'

#endif // mvdDataStream_h
