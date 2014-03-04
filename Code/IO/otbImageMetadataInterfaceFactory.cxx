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


#include "otbImageMetadataInterfaceFactory.h"

#include "otbDefaultImageMetadataInterface.h"

// Optical sensors
#include "otbIkonosImageMetadataInterfaceFactory.h"
#include "otbSpotImageMetadataInterfaceFactory.h"
#include "otbPleiadesImageMetadataInterfaceFactory.h"
#include "otbSpot6ImageMetadataInterfaceFactory.h"
#include "otbFormosatImageMetadataInterfaceFactory.h"
#include "otbQuickBirdImageMetadataInterfaceFactory.h"
#include "otbWorldView2ImageMetadataInterfaceFactory.h"

// SAR Sensors
#include "otbTerraSarImageMetadataInterfaceFactory.h"

#include "itkObjectFactoryBase.h"
#include "itkMutexLock.h"
#include "itkMutexLockHolder.h"

#include <iostream>
#include <iterator>
#include <vector>

namespace otb
{
ImageMetadataInterfaceFactory::ImageMetadataInterfaceBasePointerType
ImageMetadataInterfaceFactory
::CreateIMI(const MetaDataDictionaryType& dict)
{
  RegisterBuiltInFactories();

  std::list<ImageMetadataInterfaceBasePointerType> possibleIMI;
  std::list<itk::LightObject::Pointer>             allOpticalObjects =
    itk::ObjectFactoryBase::CreateAllInstance("OpticalImageMetadataInterface");
  std::list<itk::LightObject::Pointer>             allSarObjects =
    itk::ObjectFactoryBase::CreateAllInstance("SarImageMetadataInterface");
  std::list<itk::LightObject::Pointer>             allObjects;

  std::copy(allOpticalObjects.begin(), allOpticalObjects.end(), std::back_inserter(allObjects) );
  std::copy(allSarObjects.begin(), allSarObjects.end(), std::back_inserter(allObjects) );


  for (std::list<itk::LightObject::Pointer>::iterator i = allObjects.begin();
       i != allObjects.end(); ++i)
    {
    ImageMetadataInterfaceBase * io = dynamic_cast<ImageMetadataInterfaceBase*>(i->GetPointer());
    if (io)
      {
      possibleIMI.push_back(io);
      }
    else
      {
      itkGenericExceptionMacro(<< "Error ImageMetadataInterface factory did not return an ImageMetadataInterfaceBase: "
                               << (*i)->GetNameOfClass());
      }
    }

  for (std::list<ImageMetadataInterfaceBasePointerType>::iterator k = possibleIMI.begin();
       k != possibleIMI.end(); ++k)
    {
    (*k)->SetMetaDataDictionary(dict);
    if ((*k)->CanRead())
      {
      return *k;
      }
    }

  DefaultImageMetadataInterface::Pointer defaultIMI = DefaultImageMetadataInterface::New();
  defaultIMI->SetMetaDataDictionary(dict);
  return dynamic_cast<ImageMetadataInterfaceBase*>(static_cast<DefaultImageMetadataInterface*>(defaultIMI));
}

void
ImageMetadataInterfaceFactory
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
      itk::ObjectFactoryBase::RegisterFactory(IkonosImageMetadataInterfaceFactory::New());
      itk::ObjectFactoryBase::RegisterFactory(SpotImageMetadataInterfaceFactory::New());
      itk::ObjectFactoryBase::RegisterFactory(PleiadesImageMetadataInterfaceFactory::New());
      itk::ObjectFactoryBase::RegisterFactory(Spot6ImageMetadataInterfaceFactory::New());
      itk::ObjectFactoryBase::RegisterFactory(FormosatImageMetadataInterfaceFactory::New());
      itk::ObjectFactoryBase::RegisterFactory(QuickBirdImageMetadataInterfaceFactory::New());
      itk::ObjectFactoryBase::RegisterFactory(WorldView2ImageMetadataInterfaceFactory::New());
      itk::ObjectFactoryBase::RegisterFactory( TerraSarImageMetadataInterfaceFactory::New() );
      firstTime = false;
      }
    }
}

} // end namespace otb
