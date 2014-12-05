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
#ifndef __mvdImageImporter_h
#define __mvdImageImporter_h

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

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdAbstractWorker.h"

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
 * \class ImageImporter
 *
 * \brief Import image worker. It is parametrized by the image
 * filename and the desired (width, height) best-fit size.
 */
class Monteverdi2_EXPORT ImageImporter :
    public AbstractWorker
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public methods.
public:

  /**
   * \brief Constructor (dataset-model).
   *
   * \param filename Filename of image to import.
   * \param width Width of the best-fit size or -1 if none.
   * \param height Height of the best-fit size or -1 if none.
   * \param isForceCreateEnabled true to force creation of dataset.
   */
  ImageImporter( const QString& filename,
		 bool isForceCreateEnabled,
		 int width =-1,
		 int height =-1,
		 QObject* parent =NULL );

  /**
   * \brief Constructor (image-model).
   *
   * \param filename Filename of image to import.
   * \param width Width of the best-fit size or -1 if none.
   * \param height Height of the best-fit size or -1 if none.
   * \param isForceCreateEnabled true to force creation of dataset.
   */
  ImageImporter( const QString& filename,
		 int width =-1,
		 int height =-1,
		 QObject* parent =NULL );

  /**
   * \brief Destructor.
   */
  virtual ~ImageImporter();

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
  enum ModelType
  {
    DATASET,
    IMAGE,
  };

//
// Private methods.
private:

  //
  // AbstractWorker oveloads.

  QObject* virtual_Do();

  QString virtual_GetFirstProgressText() const;


//
// Private attributes.
private:
  /**
   */
  QString m_Filename;
  /**
   */
  ModelType m_ModelType;
  /**
   */
  int m_Width;
  /**
   */
  int m_Height;
  /**
   */
  bool m_IsForceCreateEnabled: 1;

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

#endif // __mvdImageImporter_h
