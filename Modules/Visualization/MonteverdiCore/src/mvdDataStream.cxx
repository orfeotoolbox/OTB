/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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
  inline void Initialize()
  {
    //
    // Call qRegisterMetaType<>() to make type available in
    // non-template signatures and serialization.

    //
    // Register serialization operators for custom meta-types.
  }

  inline void Finalize()
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
QDataStream& operator<<(QDataStream& out, void const*& pointer)
{
  /*
  qDebug()
    << "QDataStream& operator << ( QDataStream&, void const * & );";
  */

  DATA_STREAM_OUT(out, void, pointer);

  return out;
}

/*****************************************************************************/
QDataStream& operator>>(QDataStream& in, void*& pointer)
{
  /*
  qDebug()
    << "QDataStream& operator >> ( QDataStream&, void * & );";
  */

  DATA_STREAM_IN(in, void, pointer);

  return in;
}
