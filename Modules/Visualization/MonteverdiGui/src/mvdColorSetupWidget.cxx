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
#include "mvdColorSetupWidget.h"
#include "ui_mvdColorSetupWidget.h"


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
  "rComboBox",
  "gComboBox",
  "bComboBox",
  "wComboBox"
};

} // end of anonymous namespace.


/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
ColorSetupWidget
::ColorSetupWidget( QWidget* p, Qt::WindowFlags flags ) :
  QWidget( p, flags ),
  m_UI( new mvd::Ui::ColorSetupWidget() )
{
  m_UI->setupUi( this );
}

/*******************************************************************************/
ColorSetupWidget
::~ColorSetupWidget()
{
  delete m_UI;
  m_UI = NULL;
}

/*******************************************************************************/
void
ColorSetupWidget
::SetComponents( const QStringList& components )
{
  m_Components = components;

  m_UI->rComboBox->clear();
  m_UI->gComboBox->clear();
  m_UI->bComboBox->clear();
  m_UI->wComboBox->clear();

  QStringList itemTexts( components );

  // TODO: Move to mvd::core::AbstractImageModel (see ENHANCED_BAND_NAMES[]).
  for( int i=0;
       i<itemTexts.size();
       ++ i )
    {
    if( itemTexts[ i ].isEmpty() )
      {
      itemTexts[ i ] = tr( "BAND %1" ).arg( i+1 );
      }
    else
      {
      itemTexts[ i ].prepend( QString( "%1: " ).arg( i+1 ) );
      }
    }

  m_UI->rComboBox->addItems( itemTexts );
  m_UI->gComboBox->addItems( itemTexts );
  m_UI->bComboBox->addItems( itemTexts );
  m_UI->wComboBox->addItems( itemTexts );

  // Black screen.
  m_UI->rComboBox->setCurrentIndex( 0 );
  m_UI->gComboBox->setCurrentIndex( 0 );
  m_UI->bComboBox->setCurrentIndex( 0 );
  m_UI->wComboBox->setCurrentIndex( 0 );
}

/*******************************************************************************/
void
ColorSetupWidget
::SetCurrentRgbIndex( RgbwChannel channel, int index )
{
  QComboBox* comboBox = findChild< QComboBox* >( QCOMBOBOX_NAMES[ channel ] );

  comboBox->setCurrentIndex( index );
}

/*******************************************************************************/
int
ColorSetupWidget
::GetCurrentRgbIndex( RgbwChannel channel ) const
{
  QComboBox* comboBox = findChild< QComboBox* >( QCOMBOBOX_NAMES[ channel ] );

  return comboBox->currentIndex();
}

/*****************************************************************************/
void
ColorSetupWidget
::SetGrayscaleEnabled( bool enable )
{
  m_UI->wCheckBox->setEnabled( enable );
}

/*****************************************************************************/
void
ColorSetupWidget
::SetGrayscaleActivated( bool activated )
{
  m_UI->wCheckBox->setChecked( activated );
}

/*******************************************************************************/
void
ColorSetupWidget
::SetCurrentGrayIndex( int index )
{
  m_UI->wComboBox->setCurrentIndex( index );
}

/*******************************************************************************/
int
ColorSetupWidget
::GetCurrentGrayIndex() const
{
  return m_UI->wComboBox->currentIndex();
}

/*****************************************************************************/
void
ColorSetupWidget
::SetAlpha( double value )
{
  assert( m_UI!=NULL );

  m_UI->alphaSlider->setValue( FromAlpha( value ) );
}

/*****************************************************************************/
double
ColorSetupWidget
::GetAlpha() const
{
  assert( m_UI!=NULL );

  return ToAlpha( m_UI->alphaSlider->value() );
}

/*****************************************************************************/
double
ColorSetupWidget
::ToAlpha( int value ) const
{
  assert( m_UI!=NULL );

  return
    static_cast< double >( value - m_UI->alphaSlider->minimum() ) /
    static_cast< double >( m_UI->alphaSlider->maximum() - m_UI->alphaSlider->minimum() );
}

/*******************************************************************************/
int
ColorSetupWidget
::FromAlpha( double alpha ) const
{
  assert( m_UI!=NULL );

  if( alpha>=1.0 )
    alpha = m_UI->alphaSlider->maximum();

  else if( alpha<=0.0 )
    alpha = m_UI->alphaSlider->minimum();

  else
    alpha =
      static_cast< double >( m_UI->alphaSlider->minimum() ) +
      alpha * static_cast< double >( m_UI->alphaSlider->maximum() -
                                     m_UI->alphaSlider->minimum() );

  return static_cast< int >( alpha );
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/
void
ColorSetupWidget
::on_alphaSlider_valueChanged( int value )
{
  // qDebug() << "on_alphaSlider_valueChanged(" << value << ")";

  emit AlphaValueChanged( ToAlpha( value ) );
}

/*******************************************************************************/

} // end namespace 'mvd'
