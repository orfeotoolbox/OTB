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
#include "otbMachineLearningModelFactory.h"
#include "itkMutexLock.h"
#include "itkMutexLockHolder.h"

//#include "otbKNearestNeighborsMachineLearningModelFactory.h"
#include "otbRandomForestsMachineLearningModelFactory.h"
#include "otbSVMMachineLearningModelFactory.h"
#include "otbLibSVMMachineLearningModelFactory.h"


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
  return 0;
}

template <class TInputValue, class TOutputValue>
void
MachineLearningModelFactory<TInputValue,TOutputValue>
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
      // KNN Format for OTB
      //itk::ObjectFactoryBase::RegisterFactory(KNearestNeighborsMachineLearningModelFactory<TInputValue,TOutputValue>::New());
      itk::ObjectFactoryBase::RegisterFactory(RandomForestsMachineLearningModelFactory<TInputValue,TOutputValue>::New());
      itk::ObjectFactoryBase::RegisterFactory(LibSVMMachineLearningModelFactory<TInputValue,TOutputValue>::New());
      itk::ObjectFactoryBase::RegisterFactory(SVMMachineLearningModelFactory<TInputValue,TOutputValue>::New());

      firstTime = false;
      }
    }

}

} // end namespace otb
