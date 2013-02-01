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
#ifndef __mvdModelController_h
#define __mvdModelController_h

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
 * \class ModelController
 */
class Monteverdi2_EXPORT ModelController :
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
  ModelController( QWidget* parent =NULL );

  /** Destructor */
  virtual ~ModelController();

  /** */
  inline void SetModel( AbstractModel* );

  /** */
  inline const AbstractModel* GetModel() const;

  /** */
  inline AbstractModel* GetModel();

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

//
// Private methods.
private:

  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

//
// Private attributes.
private:

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
ModelController
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
inline
const AbstractModel*
ModelController
::GetModel() const
{
  return m_Model;
}

/*****************************************************************************/
inline
AbstractModel*
ModelController
::GetModel()
{
  return m_Model;
}

} // end namespace 'mvd'

#endif // __mvdModelController_h
