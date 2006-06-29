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
#include "otbCAIImageIOFactory.h"
#include "itkCreateObjectFunction.h"
#include "otbCAIImageIO.h"
#include "itkVersion.h"

  
namespace otb
{

CAIImageIOFactory::CAIImageIOFactory()
{
  this->RegisterOverride("itkImageIOBase",
                         "otbCAIImageIO",
                         "CAI Image IO",
                         1,
                         itk::CreateObjectFunction<CAIImageIO>::New());
}
  
CAIImageIOFactory::~CAIImageIOFactory()
{
}

const char* 
CAIImageIOFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char* 
CAIImageIOFactory::GetDescription() const
{
  return "CAI ImageIO Factory, permettant le chargement d'image CAI dans l'OTB";
}

} // end namespace otb

