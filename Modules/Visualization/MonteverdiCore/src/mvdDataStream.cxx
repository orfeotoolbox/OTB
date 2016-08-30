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
#include "mvdDataStream.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)

namespace mvd
{
/*
  TRANSLATOR mvd::DataStream

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */

namespace
{
}


/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */

class StaticInitializer
{
public:
  StaticInitializer()
  {
    Initialize();
  }

  ~StaticInitializer()
  {
    Finalize();
  }

private:
  inline
  void
  Initialize()
  {
    //
    // Call qRegisterMetaType<>() to make type available in
    // non-template signatures and serialization.

    //
    // Register serialization operators for custom meta-types.
  }

  inline
  void
  Finalize()
  {
  }
};

namespace
{
static const StaticInitializer STATIC_INITIALIZER;
}

/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

} // end namespace 'mvd'

/*****************************************************************************/
/* GLOBAL FUNCTION IMPLEMENTATION SECTION                                    */
/*****************************************************************************/
QDataStream&
operator << ( QDataStream& out, void const * & pointer )
{
  /*
  qDebug()
    << "QDataStream& operator << ( QDataStream&, void const * & );";
  */

  DATA_STREAM_OUT( out, void, pointer );

  return out;
}

/*****************************************************************************/
QDataStream&
operator >>( QDataStream& in, void * & pointer )
{
  /*
  qDebug()
    << "QDataStream& operator >> ( QDataStream&, void * & );";
  */

  DATA_STREAM_IN( in, void, pointer );

  return in;
}
