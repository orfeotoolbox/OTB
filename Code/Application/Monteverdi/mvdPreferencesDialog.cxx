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
#include "mvdPreferencesDialog.h"
#include "ui_mvdPreferencesDialog.h"


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
#include "Gui/mvdGui.h"
#include "Gui/mvdI18nApplication.h"
#include "Gui/mvdI18nMainWindow.h"

namespace mvd
{
/*
  TRANSLATOR mvd::PreferencesDialog

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/

bool
IsPathModified( I18nCoreApplication::SettingsKey,
		bool,
		I18nCoreApplication::SettingsKey,
		const QString & );

/*****************************************************************************/
/* CONSTANTS                                                                 */


/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */
/*****************************************************************************/
bool
IsPathModified( I18nCoreApplication::SettingsKey enabledKey,
		bool isEnabled,
		I18nCoreApplication::SettingsKey pathKey,
		const QString & pathname )
{
  // qDebug()
  //   << "::IsPathModified("
  //   << I18nApplication::Instance()->RetrieveSettingsKey( enabledKey ).toBool()
  //   << ","
  //   << isEnabled
  //   << ","
  //   << I18nApplication::Instance()->RetrieveSettingsKey(
  //        pathKey
  //      ).toString()
  //   << ","
  //   << pathname
  //   << ")";

  return
    I18nApplication::Instance()->RetrieveSettingsKey( enabledKey ).toBool()
    != isEnabled
    ||
    QDir(
      I18nApplication::Instance()->RetrieveSettingsKey(
	pathKey
      ).toString()
    )
    != QDir( pathname );
}

/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */
/*****************************************************************************/
PreferencesDialog
::PreferencesDialog( QWidget* parent, Qt::WindowFlags flags ) :
  QDialog( parent, flags ),
  m_UI( new mvd::Ui::PreferencesDialog() ),
  m_ResultsDirModified( false ),
  m_GeoidFileModified( false ),
  m_SrtmDirModified( false )
{
  assert( m_UI!=NULL );
  assert( I18nApplication::Instance()!=NULL );

  m_UI->setupUi( this );

  //
  // Runtime UI initilization.
  m_UI->settingsGroupBox->setVisible( false );

  AddItemsInto( m_UI->resolutionComboBox, "mvd", RESOLUTION_NAMES, RESOLUTION_COUNT );

  AddItemsInto( m_UI->tileSizeComboBox, "mvd", TILE_SIZE_NAMES, TILE_SIZE_COUNT );

  //
  // General settings.
  m_UI->resultDirPathLineEdit->setText(
    I18nApplication::Instance()->RetrieveSettingsKey(
      I18nCoreApplication::SETTINGS_KEY_RESULTS_DIR
    )
    .toString()
  );

  //
  // Rendering settings.
  {
    QVariant value(
      I18nApplication::Instance()->RetrieveSettingsKey(
	I18nCoreApplication::SETTINGS_KEY_RESOLUTION
      )
    );

    m_UI->resolutionComboBox->setCurrentIndex(
      !value.isValid()
      ? RESOLUTION_LOWER
      : value.toInt()
    );
  }

  {
    QVariant value(
      I18nApplication::Instance()->RetrieveSettingsKey(
	I18nCoreApplication::SETTINGS_KEY_TILE_SIZE
      )
    );

    m_UI->tileSizeComboBox->setCurrentIndex( TILE_SIZE_DEFAULT );

    int val = value.toInt();

    for( int i=0; i<TILE_SIZE_COUNT; ++i)
      if( val==TILE_SIZE_VALUE[ i ] )
	{
	m_UI->tileSizeComboBox->setCurrentIndex( i );

	break;
	}
  }

  //
  // Elevation management settings.
  m_UI->srtmLineEdit->setText(
    I18nApplication::Instance()->RetrieveSettingsKey(
      I18nCoreApplication::SETTINGS_KEY_SRTM_DIR
    )
    .toString()
  );

  m_UI->srtmCheckbox->setChecked(
    I18nApplication::Instance()->RetrieveSettingsKey(
      I18nCoreApplication::SETTINGS_KEY_SRTM_DIR_ACTIVE
    )
    .toBool()
  );
  m_UI->srtmLineEdit->setEnabled( m_UI->srtmCheckbox->isChecked() );
  m_UI->srtmButton->setEnabled( m_UI->srtmCheckbox->isChecked() );

  m_UI->geoidLineEdit->setText(
    I18nApplication::Instance()->RetrieveSettingsKey(
      I18nCoreApplication::SETTINGS_KEY_GEOID_PATH
    )
    .toString()
  );

  m_UI->geoidCheckbox->setChecked(
    I18nApplication::Instance()->RetrieveSettingsKey(
      I18nCoreApplication::SETTINGS_KEY_GEOID_PATH_ACTIVE
    )
    .toBool()
  );

  m_UI->geoidLineEdit->setEnabled( m_UI->geoidCheckbox->isChecked() );
  m_UI->geoidButton->setEnabled( m_UI->geoidCheckbox->isChecked() );

  // Connect centralWidget manipulator to Ql renderer when viewportRegionChanged
  QObject::connect(
    m_UI->buttonBox, SIGNAL( rejected() ), 
    this, SLOT( close() )
  );

  assert( !m_ResultsDirModified );
  assert( !m_GeoidFileModified );
  assert( !m_SrtmDirModified );

  // qDebug() << "results-dir:" << m_ResultsDirModified;
  // qDebug() << "geoid-file:" << m_GeoidFileModified;
  // qDebug() << "srtm-dir:" << m_SrtmDirModified;
}

/*****************************************************************************/
PreferencesDialog
::~PreferencesDialog()
{
  delete m_UI;
  m_UI = NULL;
}

/*****************************************************************************/
/* SLOTS                                                                     */
/*****************************************************************************/
void
PreferencesDialog
::on_buttonBox_accepted()
{
  //
  // General settings.
  if( m_ResultsDirModified )
    {
    // Set the result dir
    I18nApplication::Instance()->StoreSettingsKey(
      I18nCoreApplication::SETTINGS_KEY_RESULTS_DIR,
      QDir::cleanPath( m_UI->resultDirPathLineEdit->text()
      )
    );
    }

  I18nApplication::Instance()->StoreSettingsKey(
    I18nCoreApplication::SETTINGS_KEY_SRTM_DIR_ACTIVE,
    m_UI->srtmCheckbox->isChecked()
  );
  I18nApplication::Instance()->StoreSettingsKey(
    I18nCoreApplication::SETTINGS_KEY_SRTM_DIR,
    QDir::cleanPath( m_UI->srtmLineEdit->text()
    )
  );

  I18nApplication::Instance()->StoreSettingsKey(
    I18nCoreApplication::SETTINGS_KEY_GEOID_PATH_ACTIVE,
    m_UI->geoidCheckbox->isChecked()
  );
  I18nApplication::Instance()->StoreSettingsKey(
    I18nCoreApplication::SETTINGS_KEY_GEOID_PATH,
    m_UI->geoidLineEdit->text()
  );

  //
  // Rendering settings.
  I18nApplication::Instance()->StoreSettingsKey(
    I18nCoreApplication::SETTINGS_KEY_RESOLUTION,
    m_UI->resolutionComboBox->currentIndex()
  );

  {
  bool isOk = true;

  int value = m_UI->tileSizeComboBox->currentText().toInt( &isOk );

  assert( isOk );

  if( !isOk )
    value = TILE_SIZE_VALUE[ TILE_SIZE_DEFAULT ];

  I18nApplication::Instance()->StoreSettingsKey(
    I18nCoreApplication::SETTINGS_KEY_TILE_SIZE,
    value
  );
  }

  //
  // Elevation management settings.
  if( m_GeoidFileModified )
    {
    try
      {
      if( I18nApplication::Instance()->ElevationSetup() )
	{
	QMessageBox::warning(
	  this,
	  "Warning!",
	  tr( "Geoid file has changed since application has been started. This setting needs the application to be restarted to be taken into account." )
	);
	}
      }
    catch( const std::exception & exception )
      {
      QMessageBox::warning(
	this,
	"Warning!",
	tr(
	  "The following exception has been caught when setting up Elevation Settings:\n\n%1"
	).arg( exception.what() )
      );
      }
    }

  m_ResultsDirModified = false;
  m_GeoidFileModified = false;
  m_SrtmDirModified = false;

  close();
}

/*****************************************************************************/
void
PreferencesDialog
::on_srtmCheckbox_toggled( bool checked )
{
  assert( m_UI!=NULL );

  m_UI->srtmLineEdit->setEnabled( checked );
  m_UI->srtmButton->setEnabled( checked );

  m_SrtmDirModified =
    IsPathModified(
      I18nCoreApplication::SETTINGS_KEY_SRTM_DIR_ACTIVE,
      checked,
      I18nCoreApplication::SETTINGS_KEY_SRTM_DIR,
      m_UI->srtmLineEdit->text()
    );
}

/***************************************************************************/
void
PreferencesDialog
::on_srtmLineEdit_textChanged( const QString & text )
{
  assert( m_UI!=NULL );

  m_SrtmDirModified = 
    IsPathModified(
      I18nCoreApplication::SETTINGS_KEY_SRTM_DIR_ACTIVE,
      m_UI->srtmCheckbox->isChecked(),
      I18nCoreApplication::SETTINGS_KEY_SRTM_DIR,
      text
    );
}

/***************************************************************************/
void
PreferencesDialog
::on_geoidCheckbox_toggled( bool checked )
{
  m_UI->geoidLineEdit->setEnabled( checked );
  m_UI->geoidButton->setEnabled( checked );

  m_GeoidFileModified =
    IsPathModified(
      I18nCoreApplication::SETTINGS_KEY_GEOID_PATH_ACTIVE,
      checked,
      I18nCoreApplication::SETTINGS_KEY_GEOID_PATH,
      m_UI->geoidLineEdit->text()
    );
}

/***************************************************************************/
void
PreferencesDialog
::on_geoidLineEdit_textChanged( const QString & text )
{
  m_GeoidFileModified =
    IsPathModified(
      I18nCoreApplication::SETTINGS_KEY_GEOID_PATH_ACTIVE,
      m_UI->geoidCheckbox->isChecked(),
      I18nCoreApplication::SETTINGS_KEY_GEOID_PATH,
      text
    );
}

/***************************************************************************/
void
PreferencesDialog
::on_srtmButton_clicked()
{
  QString srtmDir(
    I18nMainWindow::GetExistingDirectory(
      this,
      tr( "Select the directory containing DEM files." )
    )
  );

  if( srtmDir.isEmpty() )
    return;

  m_UI->srtmLineEdit->setText(
    QDir( srtmDir ).absolutePath()
  );
}

/***************************************************************************/
void
PreferencesDialog
::on_geoidButton_clicked()
{
  QString geoidFile(
    I18nMainWindow::GetOpenFileName(
      this,
      tr( "Select a geoid file." )
    )
  );

  if( geoidFile.isEmpty() )
    return;

  m_UI->geoidLineEdit->setText(
    QDir( geoidFile ).absolutePath()
  );
}

/***************************************************************************/
void
PreferencesDialog
::on_resultDirButton_clicked()
{
  QString resultsDir(
    I18nMainWindow::GetExistingDirectory(
      this,
      tr( "Select the default directory for results data: " )
    )
  );

  if( resultsDir.isEmpty() )
    return;

  m_UI->resultDirPathLineEdit->setText(
    QDir( resultsDir ).absolutePath()
  );
}

} // end namespace 'mvd'
