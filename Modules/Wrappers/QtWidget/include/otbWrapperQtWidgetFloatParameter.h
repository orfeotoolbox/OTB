/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbWrapperQtWidgetFloatParameter_h
#define otbWrapperQtWidgetFloatParameter_h

#include <QtWidgets>
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829  //tag=QT4-boost-compatibility
#include "otbWrapperNumericalParameter.h"
#include "otbWrapperQtWidgetParameterBase.h"
#endif //tag=QT4-boost-compatibility

namespace otb
{
namespace Wrapper
{

/** \class QtWidgetFloatParameter
 * \brief
 *
 * \ingroup OTBQtWidget
 */
class OTBQtWidget_EXPORT QtWidgetFloatParameter : public QtWidgetParameterBase
{
  Q_OBJECT
public:
  QtWidgetFloatParameter(FloatParameter*, QtWidgetModel*);
  ~QtWidgetFloatParameter() override;

protected slots:
  void SetValue( double value );

private:
  QtWidgetFloatParameter(const QtWidgetFloatParameter&); //purposely not implemented
  void operator=(const QtWidgetFloatParameter&); //purposely not implemented

  void DoCreateWidget() override;

  void DoUpdateGUI() override;

  QHBoxLayout *           m_QHBoxLayout;
  QDoubleSpinBox *        m_QDoubleSpinBox;

  FloatParameter::Pointer m_FloatParam;
};


}
}

#endif
