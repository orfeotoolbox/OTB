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


#include "otbSarImageMetadataInterfaceFactory.h"
#include "otbSarDefaultImageMetadataInterface.h"

// SAR Sensors
#include "otbSentinel1ImageMetadataInterfaceFactory.h"
#include "otbTerraSarImageMetadataInterfaceFactory.h"
#include "otbRadarsat2ImageMetadataInterfaceFactory.h"
#include "otbCosmoSkymedImageMetadataInterfaceFactory.h"

#include "itkMutexLock.h"
#include "itkMutexLockHolder.h"

namespace otb
{
SarImageMetadataInterfaceFactory::SarImageMetadataInterfacePointerType
SarImageMetadataInterfaceFactory
::CreateIMI(const MetaDataDictionaryType& dict)
{
  RegisterBuiltInFactories();

  std::list<SarImageMetadataInterfacePointerType> possibleIMI;
  std::list<itk::LightObject::Pointer>             allobjects =
    itk::ObjectFactoryBase::CreateAllInstance("SarImageMetadataInterface");
  for (std::list<itk::LightObject::Pointer>::iterator i = allobjects.begin();
       i != allobjects.end(); ++i)
    {
    SarImageMetadataInterface * io = dynamic_cast<SarImageMetadataInterface*>(i->GetPointer());
    if (io)
      {
      possibleIMI.push_back(io);
      }
    else
      {
      itkGenericExceptionMacro(<< "Error SarImageMetadataInterface factory did not return an SarImageMetadataInterface: "
                               << (*i)->GetNameOfClass());
      }
    }
  for (std::list<SarImageMetadataInterfacePointerType>::iterator k = possibleIMI.begin();
       k != possibleIMI.end(); ++k)
    {
    (*k)->SetMetaDataDictionary(dict);
    if ((*k)->CanRead())
      {
      return *k;
      }
    }

  SarDefaultImageMetadataInterface::Pointer defaultIMI = SarDefaultImageMetadataInterface::New();
  return dynamic_cast<SarImageMetadataInterface*>(static_cast<SarDefaultImageMetadataInterface*>(defaultIMI));
}

void
SarImageMetadataInterfaceFactory
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
      itk::ObjectFactoryBase::RegisterFactory( TerraSarImageMetadataInterfaceFactory::New() );
      itk::ObjectFactoryBase::RegisterFactory( Sentinel1ImageMetadataInterfaceFactory::New() );
      itk::ObjectFactoryBase::RegisterFactory( Radarsat2ImageMetadataInterfaceFactory::New() );
      itk::ObjectFactoryBase::RegisterFactory( CosmoSkymedImageMetadataInterfaceFactory::New() );
      firstTime = false;
      }
    }
}

} // end namespace otb
