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
#include "Gui/mvdColorSetupWidget.h"
#include "Gui/ui_mvdColorSetupWidget.h"


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
  TRANSLATOR mvd::ColorSetupWidget

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */

namespace
{

/**
 */
const char* QCOMBOBOX_NAMES[] = {
  "redComboBox",
  "greenComboBox",
  "blueComboBox"
};

} // end of anonymous namespace.


/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
ColorSetupWidget
::ColorSetupWidget( QWidget* parent, Qt::WindowFlags flags ) :
  QWidget( parent, flags ),
  m_UI( new mvd::Ui::ColorSetupWidget() )
{
  m_UI->setupUi( this );
}

/*******************************************************************************/
ColorSetupWidget
::~ColorSetupWidget()
{
}

/*******************************************************************************/
void
ColorSetupWidget
::SetComponents( const QStringList& components )
{
  m_Components = components;

  m_UI->redComboBox->clear();
  m_UI->greenComboBox->clear();
  m_UI->blueComboBox->clear();

  QStringList itemTexts( components );

  // TODO: Move to mvd::core::AbstractImageModel (see ENHANCED_BAND_NAMES[]).
  for( int i=0;
       i<itemTexts.size();
       ++ i )
    {
    if( itemTexts[ i ].isEmpty() )
      {
      itemTexts[ i ] = tr( "BAND %1" ).arg( i );
      }
    else
      {
      itemTexts[ i ].prepend( QString( "%1: " ).arg( i ) );
      }
    }

  m_UI->redComboBox->addItems( itemTexts );
  m_UI->greenComboBox->addItems( itemTexts );
  m_UI->blueComboBox->addItems( itemTexts );

  // Black screen.
  m_UI->redComboBox->setCurrentIndex( 0 );
  m_UI->greenComboBox->setCurrentIndex( 0 );
  m_UI->blueComboBox->setCurrentIndex( 0 );
}

/*******************************************************************************/
void
ColorSetupWidget
::SetCurrentIndex( RgbaChannel channel, int index )
{
  QComboBox* comboBox = findChild< QComboBox* >( QCOMBOBOX_NAMES[ channel ] );

  comboBox->setCurrentIndex( index );
}

/*******************************************************************************/
int
ColorSetupWidget
::GetCurrentIndex( RgbaChannel channel )
{
  QComboBox* comboBox = findChild< QComboBox* >( QCOMBOBOX_NAMES[ channel ] );

  return comboBox->currentIndex();
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

/*******************************************************************************/

} // end namespace 'mvd'
