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


#ifndef mvdOTBApplicationsModel_h
#define mvdOTBApplicationsModel_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)
#include "OTBMonteverdiGUIExport.h"
//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdAbstractModel.h"


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
class ApplicationsBrowser;
class ApplicationLauncher;

/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/** \class OTBApplicationsModel
 *
 * \ingroup OTBMonteverdiGUI
 *
 */
class OTBMonteverdiGUI_EXPORT OTBApplicationsModel : public AbstractModel
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
  /** \brief Constructor. */
  OTBApplicationsModel(QObject* p = NULL);

  /** \brief Destructor. */
  ~OTBApplicationsModel() override;

  /** \brief Fill Widget Tree */
  void FillTree();

  /**
   */
  inline const ApplicationsBrowser* GetBrowser() const;

  /**
   */
  inline ApplicationsBrowser* GetBrowser();

  /**
   */
  inline const ApplicationLauncher* GetLauncher() const;

  /**
   */
  inline ApplicationLauncher* GetLauncher();

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

  //
  // Signals.
Q_SIGNALS:

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

  //
  // Protected methods.
protected:
  //
  // AbstractModel methods.

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
  /**
   */
  ApplicationsBrowser* m_ApplicationsBrowser;

  /**
   */
  ApplicationLauncher* m_ApplicationLauncher;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

  //
  // Slots.
private Q_SLOTS:
};

} // end namespace 'mvd'

/*****************************************************************************/
/* INLINE SECTION                                                            */

//
// Monteverdi deferred includes (sorted by alphabetic order)

namespace mvd
{
/*****************************************************************************/
inline const ApplicationsBrowser* OTBApplicationsModel::GetBrowser() const
{
  return m_ApplicationsBrowser;
}

/*****************************************************************************/
inline ApplicationsBrowser* OTBApplicationsModel::GetBrowser()
{
  return m_ApplicationsBrowser;
}
/*****************************************************************************/
inline const ApplicationLauncher* OTBApplicationsModel::GetLauncher() const
{
  return m_ApplicationLauncher;
}

/*****************************************************************************/
inline ApplicationLauncher* OTBApplicationsModel::GetLauncher()
{
  return m_ApplicationLauncher;
}

} // end namespace 'mvd'

#endif // mvdOTBApplicationsModel_h
