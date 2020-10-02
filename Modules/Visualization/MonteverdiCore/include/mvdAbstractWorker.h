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

#ifndef mvdAbstractWorker_h
#define mvdAbstractWorker_h

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


/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class AbstractWorker
 *
 * \ingroup OTBMonteverdiCore
 *
 * \brief Abstract worker object used as a base-class for threaded tasks.
 */
class OTBMonteverdiCore_EXPORT AbstractWorker : public QObject
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

  //
  // Public methods.
public:
  /** \brief Destructor. */
  ~AbstractWorker() override;

  /**
   */
  inline QString GetFirstProgressText() const;

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

  //
  // Public SLOTS.
public Q_SLOTS:

  /**
   * \brief Call the implemented do job/task routine.
   */
  void Do() noexcept;

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

  //
  // Signals.
Q_SIGNALS:
  /**
   * \brief Signal emitted when progress text has changed.
   *
   * \param text New progress text to display;
   */
  void ProgressTextChanged(const QString& text);

  /**
   * \brief Signal emitted when progress value has changed.
   *
   * \param value New progress value to display.
   */
  void ProgressValueChanged(int value);

  /**
   * \brief Signal emitted when progress range has changed.
   *
   * \param min Minimum progress value.
   * \param max Maximum progress value.
   */
  void ProgressRangeChanged(int min, int max);

  /**
   * \brief Signal emitted when job has correctly been done.
   *
   * Use this signal to get resulting object when job has been done.
   *
   * \param result Resulting QObject instance of NULL if none.
   */
  void Done(QObject* result = NULL);

  /**
   *  \brief Signal emitted when task (i.e. thread) has finished
   *  correctly or incorrectly.
   *
   * Use this signal to forward to thead quit() slot when job has
   * finished.
   */
  void Finished();

  /**
   * \brief Signal emitted when an exception has been caught by this worker.
   *
   * \param
   */
  void ExceptionRaised(QString what);

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

  //
  // Protected methods.
protected:
  /** \brief Constructor. */
  AbstractWorker(QObject* p = NULL);

  //
  // Protected attributes.
protected:
  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

  //
  // Private methods.
private:
  /**
   * \brief Do job/task abstract method to implement.
   */
  virtual QObject* virtual_Do() = 0;

  /**
   */
  virtual QString virtual_GetFirstProgressText() const = 0;


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

/*****************************************************************************/
inline QString AbstractWorker::GetFirstProgressText() const
{
  return virtual_GetFirstProgressText();
}

} // end namespace 'mvd'

#endif // mvdAbstractWorker_h
