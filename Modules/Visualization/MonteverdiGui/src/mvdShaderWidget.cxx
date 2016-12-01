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
#include "mvdAlgorithm.h"
#include "mvdGui.h"
#include "mvdVectorImageSettings.h"

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
::ShaderWidget( QWidget * p, Qt::WindowFlags flags  ):
  QWidget( p, flags ),
  m_UI( new mvd::Ui::ShaderWidget() )
{
  m_UI->setupUi( this );

  assert( qApp!=NULL );

  AddItemsInto( m_UI->effectComboBox, "mvd", EFFECT_NAMES, EFFECT_COUNT );

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

/*****************************************************************************/
void
ShaderWidget
::SetGLSLEnabled( bool isGLSLEnabled )
{
  setEnabled( isGLSLEnabled );

  if( !isGLSLEnabled )
    {
    typedef QList< QWidget * > WidgetList;

    WidgetList childrenList( findChildren< QWidget * >() );

    for( WidgetList::iterator it( childrenList.begin() );
	 it!=childrenList.end();
	 ++ it )
      ( *it )->setToolTip(
	tr( "Rendering effects have been disabled because OpenGL Shading Language minimal requirements have not been met while running on this system." )
      );
    }
}

/*******************************************************************************/
void
ShaderWidget
::SetGLSL140Enabled( bool isGLSLEnabled )
{
  SetEffectVisible(EFFECT_GRADIENT,isGLSLEnabled);
}

void ShaderWidget
::SetEffectVisible(const Effect & effect, bool visible)
{
  assert( qApp!=NULL );

  int index =
    m_UI->effectComboBox->findText(
      qApp->translate( "mvd", EFFECT_NAMES[ effect ] )
    );

  if( visible )
    {
    if( index<0 )
      m_UI->effectComboBox->addItem( qApp->translate( "mvd", EFFECT_NAMES[ effect ] ) );
    }
  else if( index>=0 )
    m_UI->effectComboBox->removeItem( index );
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
    assert( qApp!=NULL );

    for( int i=0; i<m_UI->effectComboBox->count(); ++i )
      if( m_UI->effectComboBox->itemText( i )
	  .compare(
	    qApp->translate( "mvd",
			     EFFECT_NAMES[ settings->GetEffect() ] ) )==0 )
        {
        m_UI->effectComboBox->setCurrentIndex( i );

        break;
        }

    m_UI->sizeSpinBox->setValue( settings->GetSize() );

    char const * const text = settings->GetValueName();

    if( text==NULL )
      {
      m_UI->valueLabel->setVisible( false );
      m_UI->valueLabel->setText( QString() );
      }
    else
      {
      m_UI->valueLabel->setVisible( true );
      m_UI->valueLabel->setText( QString( text ).append( ":" ) );
      }

    m_UI->valueLineEdit->setVisible( text!=NULL );
    m_UI->valueLineEdit->setText(
      settings->HasValue()
      ? QString::number( settings->GetValue(), 'g', std::numeric_limits< double >::digits10 ) // ToQString( settings->GetValue() )
      : QString()
    );
    m_UI->valueLineEdit->setCursorPosition( 0 );
    }

  VectorImageSettings * vis = dynamic_cast<VectorImageSettings*>(settings);

  if(vis!=NULL)
    {
    GrayscaleActivated(vis->IsGrayscaleActivated());
    }
  
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/
void
ShaderWidget
::on_effectComboBox_currentIndexChanged( const QString & text )
{
  if( !HasSettings() )
    return;

  assert( qApp!=NULL );

  for( int i=0; i<EFFECT_COUNT; ++i )
    if( QString::compare( text, qApp->translate( "mvd", EFFECT_NAMES[ i ] ) )==0 )
      {
      ImageSettings * settings = GetSettings();
      assert( settings!=NULL );

      settings->SetEffect( static_cast< Effect >( i ) );

      m_UI->sizeSpinBox->setEnabled( GetSettings()->HasSize() );
      m_UI->valueLineEdit->setEnabled( GetSettings()->HasValue() );

      m_UI->sizeSpinBox->setValue( settings->GetSize() );

      char const * const textName = settings->GetValueName();

      if( textName==NULL )
	{
	m_UI->valueLabel->setVisible( false );
	m_UI->valueLabel->setText( QString() );
	}
      else
	{
	m_UI->valueLabel->setVisible( true );
	m_UI->valueLabel->setText( QString( textName ).append( ":" ) );
	}

      m_UI->valueLineEdit->setVisible( textName!=NULL );
      m_UI->valueLineEdit->setText(
	settings->HasValue()
	? ToQString( settings->GetValue() )
	: QString()
      );

      emit SettingsChanged();

      return;
      }
}

/*******************************************************************************/
void
ShaderWidget
::on_sizeSpinBox_valueChanged( int value )
{
  if( !HasSettings() )
    return;

  GetSettings()->SetSize( value );

  emit SettingsChanged();
}

/*******************************************************************************/
void
ShaderWidget
::on_valueLineEdit_editingFinished()
{
  if( !HasSettings() ||
      !GetSettings()->HasValue() )
    return;

  bool isOk = true;

  assert( m_UI!=NULL );
  assert( m_UI->valueLineEdit!=NULL );

  GetSettings()->SetValue( m_UI->valueLineEdit->text().toDouble( &isOk ) );

  // assert( isOk );

  if( !isOk )
    return;

  emit SettingsChanged();
}


void ShaderWidget
::GrayscaleActivated(bool status)
{  
  SetEffectVisible(EFFECT_LUT_JET,status);
  SetEffectVisible(EFFECT_LUT_LOCAL_JET,status);
  SetEffectVisible(EFFECT_LUT_HOT,status);
  SetEffectVisible(EFFECT_LUT_LOCAL_HOT,status);
  SetEffectVisible(EFFECT_LUT_WINTER,status);
  SetEffectVisible(EFFECT_LUT_LOCAL_WINTER,status);
  SetEffectVisible(EFFECT_LUT_SUMMER,status);
  SetEffectVisible(EFFECT_LUT_LOCAL_SUMMER,status);
  SetEffectVisible(EFFECT_LUT_COOL,status);
  SetEffectVisible(EFFECT_LUT_LOCAL_COOL,status);
  SetEffectVisible(EFFECT_SPECTRAL_ANGLE,!status);
}

} // end namespace 'mvd'
