/*=========================================================================

  Program:   Monteverdi
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See Copyright.txt for details.

  Monteverdi is distributed under the CeCILL licence version 2. See
  Licence_CeCILL_V2-en.txt or
  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt for more details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __mvdOTBApplicationsModel_h
#define __mvdOTBApplicationsModel_h

//
// Configuration include.
//// Included at first position before any other ones.
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829  //tag=QT4-boost-compatibility
#include "ConfigureMonteverdi.h"
#endif //tag=QT4-boost-compatibility


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

//
// Monteverdi includes (sorted by alphabetic order)
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829  //tag=QT4-boost-compatibility
#include "mvdAbstractModel.h"
#endif //tag=QT4-boost-compatibility


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
class ITK_EXPORT OTBApplicationsModel :
    public AbstractModel
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
  OTBApplicationsModel( QObject* p =NULL );

  /** \brief Destructor. */
  virtual ~OTBApplicationsModel();

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
signals:

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:

  //
  // AbstractModel overrides.

  void virtual_BuildModel( void* context =NULL );

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
private slots:
};

} // end namespace 'mvd'

/*****************************************************************************/
/* INLINE SECTION                                                            */

//
// Monteverdi deferred includes (sorted by alphabetic order)

namespace mvd
{
/*****************************************************************************/
inline
const ApplicationsBrowser*
OTBApplicationsModel
::GetBrowser() const
{
  return m_ApplicationsBrowser;
}

/*****************************************************************************/
inline
ApplicationsBrowser*
OTBApplicationsModel
::GetBrowser()
{
  return m_ApplicationsBrowser;
}  
/*****************************************************************************/
inline
const ApplicationLauncher*
OTBApplicationsModel
::GetLauncher() const
{
  return m_ApplicationLauncher;
}

/*****************************************************************************/
inline
ApplicationLauncher*
OTBApplicationsModel
::GetLauncher()
{
  return m_ApplicationLauncher;
}  

} // end namespace 'mvd'

#endif // __mvdOTBApplicationsModel_h
