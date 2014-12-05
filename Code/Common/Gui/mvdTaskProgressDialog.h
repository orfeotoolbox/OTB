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
#ifndef __mvdTaskProgressDialog_h
#define __mvdTaskProgressDialog_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi2.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QtGui>

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
class BackgroundTask;

/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class TaskProgressDialog
 *
 * \brief Widget template skeleton to copy-paste when adding a new
 * widget class.
 */
class Monteverdi2_EXPORT TaskProgressDialog :
    public QProgressDialog
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
  TaskProgressDialog( BackgroundTask* task,
		      QWidget* parent =NULL,
		      Qt::WindowFlags flags =0 );

  /**
   * \brief Destructor.
   */
  virtual ~TaskProgressDialog();

  /**
   */
  int Exec();

  /**
   */
  inline QObject * GetObject() const;

  /**
   */
  template< typename T >
    inline
    T* GetObject() const;

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

//
// Public SLOTS.
public slots:

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
signals:

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
private slots:
  /**
   */
  void OnDone( QObject* result );
  /**
   */
  void OnExceptionRaised( QString what );
  /**
   */
  void OnObjectDestroyed( QObject* object );
};

} // end namespace 'mvd'

/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace mvd
{
/*****************************************************************************/
inline
QObject *
TaskProgressDialog
::GetObject() const
{
  return m_Object;
}

/*****************************************************************************/
template< typename T >
inline
T*
TaskProgressDialog
::GetObject() const
{
  return qobject_cast< T* >( m_Object );
}

} // end namespace 'mvd'

#endif // __mvdTaskProgressDialog_h
