/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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


#include "otbSarImageMetadataInterfaceFactory.h"
#include "otbSarDefaultImageMetadataInterface.h"

// SAR Sensors
#include "otbTerraSarXSarImageMetadataInterfaceFactory.h"
#include "otbSentinel1ImageMetadataInterfaceFactory.h"
#include "otbCosmoImageMetadataInterfaceFactory.h"
#include "otbRadarsat2ImageMetadataInterfaceFactory.h"

#include "itkMutexLock.h"
#include "itkMutexLockHolder.h"

namespace otb
{

SarImageMetadataInterfaceFactory::SarImageMetadataInterfacePointerType
SarImageMetadataInterfaceFactory
::CreateIMI(ImageMetadata & imd, const MetadataSupplierInterface & mds)
{
  RegisterBuiltInFactories();

  auto allObjects     = itk::ObjectFactoryBase::CreateAllInstance("SarImageMetadataInterface");

  for (auto i = allObjects.begin(); i != allObjects.end(); ++i)
  {
    SarImageMetadataInterface* io = dynamic_cast<SarImageMetadataInterface*>(i->GetPointer());
    if (io)
    {
      // the static part of ImageMetadata is already filled
      io->SetMetadataSupplierInterface(mds);
      try
      {
        io->Parse(imd);
        return io;
      }
      catch(MissingMetadataException& e)
      {
        // silent catch of MissingMetadataException
        // just means that this IMI can't parse the file
        (void)e; // keep this line to silent unreferenced local var warning
      }
    }
    else
    {
      itkGenericExceptionMacro(<< "Error SarImageMetadataInterface factory did not return an SarImageMetadataInterface: " << (*i)->GetNameOfClass());
    }
  }

  SarDefaultImageMetadataInterface::Pointer defaultIMI = SarDefaultImageMetadataInterface::New();
  return dynamic_cast<SarImageMetadataInterface*>(static_cast<SarDefaultImageMetadataInterface*>(defaultIMI));
}

void SarImageMetadataInterfaceFactory::RegisterBuiltInFactories()
{
  static bool firstTime = true;

  static itk::SimpleMutexLock mutex;
  {
    // This helper class makes sure the Mutex is unlocked
    // in the event an exception is thrown.
    itk::MutexLockHolder<itk::SimpleMutexLock> mutexHolder(mutex);
    if (firstTime)
    {
      itk::ObjectFactoryBase::RegisterFactory(TerraSarXSarImageMetadataInterfaceFactory::New());
      itk::ObjectFactoryBase::RegisterFactory(Sentinel1ImageMetadataInterfaceFactory::New());
      itk::ObjectFactoryBase::RegisterFactory(CosmoImageMetadataInterfaceFactory::New());
      itk::ObjectFactoryBase::RegisterFactory(Radarsat2ImageMetadataInterfaceFactory::New());
      firstTime = false;
    }
  }
}

} // end namespace otb
