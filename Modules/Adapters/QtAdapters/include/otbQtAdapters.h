/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Monteverdi is distributed under the CeCILL licence version 2. See
  Licence_CeCILL_V2-en.txt or
  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt for more details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef otbQtAdapters_h
#define otbQtAdapters_h


#include "OTBQtAdaptersExport.h"

/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829  //tag=QT4-boost-compatibility
#include <QFileDialog>
#endif //tag=QT4-boost-compatibility

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)


/*****************************************************************************/
/* PRE-DECLARATION SECTION                                                   */

//
// External classes pre-declaration.
namespace
{
}

//
// Internal classes pre-declaration.
namespace otb
{
} // end namespace 'otb'


/*****************************************************************************/
/* TYPE DEFINITION SECTION                                                   */

namespace otb
{
} // end of namespace otb.


/*****************************************************************************/
/* FUNCTION DECLARATION SECTION                                              */

namespace otb
{

/**
 * \brief This is a convenience static method that will return an
 * existing directory path selected by the user.
 *
 * This method stores the last accepted directory for next call.
 *
 * \see http://qt-project.org/doc/qt-4.8/qfiledialog.html#getExistingDirectory
 *
 * \param parent Parent widget of that QFileDialog.
 * \param caption Title-bar caption of that QFileDialog.
 * \param dir Directory location where to pick path.
 * \param options of that QFileDialog.
 *
 * \return The selected path or an null QString if QFileDialog has
 * been rejected.
 */
QString
OTBQtAdapters_EXPORT
GetExistingDirectory( QWidget * p = 0,
		      const QString & caption = QString(),
		      const QString & dir =  QString(),
		      QFileDialog::Options options = QFileDialog::ShowDirsOnly );

/**
 * \brief This is a convenience static method that will return an
 * existing filename selected by the user.
 *
 * This method stores the last accepted directory for next call.
 *
 * \see http://qt-project.org/doc/qt-4.8/qfiledialog.html#getOpenFileName
 *
 * \param parent Parent widget of that QFileDialog.
 * \param caption Title-bar caption of that QFileDialog.
 * \param dir Directory location where to pick path.
 * \param filter Filename filter (e.g. wildcards).
 * \param selectedFilter Filter which has been selected by the user.
 * \param options Options of that QFileDialog.
 *
 * \return The selected filename or an null QString if QFileDialog has
 * been rejected.
 */
QString
OTBQtAdapters_EXPORT
GetOpenFileName( QWidget * p =0,
		 const QString & caption = QString(),
		 const QString & dir = QString(),
		 const QString & filter = QString(),
		 QString * selectedFilter = 0,
		 QFileDialog::Options options = 0 );

/**
 * \brief This is a convenience static method that will return a
 * list of existing filenames selected by the user.
 *
 * This method stores the last accepted directory for next call.
 *
 * \see http://qt-project.org/doc/qt-4.8/qfiledialog.html#getOpenFileNames
 *
 * \param parent Parent widget of that QFileDialog.
 * \param caption Title-bar caption of that QFileDialog.
 * \param dir Directory location where to pick path.
 * \param filter Filename filter (e.g. wildcards).
 * \param selectedFilter Filter which has been selected by the user.
 * \param options of that QFileDialog.
 *
 * \return The selected filename list or an empty QStringLIst if
 * QFileDialog has been rejected.
 */
QStringList
OTBQtAdapters_EXPORT
GetOpenFileNames( QWidget * p =0,
		  const QString & caption = QString(),
		  const QString & dir = QString(),
		  const QString & filter = QString(),
		  QString * selectedFilter = 0,
		  QFileDialog::Options options = 0 );

/**
 * \brief This is a convenience static method that will return a
 * filename selected by the user. This file does not have to exist.
 *
 * This method stores the last accepted directory for next call.
 *
 * \see http://qt-project.org/doc/qt-4.8/qfiledialog.html#getSaveFileName
 *
 * \param parent Parent widget of that QFileDialog.
 * \param caption Title-bar caption of that QFileDialog.
 * \param dir Directory location where to pick path.
 * \param filter Filename filter (e.g. wildcards).
 * \param selectedFilter Filter which has been selected by the user.
 * \param options of that QFileDialog.
 *
 * \return The selected filename list or an empty QStringLIst if
 * QFileDialog has been rejected.
 */
QString
OTBQtAdapters_EXPORT
GetSaveFileName( QWidget * p =0,
		 const QString & caption = QString(),
		 const QString & dir = QString(),
		 const QString & filter = QString(),
		 QString * selectedFilter = 0,
		 QFileDialog::Options options = 0 );

/**
 * \return Working directory of Qt-based applications (see
 * http://doc.qt.io/qt-4.8/qdir.html#currentPath).
 */
QString
OTBQtAdapters_EXPORT
GetWorkingDir();

/**
 * \brief Set working directory for Qt-based applications (see
 * http://doc.qt.io/qt-4.8/qdir.html#setCurrent).
 *
 * \param path Path to working directory.
 *
 * \return <code>true</code> if working directory has successfully
 * been changed.
 */
bool
OTBQtAdapters_EXPORT
SetWorkingDir( const QString & path );

} // end namespace 'otb'.


/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace otb
{
} // end namespace 'otb'.

#endif // otbQtAdapters_h
