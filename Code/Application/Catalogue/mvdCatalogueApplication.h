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

#ifndef __mvdCatalogueApplication_h
#define __mvdCatalogueApplication_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi2.h"


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
#include "Core/mvdI18nApplication.h"
#include "Core/mvdAbstractModel.h"

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
// Class declaration.

/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/** \class CatalogueApplication
 *
 */
class Monteverdi2_EXPORT CatalogueApplication
  : public I18nApplication
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public constants.
public:

//
// Public methods.
public:

  /**
   * \brief Constructor.
   *
   * \param argc Command-line argument count in the argv array.
   * \param argv Array of command-argument (whitespace-separated) strings.
   */
  CatalogueApplication( int& argc, char** argv );

  /** \brief Destructor. */
  virtual ~CatalogueApplication();

  /**
   */
  void OpenDatabase();

  /***/
  void OpenApplicationsBrowser();

  /** Get the OTBApp Model */
  AbstractModel* GetOTBApplicationsModel()
  {
    return m_OTBApplicationsModel;
  }

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

  AbstractModel* m_OTBApplicationsModel;

//
// Private attributes.
private:

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Slots
private slots:
};

/*****************************************************************************/
/* INLINE SECTION                                                            */


} // end namespace 'mvd'

#endif // __CatalogueApplication_h
