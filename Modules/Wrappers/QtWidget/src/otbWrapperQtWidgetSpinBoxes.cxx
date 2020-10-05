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

#include "otbWrapperQtWidgetSpinBoxes.h"

#include <sstream>
#include <limits>
#include <locale>
#include <QAction>

namespace otb
{
namespace Wrapper
{

QtWidgetLineEdit::QtWidgetLineEdit(QWidget* parent) : QLineEdit(parent)
{
  // Setup the clear button icon
  m_ClearIcon.addPixmap(QIcon(":/Utilities/Data/Icons/mIconClearText.png").pixmap(QSize(CLEAR_ICON_SIZE, CLEAR_ICON_SIZE)), QIcon::Normal, QIcon::On);
  m_ClearIcon.addPixmap(QIcon(":/Utilities/Data/Icons/mIconClearTextHover.png").pixmap(QSize(CLEAR_ICON_SIZE, CLEAR_ICON_SIZE)), QIcon::Selected, QIcon::On);
}

void QtWidgetLineEdit::EnableClearButton()
{
  if (m_ClearAction == nullptr)
  {
    m_ClearAction = new QAction(m_ClearIcon, QString("Reset"), this);
    m_ClearAction->setCheckable(false);
    this->addAction(m_ClearAction, QLineEdit::TrailingPosition);

    // Forward the trigger signal
    connect(m_ClearAction, &QAction::triggered, this, &QtWidgetLineEdit::Cleared);
  }
}

void QtWidgetLineEdit::DisableClearButton()
{
  if (m_ClearAction != nullptr)
  {
    m_ClearAction->deleteLater();
    m_ClearAction = nullptr;
  }
}

QtWidgetSpinBox::QtWidgetSpinBox(QWidget* parent) : QSpinBox(parent)
{
  // Use a custom LineEdit a forward its Cleared signal
  m_LineEdit = new QtWidgetLineEdit(this);
  this->setLineEdit(m_LineEdit);
  connect(m_LineEdit, &QtWidgetLineEdit::Cleared, this, &QtWidgetSpinBox::Cleared);

  // Small Qt hack to prevent highlighting the text after it has changed (to improve UX a bit)
  connect(this, static_cast<void (QtWidgetSpinBox::*)(int)>(&QtWidgetSpinBox::valueChanged), this, [&](int) { m_LineEdit->deselect(); }, Qt::QueuedConnection);

  // Add icon size and a 10px margin to minimum size hint
  QSize msize = minimumSizeHint();
  setMinimumSize(msize.width() + QtWidgetLineEdit::CLEAR_ICON_SIZE + 10, std::max(msize.height(), QtWidgetLineEdit::CLEAR_ICON_SIZE + 10));
}

void QtWidgetSpinBox::EnableClearButton()
{
  m_LineEdit->EnableClearButton();
}

void QtWidgetSpinBox::DisableClearButton()
{
  m_LineEdit->DisableClearButton();
}

// Set the SpinBox value without triggering its valueChanged signal
void QtWidgetSpinBox::SetValueNoSignal(int value)
{
  this->blockSignals(true);
  this->setValue(value);
  this->blockSignals(false);
}

// We use a custom valueFromText to allow more flexible input than QSpinBox default behavior:
// - all inputs are allowed in our custom QtWidgetSpinBox::validate()
// - this method parses the text to int, if it fails keep the previous value
int QtWidgetSpinBox::valueFromText(const QString& text) const
{
  bool ok;
  // Force C locale because OTB gui is not i18n
  int result = QLocale::c().toInt(text, &ok);
  if (ok)
  {
    return result;
  }
  else
  {
    return this->value();
  }
}

QValidator::State QtWidgetSpinBox::validate(QString&, int&) const
{
  return QValidator::Acceptable;
}

QtWidgetDoubleSpinBox::QtWidgetDoubleSpinBox(QWidget* parent) : QDoubleSpinBox(parent)
{
  // Use a custom LineEdit and forward its Cleared signal
  m_LineEdit = new QtWidgetLineEdit(this);
  this->setLineEdit(m_LineEdit);
  connect(m_LineEdit, &QtWidgetLineEdit::Cleared, this, &QtWidgetDoubleSpinBox::Cleared);

  // Small Qt hack to prevent highlighting the text after it has changed (to improve UX a bit)
  connect(this, static_cast<void (QtWidgetDoubleSpinBox::*)(double)>(&QtWidgetDoubleSpinBox::valueChanged), this, [&](double) { m_LineEdit->deselect(); },
          Qt::QueuedConnection);

  // Add icon size and a 10px margin to minimum size hint
  QSize msize = minimumSizeHint();
  setMinimumSize(msize.width() + QtWidgetLineEdit::CLEAR_ICON_SIZE + 10, std::max(msize.height(), QtWidgetLineEdit::CLEAR_ICON_SIZE + 10));
}

void QtWidgetDoubleSpinBox::EnableClearButton()
{
  m_LineEdit->EnableClearButton();
}

void QtWidgetDoubleSpinBox::DisableClearButton()
{
  m_LineEdit->DisableClearButton();
}

// Set the SpinBox value without triggering its valueChanged signal
void QtWidgetDoubleSpinBox::SetValueNoSignal(double value)
{
  this->blockSignals(true);
  this->setValue(value);
  this->blockSignals(false);
}

// We use a custom valueFromText to allow more flexible input than QDoubleSpinBox default behavior:
// - all inputs are allowed in our custom QtWidgetDoubleSpinBox::validate()
// - this method parses the text to double, if it fails keep the previous value
double QtWidgetDoubleSpinBox::valueFromText(const QString& text) const
{
  bool ok;
  // Force C locale because OTB gui is not i18n
  double result = QLocale::c().toDouble(text, &ok);
  if (ok)
  {
    return result;
  }
  else
  {
    return this->value();
  }
}

QString QtWidgetDoubleSpinBox::textFromValue(double value) const
{
  // Format the spinbox value for display
  // We overwrite the default QDoubleSpinBox::textFromValue, because it uses fixed precision,
  // which leads to ugly trailing zeros for small values (e.g 1.50000)
  // We use std::ostringstream because QString::arg formatting support is too limited
  std::ostringstream oss;

  // Force C locale because OTB gui is not i18n
  oss.imbue(std::locale::classic());

  // Set precision to the number of decimal digits that can be represented without change.
  // Use float precision because OTB parameter is float
  // For IEEE float it's 6.
  oss.precision(std::numeric_limits<float>::digits10);
  oss << value;

  // Add a trailing dot if the number is integer,
  // so that int and float parameters are more visually different.
  // This is an ok convention as long as we stay in C or english locale
  const char dot = std::use_facet<std::numpunct<char>>(std::locale::classic()).decimal_point();
  if (oss.str().find(dot) == std::string::npos)
  {
    oss << dot;
  }

  return QString::fromStdString(oss.str());
}

QValidator::State QtWidgetDoubleSpinBox::validate(QString&, int&) const
{
  return QValidator::Acceptable;
}
}
}
