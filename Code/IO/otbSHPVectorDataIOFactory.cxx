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
#include "otbSHPVectorDataIOFactory.h"

#include "itkCreateObjectFunction.h"
#include "itkVersion.h"
#include "otbSHPVectorDataIO.h"

namespace otb
{

SHPVectorDataIOFactory::SHPVectorDataIOFactory()
{
  this->RegisterOverride("otbVectorDataIOBase",
                         "otbSHPVectorDataIO",
                         "SHP Vectordata IO",
                         1,
                         itk::CreateObjectFunction<SHPVectorDataIO>::New());
}
  
SHPVectorDataIOFactory::~SHPVectorDataIOFactory()
{
}

const char* 
SHPVectorDataIOFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char* 
SHPVectorDataIOFactory::GetDescription() const
{
  return "SHP VectorDataIO Factory, allows the loading of SHP vector data into OTB";
}

} // end namespace otb

