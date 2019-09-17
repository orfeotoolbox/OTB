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

#ifndef mvdAbstractModelController_h
#define mvdAbstractModelController_h

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
class AbstractModel;

/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class AbstractModelController
 *
 * \ingroup OTBMonteverdiGUI
 */
class OTBMonteverdiGUI_EXPORT AbstractModelController : public QObject
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
  ~AbstractModelController() override;

  /** */
  void SetModel(AbstractModel*);

  /** */
  inline const AbstractModel* GetModel() const;

  /** */
  inline AbstractModel* GetModel();

  /** */
  template <typename TModel>
  inline const TModel* GetModel() const;

  /** */
  template <typename TModel>
  inline TModel* GetModel();

  /** */
  inline const QWidget* GetWidget() const;

  /** */
  inline QWidget* GetWidget();

  /** */
  template <typename TWidget>
  inline const TWidget* GetWidget() const;

  /** */
  template <typename TWidget>
  inline TWidget* GetWidget();

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

  //
  // Slots.
public slots:

  /**
   */
  void ResetWidget();

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

  //
  // Signals.
signals:
  /**
   */
  void AboutToDisconnectModel(AbstractModel*);

  /**
   */
  void ModelDisconnected(AbstractModel*);

  /**
   */
  void AboutToConnectModel(AbstractModel*);

  /**
   */
  void ModelConnected(AbstractModel*);

  /**
   */
  void ModelUpdated();

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

  //
  // Protected methods.
protected:
  /** Constructor */
  AbstractModelController(QWidget* widget, QObject* p = NULL);

  //
  // Protected attributes.
protected:
  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

  //
  // Private methods.
private:
  /**
   */
  void private_Connect(AbstractModel*);

  /**
   */
  void private_Disconnect(AbstractModel*);

  /**
   */
  void private_ClearWidget();

  /**
   */
  virtual void Connect(AbstractModel*) = 0;

  /**
   */
  virtual void Disconnect(AbstractModel*) = 0;

  /**
   */
  virtual void ClearWidget() = 0;

  /**
   */
  virtual void virtual_ResetWidget(bool) = 0;

  //
  // Private attributes.
private:
  /**
   */
  QWidget* m_Widget;

  /**
   */
  AbstractModel* m_Model;

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
  void OnDestroyed(QObject* = 0);

  /**
   */
  virtual void RefreshWidget();
};

} // end namespace 'mvd'.

/*****************************************************************************/
/* INLINE SECTION                                                            */
namespace mvd
{

/*****************************************************************************/
inline const AbstractModel* AbstractModelController::GetModel() const
{
  return m_Model;
}

/*****************************************************************************/
inline AbstractModel* AbstractModelController::GetModel()
{
  return m_Model;
}

/*****************************************************************************/
template <typename TModel>
inline const TModel* AbstractModelController::GetModel() const
{
  return qobject_cast<const TModel*>(m_Model);
}

/*****************************************************************************/
template <typename TModel>
inline TModel* AbstractModelController::GetModel()
{
  return qobject_cast<TModel*>(m_Model);
}

/*****************************************************************************/
inline const QWidget* AbstractModelController::GetWidget() const
{
  return m_Widget;
}

/*****************************************************************************/
inline QWidget* AbstractModelController::GetWidget()
{
  return m_Widget;
}

/*****************************************************************************/
template <typename TWidget>
inline const TWidget* AbstractModelController::GetWidget() const
{
  return qobject_cast<const TWidget*>(m_Widget);
}

/*****************************************************************************/
template <typename TWidget>
inline TWidget* AbstractModelController::GetWidget()
{
  return qobject_cast<TWidget*>(m_Widget);
}

} // end namespace 'mvd'

#endif // mvdAbstractModelController_h
