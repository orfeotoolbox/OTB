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

#ifndef otbHaralickTexturesIFFactory_h
#define otbHaralickTexturesIFFactory_h

#include "itkLightObject.h"

#include "otbMetaImageFunction.h"
#include "itkDataObject.h"
#include "otbImage.h"
#include "otbImageFunctionAdaptor.h"
#include "otbHaralickTexturesImageFunction.h"


namespace otb
{
/** \class HaralickTexturesFFactory
 *  \brief add a HaralickTextures image function to a
 *  MetaImageFunction
 *
 *  This class aims at adding an adapted
 *  HaralickTexturesImageFunction to an existing
 *  MetaImageFunction through the method Create.
 *
 *
 * \ingroup OTBObjectDetection
 */

template <class TImageType, class TCoordRep = double, class TPrecision = double>
class ITK_EXPORT HaralickTexturesIFFactory :
    public itk::LightObject
{
public:
  /** Standard class typedefs. */
  typedef HaralickTexturesIFFactory                                       Self;
  typedef itk::LightObject                                                Superclass;
  typedef itk::SmartPointer<Self>                                         Pointer;
  typedef itk::SmartPointer<const Self>                                   ConstPointer;

  // New macro
  itkNewMacro(Self);

  // RTTI typeinfo
  itkTypeMacro(haralickTexturesIFFactory, itk::LightObject);

  // Input and output typedef
  typedef TImageType              InputImageType;
  typedef TCoordRep               CoordRepType;
  typedef TPrecision              PrecisionType;

  // Other typedef
  typedef typename MetaImageFunction<TPrecision>::Pointer  MetaImageFunctionPointerType;
  typedef typename std::vector<itk::DataObject::Pointer>   DataObjectContainerType;
  typedef typename std::vector<PrecisionType>              ParamContainerType;
  typedef HaralickTexturesImageFunction<InputImageType, CoordRepType>
                                                           HaralickTexturesIF;
  typedef typename HaralickTexturesIF::OffsetType          OffsetType;
  typedef ImageFunctionAdaptor<HaralickTexturesIF, TPrecision>
                                                           AdaptedHaralickTexturesIF;

  void Create(InputImageType * image,
              ParamContainerType param,
              MetaImageFunctionPointerType metaIF,
              DataObjectContainerType * container);

protected:
  HaralickTexturesIFFactory(){}
  ~HaralickTexturesIFFactory() override{}
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  HaralickTexturesIFFactory(const Self& ) = delete;
  void operator=(const Self& ) = delete;

};

} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbHaralickTexturesIFFactory.hxx"
#endif

#endif
