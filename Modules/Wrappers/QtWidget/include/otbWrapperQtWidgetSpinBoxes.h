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

#ifndef otbWrapperQtWidgetSpinBoxes_h
#define otbWrapperQtWidgetSpinBoxes_h

#include <QIcon>
#include <QLineEdit>
#include <QSpinBox>

#include "otbWrapperQtWidgetParameterBase.h"

namespace otb
{
namespace Wrapper
{

// A QLineEdit with a built-in reset button
class OTBQtWidget_EXPORT QtWidgetLineEdit : public QLineEdit
{
  Q_OBJECT

public:
  QtWidgetLineEdit(QWidget* parent = nullptr);

  // Show, hide or get state of the clear button
  void EnableClearButton();
  void DisableClearButton();

  static const int CLEAR_ICON_SIZE = 16;

signals:
  // Triggered when the clear button is pressed
  void Cleared();

private:
  QIcon    m_ClearIcon;
  QAction* m_ClearAction = nullptr;
};

// A QSpinBox with QtWidgetLineEdit as a custom LineEdit
class OTBQtWidget_EXPORT QtWidgetSpinBox : public QSpinBox
{
  Q_OBJECT

public:
  explicit QtWidgetSpinBox(QWidget* parent = nullptr);

  int valueFromText(const QString& text) const override;
  QValidator::State validate(QString& input, int& pos) const override;

  // Show, hide or get state of the clear button
  void EnableClearButton();
  void DisableClearButton();
  bool IsClearButtonEnabled() const;

  void SetValueNoSignal(int);

signals:
  // Triggered when the clear button is pressed
  void Cleared();

private:
  QtWidgetLineEdit* m_LineEdit;
};

// A QDoubleSpinBox with QtWidgetLineEdit as a custom LineEdit
// and custom handling of textFromValue / valueFromText for variable precision decimals
class OTBQtWidget_EXPORT QtWidgetDoubleSpinBox : public QDoubleSpinBox
{
  Q_OBJECT

public:
  explicit QtWidgetDoubleSpinBox(QWidget* parent = nullptr);

  double valueFromText(const QString& text) const override;
  QString textFromValue(double value) const override;
  QValidator::State validate(QString& input, int& pos) const override;

  // Show, hide or get state of the clear button
  void EnableClearButton();
  void DisableClearButton();
  bool IsClearButtonEnabled() const;

  void SetValueNoSignal(double);

signals:
  // Triggered when the clear button is pressed
  void Cleared();

private:
  QtWidgetLineEdit* m_LineEdit;
};
}
}

#endif
