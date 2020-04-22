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

#ifndef mvdProcessObjectObserver_h
#define mvdProcessObjectObserver_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi.h"

#include "OTBMonteverdiCoreExport.h"

/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QtCore>

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)
#include <itkCommand.h>

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)


/*****************************************************************************/
/* PRE-DECLARATION SECTION                                                   */

//
// External classes pre-declaration.
namespace
{
}

namespace mvd
{
//
// Internal classes pre-declaration.
class ProgressInterface;

/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class ProcessObjectObserver
 *
 * \ingroup OTBMonteverdiCore
 *
 * \brief WIP.
 */
class OTBMonteverdiCore_EXPORT ProcessObjectObserver : public itk::Command
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  // Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

  //
  // Public types.
public:
  typedef ProcessObjectObserver   Self;
  typedef itk::Command            Superclass;
  typedef itk::SmartPointer<Self> Pointer;

  itkNewMacro(Self);

  //
  // Public methods.
public:
  /** \brief Destructor. */
  ~ProcessObjectObserver() override;

  const ProgressInterface* GetProgressInterface() const;
  ProgressInterface*       GetProgressInterface();

  void SetProgressInterface(ProgressInterface*);

  //
  // itk::Command overloads.

  void Execute(itk::Object* caller, const itk::EventObject& event) override;

  void Execute(const itk::Object* caller, const itk::EventObject& event) override;

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

  //
  // Public SLOTS.
public Q_SLOTS:

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

  //
  // Signals.
Q_SIGNALS:

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

  //
  // Protected methods.
protected:
  /** \brief Constructor. */
  ProcessObjectObserver();

  //
  // Protected attributes.
protected:
  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

  //
  // Private methods.
private:
  ProgressInterface* m_ProgressInterface;

  //
  // Private attributes.
private:
  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

  //
  // Slots.
private Q_SLOTS:
};

} // end namespace 'mvd'.

/*****************************************************************************/
/* INLINE SECTION                                                            */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)

namespace mvd
{
} // end namespace 'mvd'

#endif // mvdProcessObjectObserver_h
