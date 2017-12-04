/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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
#ifndef AutoencoderModelFactory_h
#define AutoencoderModelFactory_h


//#include <shark/Models/TiedAutoencoder.h>
//#include <shark/Models/Autoencoder.h>
#include "itkObjectFactoryBase.h"
#include "itkImageIOBase.h"

namespace otb
{
	
template <class TInputValue, class TTargetValue, class NeuronType>
class ITK_EXPORT AutoencoderModelFactory : public itk::ObjectFactoryBase
{
public:
  /** Standard class typedefs. */
  typedef AutoencoderModelFactory   Self;
  typedef itk::ObjectFactoryBase        Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Class methods used to interface with the registered factories. */
  const char* GetITKSourceVersion(void) const ITK_OVERRIDE;
  const char* GetDescription(void) const ITK_OVERRIDE;

  /** Method for class instantiation. */
  itkFactorylessNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(AutoencoderModelFactory, itk::ObjectFactoryBase);

  /** Register one factory of this type  */
  static void RegisterOneFactory(void)
  {
    Pointer AEFactory = AutoencoderModelFactory::New();
    itk::ObjectFactoryBase::RegisterFactory(AEFactory);
  }

protected:
  AutoencoderModelFactory();
  ~AutoencoderModelFactory() ITK_OVERRIDE;

private:
  AutoencoderModelFactory(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};



/*
template <class TInputValue, class TTargetValue>
using AutoencoderModelFactory = AutoencoderModelFactoryBase<TInputValue, TTargetValue, shark::Autoencoder< shark::TanhNeuron, shark::LinearNeuron>>  ;


template <class TInputValue, class TTargetValue>
using TiedAutoencoderModelFactory = AutoencoderModelFactoryBase<TInputValue, TTargetValue, shark::TiedAutoencoder< shark::TanhNeuron, shark::LinearNeuron>>  ;
*/

} //namespace otb


#ifndef OTB_MANUAL_INSTANTIATION
#include "otbAutoencoderModelFactory.txx"
#endif

#endif


