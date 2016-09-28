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
      caption,
      dir,
      options
    )
  );

  if( !path.isNull() )
    SetWorkingDir( path );

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
      caption,
      dir,
      filter,
      selectedFilter,
      options
    )
  );

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
      caption,
      dir,
      filter,
      selectedFilter,
      options
    )
  );

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
      caption,
      dir,
      filter,
      selectedFilter,
      options
    )
  );

  if( !filename.isNull() )
    SetWorkingDir( filename );

  return filename;
}

/*****************************************************************************/
QString
GetWorkingDir()
{
  return QDir::currentPath();
}

/*****************************************************************************/
bool
SetWorkingDir( const QString & filepath )
{
  assert( !filepath.isEmpty() );

  return QDir::setCurrent( QFileInfo( filepath ).path() );
}

} // end namespace 'otb'
