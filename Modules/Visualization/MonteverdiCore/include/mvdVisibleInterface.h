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
#ifndef __mvdVisibleInterface_h
#define __mvdVisibleInterface_h

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

/**
 * \class VisibleInterface
 *
 * \ingroup OTBMonteverdiCore
 *
 * \brief WIP.
 */
class OTBMonteverdiCore_EXPORT VisibleInterface
{

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public methods.
public:

  /** \brief Constructor. */
  VisibleInterface( bool =true );

  /** \brief Destructor. */
  virtual ~VisibleInterface();

  /**
   */
  inline void SetVisible( bool );

  /**
   */
  inline bool IsVisible() const;

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
  /**
   */
  virtual void virtual_SignalVisibilityChanged( bool );

//
// Private attributes.
private:
  bool m_IsVisible: 1;
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

/*****************************************************************************/
inline
bool
VisibleInterface
::IsVisible() const
{
  return m_IsVisible;
}

/*****************************************************************************/
inline
void
VisibleInterface
::SetVisible( bool state )
{
  m_IsVisible = state;

  virtual_SignalVisibilityChanged( state );
}

} // end namespace 'mvd'

#endif // __mvdVisibleInterface_h
