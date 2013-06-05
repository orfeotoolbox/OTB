/*=========================================================================

  Program:   Monteverdi2
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

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
#include "Core/mvdVectorImageModel.h"

namespace mvd
{

/*****************************************************************************/
ImagePlacenameLoader
::ImagePlacenameLoader(VectorImageModel* model):
  m_Model(model)
{
}

/*****************************************************************************/
void
ImagePlacenameLoader
::LoadPlacename()
{
  // This method is executed in a separate thread

  // get the placename if any
  std::string placename = m_Model->GetCenterPixelPlaceName();
  if (!placename.empty())
    {
    // emit a signal to
    emit PlacenameLoaded( FromStdString( placename) );
    }

  emit Finished();
}

} // end namespace 'mvd'
