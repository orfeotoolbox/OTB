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

#ifndef otbWrapperQtWidgetChoiceParameter_h
#define otbWrapperQtWidgetChoiceParameter_h

#include <QtWidgets>
#include "otbWrapperChoiceParameter.h"
#include "otbWrapperQtWidgetParameterBase.h"

namespace otb
{
namespace Wrapper
{

/** \class QtWidgetChoiceParameter
 * \brief
 *
 * \ingroup OTBQtWidget
 */
class OTBQtWidget_EXPORT QtWidgetChoiceParameter : public QtWidgetParameterBase
{
  Q_OBJECT
public:
  QtWidgetChoiceParameter(ChoiceParameter*, QtWidgetModel*, QWidget*);
  ~QtWidgetChoiceParameter() override;

protected Q_SLOTS:
  void SetValue(int value);

private:
  QtWidgetChoiceParameter(const QtWidgetChoiceParameter&) = delete;
  void operator=(const QtWidgetChoiceParameter&) = delete;

  void DoCreateWidget() override;

  void DoUpdateGUI() override;

  ChoiceParameter::Pointer m_ChoiceParam;

  QHBoxLayout* m_MainHLayout;

  QComboBox*      m_ComboBox;
  QStackedWidget* m_StackWidget;

  QVBoxLayout* m_VLayout;
  QGroupBox*   m_VLayoutGroup;

  typedef std::vector<QtWidgetParameterBase*> WidgetListType;
  typedef WidgetListType::iterator            WidgetListIteratorType;
  WidgetListType                              m_WidgetList;
};
}
}

#endif
