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


#ifndef otbQtAdapters_h
#define otbQtAdapters_h


#include "OTBQtAdaptersExport.h"

/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QFileDialog>

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
QString OTBQtAdapters_EXPORT GetExistingDirectory(QWidget* p = 0, const QString& caption = QString(), const QString& dir = QString(),
                                                  QFileDialog::Options options = QFileDialog::ShowDirsOnly);

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
QString OTBQtAdapters_EXPORT GetOpenFilename(QWidget* p = 0, const QString& caption = QString(), const QString& dir = QString(),
                                             const QString& filter = QString(), QString* selectedFilter = 0, QFileDialog::Options options = 0);

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
QStringList OTBQtAdapters_EXPORT GetOpenFilenames(QWidget* p = 0, const QString& caption = QString(), const QString& dir = QString(),
                                                  const QString& filter = QString(), QString* selectedFilter = 0, QFileDialog::Options options = 0);

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
QString OTBQtAdapters_EXPORT GetSaveFilename(QWidget* p = 0, const QString& caption = QString(), const QString& dir = QString(),
                                             const QString& filter = QString(), QString* selectedFilter = 0, QFileDialog::Options options = 0);

/**
 * \return Working directory of Qt-based applications (see
 * http://doc.qt.io/qt-4.8/qdir.html#currentPath).
 */
QString OTBQtAdapters_EXPORT GetWorkingDir();

/**
 * \brief Set working directory for Qt-based applications (see
 * http://doc.qt.io/qt-4.8/qdir.html#setCurrent).
 *
 * \param path Path to working directory.
 *
 * \return <code>true</code> if working directory has successfully
 * been changed.
 */
bool OTBQtAdapters_EXPORT SetWorkingDir(const QString& path);

/**
 * \brief Static path to the last directory used in file dialogs
 */
static QString RecentDirectory;

} // end namespace 'otb'.


/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace otb
{
} // end namespace 'otb'.

#endif // otbQtAdapters_h
