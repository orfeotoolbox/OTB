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
#include "otbWrapperQtWidgetOutputProcessXMLParameter.h"

#include "ui_appmainwindow.h"

namespace otb
{
namespace Wrapper
{

QtMainWindow::QtMainWindow(Application::Pointer app, QtWidgetView* gui, QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::AppMainWindow),
    gui(gui),
    cmdLineView(new QPlainTextEdit)
{
  ui->setupUi(this);

  // Setup the "View command line" widget
  this->setWindowTitle(QString(app->GetName()).append(" - OTB ").append(OTB_VERSION_STRING));
  cmdLineView->setWindowTitle(this->windowTitle());
  cmdLineView->setReadOnly(true);
  cmdLineView->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  cmdLineView->setMinimumSize(600, 130);
  cmdLineView->resize(600, 130);
  this->UpdateCommandLine();

  // Set the given application view widget
  gui->setParent(this);
  ui->scrollArea->setWidget(gui);

  // Connect menu buttons
  connect(ui->actionQuit, &QAction::triggered, this, &QMainWindow::close);
  const auto url = std::string("https://www.orfeo-toolbox.org/CookBook/Applications/app_") + app->GetName() + std::string(".html");
  connect(ui->actionDocumentation, &QAction::triggered, this, [=] { QDesktopServices::openUrl(QUrl(QString::fromStdString(url))); });
  connect(ui->actionView_command_line, &QAction::triggered, cmdLineView, &QWidget::show);

  // Setup execute / cancel button
  ui->executeButton->setDefault(true);
  ui->executeButton->setEnabled(false);
  ui->executeButton->setText(QObject::tr("Execute"));

  connect(gui->GetModel(), &QtWidgetModel::SetApplicationReady, ui->executeButton, &QPushButton::setEnabled);
  connect(this, &QtMainWindow::ExecuteAndWriteOutput, gui->GetModel(), &QtWidgetModel::ExecuteAndWriteOutputSlot);

  connect( gui->GetModel(), &QtWidgetModel::SetApplicationReady, this, &QtMainWindow::UpdateMessageAfterApplicationReady );
  connect( gui->GetModel(), &QtWidgetModel::SetProgressReportDone, this, &QtMainWindow::UpdateMessageAfterExecution );

  connect(gui->GetModel(), &QtWidgetModel::ExceptionRaised, [&] { ui->tabWidget->setCurrentIndex(1); });

  // Status bar and message default text
  ui->statusBar->showMessage(tr("Select parameters"));
  ui->message->setText("");

  // Setup the progress bar to observe the model
  ui->progressBar->SetModel(gui->GetModel());

  // Connect application progress text to the QLabel
  connect(ui->progressBar, &QtWidgetSimpleProgressReport::SetText, ui->message, &QLabel::setText);

  // Connect log output to the textEdit area
  connect(gui->GetModel()->GetLogOutput(), &QtLogOutput::NewContentLog, ui->plainTextEdit, &QPlainTextEdit::appendPlainText);

  // Update the "view command line" widget content when application parameters change
  connect(gui->GetModel(), &QtWidgetModel::UpdateGui, this, &QtMainWindow::UpdateCommandLine);
}

void QtMainWindow::UpdateMessageAfterApplicationReady( bool val )
{
  if (!gui->GetModel()->IsRunning())
  {
    if (val == true)
    {
      ui->statusBar->showMessage(tr("Ready to run"));
    }
    else
    {
      ui->statusBar->showMessage(tr("Select parameters"));
    }
  }
}

void QtMainWindow::UpdateMessageAfterExecution(int status)
{
  if (status >= 0)
  {
    ui->statusBar->showMessage(tr("Done"));
  }
  else
  {
    ui->statusBar->showMessage(tr("Failed!"));
  }
  ui->executeButton->setText(tr("Execute"));
  gui->Enable();
}

void QtMainWindow::on_executeButton_clicked()
{
  if (gui->GetModel()->IsRunning())
  {
    ui->statusBar->showMessage(tr("Cancelling..."));
    gui->GetModel()->Stop();
  }
  else
  {
    gui->Disable();
    gui->BeforeExecuteButtonClicked();
    ui->statusBar->showMessage(tr("Running..."));
    ui->executeButton->setText(tr("Cancel"));
    emit ExecuteAndWriteOutput();
  }
}

void QtMainWindow::UpdateCommandLine()
{
  cmdLineView->clear();
  auto cmdLine = OutputProcessXMLParameter::MakeCommandLine(gui->GetModel()->m_Application);
  cmdLineView->appendPlainText(QString::fromStdString(cmdLine));
}

QtMainWindow::~QtMainWindow()
{
  delete ui;
  delete cmdLineView;
}

otb::Wrapper::QtWidgetView* QtMainWindow::Gui() const
{
  return gui;
}

void QtMainWindow::UnhandledException(QString message)
{
  ui->plainTextEdit->appendPlainText(message);
  ui->tabWidget->setCurrentIndex(1);
}

void QtMainWindow::closeEvent(QCloseEvent* event)
{
  gui->GetModel()->Stop();
  cmdLineView->close();
  QMainWindow::closeEvent(event);
}

} // namespace Wrapper
} // namespace otb
