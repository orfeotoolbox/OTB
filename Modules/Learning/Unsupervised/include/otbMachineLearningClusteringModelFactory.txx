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
#ifndef otbMachineLearningModelFactory_txx
#define otbMachineLearningModelFactory_txx

#include "otbMachineLearningClusteringModelFactory.h"
#include "otbConfigure.h"

#ifdef OTB_USE_SHARK
#include "otbSharkKMeansMachineLearningModelFactory.h"
#endif

#include "itkMutexLockHolder.h"


namespace otb
{
template <class TInputValue, class TOutputValue>
typename MachineLearningModel<TInputValue,TOutputValue>::Pointer
MachineLearningModelFactory<TInputValue,TOutputValue>
::CreateMachineLearningModel(const std::string& path, FileModeType mode)
{
  RegisterBuiltInFactories();

  std::list<MachineLearningModelTypePointer> possibleMachineLearningModel;
  std::list<LightObject::Pointer> allobjects =
          itk::ObjectFactoryBase::CreateAllInstance("otbMachineLearningModel");
  for(std::list<LightObject::Pointer>::iterator i = allobjects.begin();
      i != allobjects.end(); ++i)
    {
    MachineLearningModel<TInputValue,TOutputValue> * io = dynamic_cast<MachineLearningModel<TInputValue,TOutputValue>*>(i->GetPointer());
    if(io)
      {
      possibleMachineLearningModel.push_back(io);
      }
    else
      {
      std::cerr << "Error MachineLearningModel Factory did not return an MachineLearningModel: "
                << (*i)->GetNameOfClass()
                << std::endl;
      }
    }
  for(typename std::list<MachineLearningModelTypePointer>::iterator k = possibleMachineLearningModel.begin();
      k != possibleMachineLearningModel.end(); ++k)
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
  return ITK_NULLPTR;
}

template <class TInputValue, class TOutputValue>
void
MachineLearningModelFactory<TInputValue,TOutputValue>
::RegisterBuiltInFactories()
{
  itk::MutexLockHolder<itk::SimpleMutexLock> lockHolder(mutex);

#ifdef OTB_USE_SHARK
  RegisterFactory(SharkKMeansMachineLearningModelFactory<TInputValue,TOutputValue>::New());
#endif

}

template <class TInputValue, class TOutputValue>
void
MachineLearningModelFactory<TInputValue,TOutputValue>
::RegisterFactory(itk::ObjectFactoryBase * factory)
{
  // Unregister any previously registered factory of the same class
  // Might be more intensive but static bool is not an option due to
  // ld error.
  itk::ObjectFactoryBase::UnRegisterFactory(factory);
  itk::ObjectFactoryBase::RegisterFactory(factory);
}

template <class TInputValue, class TOutputValue>
void
MachineLearningModelFactory<TInputValue,TOutputValue>
::CleanFactories()
{
  itk::MutexLockHolder<itk::SimpleMutexLock> lockHolder(mutex);

  std::list<itk::ObjectFactoryBase*> factories = itk::ObjectFactoryBase::GetRegisteredFactories();
  std::list<itk::ObjectFactoryBase*>::iterator itFac;

  for (itFac = factories.begin(); itFac != factories.end() ; ++itFac)
  {

#ifdef OTB_USE_SHARK
    SharkKMeansMachineLearningModelFactory<TInputValue,TOutputValue> *sharkKMeansFactory =
            dynamic_cast<SharkKMeansMachineLearningModelFactory<TInputValue,TOutputValue> *>(*itFac);
    if (sharkKMeansFactory)
      {
      itk::ObjectFactoryBase::UnRegisterFactory(sharkKMeansFactory);
      continue;
      }
#endif

    }

}

} // end namespace otb

#endif
