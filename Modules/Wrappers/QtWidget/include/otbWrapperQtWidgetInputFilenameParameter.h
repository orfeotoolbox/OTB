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

#ifndef otbWrapperQtWidgetInputFilenameParameter_h
#define otbWrapperQtWidgetInputFilenameParameter_h

#include <QtWidgets>
#include "otbWrapperInputFilenameParameter.h"
#include "otbWrapperQtWidgetParameterBase.h"


namespace otb
{
namespace Wrapper
{

/** \class QtWidgetInputFilenameParameter
 * \brief
 *
 * \ingroup OTBQtWidget
 */
class OTBQtWidget_EXPORT QtWidgetInputFilenameParameter : public QtWidgetParameterBase
{
  Q_OBJECT
public:
  QtWidgetInputFilenameParameter(InputFilenameParameter*, QtWidgetModel*);
  ~QtWidgetInputFilenameParameter() override;

  inline const QLineEdit* GetInput() const;
  inline QLineEdit* GetInput();

protected slots:
  void SetFileName( const QString& value );
  void SelectFile();

private:
  QtWidgetInputFilenameParameter(const QtWidgetInputFilenameParameter&); //purposely not implemented
  void operator=(const QtWidgetInputFilenameParameter&); //purposely not implemented

  void DoCreateWidget() override;

  void DoUpdateGUI() override;


  InputFilenameParameter::Pointer m_FilenameParam;

  QHBoxLayout * m_HLayout;
  QLineEdit*    m_Input;
  QPushButton * m_Button;
};


inline
const QLineEdit*
QtWidgetInputFilenameParameter
::GetInput() const
{
  return m_Input;
}

inline
QLineEdit*
QtWidgetInputFilenameParameter
::GetInput()
{
  return m_Input;
}

} // Wrapper

} // otb

#endif
