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

#ifndef otbWrapperQtWidgetParameterBase_h
#define otbWrapperQtWidgetParameterBase_h

#include <QtWidgets>
#include "otbWrapperParameter.h"
#include "otbWrapperQtWidgetModel.h"
#include "OTBQtWidgetExport.h"

namespace otb
{
namespace Wrapper
{

class QtWidgetParameterGroup;

/** \class QtWidgetParameterBase
 * \brief
 *
 * \ingroup OTBQtWidget
 */
class OTBQtWidget_EXPORT QtWidgetParameterBase : public QWidget
{
  Q_OBJECT
  friend class QtWidgetParameterGroup;

public:
  QtWidgetParameterBase(Parameter*, QtWidgetModel*, QWidget* parent);
  ~QtWidgetParameterBase() override;

  void CreateWidget();

  /** Store the state of the check box relative to this parameter
    */
  virtual bool IsChecked() const
  {
    return m_IsChecked;
  }

  /** Modify the state of the checkbox relative to this parameter */
  virtual void SetChecked(const bool value)
  {
    m_IsChecked = value;
  }

public slots:
  void         UpdateGUI();
  virtual void SetActivationState(bool value);

protected slots:
  void ParameterChanged(const QString& key);

signals:
  void ParameterActiveStatus(bool value);

protected:
  QtWidgetModel* GetModel();

  const Parameter* GetParam() const;

  Parameter* GetParam();

  bool eventFilter(QObject* o, QEvent* e) override;

private:
  QtWidgetParameterBase(const QtWidgetParameterBase&) = delete;
  void operator=(const QtWidgetParameterBase&) = delete;

  virtual void DoUpdateGUI() = 0;

  virtual void DoCreateWidget() = 0;

private:
  QtWidgetModel* m_Model;

  Parameter* m_Param;

  /** Store the status of the checkbox */
  bool m_IsChecked;
};
}
}

#endif
