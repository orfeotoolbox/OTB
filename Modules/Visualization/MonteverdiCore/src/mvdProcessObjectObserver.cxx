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

#include "mvdProcessObjectObserver.h"

/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)
#include <cassert>

//
// ITK includes (sorted by alphabetic order)
#include <itkLightProcessObject.h>
#include <itkProcessObject.h>

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdProgressInterface.h"


namespace mvd
{
/*
  TRANSLATOR mvd::ProcessObjectObserver

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */

namespace
{
} // end of anonymous namespace.


/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
ProcessObjectObserver::ProcessObjectObserver() : itk::Command(), m_ProgressInterface(NULL)
{
}

/*******************************************************************************/
ProcessObjectObserver::~ProcessObjectObserver()
{
}

/*******************************************************************************/
void ProcessObjectObserver::SetProgressInterface(ProgressInterface* interface)
{
  m_ProgressInterface = interface;
}

/*******************************************************************************/
void ProcessObjectObserver::Execute(itk::Object* caller, const itk::EventObject& event)
{
  Execute(const_cast<const itk::Object*>(caller), event);
}

/*******************************************************************************/
void ProcessObjectObserver::Execute(const itk::Object* caller, const itk::EventObject& event)
{
  assert(caller != NULL);

  if (typeid(event) == typeid(itk::ProgressEvent))
  {
    assert(m_ProgressInterface != NULL);

    // itk::LightProcessObject and itk::ProcessObject don't have any
    // common ::GetProgress() interface (because there is no common
    // super-class providing this interface). So, itk::Object must be
    // dynamically casted to both types in order to get progress.
    assert(dynamic_cast<const itk::LightProcessObject*>(caller) != NULL || dynamic_cast<const itk::ProcessObject*>(caller) != NULL);

    const itk::LightProcessObject* lpo = dynamic_cast<const itk::LightProcessObject*>(caller);

    if (lpo != NULL)
      m_ProgressInterface->SetProgress(lpo->GetProgress());
    else
    {
      const itk::ProcessObject* po = dynamic_cast<const itk::ProcessObject*>(caller);

      m_ProgressInterface->SetProgress(po->GetProgress());
    }
  }
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

} // end namespace 'mvd'
