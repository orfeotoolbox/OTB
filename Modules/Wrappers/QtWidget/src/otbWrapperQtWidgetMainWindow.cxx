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

#include "otbWrapperQtWidgetMainWindow.h"

#include <QtWidgets>
#include "otbWrapperQtWidgetView.h"

#include "ui_appmainwindow.h"

namespace otb
{
namespace Wrapper
{

QtMainWindow::QtMainWindow(Application::Pointer app, QtWidgetView* gui, QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::AppMainWindow),
    gui(gui)
{
  ui->setupUi(this);
  this->setWindowIcon(QIcon(":/otb_small.png"));
  this->setWindowTitle(QString(app->GetDocName()).append(" - ").append(OTB_VERSION_STRING));

  // Set the given application view widget
  gui->setParent(this);
  ui->scrollArea->setWidget(gui);

  // Connect menu buttons
  connect(ui->actionQuit, &QAction::triggered, this, &QMainWindow::close);

  const auto url = std::string("https://www.orfeo-toolbox.org/CookBook/Applications/app_") + app->GetName() + std::string(".html");
  connect(ui->actionDocumentation, &QAction::triggered, this, [=] { QDesktopServices::openUrl(QUrl(QString::fromStdString(url))); });
}

QtMainWindow::~QtMainWindow()
{
  delete ui;
}

void QtMainWindow::UnhandledException(QString message)
{
  gui->UnhandledException(message);
}

} // namespace Wrapper
} // namespace otb
