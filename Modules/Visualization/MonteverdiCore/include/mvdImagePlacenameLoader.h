/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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


#ifndef mvdImagePlacenameLoader_h
#define mvdImagePlacenameLoader_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi.h"

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QtCore>

#include "OTBMonteverdiCoreExport.h"

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)

//
// External classes pre-declaration.
namespace
{
}

namespace mvd
{

//
// Internal class pre-declaration.
class VectorImageModel;

/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class ImagePlacenameLoader
 *
 * \ingroup OTBMonteverdiCore
 *
 * \brief
 *
 */
class OTBMonteverdiCore_EXPORT ImagePlacenameLoader : public QObject
{
  Q_OBJECT;

public:
  /** \brief Constructor */
  ImagePlacenameLoader(VectorImageModel* model);

public slots:
  /** \brief load the placename */
  void LoadPlacename();

signals:
  /** \brief Triggered after a non empty placename was loaded */
  void PlacenameLoaded(const QString&);

  /** \brief Triggered when processing is finished */
  void Finished();

private:
  VectorImageModel* m_Model;
};

} // end namespace 'mvd'

#endif // mvdImageModel_h
