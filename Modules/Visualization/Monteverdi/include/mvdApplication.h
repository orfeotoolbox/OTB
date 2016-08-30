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

#ifndef __mvdApplication_h
#define __mvdApplication_h

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

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdI18nApplication.h"

/*****************************************************************************/
/* PRE-DECLARATION SECTION                                                   */

//
// External classes pre-declaration.
namespace
{
}

namespace mvd
{
class OTBApplicationsModel;

//
// Class declaration.

/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/** \class Application
 *
 * \ingroup OTBMonteverdi
 *
 */
class ITK_EXPORT Application
  : public I18nApplication
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public constants/types.
public:

//
// Public methods.
public:

  /**
   * \brief Constructor.
   *
   * \param qtApp The parent Qt application (\see I18nApplication).
   */
  Application( QApplication* qtApp );

  /** \brief Destructor. */
  virtual ~Application();

  /**
   * \return The number of outdated dataset-models present in the
   * cache-directory.
   */
  // CountType OpenDatabase();

  /**
   */
  // void CloseDatabase();

  /**
   * \brief Get the OTB application model.
   */
  inline
    const OTBApplicationsModel* GetOTBApplicationsModel() const;

  /**
   * \brief Get the OTB application model.
   */
  inline
    OTBApplicationsModel* GetOTBApplicationsModel();

  //
  // STATIC METHODS.

  /**
   * \brief Access the const instance of application singleton.
   * \return The const instance to the singleton Application.
   */
  inline
    static const Application* ConstInstance();

  /**
   * \brief Access the non-const instance of application singleton.
   * \return The non-const instance to the singleton Application.
   */
  inline
    static Application* Instance();

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

public slots:

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
signals:

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:

//
// Protected attributes.
protected:

  void virtual_InitializeCore();

  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

//
// Private methods.
private:

  /**
   */
  void OpenApplicationsBrowser();

//
// Private attributes.
private:

  /**
   */
  OTBApplicationsModel* m_OTBApplicationsModel;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Slots
private slots:
};

} // end namespace 'mvd'

/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace mvd
{

/*****************************************************************************/
inline
const Application*
Application
::ConstInstance()
{
  return I18nCoreApplication::ConstInstance< Application >();
}

/*****************************************************************************/
inline
Application*
Application
::Instance()
{
  return I18nCoreApplication::Instance< Application >();
}

/*****************************************************************************/
inline
const OTBApplicationsModel*
Application
::GetOTBApplicationsModel() const
{
  return m_OTBApplicationsModel;
}

/*****************************************************************************/
inline
OTBApplicationsModel*
Application
::GetOTBApplicationsModel()
{
  return m_OTBApplicationsModel;
}

} // end namespace 'mvd'

#endif // __Application_h
