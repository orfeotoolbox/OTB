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
#ifndef __mvdOverviewBuilder_h
#define __mvdOverviewBuilder_h

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

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)
#include "otbGDALOverviewsBuilder.h"

//
// Monteverdi includes (sorted by alphabetic order)
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829  //tag=QT4-boost-compatibility
#include "mvdAbstractWorker.h"
#include "mvdProgressInterface.h"
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

/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class OverviewBuilder
 *
 * \brief Import image worker. It is parametrized by the image
 * filename and the desired (width, height) best-fit size.
 */
class Monteverdi_EXPORT OverviewBuilder :
    public AbstractWorker,
    public ProgressInterface
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public types
public:
  typedef std::vector< otb::GDALOverviewsBuilder::Pointer > GDALOverviewsBuilderVector;


//
// Public methods.
public:

  /**
   * \brief Constructor.
   */
  OverviewBuilder( const GDALOverviewsBuilderVector & builders,
		   QObject * p =NULL );

  /**
   * \brief Destructor.
   */
  virtual ~OverviewBuilder();

  //
  // ProgressInterface overloads.

  virtual void SetProgress( double );

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

//
// Public SLOTS.
public slots:

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
signals:

  /**
   * \brief Signal emitted when job/task has correctly been done.
   *
   * \param result Resulting QObject instance of NULL if none.
   */
  //void Done( QObject* result =NULL );

  /**
   * \brief Signal emitted when an exception has been caught by this worker.
   *
   * \param exc The copy (thread safety) exception which has been caught.
   */
  //void ExceptionRaised( std::exception exc );

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:

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
  // AbstractWorker oveloads.

  QObject * virtual_Do();

  QString virtual_GetFirstProgressText() const;


//
// Private attributes.
private:
  /**
   */
  GDALOverviewsBuilderVector m_GDALOverviewsBuilders;
  /**
   */
  int m_Index;
  /**
   */
  int m_Count;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Slots.
private slots:
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

} // end namespace 'mvd'

#endif // __mvdOverviewBuilder_h
