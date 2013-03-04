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
#include "mvdApplication.h"

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
  m_CacheDirRoot(),
  m_CacheDirRootModified(false),
  m_UI( new mvd::Ui::PreferencesDialog() )
{
  m_UI->setupUi( this );
  
  QSettings settings;
  if ( settings.contains("cacheDir") )
    {
    m_UI->cacheDirPathLineEdit->setText( settings.value( "cacheDir" ).toString() );
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
::on_cacheDirButton_clicked()
{ 
  while (true)
    {
    QString cacheDirStr = QFileDialog::getExistingDirectory(
        this,
        tr("Select the repository to store the cache repository for Monteverdi2"),
        QDir::homePath());
    if (cacheDirStr.isEmpty())
      { // User push default button => don't modify the value
      break;
      }
    else
      { // User select something, test if it is correct
      if ( Application::Instance()->TestDirExistenceAndWriteAcess(QDir(cacheDirStr)) )
        {
        m_CacheDirRootModified = true;
        m_CacheDirRoot = cacheDirStr;
        QString tree (Application::Instance()->GetCacheDirName());
        tree.prepend("/");
        m_UI->cacheDirPathLineEdit->setText(cacheDirStr.append(tree));
        break;
        }
      else
        {
        QMessageBox::warning( this,
            tr("Warning"),
            tr("This repository seems incorrect to store the cache directory."
               "\nPlease choose another one.") );
        }
      }
    }
}

void
PreferencesDialog
::on_buttonBox_accepted()
{

  if (m_CacheDirRootModified)
    {
    // Create the cache dir
    Application::Instance()->MakeCacheDir(m_CacheDirRoot);

    // Save the cache directory into the settings file
    Application::Instance()->WriteCacheDirIntoSettings();
    }
    
  close();
}

/*******************************************************************************/

} // end namespace 'mvd'
