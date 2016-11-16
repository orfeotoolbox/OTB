/*=========================================================================

  Program:   ORFEO Toolbox
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

#include "otbQtAdapters.h"


#include <cassert>

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

namespace otb
{
/*
  TRANSLATOR otb::QtAdapters

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */

namespace
{
} // end of anonymous namespace.


/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */
/*****************************************************************************/
QString
GetExistingDirectory( QWidget * p,
		      const QString& caption,
		      const QString& dir,
		      QFileDialog::Options options )
{
  QString path(
    QFileDialog::getExistingDirectory(
      p,
      caption.isEmpty()
      ? QObject::tr( "Select directory..." )
      : caption,
      dir.isEmpty() ? RecentDirectory : dir,
      options
    )
  );

  if( !path.isNull() )
    {
    // Absolute path expected from QFileDialog
    SetWorkingDir( path );
    }

  return path;
}

/*****************************************************************************/
QString
GetOpenFileName( QWidget * p,
		 const QString& caption,
		 const QString& dir,
		 const QString& filter,
		 QString* selectedFilter,
		 QFileDialog::Options options )
{
  QString filename(
    QFileDialog::getOpenFileName(
      p,
      caption.isEmpty()
      ? QObject::tr( "Open file..." )
      : caption,
      dir.isEmpty() ? RecentDirectory : dir,
      filter,
      selectedFilter,
      options
    )
  );

  // Absolute path expected from QFileDialog
  if( !filename.isNull() )
    SetWorkingDir( filename );

  return filename;
}

/*****************************************************************************/
QStringList
GetOpenFileNames( QWidget * p,
		  const QString & caption,
		  const QString & dir,
		  const QString & filter,
		  QString * selectedFilter,
		  QFileDialog::Options options )
{
  QStringList filenames(
    QFileDialog::getOpenFileNames(
      p,
      caption.isEmpty()
      ? QObject::tr( "Open file..." )
      : caption,
      dir.isEmpty() ? RecentDirectory : dir,
      filter,
      selectedFilter,
      options
    )
  );

  // Absolute path expected from QFileDialog
  if( !filenames.isEmpty() )
    SetWorkingDir( filenames.back() );

  return filenames;
}

/*****************************************************************************/
QString
GetSaveFileName( QWidget * p,
		 const QString & caption,
		 const QString & dir,
		 const QString & filter,
		 QString * selectedFilter,
		 QFileDialog::Options options )
{
  QString filename(
    QFileDialog::getSaveFileName(
      p,
      caption.isEmpty()
      ? QObject::tr( "Save file..." )
      : caption,
      dir.isEmpty() ? RecentDirectory : dir,
      filter,
      selectedFilter,
      options
    )
  );

  // Absolute path expected from QFileDialog
  if( !filename.isNull() )
    SetWorkingDir( filename );

  return filename;
}

/*****************************************************************************/
QString
GetWorkingDir()
{
  return RecentDirectory;
}

/*****************************************************************************/
bool
SetWorkingDir( const QString & filepath )
{
  assert( !filepath.isEmpty() );

  QFileInfo finfo( filepath );

#if 0
  return QDir::setCurrent(
    finfo.isDir()
    ? filepath
    : finfo.path()
  );
#else
  // TODO : add mutex if needed
  QString dir = finfo.isDir() ? filepath : finfo.path();
  if ( !QFileInfo(dir).isDir() )
    return false;
  RecentDirectory = finfo.isDir() ? filepath : finfo.path();
  return true;
#endif
}

} // end namespace 'otb'
