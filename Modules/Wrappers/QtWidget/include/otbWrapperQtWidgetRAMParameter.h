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

#ifndef otbWrapperQtWidgetRAMParameter_h
#define otbWrapperQtWidgetRAMParameter_h

#include <QtWidgets>
#include "otbWrapperRAMParameter.h"
#include "otbWrapperQtWidgetParameterBase.h"


namespace otb
{
namespace Wrapper
{

/** \class QtWidgetRAMParameter
 * \brief
 *
 * \ingroup OTBQtWidget
 */
class OTBQtWidget_EXPORT QtWidgetRAMParameter : public QtWidgetParameterBase
{
  Q_OBJECT
public:
  QtWidgetRAMParameter(RAMParameter*, QtWidgetModel*, QWidget*);
  ~QtWidgetRAMParameter() override;

protected slots:
  void SetValue( int value );

private:
  QtWidgetRAMParameter(const QtWidgetRAMParameter&) = delete;
  void operator=(const QtWidgetRAMParameter&) = delete;

  void DoCreateWidget() override;

  void DoUpdateGUI() override;

  QHBoxLayout *         m_QHBoxLayout;
  QSpinBox *            m_QSpinBox;

  RAMParameter::Pointer m_RAMParam;
};


}
}

#endif
