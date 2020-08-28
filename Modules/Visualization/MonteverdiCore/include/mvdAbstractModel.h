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


#ifndef mvdAbstractModel_h
#define mvdAbstractModel_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi.h"

/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QtCore>

//
// System includes (sorted by alphabetic order)
#include <cassert>

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)
#include "OTBMonteverdiCoreExport.h"
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

/** \class AbstractModel
 *
 * \ingroup OTBMonteverdiCore
 *
 */
class OTBMonteverdiCore_EXPORT AbstractModel : public QObject
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

  //
  // Public methods.
public:
  /** Destructor */
  ~AbstractModel() override;

  /** */
  template <typename TModel>
  inline const TModel* GetParentModel() const;

  /** */
  template <typename TModel>
  inline TModel* GetParentModel();

  /** */
  template <typename TModel>
  TModel* newChildModel(void* context = NULL);

  /** */
  inline void attachChildModel(AbstractModel* model, void* context = NULL);

  /** */
  inline void BuildModel(void* context = NULL);

  /**
   */
  virtual bool IsModified() const;

  /**
   */
  virtual void ClearModified();

  /**
   */
  inline void Save();

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

  //
  // Signals.
Q_SIGNALS:

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

  //
  // Protected methods.
protected:
  /** Constructor */
  AbstractModel(QObject* p = NULL);

  /**
   */
  virtual void virtual_BuildModel(void* context = NULL);

  //
  // Protected attributes.
protected:
  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

  //
  // Private methods.
private:
  /**
   */
  virtual void virtual_Save();

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

namespace mvd
{

/*****************************************************************************/
template <typename TModel>
const TModel* AbstractModel::GetParentModel() const
{
  return qobject_cast<const TModel*>(parent());
}

/*****************************************************************************/
template <typename TModel>
TModel* AbstractModel::GetParentModel()
{
  // qDebug()
  //   << "parent:" << parent()
  //   << ";" << qobject_cast< TModel* >( parent() );

  return qobject_cast<TModel*>(parent());
}

/*****************************************************************************/
template <typename T>
T* AbstractModel::newChildModel(void* context)
{
  T* model = new T(this);

  try
  {
    attachChildModel(model, context);
  }
  catch (...)
  {
    delete model;
    model = NULL;

    throw;
  }

  return model;
}

/*****************************************************************************/
inline void AbstractModel::attachChildModel(AbstractModel* model, void* context)
{
  assert(model != NULL);

  model->setParent(this);
  model->BuildModel(context);
}

/*****************************************************************************/
inline void AbstractModel::BuildModel(void* context)
{
  virtual_BuildModel(context);
}

/*****************************************************************************/
inline void AbstractModel::Save()
{
  virtual_Save();

  ClearModified();
}

} // end namespace 'mvd'

#endif // mvdAbstractModel_h
