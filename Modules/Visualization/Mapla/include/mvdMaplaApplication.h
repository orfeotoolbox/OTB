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

#ifndef __mvdMaplaApplication_h
#define __mvdMaplaApplication_h

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

/** \class MaplaApplication
 *
 * \ingroup OTBMapla
 *
 */
class ITK_EXPORT MaplaApplication
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
  MaplaApplication( QApplication* qtApp );

  /** \brief Destructor. */
  virtual ~MaplaApplication();

  //
  // STATIC METHODS.

  /**
   * \brief Access the const instance of application singleton.
   * \return The const instance to the singleton MaplaApplication.
   */
  inline
    static const MaplaApplication* ConstInstance();

  /**
   * \brief Access the non-const instance of application singleton.
   * \return The non-const instance to the singleton MaplaApplication.
   */
  inline
    static MaplaApplication* Instance();

  /**
   */
  void Foo();

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

//
// Private attributes.
private:

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
const MaplaApplication*
MaplaApplication
::ConstInstance()
{
  return I18nCoreApplication::ConstInstance< MaplaApplication >();
}

/*****************************************************************************/
inline
MaplaApplication*
MaplaApplication
::Instance()
{
  return I18nCoreApplication::Instance< MaplaApplication >();
}

} // end namespace 'mvd'

#endif // __maplaApplication_h
