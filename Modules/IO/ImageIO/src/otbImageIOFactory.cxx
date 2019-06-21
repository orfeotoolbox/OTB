/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "otbImageIOFactory.h"
#include "otbConfigure.h"

#include "otbONERAImageIOFactory.h"
#include "otbMSTARImageIOFactory.h"
#include "otbGDALImageIOFactory.h"
#include "otbLUMImageIOFactory.h"
#include "otbBSQImageIOFactory.h"
#include "otbRADImageIOFactory.h"

#if ITK_VERSION_MAJOR < 5
#include "itkMutexLock.h"
#include "itkMutexLockHolder.h"
#else
#include <mutex>
#endif

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
      itkGenericExceptionMacro(<< "ImageIO factory did not return an ImageIOBase but a " << (*i)->GetNameOfClass());
    }
  }

  for(std::list<otb::ImageIOBase::Pointer>::iterator k = possibleImageIO.begin(); k != possibleImageIO.end(); ++k)
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
  return nullptr;
}

void
ImageIOFactory::RegisterBuiltInFactories()
{
  static bool firstTime = true;

  #if ITK_VERSION_MAJOR < 5
  static itk::SimpleMutexLock mutex;
  #else
  static std::mutex mutex;
  #endif
    {
    // This helper class makes sure the Mutex is unlocked
    // in the event an exception is thrown.
    #if ITK_VERSION_MAJOR < 5
    itk::MutexLockHolder<itk::SimpleMutexLock> mutexHolder(mutex);
    #else
    std::lock_guard<std::mutex> mutexHolder(mutex);
    #endif
    if (firstTime)
      {
      itk::ObjectFactoryBase::RegisterFactory(RADImageIOFactory::New());
      itk::ObjectFactoryBase::RegisterFactory(BSQImageIOFactory::New());
      itk::ObjectFactoryBase::RegisterFactory(LUMImageIOFactory::New());
      itk::ObjectFactoryBase::RegisterFactory(GDALImageIOFactory::New());
      itk::ObjectFactoryBase::RegisterFactory(ONERAImageIOFactory::New());
      itk::ObjectFactoryBase::RegisterFactory(MSTARImageIOFactory::New());
      firstTime = false;
      }
    }
}

} // end namespace otb
