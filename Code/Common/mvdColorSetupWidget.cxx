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
#include "mvdColorSetupWidget.h"
#include "ui_mvdColorSetupWidget.h"

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

/*******************************************************************************/
ColorSetupWidget
::ColorSetupWidget( QWidget* parent, Qt::WindowFlags flags ) :
  QWidget( parent, flags ),
  m_UI( new mvd::Ui::ColorSetupWidget() )
{
  m_UI->setupUi( this );

  QObject::connect(
    m_UI->m_RedComboBox, SIGNAL( currentIndexChanged( int ) ),
    this, SLOT( onRedIndexChanged( int ) )
  );

  QObject::connect(
    m_UI->m_GreenComboBox, SIGNAL( currentIndexChanged( int ) ),
    this, SLOT( onGreenIndexChanged( int ) )
  );

  QObject::connect(
    m_UI->m_BlueComboBox, SIGNAL( currentIndexChanged( int ) ),
    this, SLOT( onBlueIndexChanged( int ) )
  );
}

/*******************************************************************************/
ColorSetupWidget
::~ColorSetupWidget()
{
}

/*******************************************************************************/
void
ColorSetupWidget::SetComponents( const QStringList& components )
{
  m_UI->m_RedComboBox->clear();
  m_UI->m_GreenComboBox->clear();
  m_UI->m_BlueComboBox->clear();

  /*
  m_UI->m_RedComboBox->addItem( 0, "0 - NIL" );
  m_UI->m_GreenComboBox->addItem( 0, "0 - NIL" );
  m_UI->m_BlueComboBox->addItem( 0, "0 - NIL" );
  */

  QStringList itemTexts( components );

  itemTexts.prepend( "NIL" );

  for( int i=1;
       i<itemTexts.size();
       ++ i )
    {
    itemTexts[ i ].prepend( i + " - " );
    }

  m_UI->m_RedComboBox->addItems( itemTexts );
  m_UI->m_GreenComboBox->addItems( itemTexts );
  m_UI->m_BlueComboBox->addItems( itemTexts );

  switch( components.size() )
    {
    case 0:
      // No components should not be met but in case...
      // ...disable all (RGB) video channels.
      m_UI->m_RedComboBox->setCurrentIndex( 0 );
      m_UI->m_GreenComboBox->setCurrentIndex( 0 );
      m_UI->m_BlueComboBox->setCurrentIndex( 0 );
      break;

    case 1:
    case 2:
      m_UI->m_RedComboBox->setCurrentIndex( 1 );
      m_UI->m_GreenComboBox->setCurrentIndex( 1 );
      m_UI->m_BlueComboBox->setCurrentIndex( 1 );
      break;

    default:
      // TODO: Choose indices depending on component name.
      m_UI->m_RedComboBox->setCurrentIndex( 1 );
      m_UI->m_GreenComboBox->setCurrentIndex( 2 );
      m_UI->m_BlueComboBox->setCurrentIndex( 3 );
      break;
    }
}

/*******************************************************************************/
QStringList
ColorSetupWidget
::GetComponents() const
{
  QStringList components;
  for( int i=0; i<m_UI->m_RedComboBox->count(); ++i )
    {
    QStringList item( m_UI->m_RedComboBox->itemText( i ).split( "-" ) );

    components.push_back( item[ 1 ].simplified() );
    }

  return components;
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

/*******************************************************************************/

} // end namespace 'mvd'
