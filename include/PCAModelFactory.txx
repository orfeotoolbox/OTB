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
#include <shark/Algorithms/Trainers/PCA.h>
#include "itkVersion.h"

namespace otb
{
template <class TInputValue, class TOutputValue>
PCAModelFactory<TInputValue,TOutputValue>::PCAModelFactory()
{

  std::string classOverride = std::string("DimensionalityReductionModel");
  std::string subclass = std::string("PCAModel");

  this->RegisterOverride(classOverride.c_str(),
                         subclass.c_str(),
                         "Shark PCA ML Model",
                         1,
                      //   itk::CreateObjectFunction<AutoencoderModel<TInputValue,TOutputValue> >::New());
						itk::CreateObjectFunction<PCAModel<TInputValue,shark::LinearModel<> > >::New());
}

template <class TInputValue, class TOutputValue>
PCAModelFactory<TInputValue,TOutputValue>::~PCAModelFactory()
{
}

template <class TInputValue, class TOutputValue>
const char* PCAModelFactory<TInputValue,TOutputValue>::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

template <class TInputValue, class TOutputValue>
const char* PCAModelFactory<TInputValue,TOutputValue>::GetDescription() const
{
  return "PCA model factory";
}

} // end namespace otb

#endif
