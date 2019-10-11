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

#include "mvdOverviewBuilder.h"


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
#include <itkProcessObject.h>

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdI18nCoreApplication.h"
#include "mvdProcessObjectObserver.h"
#include "mvdVectorImageModel.h"

namespace mvd
{
/*
  TRANSLATOR mvd::OverviewBuilder

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
OverviewBuilder::OverviewBuilder(const GDALOverviewsBuilderVector& builders, QObject* p)
  : AbstractWorker(p), ProgressInterface(), m_GDALOverviewsBuilders(builders), m_Index(0), m_Count(0)
{
}

/*******************************************************************************/
OverviewBuilder::~OverviewBuilder()
{
  // qDebug() << this << "destroyed.";
}

/*******************************************************************************/
void OverviewBuilder::SetProgress(double value)
{
  assert(m_Count > 0);
  assert(m_Index >= 0 && m_Index < m_Count);
  assert(value >= 0.0 && value <= 1.0);

  // qDebug()
  //   << m_Index << "/" << m_Count
  //   << value << "\t"
  //   << ( 100 * m_Index + static_cast< int >( 100.0 * value ) );

  emit ProgressValueChanged(100 * m_Index + static_cast<int>(100.0 * value));
}

/*******************************************************************************/
QObject* OverviewBuilder::virtual_Do()
{
  /*
  emit ProgressTextChanged(
    tr( "Importing image '%1' as dataset into cache directory..." )
    .arg( QFileInfo( m_Filename ).fileName() )
  );
  */

  // emit ProgressTextChanged( GetFirstProgressText() );

  //
  // Count elements to process.
  m_Count = 0;

  for (GDALOverviewsBuilderVector::const_iterator it(m_GDALOverviewsBuilders.begin()); it != m_GDALOverviewsBuilders.end(); ++it)
    if (!it->IsNull() && (*it)->GetNbResolutions() > 0)
      ++m_Count;

  emit ProgressRangeChanged(0, 100 * m_Count);

  //
  // Process elements.
  m_Index = 0;

  {
    ProcessObjectObserver::Pointer observer(ProcessObjectObserver::New());

    observer->SetProgressInterface(this);

    for (GDALOverviewsBuilderVector::const_iterator it(m_GDALOverviewsBuilders.begin()); it != m_GDALOverviewsBuilders.end(); ++it, ++m_Index)
      if (!it->IsNull() && (*it)->GetNbResolutions() > 0)
      {
        unsigned long id = (*it)->AddObserver(itk::ProgressEvent(), observer);

        (*it)->Update();

        (*it)->RemoveObserver(id);

        emit ProgressTextChanged(
            QString(tr("Generating overviews for file %1/%2 '%3'.")).arg(m_Index + 1).arg(m_Count).arg(QFile::decodeName((*it)->GetInputFileName().c_str())));
      }
  }

  emit ProgressValueChanged(100 * m_Count);

  return NULL;
}

/*******************************************************************************/
QString OverviewBuilder::virtual_GetFirstProgressText() const
{
  return QString(tr("Preparing to build GDAL overviews..."));
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

} // end namespace 'mvd'
