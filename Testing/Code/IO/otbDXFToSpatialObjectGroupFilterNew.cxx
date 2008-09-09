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
#include "itkExceptionObject.h"

#include "otbDXFToSpatialObjectGroupFilter.h"
#include "itkGroupSpatialObject.h"

int otbDXFToSpatialObjectGroupFilterNew(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  typedef itk::GroupSpatialObject<Dimension> GroupType;
  typedef otb::DXFToSpatialObjectGroupFilter<GroupType> DXFToSpatialObjectGroupFilterType;
  
  // Instantiating object
  DXFToSpatialObjectGroupFilterType::Pointer object = DXFToSpatialObjectGroupFilterType::New();
  
  
  return EXIT_SUCCESS;
}
