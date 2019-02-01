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
  QString dialogCaption(caption.isEmpty()
    ? QObject::tr( "Select directory..." )
    : caption);
  QString dialogDir(dir.isEmpty()
    ? RecentDirectory
    : dir);

  QString path(
    QFileDialog::getExistingDirectory(
      p,
      dialogCaption,
      dialogDir,
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
GetOpenFilename( QWidget * p,
		 const QString& caption,
		 const QString& dir,
		 const QString& filter,
		 QString* selectedFilter,
		 QFileDialog::Options options )
{
  QString dialogCaption(caption.isEmpty()
    ? QObject::tr( "Open file..." )
    : caption);
  QString dialogDir(dir.isEmpty()
    ? RecentDirectory
    : dir);

  QString filename(
    QFileDialog::getOpenFileName(
      p,
      dialogCaption,
      dialogDir,
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
GetOpenFilenames( QWidget * p,
		  const QString & caption,
		  const QString & dir,
		  const QString & filter,
		  QString * selectedFilter,
		  QFileDialog::Options options )
{
  QString dialogCaption(caption.isEmpty()
    ? QObject::tr( "Open file..." )
    : caption);
  QString dialogDir(dir.isEmpty()
    ? RecentDirectory
    : dir);

  QStringList filenames(
    QFileDialog::getOpenFileNames(
      p,
      dialogCaption,
      dialogDir,
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
GetSaveFilename( QWidget * p,
		 const QString & caption,
		 const QString & dir,
		 const QString & filter,
		 QString * selectedFilter,
		 QFileDialog::Options options )
{
  QString dialogCaption(caption.isEmpty()
    ? QObject::tr( "Save file..." )
    : caption);
  QString dialogDir(dir.isEmpty()
    ? RecentDirectory
    : dir);

  QString filename(
    QFileDialog::getSaveFileName(
      p,
      dialogCaption,
      dialogDir,
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
  return
    QDir::setCurrent(
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
