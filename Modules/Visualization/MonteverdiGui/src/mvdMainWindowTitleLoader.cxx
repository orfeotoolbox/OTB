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


#include "mvdMainWindowTitleLoader.h"

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
#include "mvdVectorImageModel.h"

namespace mvd
{

MainWindowTitleLoader::MainWindowTitleLoader(VectorImageModel* model) : m_Model(model)
{
}

void MainWindowTitleLoader::LoadTitle()
{
  // This method is executed in a separate thread

  assert(m_Model != NULL);

#if 0
  std::ostringstream oss;
  oss<<PROJECT_NAME<<" - "<<otb::System::GetShortFileName( QFile::encodeName( m_Model->GetFilename() ));
  oss<<" ("<<m_Model->GetNbComponents()<<tr(" bands, ").toLatin1().constData();
  oss<<m_Model->GetNativeLargestRegion().GetSize()[0];
  oss<<"x"<<m_Model->GetNativeLargestRegion().GetSize()[1]<<tr(" pixels)").toLatin1().constData();
  
  Q_EMIT TitleLoaded(FromStdString(oss.str()) );

  // add the placename to the title if any
  std::string placename = m_Model->GetCenterPixelPlaceName();
  if (!placename.empty())
    {
    oss <<" - " <<ToStdString( tr("Location") )<< " : " << placename;
    Q_EMIT TitleLoaded(FromStdString(oss.str()) );
    }

#else
  SizeType size(m_Model->GetNativeLargestRegion().GetSize());

  QString placename(FromStdString(m_Model->GetCenterPixelPlaceName()));

  QString title(tr(PROJECT_NAME " - %1 (%2 bands, %3x%4 pixels)%5")
                    .arg(QFileInfo(m_Model->GetFilename()).fileName())
                    .arg(m_Model->GetNbComponents())
                    .arg(size[0])
                    .arg(size[1])
                    .arg(placename.isEmpty() ? "" : placename.prepend(" ")));

  qDebug() << "Title:" << title;

  Q_EMIT TitleLoaded(title);

#endif

  Q_EMIT Finished();
}

} // end namespace 'mvd'
