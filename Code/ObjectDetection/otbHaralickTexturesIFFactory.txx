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
#ifndef __otbHaralickTexturesIFFactory_txx
#define __otbHaralickTexturesIFFactory_txx

#include "otbHaralickTexturesIFFactory.h"


namespace otb
{
template <class TImageType, class TCoordRep, class TPrecision>
void
HaralickTexturesIFFactory<TImageType, TCoordRep, TPrecision>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

template <class TImageType, class TCoordRep, class TPrecision>
void
HaralickTexturesIFFactory<TImageType, TCoordRep, TPrecision>
::Create(InputImageType * image,
         ParamContainerType param,
         MetaImageFunctionPointerType metaIF,
         DataObjectContainerType * container)
{
  typename AdaptedHaralickTexturesIF::Pointer function = AdaptedHaralickTexturesIF::New();

  function->SetInputImage(image);
  function->GetInternalImageFunction()->SetNeighborhoodRadius(param[0]);
  function->GetInternalImageFunction()->SetInputImageMinimum(param[1]);
  function->GetInternalImageFunction()->SetInputImageMaximum(param[2]);
  function->GetInternalImageFunction()->SetNumberOfBinsPerAxis(param[3]);

  OffsetType offset;
  offset.Fill(param[4]);
  function->GetInternalImageFunction()->SetOffset(offset);

  metaIF->AddFunction(function);
  container->push_back(image);
}

}//end namespace
#endif
