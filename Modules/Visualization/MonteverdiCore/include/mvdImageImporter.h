/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef mvdImageImporter_h
#define mvdImageImporter_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi.h"

#include "OTBMonteverdiCoreExport.h"

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
 * \ingroup OTBMonteverdiCore
 *
 * \brief Import image worker. It is parametrized by the image
 * filename and the desired (width, height) best-fit size.
 */
class OTBMonteverdiCore_EXPORT ImageImporter : public AbstractWorker
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
  ImageImporter(const QString& filename, bool isForceCreateEnabled, int width = -1, int height = -1, QObject* p = NULL);

  /**
   * \brief Constructor (image-model).
   *
   * \param filename Filename of image to import.
   * \param width Width of the best-fit size or -1 if none.
   * \param height Height of the best-fit size or -1 if none.
   * \param isForceCreateEnabled true to force creation of dataset.
   */
  ImageImporter(const QString& filename, int width = -1, int height = -1, QObject* p = NULL);

  /**
   * \brief Destructor.
   */
  ~ImageImporter() override;

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

  //
  // Public SLOTS.
public Q_SLOTS:

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

  //
  // Signals.
Q_SIGNALS:

  /**
   * \brief Signal emitted when job/task has correctly been done.
   *
   * \param result Resulting QObject instance of NULL if none.
   */
  // void Done( QObject* result =NULL );

  /**
   * \brief Signal emitted when an exception has been caught by this worker.
   *
   * \param exc The copy (thread safety) exception which has been caught.
   */
  // void ExceptionRaised( std::exception exc );

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
    IMAGE,
  };

  //
  // Private methods.
private:
  //
  // AbstractWorker oveloads.

  QObject* virtual_Do() override;

  QString virtual_GetFirstProgressText() const override;


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
  bool m_IsForceCreateEnabled : 1;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

  //
  // Slots.
private Q_SLOTS:
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

#endif // mvdImageImporter_h
