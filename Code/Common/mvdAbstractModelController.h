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
#ifndef __mvdAbstractModelController_h
#define __mvdAbstractModelController_h

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
class AbstractModel;

/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class AbstractModelController
 */
class Monteverdi2_EXPORT AbstractModelController :
    public QObject
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public types.
public:

//
// Public methods.
public:

  /** Constructor */
  AbstractModelController( QWidget* widget, QObject* parent =NULL );

  /** Destructor */
  virtual ~AbstractModelController();

  /** */
  inline void SetModel( AbstractModel* );

  /** */
  template< typename TModel >
  inline const TModel* GetModel() const;

  /** */
  template< typename TModel >
  inline TModel* GetModel();

  /** */
  template< typename TWidget >
  inline const TWidget* GetWidget() const;

  /** */
  template< typename TWidget >
  inline TWidget* GetWidget();

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
signals:
  /** */
  void AboutToDisconnectModel( AbstractModel* );
  /** */
  void ModelDisconnected( AbstractModel* );
  /** */
  void AboutToConnectModel( AbstractModel* );
  /** */
  void ModelConnected( AbstractModel* );

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:

  /** */
  virtual void Connect( AbstractModel* ) =0;

  /** */
  virtual void Disconnect( AbstractModel* ) =0;

//
// Protected attributes.
protected:

  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

//
// Private methods.
private:
  /** */
  inline void DisconnectOnDestroy();

//
// Private attributes.
private:
  /** */
  QWidget* m_Widget;

  /** */
  AbstractModel* m_Model;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Slots.
private slots:
};

} // end namespace 'mvd'.

/*****************************************************************************/
/* INLINE SECTION                                                            */
namespace mvd
{

/*****************************************************************************/
inline
void
AbstractModelController
::SetModel( AbstractModel* model )
{
  // Disconnect previously connected model and signal listeners.
  emit AboutToDisconnectModel( m_Model );
  Disconnect( m_Model );
  emit ModelDisconnected( m_Model );

  // Forget previously disconnected model before new model is
  // connected. This is done in order to stay in a consistent internal
  // state whenever the connection is aborted (e.g. by an exception).
  m_Model = NULL;

  // Connect new model and signal listeners.
  emit AboutToConnectModel( model );
  Connect( model );
  emit ModelDisconnected( model );

  // Remember newly connected model.
  m_Model = model;
}

/*****************************************************************************/
template< typename TModel >
inline
const TModel*
AbstractModelController
::GetModel() const
{
  return qobject_cast< const TModel* >( m_Model );
}

/*****************************************************************************/
template< typename TModel >
inline
TModel*
AbstractModelController
::GetModel()
{
  return qobject_cast< TModel* >( m_Model );
}

/*****************************************************************************/
template< typename TWidget >
inline
const TWidget*
AbstractModelController
::GetWidget() const
{
  return qobject_cast< const TWidget* >( m_Widget );
}

/*****************************************************************************/
template< typename TWidget >
inline
TWidget*
AbstractModelController
::GetWidget()
{
  return qobject_cast< TWidget* >( m_Widget );
}

/*****************************************************************************/
inline
void
AbstractModelController
::DisconnectOnDestroy()
{
  Disconnect( m_Model );
}

} // end namespace 'mvd'

#endif // __mvdAbstractModelController_h
