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


#include "mvdImagePlacenameLoader.h"

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

/*****************************************************************************/
ImagePlacenameLoader::ImagePlacenameLoader(VectorImageModel* model) : m_Model(model)
{
}

/*****************************************************************************/
void ImagePlacenameLoader::LoadPlacename()
{
  // This method is executed in a separate thread

  // get the placename if any
  std::string placename = m_Model->GetCenterPixelPlaceName();
  if (!placename.empty())
  {
    // emit a signal to
    emit PlacenameLoaded(FromStdString(placename));
  }

  emit Finished();
}

} // end namespace 'mvd'
