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

#ifndef otbWrapperQtWidgetStringListParameter_h
#define otbWrapperQtWidgetStringListParameter_h

#include <QtGui>
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829  //tag=QT4-boost-compatibility
#include "otbQtStringSelectionWidget.h"
#endif //tag=QT4-boost-compatibility

namespace otb
{
namespace Wrapper
{

/** \class QtWidgetStringListParameter
 * \brief
 *
 * \ingroup OTBQtWidget
 */
class OTBQtWidget_EXPORT QtWidgetStringListParameter : public QtWidgetParameterBase
{
  Q_OBJECT
public:
  QtWidgetStringListParameter(StringListParameter*, QtWidgetModel*);
  ~QtWidgetStringListParameter() override;

signals:
  void Change();

protected slots:
  void SetString( const QString& value );
  virtual void AddString();
  virtual void SuppressString();
  virtual void UpdateStringList();

private:
  QtWidgetStringListParameter(const QtWidgetStringListParameter&); //purposely not implemented
  void operator=(const QtWidgetStringListParameter&); //purposely not implemented

  void DoCreateWidget() override;

  void DoUpdateGUI() override;

  StringListParameter::Pointer m_StringListParam;

  QHBoxLayout * m_HLayout;
  QVBoxLayout * m_StringLayout;
  QScrollArea * m_Scroll;

  std::vector<QtStringSelectionWidget *> m_LineEditList;
};


}
}

#endif
