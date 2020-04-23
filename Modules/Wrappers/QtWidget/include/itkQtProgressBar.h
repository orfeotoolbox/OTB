/*
 * Copyright (C) 1999-2011 Insight Software Consortium
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

#ifndef __itkQtProgressBar_h
#define __itkQtProgressBar_h

#include <QtWidgets>
#include "itkCommand.h"
#include "OTBQtWidgetExport.h"

namespace itk
{

class OTBQtWidget_EXPORT QtProgressBar : public ::QProgressBar
{
  Q_OBJECT
public:
  /** Command Class invoked for button redraw */
  typedef itk::MemberCommand<QtProgressBar> RedrawCommandType;

  /** Constructor */
  QtProgressBar(QWidget* parent);

  /** Destructor */
  ~QtProgressBar() override;

  /** Get Command */
  RedrawCommandType* GetRedrawCommand(void) const;


  /** Manage a Progress event */
  void ProcessEvent(itk::Object* caller, const itk::EventObject& event);
  void ConstProcessEvent(const itk::Object* caller, const itk::EventObject& event);


  /** Manage a Progress event */
  void Observe(itk::Object* caller);

signals:
  void SetValueChanged(int);

private:
  itk::Object::Pointer       m_Caller;
  RedrawCommandType::Pointer m_RedrawCommand;
};


} // end of namespace

#endif
