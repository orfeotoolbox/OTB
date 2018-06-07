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

#ifndef otbWrapperQtWidgetParameterGroup_h
#define otbWrapperQtWidgetParameterGroup_h

#include <QtWidgets>
#include "otbWrapperParameterGroup.h"
#include "otbWrapperRadiusParameter.h"
#include "otbWrapperRAMParameter.h"
#include "otbWrapperQtWidgetParameterBase.h"
#include "otbWrapperInputProcessXMLParameter.h"
#include "otbWrapperOutputProcessXMLParameter.h"

namespace otb
{
namespace Wrapper
{

/** \class QtWidgetParameterGroup
 * \brief
 *
 * \ingroup OTBQtWidget
 */
class OTBQtWidget_EXPORT QtWidgetParameterGroup : public QtWidgetParameterBase
{
  Q_OBJECT
public:
  QtWidgetParameterGroup(ParameterGroup::Pointer, QtWidgetModel*);
  ~QtWidgetParameterGroup() override;

public slots:
  void SetActivationState( bool value ) override;

private:
  QtWidgetParameterGroup(const QtWidgetParameterGroup&); //purposely not implemented
  void operator=(const QtWidgetParameterGroup&); //purposely not implemented

  void DoCreateWidget() override;

  void DoUpdateGUI() override;

  virtual void ProcessChild(QObject* currentNode, bool status);

  ParameterGroup::Pointer m_ParamList;

  typedef std::vector<QtWidgetParameterBase*> WidgetListType;
  typedef WidgetListType::iterator WidgetListIteratorType;
  WidgetListType m_WidgetList;

};


}
}

#endif
