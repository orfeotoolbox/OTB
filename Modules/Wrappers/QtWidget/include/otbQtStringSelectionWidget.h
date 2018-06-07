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

#ifndef otbQtStringSelectionWidget_h
#define otbQtStringSelectionWidget_h

#include <QtWidgets>
#include "otbWrapperStringListParameter.h"
#include "otbWrapperQtWidgetParameterBase.h"
#include "OTBQtWidgetExport.h"

namespace otb
{
namespace Wrapper
{

/** \class QtStringSelectionWidget
 * \brief
 *
 * \ingroup OTBQtWidget
 */
class OTBQtWidget_EXPORT QtStringSelectionWidget : public QWidget
{
  Q_OBJECT

  Q_PROPERTY( QString text READ GetText WRITE SetText RESET ClearText );

signals:
  void InternalQLineEditEditionFinished();

public:
  QtStringSelectionWidget();
  ~QtStringSelectionWidget() override;

  bool IsChecked()
  {
    return m_Checkbox->isChecked();
  }

  void SetChecked( bool val )
  {
    return m_Checkbox->setChecked( val );
  }

  inline const QString GetText() const
  {
    return m_Input->text();
  }

  inline void SetText( const QString& qString)
  {
    m_Input->setText(qString);
  }

  std::string ToStdString()
  {
    return m_Input->text().toLatin1().constData();
  }

  void ClearText()
  {
    m_Input->clear();
  }


protected slots:
   void OnEditionFinished();

private:
  QtStringSelectionWidget(const QtStringSelectionWidget&); //purposely not implemented
  void operator=(const QtStringSelectionWidget&); //purposely not implemented

  virtual void DoCreateWidget();

  virtual void DoUpdateGUI();


  QHBoxLayout * m_HLayout;
  QLineEdit*    m_Input;
  QCheckBox *   m_Checkbox;
};


}
}

#endif
