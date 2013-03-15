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

#include "otbConfigure.h"
#include "otbCurlHelperInterface.h"
#include "otbImageIOFactory.h"
#include "itkMutexLock.h"
#include "itkMutexLockHolder.h"

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

#include "otbTileMapImageIOFactory.h"

namespace otb
{

otb::ImageIOBase::Pointer
ImageIOFactory::CreateImageIO(const char* path, FileModeType mode)
{

  RegisterBuiltInFactories();
  return (Superclass::CreateImageIO(path, mode));

}

void
ImageIOFactory::RegisterBuiltInFactories()
{
  static bool firstTime = true;

  static itk::SimpleMutexLock mutex;
    {
    // This helper class makes sure the Mutex is unlocked
    // in the event an exception is thrown.
    itk::MutexLockHolder<itk::SimpleMutexLock> mutexHolder(mutex);
    if (firstTime)
      {
      // RAD Format for OTB
      itk::ObjectFactoryBase::RegisterFactory(RADImageIOFactory::New());

      // BSQ format for OTB
      itk::ObjectFactoryBase::RegisterFactory(BSQImageIOFactory::New());

      // LUM format for OTB
      itk::ObjectFactoryBase::RegisterFactory(LUMImageIOFactory::New());

#ifdef OTB_USE_JPEG2000
      // JPEG2000 : New format for OTB
      itk::ObjectFactoryBase::RegisterFactory(JPEG2000ImageIOFactory::New());
#endif

      if (CurlHelperInterface::IsCurlAvailable())
        {
        // TileMap : New format for OTB
        itk::ObjectFactoryBase::RegisterFactory(TileMapImageIOFactory::New());
        }

      // GDAL : New format for OTB
      itk::ObjectFactoryBase::RegisterFactory(GDALImageIOFactory::New());
      // MegaWave format for OTB
      itk::ObjectFactoryBase::RegisterFactory(MWImageIOFactory::New());

      // ONERA format for OTB
      itk::ObjectFactoryBase::RegisterFactory(ONERAImageIOFactory::New());

      // MSTAR Format for OTB
      itk::ObjectFactoryBase::RegisterFactory(MSTARImageIOFactory::New());

      firstTime = false;
      }
    }

}

} // end namespace otb
