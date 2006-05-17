/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P.Imbo
  Language  :   C++
  Date      :   17 mai 2006
  Version   :   
  Role      :   Read/Write ONERA format data
  $Id$

=========================================================================*/
#include "otbONERAImageIOFactory.h"

#include "itkCreateObjectFunction.h"
#include "otbONERAImageIO.h"
#include "itkVersion.h"

  
namespace otb
{

ONERAImageIOFactory::ONERAImageIOFactory()
{
  this->RegisterOverride("itkImageIOBase",
                         "otbONERAImageIO",
                         "ONERA Image IO",
                         1,
                         itk::CreateObjectFunction<ONERAImageIO>::New());
}
  
ONERAImageIOFactory::~ONERAImageIOFactory()
{
}

const char* 
ONERAImageIOFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char* 
ONERAImageIOFactory::GetDescription() const
{
  return "ONERA ImageIO Factory, permettant le chargement d'image au format ONERA dans l'OTB";
}

} // end namespace otb

