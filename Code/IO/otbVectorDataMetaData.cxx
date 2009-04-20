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

#include "otbVectorDataMetaData.h"

void
otb::VectorDataMetaData
  ::Print(std::ostream& os) const
{
  os << "[UNKNOWN_PRINT_CHARACTERISTICS]" << std::endl;
}

otb::VectorDataMetaData
  ::VectorDataMetaData()
{
  //Nothing to do here
}

otb::VectorDataMetaData
  ::~VectorDataMetaData()
{
  //Nothing to do here
}
