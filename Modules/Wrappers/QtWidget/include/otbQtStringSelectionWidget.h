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

#ifndef otbQtStringSelectionWidget_h
#define otbQtStringSelectionWidget_h

#include <QtWidgets>
#include "otbWrapperStringListParameter.h"
#include "otbWrapperQtWidgetParameterBase.h"
#include "OTBQtWidgetExport.h"
#include <string>

namespace otb
{
namespace Wrapper
{

/** \class QtStringSelectionWidget
 * \brief
 *
 * \ingroup OTBQtWidget
 */
class OTBQtWidget_EXPORT QtStringSelectionWidget : public QWidget
{
  Q_OBJECT

  Q_PROPERTY(QString text READ GetText WRITE SetText RESET ClearText);

signals:
  void InternalQLineEditEditionFinished();

public:
  QtStringSelectionWidget();
  ~QtStringSelectionWidget() override;

  bool IsChecked() const;

  void SetChecked(bool val);

  const QString GetText() const;

  void SetText(const QString& qString);

  std::string ToStdString();

  void ClearText();

protected slots:
  void OnEditionFinished();

private:
  QtStringSelectionWidget(const QtStringSelectionWidget&) = delete;
  void operator=(const QtStringSelectionWidget&) = delete;

  virtual void DoCreateWidget();

  virtual void DoUpdateGUI();


  QHBoxLayout* m_HLayout;
  QLineEdit*   m_Input;
  QCheckBox*   m_Checkbox;
};
}
}

#endif
