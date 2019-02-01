/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

  QString effectName = qApp->translate( "mvd", EFFECT_NAMES[ effect ] );
  int index = m_UI->effectComboBox->findText(effectName);

  if( visible )
    {
    if( index<0 )
      m_UI->effectComboBox->addItem( effectName );
    }
  else if( index>=0 )
    {
    m_UI->effectComboBox->removeItem( index );
    }
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

    const char *imageEffect = EFFECT_NAMES[settings->GetEffect()];
    QString tradEffect = qApp->translate( "mvd", imageEffect );

    // Add or remove items from the comboBox
    UpdateComboBoxEffectItems(settings);

    for( int i = 0; i < m_UI->effectComboBox->count(); ++i )
      {
      QString comboBoxEffect = m_UI->effectComboBox->itemText( i );
      if( QString::compare( comboBoxEffect, tradEffect ) == 0 )
        {
        // This change will emit currentIndexChanged SIGNAL
        // and then call on_effectComboBox_currentIndexChanged
        QString oldText = m_UI->effectComboBox->currentText();
        if(m_UI->effectComboBox->currentIndex() != i)
          {
          m_UI->effectComboBox->setCurrentIndex( i );
          }
        break;
        }
      }
    }
  }

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/
void ShaderWidget::on_effectComboBox_currentIndexChanged(const QString &text)
{
  if( !HasSettings() )
    return;

  assert( qApp != NULL );

  for( int i = 0; i < EFFECT_COUNT; ++i )
    if( QString::compare( text, qApp->translate( "mvd", EFFECT_NAMES[i] ) ) == 0 )
      {
      ImageSettings *settings = GetSettings();
      assert( settings != NULL );

      settings->SetEffect( static_cast< Effect >( i ) );

      m_UI->sizeSpinBox->setEnabled( GetSettings()->HasSize() );
      m_UI->valueLineEdit->setEnabled( GetSettings()->HasValue() );

      m_UI->sizeSpinBox->setValue( settings->GetSize() );

      char const *const textName = settings->GetValueName();

      if( textName == NULL )
        {
        m_UI->valueLabel->setVisible( false );
        m_UI->valueLabel->setText( QString() );
        }
      else
        {
        m_UI->valueLabel->setVisible( true );
        m_UI->valueLabel->setText( QString( textName ).append( ":" ) );
        }

      m_UI->valueLineEdit->setVisible( textName != NULL );
      m_UI->valueLineEdit->setText( settings->HasValue() ? ToQString( settings->GetValue() ) : QString() );

      emit SettingsChanged();

      break;
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


void ShaderWidget::UpdateComboBoxEffectItems(ImageSettings *imageSettings)
{
  if( imageSettings != NULL )
    {
    VectorImageSettings *vis = dynamic_cast<VectorImageSettings *>(imageSettings);
    if( vis != NULL )
      {
      // Add Gray shader effect to the ComboBox if the image is grayscale.
      // Update items in the comboBox but do not fire signal each time an item is removed.
      bool oldState = m_UI->effectComboBox->blockSignals( true );
      GrayscaleActivated( vis->IsGrayscaleActivated() );
      m_UI->effectComboBox->blockSignals( oldState );
      }
    }
}

} // end namespace 'mvd'
