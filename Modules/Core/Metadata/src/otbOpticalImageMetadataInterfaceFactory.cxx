/*
 * Copyright (C) 2005-2022 Centre National d'Etudes Spatiales (CNES)
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


#include "otbOpticalImageMetadataInterfaceFactory.h"
#include "otbOpticalDefaultImageMetadataInterface.h"

// Optical sensors
#include "otbIkonosImageMetadataInterfaceFactory.h"
#include "otbSpotImageMetadataInterfaceFactory.h"
#include "otbFormosatImageMetadataInterfaceFactory.h"
#include "otbQuickBirdImageMetadataInterfaceFactory.h"
#include "otbWorldView2ImageMetadataInterfaceFactory.h"
#include "otbPleiadesImageMetadataInterfaceFactory.h"
#include "otbSpot6ImageMetadataInterfaceFactory.h"

#if ITK_VERSION_MAJOR < 5
#include "itkMutexLock.h"
#include "itkMutexLockHolder.h"
#else
#include <mutex>
#endif

namespace otb
{


void OpticalImageMetadataInterfaceFactory::RegisterBuiltInFactories()
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
      itk::ObjectFactoryBase::RegisterFactory(IkonosImageMetadataInterfaceFactory::New());
      itk::ObjectFactoryBase::RegisterFactory(SpotImageMetadataInterfaceFactory::New());
      itk::ObjectFactoryBase::RegisterFactory(FormosatImageMetadataInterfaceFactory::New());
      itk::ObjectFactoryBase::RegisterFactory(QuickBirdImageMetadataInterfaceFactory::New());
      itk::ObjectFactoryBase::RegisterFactory(WorldView2ImageMetadataInterfaceFactory::New());
      itk::ObjectFactoryBase::RegisterFactory(PleiadesImageMetadataInterfaceFactory::New());
      itk::ObjectFactoryBase::RegisterFactory(Spot6ImageMetadataInterfaceFactory::New());
      firstTime = false;
    }
  }
}

} // end namespace otb
