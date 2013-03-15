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
#include "otbLUMImageIOFactory.h"

#include "itkCreateObjectFunction.h"
#include "otbLUMImageIO.h"
#include "itkVersion.h"

namespace otb
{

LUMImageIOFactory::LUMImageIOFactory()
{
  this->RegisterOverride("otbImageIOBase",
                         "otbLUMImageIO",
                         "LUM Image IO",
                         1,
                         itk::CreateObjectFunction<LUMImageIO>::New());
}

LUMImageIOFactory::~LUMImageIOFactory()
{
}

const char*
LUMImageIOFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char*
LUMImageIOFactory::GetDescription() const
{
  return "LUM ImageIO Factory, permettant le chargement d'image au format LUM dans l'OTB";
}

} // end namespace otb
