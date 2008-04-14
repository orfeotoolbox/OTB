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

#include "otbVectorDataIOFactory.h"

#include "itkObjectFactoryBase.h"
#include "itkMutexLock.h"
#include "itkMutexLockHolder.h"

namespace otb
{

VectorDataIOBase::Pointer
VectorDataIOFactory::CreateVectorDataIO(const char* path, FileModeType mode)
{

  RegisterBuiltInFactories();

  std::list<VectorDataIOBase::Pointer> possibleVectorDataIO;
  std::list<itk::LightObject::Pointer> allobjects =
    itk::ObjectFactoryBase::CreateAllInstance("otbVectorDataIOBase");
  for(std::list<itk::LightObject::Pointer>::iterator i = allobjects.begin();
      i != allobjects.end(); ++i)
    {
    VectorDataIOBase* io = dynamic_cast<VectorDataIOBase*>(i->GetPointer());
    if(io)
      {
      possibleVectorDataIO.push_back(io);
      }
    else
      {
      std::cerr << "Error VectorDataIO factory did not return an VectorDataIOBase: "
                << (*i)->GetNameOfClass()
                << std::endl;
      }
    }
  for(std::list<VectorDataIOBase::Pointer>::iterator k = possibleVectorDataIO.begin();
      k != possibleVectorDataIO.end(); ++k)
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
VectorDataIOFactory::RegisterBuiltInFactories()
{
  static bool firstTime = true;

  static itk::SimpleMutexLock mutex;
    {
    // This helper class makes sure the Mutex is unlocked
    // in the event an exception is thrown.
    itk::MutexLockHolder<itk::SimpleMutexLock> mutexHolder( mutex );
    if( firstTime )
      {
//      itk::ObjectFactoryBase::RegisterFactory( SHPVectorDataIOFactory::New() );
//      itk::ObjectFactoryBase::RegisterFactory( KMLVectorDataIOFactory::New() );
      firstTime = false;
      }
    }
}

} // end namespace otb
