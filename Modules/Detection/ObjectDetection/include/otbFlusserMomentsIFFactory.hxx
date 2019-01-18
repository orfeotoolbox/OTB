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

#ifndef otbFlusserMomentsIFFactory_hxx
#define otbFlusserMomentsIFFactory_hxx

#include "otbFlusserMomentsIFFactory.h"


namespace otb
{
template <class TImageType, class TCoordRep, class TPrecision>
void
FlusserMomentsIFFactory<TImageType, TCoordRep, TPrecision>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

template <class TImageType, class TCoordRep, class TPrecision>
void
FlusserMomentsIFFactory<TImageType, TCoordRep, TPrecision>
::Create(InputImageType * image,
         ParamContainerType param,
         MetaImageFunctionPointerType metaIF,
         DataObjectContainerType * container)
{
  typename AdaptedFlusserMomentsIF::Pointer function = AdaptedFlusserMomentsIF::New();

  function->SetInputImage(image);
  function->GetInternalImageFunction()->SetNeighborhoodRadius(param[0]);

  metaIF->AddFunction(function);
  container->push_back(image);
}

}//end namespace
#endif
