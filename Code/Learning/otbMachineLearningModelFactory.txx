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

#ifdef OTB_USE_OPENCV
#include "otbKNearestNeighborsMachineLearningModelFactory.h"
#include "otbRandomForestsMachineLearningModelFactory.h"
#include "otbSVMMachineLearningModelFactory.h"
#include "otbBoostMachineLearningModelFactory.h"
#include "otbNeuralNetworkMachineLearningModelFactory.h"
#include "otbNormalBayesMachineLearningModelFactory.h"
#include "otbDecisionTreeMachineLearningModelFactory.h"
#include "otbGradientBoostedTreeMachineLearningModelFactory.h"
#endif
#include "otbLibSVMMachineLearningModelFactory.h"

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
  return 0;
}

template <class TInputValue, class TOutputValue>
void
MachineLearningModelFactory<TInputValue,TOutputValue>
::RegisterBuiltInFactories()
{
  itk::MutexLockHolder<itk::SimpleMutexLock> lockHolder(mutex);

  RegisterFactory(LibSVMMachineLearningModelFactory<TInputValue,TOutputValue>::New());

#ifdef OTB_USE_OPENCV
  RegisterFactory(RandomForestsMachineLearningModelFactory<TInputValue,TOutputValue>::New());
  RegisterFactory(SVMMachineLearningModelFactory<TInputValue,TOutputValue>::New());
  RegisterFactory(BoostMachineLearningModelFactory<TInputValue,TOutputValue>::New());
  RegisterFactory(NeuralNetworkMachineLearningModelFactory<TInputValue,TOutputValue>::New());
  RegisterFactory(NormalBayesMachineLearningModelFactory<TInputValue,TOutputValue>::New());
  RegisterFactory(DecisionTreeMachineLearningModelFactory<TInputValue,TOutputValue>::New());
  RegisterFactory(GradientBoostedTreeMachineLearningModelFactory<TInputValue,TOutputValue>::New());
  RegisterFactory(KNearestNeighborsMachineLearningModelFactory<TInputValue,TOutputValue>::New());
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

} // end namespace otb
