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

#ifndef otbWrapperQtWidgetDirectoryParameter_h
#define otbWrapperQtWidgetDirectoryParameter_h

#include <QtWidgets>
#include "otbWrapperDirectoryParameter.h"
#include "otbWrapperQtWidgetParameterBase.h"


namespace otb
{
namespace Wrapper
{

/** \class QtWidgetDirectoryParameter
 * \brief
 *
 * \ingroup OTBQtWidget
 */
class OTBQtWidget_EXPORT QtWidgetDirectoryParameter : public QtWidgetParameterBase
{
  Q_OBJECT
public:
  QtWidgetDirectoryParameter(DirectoryParameter*, QtWidgetModel*, QWidget*);
  ~QtWidgetDirectoryParameter() override;

protected slots:
  void SetFileName(const QString& value);
  void SelectFile();

private:
  QtWidgetDirectoryParameter(const QtWidgetDirectoryParameter&) = delete;
  void operator=(const QtWidgetDirectoryParameter&) = delete;

  void DoCreateWidget() override;

  void DoUpdateGUI() override;


  DirectoryParameter::Pointer m_DirectoryParam;

  QHBoxLayout* m_HLayout;
  QLineEdit*   m_Input;
  QPushButton* m_Button;
};
}
}

#endif
