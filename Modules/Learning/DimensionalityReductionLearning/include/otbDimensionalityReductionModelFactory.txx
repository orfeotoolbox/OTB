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
#ifndef DimensionalityReductionModelFactory_txx
#define DimensionalityReductionModelFactory_txx

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
using AutoencoderModelFactory = AutoencoderModelFactoryBase<TInputValue, TTargetValue, shark::LogisticNeuron>  ;


template <class TInputValue, class TTargetValue>
using SOM2DModelFactory = SOMModelFactory<TInputValue, TTargetValue, 2>  ;

template <class TInputValue, class TTargetValue>
using SOM3DModelFactory = SOMModelFactory<TInputValue, TTargetValue, 3>  ;

template <class TInputValue, class TTargetValue>
using SOM4DModelFactory = SOMModelFactory<TInputValue, TTargetValue, 4>  ;

template <class TInputValue, class TTargetValue>
using SOM5DModelFactory = SOMModelFactory<TInputValue, TTargetValue, 5>  ;


template <class TInputValue, class TOutputValue>
typename MachineLearningModel<itk::VariableLengthVector< TInputValue> , itk::VariableLengthVector< TOutputValue>>::Pointer
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
    MachineLearningModel<itk::VariableLengthVector< TInputValue> , itk::VariableLengthVector< TOutputValue>> * io = dynamic_cast<MachineLearningModel<itk::VariableLengthVector< TInputValue> , itk::VariableLengthVector< TOutputValue>>*>(i->GetPointer());
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
  return ITK_NULLPTR;
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
  RegisterFactory(AutoencoderModelFactory<TInputValue,TOutputValue>::New());
  // RegisterFactory(TiedAutoencoderModelFactory<TInputValue,TOutputValue>::New());
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

	// SOM
	
	SOM5DModelFactory<TInputValue,TOutputValue> *som5dFactory =
      dynamic_cast<SOM5DModelFactory<TInputValue,TOutputValue> *>(*itFac);
    if (som5dFactory)
      {
      itk::ObjectFactoryBase::UnRegisterFactory(som5dFactory);
      continue;
      }
    
    SOM4DModelFactory<TInputValue,TOutputValue> *som4dFactory =
      dynamic_cast<SOM4DModelFactory<TInputValue,TOutputValue> *>(*itFac);
    if (som4dFactory)
      {
      itk::ObjectFactoryBase::UnRegisterFactory(som4dFactory);
      continue;
      }
      
    SOM3DModelFactory<TInputValue,TOutputValue> *som3dFactory =
      dynamic_cast<SOM3DModelFactory<TInputValue,TOutputValue> *>(*itFac);
    if (som3dFactory)
      {
      itk::ObjectFactoryBase::UnRegisterFactory(som3dFactory);
      continue;
      }
      
    SOM2DModelFactory<TInputValue,TOutputValue> *som2dFactory =
      dynamic_cast<SOM2DModelFactory<TInputValue,TOutputValue> *>(*itFac);
    if (som2dFactory)
      {
      itk::ObjectFactoryBase::UnRegisterFactory(som2dFactory);
      continue;
      }
      
#ifdef OTB_USE_SHARK
	
    AutoencoderModelFactory<TInputValue,TOutputValue> *aeFactory =
      dynamic_cast<AutoencoderModelFactory<TInputValue,TOutputValue> *>(*itFac);
    if (aeFactory)
      {
      itk::ObjectFactoryBase::UnRegisterFactory(aeFactory);
      continue;
      }
    
    /*
    TiedAutoencoderModelFactory<TInputValue,TOutputValue> *taeFactory =
      dynamic_cast<TiedAutoencoderModelFactory<TInputValue,TOutputValue> *>(*itFac);
    if (taeFactory)
      {
      itk::ObjectFactoryBase::UnRegisterFactory(taeFactory);
      continue;
      }
    */
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
