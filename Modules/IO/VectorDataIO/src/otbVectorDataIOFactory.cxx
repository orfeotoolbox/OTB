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
  for (std::list<itk::LightObject::Pointer>::iterator i = allobjects.begin();
       i != allobjects.end(); ++i)
    {
    VectorDataIOBaseType * io = dynamic_cast<VectorDataIOBaseType*>(i->GetPointer());
    if (io)
      {
      possibleVectorDataIO.push_back(io);
      }
    else
      {
      itkGenericExceptionMacro(<< "Error VectorDataIO factory did not return an VectorDataIOBase: "
                               << (*i)->GetNameOfClass());
      }
    }
  for (std::list<VectorDataIOBasePointerType>::iterator k = possibleVectorDataIO.begin();
       k != possibleVectorDataIO.end(); ++k)
    {
    if (mode == ReadMode)
      {
      if ((*k)->CanReadFile(path))
        {
        return *k;
        }
      }
    else if (mode == WriteMode)
      {
      if ((*k)->CanWriteFile(path))
        {
        return *k;
        }

      }
    }
  return ITK_NULLPTR;
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

