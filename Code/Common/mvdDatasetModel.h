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

#ifndef __mvdDatasetModel_h
#define __mvdDatasetModel_h

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
class AbstractImageModel;

/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/** \class DatasetModel
 *
 */
class Monteverdi2_EXPORT DatasetModel :
    public AbstractModel
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public methods.
public:

  /** Constructor */
  DatasetModel( QObject* parent =NULL );

  /** Destructor */
  virtual ~DatasetModel();

  /** */
  void ImportImage( const QString& filename, int w, int h );

  /** */
  inline bool HasSelectedImageModel() const;

  /** */
  inline const AbstractImageModel* GetSelectedImageModel() const;

  /** */
  inline AbstractImageModel* GetSelectedImageModel();

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

//
// Monteverdi deferred includes (sorted by alphabetic order)
#include "mvdAbstractImageModel.h"

namespace mvd
{

/*****************************************************************************/
inline
bool
DatasetModel
::HasSelectedImageModel() const
{
  return !findChildren< const AbstractImageModel* >().empty();
}

/*****************************************************************************/
inline
const AbstractImageModel*
DatasetModel
::GetSelectedImageModel() const
{
  return findChildren< const AbstractImageModel* >().first();
}

/*****************************************************************************/
inline
AbstractImageModel*
DatasetModel
::GetSelectedImageModel()
{
  return findChildren< AbstractImageModel* >().first();
}

/*****************************************************************************/

} // end namespace 'mvd'

#endif // __mvdDatasetModel_h
