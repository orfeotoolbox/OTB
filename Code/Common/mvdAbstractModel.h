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

#ifndef __mvdAbstractModel_h
#define __mvdAbstractModel_h

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
#include <cassert>

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

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

/** \class AbstractModel
 *
 */
class Monteverdi2_EXPORT AbstractModel :
    public QObject
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public methods.
public:

  /** Destructor */
  virtual ~AbstractModel();

  /** */
  template< typename T >
    T* newChildModel();

  /** */
  inline void parentChildModel( AbstractModel* );

  /** */
  inline void BuildModel();

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
signals:

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:
  /** Constructor */
  AbstractModel( QObject* parent =NULL );

  /** */
  // TODO: Provide default empty virtual method body and clean-up
  // derived-classes.
  virtual void virtual_BuildModel() =0;

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

} // end namespace 'mvd'.

/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace mvd
{
/*****************************************************************************/
template< typename T >
T*
AbstractModel
::newChildModel()
{
  T* model = new T( this );

  try
    {
    parentChildModel( model );
    }
  catch( std::exception& exc )
    {
    delete model;
    model = NULL;

    throw;
    }

  return model;
}

/*****************************************************************************/
inline
void
AbstractModel
::parentChildModel( AbstractModel* model )
{
  assert( model!=NULL );

  model->BuildModel();

  model->setParent( this );
}

/*****************************************************************************/
inline
void
AbstractModel
::BuildModel()
{
  virtual_BuildModel();
}

} // end namespace 'mvd'

#endif // __mvdAbstractModel_h
