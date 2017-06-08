/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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


#include "otbVectorDataIOFactory.h"
#include "otbOGRVectorDataIOFactory.h"
#ifdef OTB_USE_LIBKML
#include "otbKMLVectorDataIOFactory.h"
#endif

#include "itkObjectFactoryBase.h"
#include "itkMutexLock.h"
#include "itkMutexLockHolder.h"

namespace otb
{

VectorDataIOFactory
::VectorDataIOBasePointerType
VectorDataIOFactory
::CreateVectorDataIO(const char* path, FileModeType mode)
{
  RegisterBuiltInFactories();

  std::string baseclassID = std::string("otbVectorDataIOBase");

  std::list<VectorDataIOBasePointerType> possibleVectorDataIO;
  std::list<itk::LightObject::Pointer>   allobjects =
    itk::ObjectFactoryBase::CreateAllInstance(baseclassID.c_str());
  for (auto & allobject : allobjects)
    {
    VectorDataIOBaseType * io = dynamic_cast<VectorDataIOBaseType*>(allobject.GetPointer());
    if (io)
      {
      possibleVectorDataIO.push_back(io);
      }
    else
      {
      itkGenericExceptionMacro(<< "Error VectorDataIO factory did not return an VectorDataIOBase: "
                               << allobject->GetNameOfClass());
      }
    }
  for (auto & k : possibleVectorDataIO)
    {
    if (mode == ReadMode)
      {
      if (k->CanReadFile(path))
        {
        return k;
        }
      }
    else if (mode == WriteMode)
      {
      if (k->CanWriteFile(path))
        {
        return k;
        }

      }
    }
  return nullptr;
}

void
VectorDataIOFactory
::RegisterBuiltInFactories()
{
  static bool firstTime = true;

  static itk::SimpleMutexLock mutex;
    {
    // This helper class makes sure the Mutex is unlocked
    // in the event an exception is thrown.
    itk::MutexLockHolder<itk::SimpleMutexLock> mutexHolder(mutex);
    if (firstTime)
      {
      itk::ObjectFactoryBase::RegisterFactory(OGRVectorDataIOFactory::New());
      #ifdef OTB_USE_LIBKML
      itk::ObjectFactoryBase::RegisterFactory(KMLVectorDataIOFactory::New());
      #endif
      firstTime = false;
      }
    }
}

} // end namespace otb

