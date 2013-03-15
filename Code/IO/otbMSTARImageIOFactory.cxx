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
#include "otbMSTARImageIOFactory.h"
#include "itkCreateObjectFunction.h"
#include "otbMSTARImageIO.h"
#include "itkVersion.h"

namespace otb
{

MSTARImageIOFactory::MSTARImageIOFactory()
{
  this->RegisterOverride("otbImageIOBase",
                         "otbMSTARImageIO",
                         "MSTAR Image IO",
                         1,
                         itk::CreateObjectFunction<MSTARImageIO>::New());
}

MSTARImageIOFactory::~MSTARImageIOFactory()
{
}

const char*
MSTARImageIOFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char*
MSTARImageIOFactory::GetDescription() const
{
  return "MSTAR ImageIO Factory, permettant le chargement d'image MSTAR dans l'OTB";
}

} // end namespace otb
