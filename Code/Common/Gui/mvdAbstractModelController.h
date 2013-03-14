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

namespace core
{
//
// Internal classes pre-declaration.
class AbstractModel;
}

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

  /** Destructor */
  virtual ~AbstractModelController();

  /** */
  void SetModel( core::AbstractModel* );

  /** */
  inline const core::AbstractModel* GetModel() const;

  /** */
  inline core::AbstractModel* GetModel();

  /** */
  template< typename TModel >
  inline const TModel* GetModel() const;

  /** */
  template< typename TModel >
  inline TModel* GetModel();

  /** */
  inline const QWidget* GetWidget() const;

  /** */
  inline QWidget* GetWidget();

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
  void AboutToDisconnectModel( core::AbstractModel* );
  /** */
  void ModelDisconnected( core::AbstractModel* );
  /** */
  void AboutToConnectModel( core::AbstractModel* );
  /** */
  void ModelConnected( core::AbstractModel* );
  /** */
  void ModelUpdated();

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:

  /** Constructor */
  AbstractModelController( QWidget* widget, QObject* parent =NULL );

  /** */
  virtual void Connect( core::AbstractModel* ) =0;

  /** */
  virtual void ResetWidget() =0;

  /** */
  virtual void Disconnect( core::AbstractModel* ) =0;

//
// Protected attributes.
protected:

  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

//
// Private methods.
private:
  /** */
  void private_Connect( core::AbstractModel* );

  /** */
  void private_Disconnect( core::AbstractModel* );

//
// Private attributes.
private:
  /** */
  QWidget* m_Widget;

  /** */
  core::AbstractModel* m_Model;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Slots.
private slots:
  /**
   * Slot called before object is destroyed.
   * It is used to disconnect model from controller when controller is
   * destroyed. This prevents the indirect call the virtual
   * Disconnect() method (which is dangerous in destructor).
   */
  void OnDestroyed( QObject* =0 );
};

} // end namespace 'mvd'.

/*****************************************************************************/
/* INLINE SECTION                                                            */
namespace mvd
{

/*****************************************************************************/
inline
const core::AbstractModel*
AbstractModelController
::GetModel() const
{
  return m_Model;
}

/*****************************************************************************/
inline
core::AbstractModel*
AbstractModelController
::GetModel()
{
  return m_Model;
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
inline
const QWidget*
AbstractModelController
::GetWidget() const
{
  return m_Widget;
}

/*****************************************************************************/
inline
QWidget*
AbstractModelController
::GetWidget()
{
  return m_Widget;
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

} // end namespace 'mvd'

#endif // __mvdAbstractModelController_h
