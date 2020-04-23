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

#ifndef otbWrapperQtWidgetOutputFilenameParameter_h
#define otbWrapperQtWidgetOutputFilenameParameter_h

#include <QtWidgets>
#include "otbWrapperOutputFilenameParameter.h"
#include "otbWrapperQtWidgetParameterBase.h"


namespace otb
{
namespace Wrapper
{

/** \class QtWidgetOutputFilenameParameter
 * \brief
 *
 * \ingroup OTBQtWidget
 */
class OTBQtWidget_EXPORT QtWidgetOutputFilenameParameter : public QtWidgetParameterBase
{
  Q_OBJECT
public:
  QtWidgetOutputFilenameParameter(OutputFilenameParameter*, QtWidgetModel*, QWidget*);
  ~QtWidgetOutputFilenameParameter() override;

  const QLineEdit* GetInput() const;
  QLineEdit*       GetInput();

public slots:
  void SetFileName(const QString& value);

protected slots:
  void SelectFile();

private:
  QtWidgetOutputFilenameParameter(const QtWidgetOutputFilenameParameter&) = delete;
  void operator=(const QtWidgetOutputFilenameParameter&) = delete;

  void DoCreateWidget() override;

  void DoUpdateGUI() override;


  OutputFilenameParameter::Pointer m_FilenameParam;

  QHBoxLayout* m_HLayout;
  QLineEdit*   m_Input;
  QPushButton* m_Button;
};
}
}

#endif
