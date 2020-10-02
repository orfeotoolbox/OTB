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

#ifndef mvdTaskProgressDialog_h
#define mvdTaskProgressDialog_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QtWidgets>

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)
#include "OTBMonteverdiGUIExport.h"
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
class BackgroundTask;

/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class TaskProgressDialog
 *
 * \ingroup OTBMonteverdiGUI
 *
 * \brief Widget template skeleton to copy-paste when adding a new
 * widget class.
 */
class OTBMonteverdiGUI_EXPORT TaskProgressDialog : public QProgressDialog
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

  //
  // Public methods.
public:
  /**
   * \brief Constructor.
   */
  TaskProgressDialog(BackgroundTask* task, QWidget* p = NULL, Qt::WindowFlags flags = 0);

  /**
   * \brief Destructor.
   */
  ~TaskProgressDialog() override;

  /**
   */
  int Exec();

  /**
   */
  inline QObject* GetObject() const;

  /**
   */
  template <typename T>
  inline T* GetObject() const;

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
  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

  //
  // Protected attributes.
protected:
  //
  // Private methods.
private:
  // Mask Qt native dialogue exec to be replaced by custom Exec()
  // method. Reason: QDialog::exec() is not virtual.
  using QProgressDialog::exec;

  //
  // Private attributes.
private:
  /**
   */
  BackgroundTask* m_BackgroundTask;
  /**
   */
  QObject* m_Object;
  /**
   */
  std::exception m_Exception;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

  //
  // Slots.
private Q_SLOTS:
  /**
   */
  void OnDone(QObject* result);
  /**
   */
  void OnExceptionRaised(QString what);
  /**
   */
  void OnObjectDestroyed(QObject* object);
};

} // end namespace 'mvd'

/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace mvd
{
/*****************************************************************************/
inline QObject* TaskProgressDialog::GetObject() const
{
  return m_Object;
}

/*****************************************************************************/
template <typename T>
inline T* TaskProgressDialog::GetObject() const
{
  return qobject_cast<T*>(m_Object);
}

} // end namespace 'mvd'

#endif // mvdTaskProgressDialog_h
