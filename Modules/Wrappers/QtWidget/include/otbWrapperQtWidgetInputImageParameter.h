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

#ifndef otbWrapperQtWidgetInputImageParameter_h
#define otbWrapperQtWidgetInputImageParameter_h

#include <QtWidgets>
#include "otbWrapperInputImageParameter.h"
#include "otbWrapperQtWidgetParameterBase.h"


namespace otb
{
namespace Wrapper
{

/** \class QtWidgetInputImageParameter
 * \brief
 *
 * \ingroup OTBQtWidget
 */
class OTBQtWidget_EXPORT QtWidgetInputImageParameter : public QtWidgetParameterBase
{
  Q_OBJECT
public:
  QtWidgetInputImageParameter(InputImageParameter*, QtWidgetModel*, QWidget*);
  ~QtWidgetInputImageParameter() override;

  const QLineEdit* GetInput() const;
  QLineEdit*       GetInput();

signals:
  void FileNameIsSet();

protected slots:
  bool SetFileName(const QString& value);
  void SelectFile();

private slots:
  void OnEditingFinished();

private:
  QtWidgetInputImageParameter(const QtWidgetInputImageParameter&) = delete;
  void operator=(const QtWidgetInputImageParameter&) = delete;

  void DoCreateWidget() override;

  void DoUpdateGUI() override;


  InputImageParameter::Pointer m_InputImageParam;

  QHBoxLayout* m_HLayout;
  QLineEdit*   m_Input;
  QPushButton* m_Button;
};


} // Wrapper

} // otb

#endif
