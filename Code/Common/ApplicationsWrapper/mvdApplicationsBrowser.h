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
#ifndef __mvdApplicationsBrowser_h
#define __mvdApplicationsBrowser_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi2.h"


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
#include "Core/mvdTypes.h"

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
 * std::vector to be sent ( via a signal ) to the
 * mvd::ApplicationsToolBox or any widget or class connected to this
 * signal.
 *
 */
class Monteverdi2_EXPORT ApplicationsBrowser :
    public QObject
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public methods.
public:

  /** typedef */

  /** \brief Constructor. */
  ApplicationsBrowser( QObject* parent =NULL );

  /** \brief Destructor. */
  virtual ~ApplicationsBrowser();

  /** set the path where to look for applications */
  void SetAutoLoadPath(const std::string & itk_auto_load_path);

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
  void AvailableApplicationsTagsChanged(const ApplicationsTagContainer &, 
                                        const ApplicationDocNameToNameMap &);

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

  ApplicationDocNameToNameMap m_DocNameToNameMap;

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

#endif // __mvdApplicationsBrowser_h
