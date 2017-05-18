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
#ifndef SOMFactory_txx
#define SOMFactory_txx


#include "SOMModelFactory.h"

#include "itkCreateObjectFunction.h"
#include "SOMModel.h"
//#include <shark/Algorithms/Trainers/PCA.h>
#include "itkVersion.h"

namespace otb
{
template <class TInputValue, class TOutputValue, unsigned int MapDimension>
SOMModelFactory<TInputValue,TOutputValue,MapDimension>::SOMModelFactory()
{

  std::string classOverride = std::string("DimensionalityReductionModel");
  std::string subclass = std::string("SOMModel");

  this->RegisterOverride(classOverride.c_str(),
                         subclass.c_str(),
                         "SOM DR Model",
                         1,
                      //   itk::CreateObjectFunction<AutoencoderModel<TInputValue,TOutputValue> >::New());
						itk::CreateObjectFunction<SOMModel<TInputValue,  MapDimension>>::New());
}

template <class TInputValue, class TOutputValue, unsigned int MapDimension>
SOMModelFactory<TInputValue,TOutputValue,MapDimension>::~SOMModelFactory()
{
}

template <class TInputValue, class TOutputValue, unsigned int MapDimension>
const char* SOMModelFactory<TInputValue,TOutputValue,MapDimension>::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

template <class TInputValue, class TOutputValue, unsigned int MapDimension>
const char* SOMModelFactory<TInputValue,TOutputValue,MapDimension>::GetDescription() const
{
  return "SOM model factory";
}

} // end namespace otb

#endif
