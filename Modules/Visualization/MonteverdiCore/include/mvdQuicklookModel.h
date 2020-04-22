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


#ifndef mvdQuicklookModel_h
#define mvdQuicklookModel_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi.h"

#include "OTBMonteverdiCoreExport.h"
/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdTypes.h"
#include "mvdVectorImageModel.h"

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)


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

/** \class QuicklookModel
 *
 * \ingroup OTBMonteverdiCore
 *
 */
class OTBMonteverdiCore_EXPORT QuicklookModel : public VectorImageModel
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
  QuicklookModel(QObject* p = NULL);

  /** Destructor */
  ~QuicklookModel() override;

  /**
   * \brief Get the parent image-model of this quicklook image as an
   * AbstractImageModel.
   *
   * \return The parent image-model of this quicklook image.
   */
  inline const AbstractImageModel* GetImageModel() const;

  /**
   * \brief Get the parent image-model of this quicklook image as an
   * AbstractImageModel.
   *
   * \return The parent image-model of this quicklook image.
   */
  inline AbstractImageModel* GetImageModel();

  /**
   * \brief Get the parent image-model of this quicklook image as a
   * TImageModel.
   *
   * \return The parent image-model of this quicklook image.
   */
  template <typename TImageModel>
  inline const TImageModel* GetImageModel() const;

  /**
   * \brief Get the parent image-model of this quicklook image as a
   * TImageModel.
   *
   * \return The parent image-model of this quicklook image.
   */
  template <typename TImageModel>
  inline TImageModel* GetImageModel();

  //
  // VectorImageModel overloads.

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

  //
  // Public Q_SLOTS.
public Q_SLOTS:

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

  //
  // Signals.
Q_SIGNALS:
  /**
   */
  void SettingsUpdated();

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

  //
  // Protected methods.
protected:
  /** */
  void virtual_BuildModel(void* context = NULL) override;

  //
  // Protected attributes.
protected:
  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

  //
  // Private types.
private:
  //
  // Private methods.
private:
  //
  // Private attributes.
private:
  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

  //
  // Slots.
private Q_SLOTS:
};

} // end namespace 'mvd'

/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace mvd
{

/*****************************************************************************/
const AbstractImageModel* QuicklookModel::GetImageModel() const
{
  return GetImageModel<AbstractImageModel>();
}

/*****************************************************************************/
AbstractImageModel* QuicklookModel::GetImageModel()
{
  return GetImageModel<AbstractImageModel>();
}

/*****************************************************************************/
template <typename TImageModel>
const TImageModel* QuicklookModel::GetImageModel() const
{
  return qobject_cast<const TImageModel*>(parent());
}

/*****************************************************************************/
template <typename TImageModel>
TImageModel* QuicklookModel::GetImageModel()
{
  return qobject_cast<TImageModel*>(parent());
}

} // end namespace 'mvd'

#endif // mvdQuicklookModel_h
