/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef otbDimensionalityReductionModelFactory_hxx
#define otbDimensionalityReductionModelFactory_hxx

#include "otbDimensionalityReductionModelFactory.h"
#include "otbConfigure.h"

#include "otbSOMModelFactory.h"

#ifdef OTB_USE_SHARK
#include "otbAutoencoderModelFactory.h"
#include "otbPCAModelFactory.h"
#endif

#include "itkMutexLockHolder.h"

namespace otb
{

template <class TInputValue, class TTargetValue>
using LogAutoencoderModelFactory = AutoencoderModelFactory<TInputValue, TTargetValue, shark::LogisticNeuron>  ;

template <class TInputValue, class TTargetValue>
using SOM2DModelFactory = SOMModelFactory<TInputValue, TTargetValue, 2>  ;

template <class TInputValue, class TTargetValue>
using SOM3DModelFactory = SOMModelFactory<TInputValue, TTargetValue, 3>  ;

template <class TInputValue, class TTargetValue>
using SOM4DModelFactory = SOMModelFactory<TInputValue, TTargetValue, 4>  ;

template <class TInputValue, class TTargetValue>
using SOM5DModelFactory = SOMModelFactory<TInputValue, TTargetValue, 5>  ;


template <class TInputValue, class TOutputValue>
typename DimensionalityReductionModelFactory<TInputValue,TOutputValue>::DimensionalityReductionModelTypePointer
DimensionalityReductionModelFactory<TInputValue,TOutputValue>
::CreateDimensionalityReductionModel(const std::string& path, FileModeType mode)
{
  RegisterBuiltInFactories();

  std::list<DimensionalityReductionModelTypePointer> possibleDimensionalityReductionModel;
  std::list<LightObject::Pointer> allobjects =
    itk::ObjectFactoryBase::CreateAllInstance("DimensionalityReductionModel");

  for(std::list<LightObject::Pointer>::iterator i = allobjects.begin();
      i != allobjects.end(); ++i)
    {
    DimensionalityReductionModelType* io =
      dynamic_cast<DimensionalityReductionModelType*>(i->GetPointer());
    if(io)
      {
      possibleDimensionalityReductionModel.push_back(io);
      }
    else
      {
      std::cerr << "Error DimensionalityReductionModel Factory did not return an DimensionalityReductionModel: "
                << (*i)->GetNameOfClass()
                << std::endl;
      }
    }

  for(typename std::list<DimensionalityReductionModelTypePointer>::iterator k = possibleDimensionalityReductionModel.begin();
      k != possibleDimensionalityReductionModel.end(); ++k)
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
  return nullptr;
}

template <class TInputValue, class TOutputValue>
void
DimensionalityReductionModelFactory<TInputValue,TOutputValue>
::RegisterBuiltInFactories()
{
  itk::MutexLockHolder<itk::SimpleMutexLock> lockHolder(mutex);

  RegisterFactory(SOM2DModelFactory<TInputValue,TOutputValue>::New());
  RegisterFactory(SOM3DModelFactory<TInputValue,TOutputValue>::New());
  RegisterFactory(SOM4DModelFactory<TInputValue,TOutputValue>::New());
  RegisterFactory(SOM5DModelFactory<TInputValue,TOutputValue>::New());

#ifdef OTB_USE_SHARK
  RegisterFactory(PCAModelFactory<TInputValue,TOutputValue>::New());
  RegisterFactory(LogAutoencoderModelFactory<TInputValue,TOutputValue>::New());
#endif
}

template <class TInputValue, class TOutputValue>
void
DimensionalityReductionModelFactory<TInputValue,TOutputValue>
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
DimensionalityReductionModelFactory<TInputValue,TOutputValue>
::CleanFactories()
{
  itk::MutexLockHolder<itk::SimpleMutexLock> lockHolder(mutex);

  std::list<itk::ObjectFactoryBase*> factories = itk::ObjectFactoryBase::GetRegisteredFactories();
  std::list<itk::ObjectFactoryBase*>::iterator itFac;

  for (itFac = factories.begin(); itFac != factories.end() ; ++itFac)
    {
    // SOM 5D
    SOM5DModelFactory<TInputValue,TOutputValue> *som5dFactory =
      dynamic_cast<SOM5DModelFactory<TInputValue,TOutputValue> *>(*itFac);
    if (som5dFactory)
      {
      itk::ObjectFactoryBase::UnRegisterFactory(som5dFactory);
      continue;
      }
    // SOM 4D
    SOM4DModelFactory<TInputValue,TOutputValue> *som4dFactory =
      dynamic_cast<SOM4DModelFactory<TInputValue,TOutputValue> *>(*itFac);
    if (som4dFactory)
      {
      itk::ObjectFactoryBase::UnRegisterFactory(som4dFactory);
      continue;
      }
    // SOM 3D
    SOM3DModelFactory<TInputValue,TOutputValue> *som3dFactory =
      dynamic_cast<SOM3DModelFactory<TInputValue,TOutputValue> *>(*itFac);
    if (som3dFactory)
      {
      itk::ObjectFactoryBase::UnRegisterFactory(som3dFactory);
      continue;
      }
    // SOM 2D
    SOM2DModelFactory<TInputValue,TOutputValue> *som2dFactory =
      dynamic_cast<SOM2DModelFactory<TInputValue,TOutputValue> *>(*itFac);
    if (som2dFactory)
      {
      itk::ObjectFactoryBase::UnRegisterFactory(som2dFactory);
      continue;
      }
#ifdef OTB_USE_SHARK
    // Autoencoder
    LogAutoencoderModelFactory<TInputValue,TOutputValue> *aeFactory =
      dynamic_cast<LogAutoencoderModelFactory<TInputValue,TOutputValue> *>(*itFac);
    if (aeFactory)
      {
      itk::ObjectFactoryBase::UnRegisterFactory(aeFactory);
      continue;
      }
    // PCA
    PCAModelFactory<TInputValue,TOutputValue> *pcaFactory =
      dynamic_cast<PCAModelFactory<TInputValue,TOutputValue> *>(*itFac);
    if (pcaFactory)
      {
      itk::ObjectFactoryBase::UnRegisterFactory(pcaFactory);
      continue;
      }
#endif
    }
}

} // end namespace otb

#endif
