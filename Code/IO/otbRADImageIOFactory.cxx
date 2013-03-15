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
#include "otbRADImageIOFactory.h"

#include "itkCreateObjectFunction.h"
#include "otbRADImageIO.h"
#include "itkVersion.h"

namespace otb
{

RADImageIOFactory::RADImageIOFactory()
{
  this->RegisterOverride("otbImageIOBase",
                         "otbRADImageIO",
                         "RAD Image IO",
                         1,
                         itk::CreateObjectFunction<RADImageIO>::New());
}

RADImageIOFactory::~RADImageIOFactory()
{
}

const char*
RADImageIOFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char*
RADImageIOFactory::GetDescription() const
{
  return "RAD ImageIO Factory, permettant le chargement d'image au format RAD dans l'OTB";
}

} // end namespace otb
