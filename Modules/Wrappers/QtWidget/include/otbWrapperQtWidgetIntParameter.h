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

#ifndef otbWrapperQtWidgetIntParameter_h
#define otbWrapperQtWidgetIntParameter_h

#include <QtWidgets>
#include "otbWrapperNumericalParameter.h"
#include "otbWrapperQtWidgetParameterBase.h"

#include "otbWrapperQtWidgetSpinBoxes.h"


namespace otb
{
namespace Wrapper
{

/** \class QtWidgetIntParameter
 * \brief
 *
 * \ingroup OTBQtWidget
 */
class OTBQtWidget_EXPORT QtWidgetIntParameter : public QtWidgetParameterBase
{
  Q_OBJECT
public:
  QtWidgetIntParameter(IntParameter*, QtWidgetModel*, QWidget*);
  ~QtWidgetIntParameter() override;

private Q_SLOTS:
  void OnCleared();
  void OnValueChanged(int);
  void OnEditingFinished();

private:
  QtWidgetIntParameter(const QtWidgetIntParameter&) = delete;
  void operator=(const QtWidgetIntParameter&) = delete;

  void DoCreateWidget() override;
  void DoUpdateGUI() override;

  QHBoxLayout*     m_QHBoxLayout;
  QtWidgetSpinBox* m_QSpinBox;

  IntParameter::Pointer m_IntParam;
};
}
}

#endif
