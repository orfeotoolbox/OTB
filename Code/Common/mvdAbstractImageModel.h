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

#ifndef __mvdAbstractImageModel_h
#define __mvdAbstractImageModel_h

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
#include "mvdAbstractModel.h"
#include "mvdTypes.h"


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

/** \class AbstractImageModel
 *
 */
class Monteverdi2_EXPORT AbstractImageModel :
    public AbstractModel
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public methods.
public:

  /** Destructor */
  virtual ~AbstractImageModel();

  /** */
  inline ImageRegionType GetLargestRegion() const;

  /** */
  inline CountType GetNbComponents() const;

  /** */
  virtual ImageBaseType::ConstPointer ToImageBase() const;

  /** */
  virtual ImageBaseType::Pointer ToImageBase();

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
signals:

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:

  /** Constructor */
  AbstractImageModel( QObject* parent =NULL );

  /** */
  virtual ImageBaseType::ConstPointer virtual_ToImageBase() const =0;

  /** */
  virtual ImageBaseType::Pointer virtual_ToImageBase() =0;

  //
  // AbstractModel overrides.

  /** */
  virtual void virtual_BuildModel();

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

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Slots.
private slots:
};

} // end namespace 'mvd'

/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace mvd
{

/*****************************************************************************/
inline
ImageRegionType
AbstractImageModel
::GetLargestRegion() const
{
  return ToImageBase()->GetLargestPossibleRegion();
}

/*****************************************************************************/
inline
CountType
AbstractImageModel
::GetNbComponents() const
{
  return ToImageBase()->GetNumberOfComponentsPerPixel();
}

/*****************************************************************************/
inline
ImageBaseType::ConstPointer
AbstractImageModel
::ToImageBase() const
{
  return virtual_ToImageBase();
}

/*****************************************************************************/
inline
ImageBaseType::Pointer
AbstractImageModel
::ToImageBase()
{
  return virtual_ToImageBase();
}

} // end namespace 'mvd'

#endif // __mvdAbstractImageModel_h
