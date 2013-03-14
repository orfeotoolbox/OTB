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
#include "Core/mvdVectorImageModel.h"

namespace mvd
{

MainWindowTitleLoader
::MainWindowTitleLoader(core::VectorImageModel* model):
  m_Model(model)
{
}

void
MainWindowTitleLoader
::LoadTitle()
{
  // This method is executed in a separate thread

  std::ostringstream oss;
  oss<<PROJECT_NAME<<" - "<<otb::System::GetShortFileName(core::ToStdString(m_Model->GetFilename()));
  oss<<" ("<<m_Model->GetNbComponents()<<tr(" bands, ").toLatin1().constData();
  oss<<m_Model->GetNativeLargestRegion().GetSize()[0];
  oss<<"x"<<m_Model->GetNativeLargestRegion().GetSize()[1]<<tr(" pixels)").toLatin1().constData();
  
  emit TitleLoaded(core::FromStdString(oss.str()) );

  // add the placename to the title if any
  std::string placename = m_Model->GetCenterPixelPlaceName();
  if (!placename.empty())
    {
    oss <<" - " <<core::ToStdString( tr("Location") )<< " : " << placename;
    emit TitleLoaded(core::FromStdString(oss.str()) );
    }

  emit Finished();
}

} // end namespace 'mvd'
