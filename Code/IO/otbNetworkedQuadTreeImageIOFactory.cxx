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
#include "otbNetworkedQuadTreeImageIOFactory.h"

#include "itkCreateObjectFunction.h"
#include "otbNetworkedQuadTreeImageIO.h"
#include "itkVersion.h"

  
namespace otb
{

NetworkedQuadTreeImageIOFactory::NetworkedQuadTreeImageIOFactory()
{
  this->RegisterOverride("itkImageIOBase",
                         "otbNetworkedQuadTreeImageIO",
                         "NetworkedQuadTree Image IO",
                         1,
                         itk::CreateObjectFunction<NetworkedQuadTreeImageIO>::New());
}
  
NetworkedQuadTreeImageIOFactory::~NetworkedQuadTreeImageIOFactory()
{
}

const char* 
NetworkedQuadTreeImageIOFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char* 
NetworkedQuadTreeImageIOFactory::GetDescription() const
{
  return "NetworkedQuadTree ImageIO Factory, enabling NetworkedQuadTree image format loading in OTB";
}

} // end namespace otb

