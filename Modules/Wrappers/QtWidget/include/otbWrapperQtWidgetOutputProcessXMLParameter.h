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

#ifndef otbWrapperQtWidgetOutputProcessXMLParameter_h
#define otbWrapperQtWidgetOutputProcessXMLParameter_h

#include <QtWidgets>
#include "otbWrapperOutputProcessXMLParameter.h"
#include "otbWrapperQtWidgetParameterBase.h"


namespace otb
{
namespace Wrapper
{

/** \class QtWidgetOutputProcessXMLParameter
 * \brief
 *
 * \ingroup OTBQtWidget
 */
class OTBQtWidget_EXPORT QtWidgetOutputProcessXMLParameter : public QtWidgetParameterBase
{
  Q_OBJECT
public:
  QtWidgetOutputProcessXMLParameter(OutputProcessXMLParameter*, QtWidgetModel*, QWidget*);
  ~QtWidgetOutputProcessXMLParameter() override;

  const QLineEdit* GetInput() const;
  QLineEdit* GetInput();

public slots:
  void SetFileName( const QString& value );

protected slots:
  void SelectFile();

private:
  QtWidgetOutputProcessXMLParameter(const QtWidgetOutputProcessXMLParameter&) = delete;
  void operator=(const QtWidgetOutputProcessXMLParameter&) = delete;

  void DoCreateWidget() override;

  void DoUpdateGUI() override;


  OutputProcessXMLParameter::Pointer m_XMLParam;

  QHBoxLayout * m_HLayout;
  QLineEdit*    m_Input;
  QPushButton * m_Button;
};


}
}

#endif
