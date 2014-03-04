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

#include "itkObjectFactoryBase.h"
#include "itkMutexLock.h"
#include "itkMutexLockHolder.h"

namespace otb
{
OpticalImageMetadataInterfaceFactory::OpticalImageMetadataInterfacePointerType
OpticalImageMetadataInterfaceFactory
::CreateIMI(const MetaDataDictionaryType& dict)
{
  RegisterBuiltInFactories();

  std::list<OpticalImageMetadataInterfacePointerType> possibleIMI;
  std::list<itk::LightObject::Pointer>             allobjects =
    itk::ObjectFactoryBase::CreateAllInstance("OpticalImageMetadataInterface");
  for (std::list<itk::LightObject::Pointer>::iterator i = allobjects.begin();
       i != allobjects.end(); ++i)
    {
    OpticalImageMetadataInterface * io = dynamic_cast<OpticalImageMetadataInterface*>(i->GetPointer());
    if (io)
      {
      possibleIMI.push_back(io);
      }
    else
      {
      itkGenericExceptionMacro(<< "Error OpticalImageMetadataInterface factory did not return an OpticalImageMetadataInterface: "
                               << (*i)->GetNameOfClass());
      }
    }
  for (std::list<OpticalImageMetadataInterfacePointerType>::iterator k = possibleIMI.begin();
       k != possibleIMI.end(); ++k)
    {
   (*k)->SetMetaDataDictionary(dict);
    if ((*k)->CanRead())
      {
      return *k;
      }
    }

  OpticalDefaultImageMetadataInterface::Pointer defaultIMI = OpticalDefaultImageMetadataInterface::New();
  return dynamic_cast<OpticalImageMetadataInterface*>(static_cast<OpticalDefaultImageMetadataInterface*>(defaultIMI));
}

void
OpticalImageMetadataInterfaceFactory
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
