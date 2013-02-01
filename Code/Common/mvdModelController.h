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
namespace Ui
{
};


/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class ModelController
 */
template< typename TWidget, typename TModel >
class Monteverdi2_EXPORT ModelController :
    public TWidget
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public types.
public:
  /** */
  typedef TWidget SuperClass;
  /** */
  typedef TWidget Widget;
  /** */
  typedef TModel Model;

//
// Public methods.
public:

  /** Constructor */
  ModelController( QWidget* parent =NULL, Qt::WindowFlags flags =0 );

  /** Destructor */
  virtual ~ModelController();

  /** */
  inline void SetModel( Model* );

  /** */
  inline const Model* GetModel() const;

  /** */
  inline Model* GetModel();

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
signals:
  /** */
  void AboutToDisconnectModel( Model* );
  /** */
  void ModelDisconnected( Model* );
  /** */
  void AboutToConnectModel( Model* );
  /** */
  void ModelConnected( Model* );

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:

  /** */
  virtual void Connect( Model* ) =0;

  /** */
  virtual void Disconnect( Model* ) =0;

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
  Model* m_Model;

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
template< typename TWidget, typename TModel >
inline
void
ModelController< TWidget, TModel >
::SetModel( Model* model )
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
template< typename TWidget, typename TModel >
inline
const TModel*
ModelController< TWidget, TModel >
::GetModel() const
{
  return m_Model;
}

/*****************************************************************************/
template< typename TWidget, typename TModel >
inline
TModel*
ModelController< TWidget, TModel >
::GetModel()
{
  return m_Model;
}

} // end namespace 'mvd'

/*****************************************************************************/
/* TEMPLATE SECTION                                                          */

#include "mvdModelController.txx"

#endif // __mvdModelController_h
