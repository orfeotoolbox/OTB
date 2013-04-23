/*=========================================================================

  Program:   Monteverdi2
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See Copyright.txt for details.

  Monteverdi2 is distributed under the CeCILL licence version 2. See
  Licence_CeCILL_V2-en.txt or
  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt for more details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __mvdAbstractWorker_h
#define __mvdAbstractWorker_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi2.h"


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
 * \brief Abstract worker object used as a base-class for threaded tasks.
 */
class Monteverdi2_EXPORT AbstractWorker :
    public QObject
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public methods.
public:

  /** \brief Destructor. */
  virtual ~AbstractWorker();

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

//
// Public SLOTS.
public slots:

  /**
   * \brief Call the implemented do job/task routine.
   */
  void Do();

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
signals:
  /**
   * \brief Signal emitted when progress text has changed.
   *
   * \param text New progress text to display;
   */
  void ProgressTextChanged( const QString& text );

  /**
   * \brief Signal emitted when progress value has changed.
   *
   * \param value New progress value to display.
   */
  void ProgressValueChanged( int value );

  /**
   * \brief Signal emitted when progress range has changed.
   *
   * \param min Minimum progress value.
   * \param max Maximum progress value.
   */
  void ProgressRangeChanged( int min, int max );

  /**
   * \brief Signal emitted when job/task has correctly been done.
   *
   * \param result Resulting QObject instance of NULL if none.
   */
  void Done( QObject* result =NULL );

  /**
   *  \brief Signal emitted when job/task has finished correctly or not.
   */
  void Finished();

  /**
   * \brief Signal emitted when an exception has been caught by this worker.
   *
   * \param 
   */
  void ExceptionRaised( const QString& what );

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:

  /** \brief Constructor. */
  AbstractWorker( QObject* parent =NULL );

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
  virtual QObject* virtual_Do() =0;


//
// Private attributes.
private:

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Slots.
private slots:
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

#endif // __mvdAbstractWorker_h
