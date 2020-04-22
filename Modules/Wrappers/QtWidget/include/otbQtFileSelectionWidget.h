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

#ifndef otbQtFileSelectionWidget_h
#define otbQtFileSelectionWidget_h

#include <QtWidgets>
#include "otbWrapperInputImageListParameter.h"
#include "otbWrapperInputFilenameListParameter.h"
#include "otbWrapperQtWidgetParameterBase.h"

#include "OTBQtWidgetExport.h"
#include <string>

namespace otb
{
namespace Wrapper
{

/** \class QtFileSelectionWidget
 * \brief
 *
 * \ingroup OTBQtWidget
 */
class OTBQtWidget_EXPORT QtFileSelectionWidget : public QWidget
{
  Q_OBJECT
public:
  enum IOMode
  {
    IO_MODE_INPUT  = 0,
    IO_MODE_OUTPUT = 1,
  };

  QtFileSelectionWidget();
  ~QtFileSelectionWidget() override;

  bool IsChecked()
  {
    return m_Checkbox->isChecked();
  }

  void SetChecked(bool val)
  {
    return m_Checkbox->setChecked(val);
  }

  std::string GetFilename()
  {
    return QFile::encodeName(m_Input->text()).constData();
  }

  void ClearFilename()
  {
    m_Input->clear();
  }

  QLineEdit* GetInput()
  {
    return m_Input;
  }

  void   SetIOMode(IOMode);
  IOMode GetIOMode() const;

Q_SIGNALS:
  void FilenameChanged();

protected Q_SLOTS:
  void SelectFile();
  void CallFilenameChanged();
  void SetFileName(const QString&);

private:
  QtFileSelectionWidget(const QtFileSelectionWidget&) = delete;
  void operator=(const QtFileSelectionWidget&) = delete;

  virtual void DoCreateWidget();

  virtual void DoUpdateGUI();


  QHBoxLayout* m_HLayout;
  QLineEdit*   m_Input;
  QPushButton* m_Button;
  QCheckBox*   m_Checkbox;
  IOMode       m_IOMode;
};
}
}

#endif
