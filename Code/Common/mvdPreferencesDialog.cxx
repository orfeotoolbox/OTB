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
#include "mvdPreferencesDialog.h"
#include "ui_mvdPreferencesDialog.h"

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
  TRANSLATOR mvd::PreferencesDialog

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/

/*******************************************************************************/
PreferencesDialog
::PreferencesDialog( QWidget* parent, Qt::WindowFlags flags ) :
  QDialog( parent ),
  m_UI( new mvd::Ui::PreferencesDialog() )
{
  m_UI->setupUi( this );
  
  QSettings settings;
  if ( settings.contains("cacheDir") )
    {
    m_UI->datasetPathLineEdit->setText( settings.value( "cacheDir" ).toString() );
    }
  
  // Connect centralWidget manipulator to Ql renderer when viewportRegionChanged
  QObject::connect(
    m_UI->buttonBox, SIGNAL( rejected() ), 
    this, SLOT( close() )
    );

}

/*******************************************************************************/
PreferencesDialog
::~PreferencesDialog()
{
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/
void
PreferencesDialog
::on_datasetPathButton_clicked()
{ 
  QFileDialog fileDialog;
  fileDialog.setConfirmOverwrite(true);
  fileDialog.setFileMode(QFileDialog::Directory);
  fileDialog.setNameFilter( tr("Select a Directory") );

  if (fileDialog.exec())
    {
    QString path = fileDialog.selectedFiles().at(0);
    m_UI->datasetPathLineEdit->setText( path );
    }
}

void
PreferencesDialog
::on_buttonBox_accepted()
{
  QSettings settings;
  
  if ( !m_UI->datasetPathLineEdit->text().isEmpty() )
    {
    settings.setValue( "cacheDir", m_UI->datasetPathLineEdit->text() );
    }
    
  close();
}

/*******************************************************************************/

} // end namespace 'mvd'
