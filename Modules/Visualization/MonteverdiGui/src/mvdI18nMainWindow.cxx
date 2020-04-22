/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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


#include "mvdI18nMainWindow.h"

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QtWidgets>

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)
#include "otbSystem.h"

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdBackgroundTask.h"
#include "mvdImageImporter.h"
#include "mvdOverviewBuilder.h"
#include "mvdVectorImageModel.h"
#include "mvdAboutDialog.h"
#include "mvdI18nApplication.h"
#include "mvdImportImagesDialog.h"
#include "mvdTaskProgressDialog.h"

namespace mvd
{

/*
  TRANSLATOR mvd::I18nMainWindow

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


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */
/*****************************************************************************/
I18nMainWindow::I18nMainWindow(QWidget* p, Qt::WindowFlags flags) : QMainWindow(p, flags)
{
}

/*****************************************************************************/
I18nMainWindow::~I18nMainWindow()
{
}

/*****************************************************************************/
void I18nMainWindow::Initialize()
{
  /*
  // Default setup.
  setObjectName( "mvd::I18nMainWindow" );
  setWindowTitle( PROJECT_NAME );
  */

  virtual_SetupUI();

  // Connect Appllication and MainWindow when selected model is about
  // to change.
  QObject::connect(I18nApplication::Instance(), SIGNAL(AboutToChangeModel(const AbstractModel*)), this, SLOT(OnAboutToChangeModel(const AbstractModel*)));

  // Connect Application and MainWindow when selected model has been
  // changed.
  QObject::connect(I18nApplication::Instance(), SIGNAL(ModelChanged(AbstractModel*)), this, SLOT(OnModelChanged(AbstractModel*)));

  virtual_ConnectUI();

  virtual_InitializeUI();
}

/*****************************************************************************/
QDockWidget* I18nMainWindow::AddWidgetToDock(QWidget* widget, const QString& dockName, const QString& dockTitle, Qt::DockWidgetArea dockArea,
                                             DockLayoutFlags flags)
{
  // New dock.
  QDockWidget* dockWidget = new QDockWidget(dockTitle, this);

  // You can use findChild( dockName ) to get dock-widget.
  dockWidget->setObjectName(dockName);
  dockWidget->setWidget(widget);

  // Features.
  dockWidget->setFloating(flags.testFlag(DOCK_LAYOUT_FLOATING));
  dockWidget->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetClosable);

  // dockWidget->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );

  // dockWidget->adjustSize();

  // Add dock.
  addDockWidget(dockArea, dockWidget);

  //
  return dockWidget;
}

/*****************************************************************************/
VectorImageModel* I18nMainWindow::ImportImage(const QString& filename, int widthVal, int heightVal)
{
  return QObjectCast<VectorImageModel*>(Import(
                                            // New dataset-importer worker.
                                            // It will be auto-deleted by background-task.
                                            new ImageImporter(filename, widthVal, heightVal)),
                                        "QObject is not a VectorImageModel.");
}

/*****************************************************************************/
bool I18nMainWindow::BuildGDALOverviews(const QStringList& filenames)
{
  ImportImagesDialog* importDialog = new ImportImagesDialog(filenames, this);
  // The import dialog should be deleted before leaving this function

  if (importDialog->GetEffectiveCount() < 1)
  {
    delete importDialog;
    importDialog = NULL;
    return true;
  }

  int result = importDialog->exec();

  if (result == QDialog::Rejected)
  {
    delete importDialog;
    importDialog = NULL;
    return false;
  }

  if (result == QDialog::Accepted)
  {
    // AbstractWorker will be automatically deleted by BackgroundTask in
    // ::Import().
    OverviewBuilder* builder = new OverviewBuilder(importDialog->GetGDALOverviewsBuilders());

    delete importDialog;
    importDialog = NULL;

    Import(builder);
  }

  if (importDialog)
  {
    delete importDialog;
    importDialog = NULL;
  }
  return true;
}

/*****************************************************************************/
QObject* I18nMainWindow::Import(AbstractWorker* importer)
{
  assert(importer);

  //
  // Background task.

  // New background-task running worker.
  // Will be self auto-deleted when worker has finished.
  BackgroundTask* task = new BackgroundTask(importer, false, this);

  //
  // Progress dialog.
  TaskProgressDialog progress(task, this, Qt::CustomizeWindowHint | Qt::WindowTitleHint);

  progress.setWindowModality(Qt::WindowModal);
  progress.setAutoReset(false);
  progress.setAutoClose(false);
  progress.setCancelButton(NULL);
  progress.setMinimumDuration(0);

  //
  // Result.
  int button = progress.Exec();

  // MANTIS-921 (synchronize deletion of BackgroungTask).
  task->wait();
  delete task;
  task = NULL;

  // MANTIS-921 (then, process result).
  if (button != QDialog::Accepted)
  {
    assert(progress.GetObject() == NULL);

    return NULL;
  }

  // qDebug() << "object:" << progress.GetObject< DatasetModel >();

  // assert( progress.GetObject()!=NULL );

  return progress.GetObject();
}

/*****************************************************************************/
void I18nMainWindow::closeEvent(QCloseEvent* e)
{
  QMainWindow::closeEvent(e);
}

/*****************************************************************************/
void I18nMainWindow::virtual_InitializeUI()
{
  // Change to NULL model to force emitting GUI signals when GUI is
  // instantiated. So, GUI will be initialized and controller-widgets
  // disabled.
  I18nApplication::Instance()->SetModel(NULL);
}

/*****************************************************************************/
void I18nMainWindow::SaveLayout(int version) const
{
  // qDebug() << this << "::SaveLayout()";

  assert(I18nCoreApplication::Instance() != NULL);

  QString name(objectName());

  I18nCoreApplication::Instance()->StoreSettingsKey(name + "Geometry", saveGeometry());

  I18nCoreApplication::Instance()->StoreSettingsKey(name + "State", saveState(version));
}

/*****************************************************************************/
bool I18nMainWindow::RestoreLayout(int version)
{
  // qDebug() << this << "::RestoreLayout()";

  I18nCoreApplication* application = I18nCoreApplication::Instance();
  assert(application != NULL);

  QString name(objectName());
  assert(!name.isEmpty());

  if (!restoreGeometry(application->RetrieveSettingsKey(name + "Geometry").toByteArray()))
    return false;

  return restoreState(application->RetrieveSettingsKey(name + "State").toByteArray(), version);
}

/*****************************************************************************/
/* SLOTS                                                                     */
/*****************************************************************************/
void I18nMainWindow::on_action_Quit_triggered()
{
  close();
}

/*****************************************************************************/
void I18nMainWindow::on_action_About_triggered()
{
  AboutDialog aboutDialog(this);

  aboutDialog.exec();
}

void I18nMainWindow::on_action_Documentation_triggered()
{
  QDesktopServices::openUrl(QUrl("https://www.orfeo-toolbox.org/CookBook/"));
}


/*****************************************************************************/

} // end namespace 'mvd'
