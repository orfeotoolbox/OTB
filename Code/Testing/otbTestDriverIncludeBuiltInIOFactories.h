
#include "otbONERAImageIOFactory.h"
#include "otbMSTARImageIOFactory.h"
#include "otbGDALImageIOFactory.h"
#include "otbLUMImageIOFactory.h"
#include "otbBSQImageIOFactory.h"
#include "otbRADImageIOFactory.h"
#include "otbMWImageIOFactory.h"

#ifdef OTB_USE_JPEG2000
#include "otbJPEG2000ImageIOFactory.h"
#endif

#ifdef OTB_USE_CURL
#include "otbTileMapImageIOFactory.h"
#endif


void RegisterOTBBuiltInFactories(void)
{
  // RAD Format for OTB
  itk::ObjectFactoryBase::RegisterFactory(otb::RADImageIOFactory::New());

  // BSQ format for OTB
  itk::ObjectFactoryBase::RegisterFactory(otb::BSQImageIOFactory::New());

  // LUM format for OTB
  itk::ObjectFactoryBase::RegisterFactory(otb::LUMImageIOFactory::New());

#ifdef OTB_USE_JPEG2000
  // JPEG2000 : New format for OTB
  itk::ObjectFactoryBase::RegisterFactory(otb::JPEG2000ImageIOFactory::New());
#endif

  // GDAL : New format for OTB
  itk::ObjectFactoryBase::RegisterFactory(otb::GDALImageIOFactory::New());
  // MegaWave format for OTB
  itk::ObjectFactoryBase::RegisterFactory(otb::MWImageIOFactory::New());

  // ONERA format for OTB
  itk::ObjectFactoryBase::RegisterFactory(otb::ONERAImageIOFactory::New());

  // MSTAR Format for OTB
  itk::ObjectFactoryBase::RegisterFactory(otb::MSTARImageIOFactory::New());

#ifdef OTB_USE_CURL
  // TileMap : New format for OTB
  itk::ObjectFactoryBase::RegisterFactory(otb::TileMapImageIOFactory::New());
#endif

}
