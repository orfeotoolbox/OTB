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

#ifndef __mvdQuicklookModel_h
#define __mvdQuicklookModel_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi2.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdTypes.h"
#include "mvdVectorImageModel.h"

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)



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

/** \class QuicklookModel
 *
 */
class Monteverdi2_EXPORT QuicklookModel :
    public VectorImageModel
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

  /** Constructor */
  QuicklookModel( QObject* parent =NULL );

  /** Destructor */
  virtual ~QuicklookModel();

  /**
   * \brief Get the parent image-model of this quicklook image as an
   * AbstractImageModel.
   *
   * \return The parent image-model of this quicklook image.
   */
  inline const AbstractImageModel* GetImageModel() const;

  /**
   * \brief Get the parent image-model of this quicklook image as an
   * AbstractImageModel.
   *
   * \return The parent image-model of this quicklook image.
   */
  inline AbstractImageModel* GetImageModel();

  /**
   * \brief Get the parent image-model of this quicklook image as a
   * TImageModel.
   *
   * \return The parent image-model of this quicklook image.
   */
  template< typename TImageModel >
    inline const TImageModel* GetImageModel() const;

  /**
   * \brief Get the parent image-model of this quicklook image as a
   * TImageModel.
   *
   * \return The parent image-model of this quicklook image.
   */
  template< typename TImageModel >
    inline TImageModel* GetImageModel();

  //
  // VectorImageModel overloads.

  virtual const DatasetModel* GetDatasetModel() const;

  virtual DatasetModel* GetDatasetModel();

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

//
// Public slots.
public slots:

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
signals:
  /**
   */
  void SettingsUpdated();

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:

  /** */
  virtual void virtual_BuildModel( void* context =NULL );

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
const AbstractImageModel*
QuicklookModel
::GetImageModel() const
{
  return GetImageModel< AbstractImageModel >();
}

/*****************************************************************************/
AbstractImageModel*
QuicklookModel
::GetImageModel()
{
  return GetImageModel< AbstractImageModel >();
}

/*****************************************************************************/
template< typename TImageModel >
const TImageModel*
QuicklookModel
::GetImageModel() const
{
  return qobject_cast< const TImageModel* >( parent() );
}

/*****************************************************************************/
template< typename TImageModel >
TImageModel*
QuicklookModel
::GetImageModel()
{
  return qobject_cast< TImageModel* >( parent() );
}

} // end namespace 'mvd'

#endif // __mvdQuicklookModel_h
