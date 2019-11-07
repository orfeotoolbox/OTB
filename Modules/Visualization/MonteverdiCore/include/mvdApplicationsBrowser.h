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

#ifndef mvdApplicationsBrowser_h
#define mvdApplicationsBrowser_h


//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi.h"

#include "OTBMonteverdiCoreExport.h"

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
#include "mvdTypes.h"

#include <string>

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
 * \class ApplicationsBrowser
 *
 * \ingroup OTBMonteverdiCore
 *
 * \brief Search for available applications in a directory.
 *
 * This class provides the functionnalities to look for
 * otbWrapperApplication apps in a given directory. The method used to
 * set the directory is SetAutoLoadPath(const std::string &).
 *
 * If applications are available in the directory set by the user,
 * tags of each application are extracted.
 *
 * An association application-tags is then setup and stored in a
 * std::vector to be sent (via a signal) to the
 * mvd::ApplicationsToolBox or any widget or class connected to this
 * signal.
 *
 */
class OTBMonteverdiCore_EXPORT ApplicationsBrowser : public QObject
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

  //
  // Public methods.
public:
  /** typedef */

  /** \brief Constructor. */
  ApplicationsBrowser(QObject* p = NULL);

  /** \brief Destructor. */
  ~ApplicationsBrowser() override;

  /** set the path where to look for applications */
  void SetAutoLoadPath(const std::string& itk_auto_load_path);

  /** get available applications in the search path */
  StringVector GetAvailableApplications();

  /** return the list applications <->tags */
  StringVector GetApplicationTags(const std::string& appName);

  /** return std::map storing tag/apps association for all the
    * applications in the search path
    */
  void SearchAvailableApplicationsTags();


  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

  //
  // Public SLOTS.
public slots:

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

  //
  // Signals.
signals:
  void AvailableApplicationsTagsChanged(const ApplicationsTagContainer&);

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

  //
  // Protected methods.
protected:
  //
  // Protected attributes.
protected:
  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

  //
  // Private methods.
private:
  //
  // Private attributes.
private:
  std::string m_AutoLoadPath;

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

#endif // mvdApplicationsBrowser_h
