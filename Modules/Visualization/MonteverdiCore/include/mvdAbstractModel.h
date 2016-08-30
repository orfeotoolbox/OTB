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

#ifndef __mvdAbstractModel_h
#define __mvdAbstractModel_h

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
#include <QtCore>

//
// System includes (sorted by alphabetic order)
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829  //tag=QT4-boost-compatibility
#include <cassert>
#endif //tag=QT4-boost-compatibility

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
 * \ingroup OTBMonteverdiCore
 *
 */
class ITK_EXPORT AbstractModel :
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
  template< typename TModel >
    inline const TModel* GetParentModel() const;

  /** */
  template< typename TModel >
    inline TModel* GetParentModel();

  /** */
  template< typename TModel >
    TModel* newChildModel( void* context =NULL );

  /** */
  inline void attachChildModel( AbstractModel* model, void* context =NULL );

  /** */
  inline void BuildModel( void* context =NULL );

  /**
   */
  virtual bool IsModified() const;

  /**
   */
  virtual void ClearModified();

  /**
   */
  inline void Save();

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
signals:

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:
  /** Constructor */
  AbstractModel( QObject* p =NULL );

  /**
   */
  virtual void virtual_BuildModel( void* context =NULL );

//
// Protected attributes.
protected:

  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

//
// Private methods.
private:

  /**
   */
  virtual void virtual_Save();

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
template< typename TModel >
const TModel*
AbstractModel
::GetParentModel() const
{
  return qobject_cast< const TModel* >( parent() );
}

/*****************************************************************************/
template< typename TModel >
TModel*
AbstractModel
::GetParentModel()
{
  // qDebug()
  //   << "parent:" << parent()
  //   << ";" << qobject_cast< TModel* >( parent() );

  return qobject_cast< TModel* >( parent() );
}

/*****************************************************************************/
template< typename T >
T *
AbstractModel
::newChildModel( void * context )
{
  T* model = new T( this );

  try
    {
    attachChildModel( model, context );
    }
  catch( ... )
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
::attachChildModel( AbstractModel* model, void* context )
{
  assert( model!=NULL );

  model->setParent( this );
  model->BuildModel( context );
}

/*****************************************************************************/
inline
void
AbstractModel
::BuildModel( void* context )
{
  virtual_BuildModel( context );
}

/*****************************************************************************/
inline
void
AbstractModel
::Save()
{
  virtual_Save();

  ClearModified();
}

} // end namespace 'mvd'

#endif // __mvdAbstractModel_h
