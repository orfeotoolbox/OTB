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

#ifndef otbWrapperQtWidgetOutputImageParameter_h
#define otbWrapperQtWidgetOutputImageParameter_h

#include <QtWidgets>
#include "otbWrapperOutputImageParameter.h"
#include "otbWrapperQtWidgetParameterBase.h"
#include <string>


namespace otb
{
namespace Wrapper
{

/** \class QtWidgetOutputImageParameter
 * \brief
 *
 * \ingroup OTBQtWidget
 */
class OTBQtWidget_EXPORT QtWidgetOutputImageParameter : public QtWidgetParameterBase
{
  Q_OBJECT
public:
  QtWidgetOutputImageParameter(OutputImageParameter*, QtWidgetModel*, QWidget*);
  ~QtWidgetOutputImageParameter() override;

  const QLineEdit* GetInput() const;
  QLineEdit*       GetInput();

  /** Get the PixelType*/
  // itkGetMacro(PixelType, int);

public slots:
  void SetFileName(const QString& value);

protected slots:
  void SelectFile();
  void SetPixelType(int pixelType);

private:
  QtWidgetOutputImageParameter(const QtWidgetOutputImageParameter&) = delete;
  void operator=(const QtWidgetOutputImageParameter&) = delete;

  void DoCreateWidget() override;

  void DoUpdateGUI() override;

  std::string                   m_FileName;
  OutputImageParameter::Pointer m_OutputImageParam;

  QHBoxLayout* m_HLayout;
  QLineEdit*   m_Input;
  QPushButton* m_Button;
  QComboBox*   m_ComboBox;
  int          m_PixelType;
};
}
}

#endif
