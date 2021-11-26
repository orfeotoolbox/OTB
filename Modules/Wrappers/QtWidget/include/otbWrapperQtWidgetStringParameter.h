/*
 * Copyright (C) 2005-2021 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbWrapperQtWidgetStringParameter_h
#define otbWrapperQtWidgetStringParameter_h

#include <QtWidgets>
#include "otbWrapperStringParameter.h"
#include "otbWrapperQtWidgetParameterBase.h"

namespace otb
{
namespace Wrapper
{

/** \class QtWidgetStringParameter
 * \brief
 *
 * \ingroup OTBQtWidget
 */
class OTBQtWidget_EXPORT QtWidgetStringParameter : public QtWidgetParameterBase
{
  Q_OBJECT
public:
  QtWidgetStringParameter(StringParameter*, QtWidgetModel*, QWidget*);
  ~QtWidgetStringParameter() override;

protected Q_SLOTS:
  void SetValue(const QString& value);

private:
  QtWidgetStringParameter(const QtWidgetStringParameter&) = delete;
  void operator=(const QtWidgetStringParameter&) = delete;

  void DoCreateWidget() override;

  void DoUpdateGUI() override;

  StringParameter::Pointer m_StringParam;
  QHBoxLayout*             m_HLayout;
  QLineEdit*               m_Input;
};
}
}

#endif
