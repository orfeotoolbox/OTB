#include "otbMSTARImageIOFactory.h"
#include "itkCreateObjectFunction.h"
#include "otbMSTARImageIO.h"
#include "itkVersion.h"

  
namespace otb
{

MSTARImageIOFactory::MSTARImageIOFactory()
{
  this->RegisterOverride("itkImageIOBase",
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

