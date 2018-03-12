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

#ifndef otbWrapperQtWidgetInputVectorDataParameter_h
#define otbWrapperQtWidgetInputVectorDataParameter_h

#include <QtGui>
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829  //tag=QT4-boost-compatibility
#include "otbWrapperInputVectorDataParameter.h"
#include "otbWrapperQtWidgetParameterBase.h"
#endif //tag=QT4-boost-compatibility


namespace otb
{
namespace Wrapper
{

/** \class QtWidgetInputVectorDataParameter
 * \brief
 *
 * \ingroup OTBQtWidget
 */
class OTBQtWidget_EXPORT QtWidgetInputVectorDataParameter : public QtWidgetParameterBase
{
  Q_OBJECT
public:
  QtWidgetInputVectorDataParameter(InputVectorDataParameter*, QtWidgetModel*);
  ~QtWidgetInputVectorDataParameter() override;

  inline const QLineEdit* GetInput() const;
  inline QLineEdit* GetInput();

protected slots:
  bool SetFileName( const QString& value );
  void SelectFile();

private:
  QtWidgetInputVectorDataParameter(const QtWidgetInputVectorDataParameter&); //purposely not implemented
  void operator=(const QtWidgetInputVectorDataParameter&); //purposely not implemented

  void DoCreateWidget() override;

  void DoUpdateGUI() override;


  InputVectorDataParameter::Pointer m_InputVectorDataParam;

  QHBoxLayout * m_HLayout;
  QLineEdit*    m_Input;
  QPushButton * m_Button;
};


inline
const QLineEdit*
QtWidgetInputVectorDataParameter
::GetInput() const
{
  return m_Input;
}

inline
QLineEdit*
QtWidgetInputVectorDataParameter
::GetInput()
{
  return m_Input;
}

} // Wrapper

} // otb

#endif
