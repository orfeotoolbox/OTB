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

#include "mvdAbstractModel.h"


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
  TRANSLATOR mvd::AbstractModel

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
AbstractModel
::AbstractModel( QObject* p ) :
  QObject( p )
{
}

/*******************************************************************************/
AbstractModel
::~AbstractModel()
{
}

/*******************************************************************************/
bool
AbstractModel
::IsModified() const
{
  return false;
}

/*******************************************************************************/
void
AbstractModel
::ClearModified()
{
}

/*******************************************************************************/
void
AbstractModel
::virtual_Save()
{
  qDebug() << this << "::Save()";
}

/*******************************************************************************/
void
AbstractModel
::virtual_BuildModel( void * model )
{
  qDebug() << this << "::virtual_BuildModel(" << model << ")";
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

/*******************************************************************************/

} // end namespace 'mvd'
