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


#include "itkProcessObject.h"
#include "itkQtProgressBar.h"


namespace itk
{


/** Constructor */
QtProgressBar::QtProgressBar(QWidget* par) : QProgressBar(par)
{
  m_RedrawCommand = RedrawCommandType::New();
  m_RedrawCommand->SetCallbackFunction(this, &QtProgressBar::ProcessEvent);
  m_RedrawCommand->SetCallbackFunction(this, &QtProgressBar::ConstProcessEvent);

  m_Caller = itk::Object::New();
  this->setMaximum(100);
  this->reset();
}


QtProgressBar::~QtProgressBar()
{
  m_Caller->RemoveAllObservers();
}

/** Get Command */
QtProgressBar::RedrawCommandType* QtProgressBar::GetRedrawCommand(void) const
{
  return m_RedrawCommand.GetPointer();
}

/** Manage a Progress event */
void QtProgressBar::ProcessEvent(itk::Object* caller, const itk::EventObject& ev)
{
  if (typeid(itk::ProgressEvent) == typeid(ev))
  {
    ::itk::ProcessObject::Pointer process = dynamic_cast<itk::ProcessObject*>(caller);

    if (process)
    {
      const int value2 = static_cast<int>(process->GetProgress() * this->maximum());
      Q_EMIT      SetValueChanged(value2);
    }
  }
}

void QtProgressBar::ConstProcessEvent(const itk::Object* caller, const itk::EventObject& ev)
{
  if (typeid(itk::ProgressEvent) == typeid(ev))
  {
    itk::ProcessObject::ConstPointer process = dynamic_cast<const itk::ProcessObject*>(caller);

    if (process)
    {
      const int v = static_cast<int>(process->GetProgress() * this->maximum());

      Q_EMIT SetValueChanged(v);
    }
  }
}

/** Manage a Progress event */
void QtProgressBar::Observe(itk::Object* caller)
{
  m_Caller = caller;
  m_Caller->AddObserver(itk::ProgressEvent(), m_RedrawCommand.GetPointer());
}

} // end namespace fltk
