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
#ifndef AutoencoderModelFactory_txx
#define AutoencoderModelFactory_txx


#include "otbAutoencoderModelFactory.h"
#include "otbAutoencoderModel.h"

#include "itkCreateObjectFunction.h"
#include "itkVersion.h"

namespace otb
{
template <class TInputValue, class TOutputValue, class NeuronType>
AutoencoderModelFactoryBase<TInputValue,TOutputValue, NeuronType>::AutoencoderModelFactoryBase()
{

  std::string classOverride = std::string("DimensionalityReductionModel");
  std::string subclass = std::string("AutoencoderModel");

  this->RegisterOverride(classOverride.c_str(),
                         subclass.c_str(),
                         "Shark AE ML Model",
                         1,
                         //   itk::CreateObjectFunction<AutoencoderModel<TInputValue,TOutputValue> >::New());
                         itk::CreateObjectFunction<AutoencoderModel<TInputValue,NeuronType > >::New());
}

template <class TInputValue, class TOutputValue, class NeuronType>
AutoencoderModelFactoryBase<TInputValue,TOutputValue, NeuronType>::~AutoencoderModelFactoryBase()
{
}

template <class TInputValue, class TOutputValue, class NeuronType>
const char* AutoencoderModelFactoryBase<TInputValue,TOutputValue, NeuronType>::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

template <class TInputValue, class TOutputValue, class NeuronType>
const char* AutoencoderModelFactoryBase<TInputValue,TOutputValue, NeuronType>::GetDescription() const
{
  return "Autoencoder model factory";
}

} // end namespace otb

#endif
