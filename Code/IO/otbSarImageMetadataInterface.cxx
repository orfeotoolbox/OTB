/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include "otbMacro.h"

#include "otbSarImageMetadataInterface.h"

#include "itkMetaDataObject.h"
#include "otbVectorDataKeywordlist.h"
#include "base/ossimKeywordlist.h"

namespace otb
{

SarImageMetadataInterface
::SarImageMetadataInterface()
{
}


void
SarImageMetadataInterface
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  //os << indent << "GetSunElevation:     " << this->GetSunElevation() << std::endl;
}



} // end namespace otb
