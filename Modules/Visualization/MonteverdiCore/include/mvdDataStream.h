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
#ifndef __mvdDataStream_h
#define __mvdDataStream_h

//
// Configuration include.
//// Included at first position before any other ones.
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829  //tag=QT4-boost-compatibility
#include "ConfigureMonteverdi.h"
#endif //tag=QT4-boost-compatibility


#include "OTBMonteverdiCoreExport.h"

/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QtCore>

//
// System includes (sorted by alphabetic order)
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829  //tag=QT4-boost-compatibility
#include <typeinfo>
#endif //tag=QT4-boost-compatibility

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829  //tag=QT4-boost-compatibility
#include "mvdAlgorithm.h"
#include "mvdSystemError.h"
#endif //tag=QT4-boost-compatibility

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

#endif // __mvdDataStream_h
