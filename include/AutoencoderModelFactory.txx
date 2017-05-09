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


#include "AutoencoderModelFactory.h"

#include "itkCreateObjectFunction.h"
#include "AutoencoderModel.h"
#include "itkVersion.h"

namespace otb
{
template <class TInputValue, class TOutputValue>
AutoencoderModelFactory<TInputValue,TOutputValue>::AutoencoderModelFactory()
{

  std::string classOverride = std::string("otbMachineLearningModel");
  std::string subclass = std::string("AutoencoderModel");

  this->RegisterOverride(classOverride.c_str(),
                         subclass.c_str(),
                         "Shark RF ML Model",
                         1,
                      //   itk::CreateObjectFunction<AutoencoderModel<TInputValue,TOutputValue> >::New());
						itk::CreateObjectFunction<AutoencoderModel<TInputValue,shark::Autoencoder< shark::TanhNeuron, shark::LinearNeuron> > >::New());
}

template <class TInputValue, class TOutputValue>
AutoencoderModelFactory<TInputValue,TOutputValue>::~AutoencoderModelFactory()
{
}

template <class TInputValue, class TOutputValue>
const char* AutoencoderModelFactory<TInputValue,TOutputValue>::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

template <class TInputValue, class TOutputValue>
const char* AutoencoderModelFactory<TInputValue,TOutputValue>::GetDescription() const
{
  return "Autoencoder model factory";
}

} // end namespace otb

#endif
