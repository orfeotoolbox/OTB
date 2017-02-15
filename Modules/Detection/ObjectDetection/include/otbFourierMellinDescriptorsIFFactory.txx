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
#ifndef otbFourierMellinDescriptorsIFFactory_txx
#define otbFourierMellinDescriptorsIFFactory_txx

#include "otbFourierMellinDescriptorsIFFactory.h"


namespace otb
{
template <class TImageType, class TCoordRep, class TPrecision>
void
FourierMellinDescriptorsIFFactory<TImageType, TCoordRep, TPrecision>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

template <class TImageType, class TCoordRep, class TPrecision>
void
FourierMellinDescriptorsIFFactory<TImageType, TCoordRep, TPrecision>
::Create(InputImageType * image,
         ParamContainerType param,
         MetaImageFunctionPointerType metaIF,
         DataObjectContainerType * container)
{
  typename AdaptedFourierMellinDescriptorsIF::Pointer function = AdaptedFourierMellinDescriptorsIF::New();

  function->SetInputImage(image);
  function->GetInternalImageFunction()->SetNeighborhoodRadius(param[0]);
  function->GetInternalImageFunction()->SetPmax(param[1]);
  function->GetInternalImageFunction()->SetQmax(param[2]);

  metaIF->AddFunction(function);
  container->push_back(image);
}

}//end namespace
#endif
