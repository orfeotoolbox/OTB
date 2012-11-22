/*=========================================================================

  Program:   Monteverdi2
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved .
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "mvdApplication.h"

//
// System includes (sorted by alphabetic order)
#include <iostream>

//
// Qt includes (sorted by alphabetic order)
#include <QtGui>

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "ConfigureMonteverdi2.h"

//
// Class pre-declaration.
//
// Class implementation.
namespace mvd
{

/*****************************************************************************/
Application
::Application( int& argc, char** argv ) :
  QApplication( argc, argv )
{
  InitializeLocale();

  QObject::connect(
    this, SIGNAL( aboutToQuit() ),
    this, SLOT( OnAboutToQuit() )
    );
}

/*****************************************************************************/
Application
::~Application()
{
}

/*****************************************************************************/
/*
bool
Application::Initialize()
{
  InitializeLocale();

  return true;
}
*/

/*****************************************************************************/
void
Application::InitializeLocale()
{
  QString localeName( QLocale::system().name() );

  QString localeFilename( QString( PROJECT_NAME ).toLower() +
			  "_" +
			  localeName );

  QTranslator localeTranslator;

  if( !localeTranslator.load( localeFilename ) )
    {
// TODO: Log error while loading locale translation file.
// TODO: Add QMessageBox::warning()!
    qDebug(
      ">DEBUG> Failed to load '%s' locale translation file",
      localeFilename.toLatin1().data()
    );

    return;
    }

  installTranslator( &localeTranslator );

  qDebug(
    ">DEBUG> Successfully loaded '%s' locale translation file.",
    localeFilename.toLatin1().data()
  );

// TODO: Log locale translation filename used.

/* May be usefull...
  QTextCodec::setCodecForTr( QTextCodec::codecForName( "utf8" ) );
*/

// TODO: Record locale translation filename used in UI component (e.g.
// AboutDialog, Settings dialog, Information dialog etc.)
}

/*****************************************************************************/
/* SLOTS                                                                     */
/*****************************************************************************/
void
Application
::OnAboutToQuit()
{
  qDebug( ">DEBUG> mvd::Application::OnAboutToQuit()" );
}

/*****************************************************************************/

} // end namespace 'mvd'
