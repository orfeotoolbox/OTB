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

#include "otbVectorDataKeywordlist.h"

void
otb::VectorDataKeywordlist
  ::Print(std::ostream& os) const
{
  os << "[UNKNOWN_PRINT_CHARACTERISTICS]" << std::endl;
}

otb::VectorDataKeywordlist
  ::VectorDataKeywordlist()
{
  //Nothing to do here
}

otb::VectorDataKeywordlist
  ::~VectorDataKeywordlist()
{
  //Nothing to do here
}
