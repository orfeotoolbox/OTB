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
#include "mvdShaderWidget.h"
#include "ui_mvdShaderWidget.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)
#include <cassert>

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "Core/mvdAlgorithm.h"
#include "Core/mvdImageSettings.h"


namespace mvd
{

/*
  TRANSLATOR mvd::ShaderWidget

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */


/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
ShaderWidget
::ShaderWidget( QWidget * parent, Qt::WindowFlags flags  ):
  QWidget( parent, flags ),
  m_UI( new mvd::Ui::ShaderWidget() ),
  m_Settings( NULL )
{
  m_UI->setupUi( this );

  for( int i=0; i<ImageSettings::EFFECT_COUNT; ++i )
    m_UI->effectComboBox->addItem( ImageSettings::EFFECT_NAME[ i ] );

  m_UI->valueLineEdit->setValidator(
    new QDoubleValidator( m_UI->valueLineEdit )
  );
} 

/*******************************************************************************/
ShaderWidget
::~ShaderWidget()
{
  delete m_UI;
  m_UI = NULL;
}

/*******************************************************************************/
void
ShaderWidget
::virtual_SetSettings( ImageSettings * settings )
{
  assert( m_UI!=NULL );

  m_UI->effectComboBox->setEnabled( settings!=NULL );
  m_UI->sizeLabel->setEnabled( settings!=NULL );
  m_UI->sizeSpinBox->setEnabled( settings!=NULL && settings->HasSize() );
  m_UI->valueLabel->setEnabled( settings!=NULL );
  m_UI->valueLineEdit->setEnabled( settings!=NULL && settings->HasValue() );

  if( settings!=NULL )
    {
    for( int i=0; i<m_UI->effectComboBox->count(); ++i )
      if( QString::compare( ImageSettings::EFFECT_NAME[ i ],
                            m_UI->effectComboBox->itemText( i ) )==0 )
        {
        m_UI->effectComboBox->setCurrentIndex( i );

        break;
        }

    m_UI->sizeSpinBox->setValue( settings->GetSize() );
    m_UI->valueLineEdit->setText( ToQString( settings->GetValue() ) );
    }
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/
void
ShaderWidget
::on_effectComboBox_currentIndexChanged( const QString & text )
{
  for( int i=0; i<ImageSettings::EFFECT_COUNT; ++i )
    if( QString::compare( text, ImageSettings::EFFECT_NAME[ i ] )==0 )
      {
      assert( m_Settings!=NULL );

      m_Settings->SetEffect( static_cast< ImageSettings::Effect >( i ) );

      m_UI->sizeSpinBox->setEnabled( m_Settings->HasSize() );
      m_UI->valueLineEdit->setEnabled( m_Settings->HasValue() );

      return;
      }
}

/*******************************************************************************/
void
ShaderWidget
::on_sizeSpinBox_valueChanged( int value )
{
  assert( m_Settings!=NULL );
  assert( m_Settings->HasSize() );

  m_Settings->SetSize( value );
}

/*******************************************************************************/
void
ShaderWidget
::on_valueLineEdit_textChanged( const QString & text )
{
  assert( m_Settings!=NULL );
  assert( m_Settings->HasValue() );

  bool isOk = true;

  m_Settings->SetValue( text.toDouble( &isOk ) );

  assert( isOk );
}

} // end namespace 'mvd'
