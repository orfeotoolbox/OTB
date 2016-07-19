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
#include "otbConfigure.h"

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
#ifdef OTB_USE_LIBSVM
#include "otbLibSVMMachineLearningModelFactory.h"
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
  
#ifdef OTB_USE_LIBSVM
  RegisterFactory(LibSVMMachineLearningModelFactory<TInputValue,TOutputValue>::New());
#endif

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
#ifdef OTB_USE_LIBSVM
    LibSVMMachineLearningModelFactory<TInputValue,TOutputValue> *libsvmFactory =
      dynamic_cast<LibSVMMachineLearningModelFactory<TInputValue,TOutputValue> *>(*itFac);
    if (libsvmFactory)
      {
      itk::ObjectFactoryBase::UnRegisterFactory(libsvmFactory);
      continue;
      }
#endif
#ifdef OTB_USE_OPENCV
    // RandomForest
    RandomForestsMachineLearningModelFactory<TInputValue,TOutputValue> *rfFactory =
      dynamic_cast<RandomForestsMachineLearningModelFactory<TInputValue,TOutputValue> *>(*itFac);
    if (rfFactory)
      {
      itk::ObjectFactoryBase::UnRegisterFactory(rfFactory);
      continue;
      }
    // SVM
    SVMMachineLearningModelFactory<TInputValue,TOutputValue> *svmFactory =
      dynamic_cast<SVMMachineLearningModelFactory<TInputValue,TOutputValue> *>(*itFac);
    if (svmFactory)
      {
      itk::ObjectFactoryBase::UnRegisterFactory(svmFactory);
      continue;
      }
    // Boost
    BoostMachineLearningModelFactory<TInputValue,TOutputValue> *boostFactory =
      dynamic_cast<BoostMachineLearningModelFactory<TInputValue,TOutputValue> *>(*itFac);
    if (boostFactory)
      {
      itk::ObjectFactoryBase::UnRegisterFactory(boostFactory);
      continue;
      }
    // ANN
    NeuralNetworkMachineLearningModelFactory<TInputValue,TOutputValue> *annFactory =
      dynamic_cast<NeuralNetworkMachineLearningModelFactory<TInputValue,TOutputValue> *>(*itFac);
    if (annFactory)
      {
      itk::ObjectFactoryBase::UnRegisterFactory(annFactory);
      continue;
      }
    // Bayes
    NormalBayesMachineLearningModelFactory<TInputValue,TOutputValue> *bayesFactory =
      dynamic_cast<NormalBayesMachineLearningModelFactory<TInputValue,TOutputValue> *>(*itFac);
    if (bayesFactory)
      {
      itk::ObjectFactoryBase::UnRegisterFactory(bayesFactory);
      continue;
      }
    // Decision Tree
    DecisionTreeMachineLearningModelFactory<TInputValue,TOutputValue> *dtFactory =
      dynamic_cast<DecisionTreeMachineLearningModelFactory<TInputValue,TOutputValue> *>(*itFac);
    if (dtFactory)
      {
      itk::ObjectFactoryBase::UnRegisterFactory(dtFactory);
      continue;
      }
    // Gradient Boosted tree
    GradientBoostedTreeMachineLearningModelFactory<TInputValue,TOutputValue> *gbtFactory =
      dynamic_cast<GradientBoostedTreeMachineLearningModelFactory<TInputValue,TOutputValue> *>(*itFac);
    if (gbtFactory)
      {
      itk::ObjectFactoryBase::UnRegisterFactory(gbtFactory);
      continue;
      }
    // KNN
    KNearestNeighborsMachineLearningModelFactory<TInputValue,TOutputValue> *knnFactory =
      dynamic_cast<KNearestNeighborsMachineLearningModelFactory<TInputValue,TOutputValue> *>(*itFac);
    if (knnFactory)
      {
      itk::ObjectFactoryBase::UnRegisterFactory(knnFactory);
      continue;
      }
#endif
    }

}

} // end namespace otb
