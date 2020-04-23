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

#include "mvdImageImporter.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)
#include <cassert>

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdI18nCoreApplication.h"
#include "mvdVectorImageModel.h"

namespace mvd
{
/*
  TRANSLATOR mvd::ImageImporter

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */

namespace
{
} // end of anonymous namespace.

/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */
/*******************************************************************************/
ImageImporter::ImageImporter(const QString& filename, int width, int height, QObject* p)
  : AbstractWorker(p), m_Filename(filename), m_ModelType(IMAGE), m_Width(width), m_Height(height), m_IsForceCreateEnabled(false)
{
}

/*******************************************************************************/
ImageImporter::~ImageImporter()
{
  // qDebug() << this << "destroyed.";
}

/*******************************************************************************/
QObject* ImageImporter::virtual_Do()
{
  /*
  emit ProgressTextChanged(
    tr( "Importing image '%1' as dataset into cache directory..." )
    .arg( QFileInfo( m_Filename ).fileName() )
  );
  */

  // emit ProgressTextChanged( GetFirstProgressText() );

  emit ProgressValueChanged(-1);

  emit ProgressRangeChanged(0, 0);

  // Load model.
  switch (m_ModelType)
  {
  case IMAGE:
    return I18nCoreApplication::LoadImageModel(m_Filename, m_Width, m_Height);
    break;

  default:
    assert(false && "Unhandled ImageImporter::ModelType value.");
    break;
  }

  return NULL;
}

/*******************************************************************************/
QString ImageImporter::virtual_GetFirstProgressText() const
{
  switch (m_ModelType)
  {
  case IMAGE:
    return tr("Loading image '%1'...").arg(QFileInfo(m_Filename).fileName());
    break;

  default:
    assert(false && "Unhandled ImageImporter::ModelType value.");
    break;
  }

  return QString();
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

} // end namespace 'mvd'
