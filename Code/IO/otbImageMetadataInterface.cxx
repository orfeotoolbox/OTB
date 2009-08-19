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

#include "otbImageMetadataInterface.h"
#include "otbIkonosImageMetadataInterface.h"
#include "otbSpotImageMetadataInterface.h"
#include "otbQuickBirdImageMetadataInterface.h"

#include "itkObjectFactoryBase.h"
#include "itkMutexLock.h"
#include "itkMutexLockHolder.h"

namespace otb
{
ImageMetadataInterface::ImageMetadataInterfaceBasePointerType
ImageMetadataInterface
::CreateIMI( const MetaDataDictionaryType & dict )
{
  RegisterBuiltInFactories();

  std::list<ImageMetadataInterfaceBasePointerType> possibleIMI;
  std::list<itk::LightObject::Pointer> allobjects =
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
  	std::cout<<"I FOUND IT??????????"<<std::endl;
    if ((*k)->CanRead(dict))
    {
    	std::cout<<"I FOUND IT!!!!!!!"<<std::endl;
      return *k;
    }
  }
  return 0;
}


void
ImageMetadataInterface
::RegisterBuiltInFactories()
{
  static bool firstTime = true;

  static itk::SimpleMutexLock mutex;
  {
    // This helper class makes sure the Mutex is unlocked
    // in the event an exception is thrown.
    itk::MutexLockHolder<itk::SimpleMutexLock> mutexHolder( mutex );
    if ( firstTime )
    {
      itk::ObjectFactoryBase::RegisterFactory( IkonosImageMetadataInterfaceFactory::New() );
      itk::ObjectFactoryBase::RegisterFactory( SpotImageMetadataInterfaceFactory::New() );
      itk::ObjectFactoryBase::RegisterFactory( QuickBirdImageMetadataInterfaceFactory::New() );
      firstTime = false;
    }
  }
}

} // end namespace otb

