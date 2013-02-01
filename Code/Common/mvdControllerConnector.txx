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

/*******************************************************************************/
template< typename TController >
ControllerConnector< TController >
::ControllerConnector( QWidget* parent, Qt::WindowFlags flags  ):
  TController( parent, flags )
{
}

/*******************************************************************************/
template< typename TController >
ControllerConnector< TController >
::~ControllerConnector()
{
}

/*******************************************************************************/
template< typename TController >
void
ControllerConnector< TController >
::Connect( typename TController::Model* model )
{
  SuperClass::Connect( model );
}

/*******************************************************************************/
template< typename TController >
void
ControllerConnector< TController >
::Disconnect( typename TController::Model* model )
{
  SuperClass::Disconnect( model );
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

/*******************************************************************************/

} // end namespace 'mvd'
