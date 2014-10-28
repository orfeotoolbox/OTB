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


#include "itkMutexLock.h"
#include "itkMutexLockHolder.h"
#include "otbCurlHelperInterface.h"

#include "otbImageIOFactory.h"

#include "otbONERAImageIOFactory.h"
#include "otbMSTARImageIOFactory.h"
#include "otbGDALImageIOFactory.h"
#include "otbLUMImageIOFactory.h"
#include "otbBSQImageIOFactory.h"
#include "otbRADImageIOFactory.h"
#include "otbMWImageIOFactory.h"
#ifdef OTB_USE_JPEG2000
#  include "otbJPEG2000ImageIOFactory.h"
#endif
#include "otbTileMapImageIOFactory.h"

namespace otb
{

otb::ImageIOBase::Pointer
ImageIOFactory::CreateImageIO(const char* path, FileModeType mode)
{
  RegisterBuiltInFactories();

  std::list<otb::ImageIOBase::Pointer> possibleImageIO;
  std::list<itk::LightObject::Pointer> allobjects =
    itk::ObjectFactoryBase::CreateAllInstance("otbImageIOBase");
  for(std::list<itk::LightObject::Pointer>::iterator i = allobjects.begin();
      i != allobjects.end(); ++i)
    {
    otb::ImageIOBase* io = dynamic_cast<otb::ImageIOBase*>(i->GetPointer());
    if(io)
      {
      possibleImageIO.push_back(io);
      }
    else
      {
      std::cerr << "Error ImageIO factory did not return an ImageIOBase: "
                << (*i)->GetNameOfClass()
                << std::endl;
      }
    }
  for(std::list<otb::ImageIOBase::Pointer>::iterator k = possibleImageIO.begin();
      k != possibleImageIO.end(); ++k)
    {
    if( mode == ReadMode )
      {
      if((*k)->CanReadFile(path))
        {
        return *k;
        }
      }
    else if( mode == WriteMode )
      {
      if((*k)->CanWriteFile(path))
        {
        return *k;
        }

      }
    }
  return 0;
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
      itk::ObjectFactoryBase::RegisterFactory(RADImageIOFactory::New());
      itk::ObjectFactoryBase::RegisterFactory(BSQImageIOFactory::New());
      itk::ObjectFactoryBase::RegisterFactory(LUMImageIOFactory::New());
#ifdef OTB_USE_JPEG2000
      itk::ObjectFactoryBase::RegisterFactory(JPEG2000ImageIOFactory::New());
#endif
      if (CurlHelperInterface::IsCurlAvailable())
        {
        itk::ObjectFactoryBase::RegisterFactory(TileMapImageIOFactory::New());
        }
      itk::ObjectFactoryBase::RegisterFactory(GDALImageIOFactory::New());
      itk::ObjectFactoryBase::RegisterFactory(MWImageIOFactory::New());
      itk::ObjectFactoryBase::RegisterFactory(ONERAImageIOFactory::New());
      itk::ObjectFactoryBase::RegisterFactory(MSTARImageIOFactory::New());

      firstTime = false;
      }
    }
}

} // end namespace otb
