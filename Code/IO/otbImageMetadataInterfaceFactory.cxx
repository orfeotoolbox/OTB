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

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include "otbImageMetadataInterfaceFactory.h"

#include "otbDefaultImageMetadataInterface.h"
// Optical sensors
#include "otbIkonosImageMetadataInterfaceFactory.h"
#include "otbSpotImageMetadataInterfaceFactory.h"
#include "otbQuickBirdImageMetadataInterfaceFactory.h"
// SAR Sensors
//#include "otbTerraSarImageMetadataInterfaceFactory.h"

#include "itkObjectFactoryBase.h"
#include "itkMutexLock.h"
#include "itkMutexLockHolder.h"

namespace otb
{
ImageMetadataInterfaceFactory::ImageMetadataInterfaceBasePointerType
ImageMetadataInterfaceFactory
::CreateIMI(const MetaDataDictionaryType& dict)
{
  RegisterBuiltInFactories();

  std::list<ImageMetadataInterfaceBasePointerType> possibleIMI;
  std::list<itk::LightObject::Pointer>             allobjects =
    itk::ObjectFactoryBase::CreateAllInstance("ImageMetadataInterfaceBase");
  for (std::list<itk::LightObject::Pointer>::iterator i = allobjects.begin();
       i != allobjects.end(); ++i)
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
    if ((*k)->CanRead(dict))
      {
      return *k;
      }
    }

  DefaultImageMetadataInterface::Pointer defaultIMI = DefaultImageMetadataInterface::New();
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
      itk::ObjectFactoryBase::RegisterFactory(QuickBirdImageMetadataInterfaceFactory::New());
      //itk::ObjectFactoryBase::RegisterFactory( TerraSarImageMetadataInterfaceFactory::New() );
      firstTime = false;
      }
    }
}

} // end namespace otb
