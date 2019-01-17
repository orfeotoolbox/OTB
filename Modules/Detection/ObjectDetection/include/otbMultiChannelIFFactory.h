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

#ifndef otbMultiChannelIFFactory_h
#define otbMultiChannelIFFactory_h

#include "otbVectorImageToImageListFilter.h"
#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbImageList.h"
#include "itkDataObject.h"
#include "otbMetaImageFunction.h"


namespace otb
{
/** \class MultiChannelIFFactory
 *  \brief Yhis class handle the image type before the use of an
 *  image function factory
 *
 *  According to the image type, the Create method of this class can
 *  call an ImageFunctionFactory for each band of the input image or
 *  once if the image type is otb::Image.
 *
 *
 * \ingroup OTBObjectDetection
 */

template <class TIFFactory, class TInputImage>
class MultiChannelIFFactory
{
public:
  typedef TIFFactory                                    IFFactoryType;
  typedef TInputImage                                   InputImageType;
  typedef typename IFFactoryType::Pointer               IFFactoryPointerType;
  typedef typename IFFactoryType::InputImageType        ImageType;
  typedef typename IFFactoryType::CoordRepType          CoordRepType;
  typedef typename IFFactoryType::PrecisionType         PrecisionType;
  typedef typename std::vector<PrecisionType>           ParameterContainerType;
  typedef typename std::vector<itk::DataObject::Pointer>   ImageContainer;
  typedef typename MetaImageFunction<PrecisionType>::Pointer  MetaIFPointerType;

  typedef ImageList<ImageType>                             ImageListType;
  typedef VectorImageToImageListFilter<InputImageType,
                                                ImageListType>      VIToILFilterType;

  void Create(InputImageType * image,
              ParameterContainerType param,
              MetaIFPointerType metaIF,
              ImageContainer * container)
  {
    m_Factory->Create(image, param, metaIF, container);
  }

  MultiChannelIFFactory()
  {
    m_Factory = IFFactoryType::New();
  }

  virtual ~MultiChannelIFFactory(){};

private:
  IFFactoryPointerType   m_Factory;

};


template <class TIFFactory, typename TPixel, unsigned int VImageDimension>
class MultiChannelIFFactory<TIFFactory, otb::VectorImage<TPixel, VImageDimension> >
{
public:
  typedef TIFFactory                                    IFFactoryType;
  typedef typename IFFactoryType::Pointer               IFFactoryPointerType;
  typedef typename IFFactoryType::InputImageType        ImageType;
  typedef typename IFFactoryType::CoordRepType          CoordRepType;
  typedef typename IFFactoryType::PrecisionType         PrecisionType;
  typedef VectorImage<TPixel, 2>                        InputImageType;

  typedef typename std::vector<PrecisionType>           ParameterContainerType;
  typedef typename std::vector<itk::DataObject::Pointer>       ImageContainer;
  typedef typename MetaImageFunction<PrecisionType>::Pointer     MetaIFPointerType;

  typedef ImageList<ImageType>                             ImageListType;
  typedef VectorImageToImageListFilter<InputImageType,
                                                ImageListType>      VIToILFilterType;

  void Create(InputImageType * image,
              ParameterContainerType param,
              MetaIFPointerType metaIF,
              ImageContainer * container)
  {
    unsigned int nbBand;
    typename VIToILFilterType::Pointer filter = VIToILFilterType::New();

    filter->SetInput(image);
    filter->UpdateOutputInformation();
    nbBand = filter->GetOutput()->Size();

    for (unsigned int i=0; i<nbBand; ++i)
      {
      filter->GetOutput()->GetNthElement(i)->UpdateOutputInformation();
      m_Factory->Create(filter->GetOutput()->GetNthElement(i), param, metaIF, container);
      }
  }

  MultiChannelIFFactory()
  {
    m_Factory = IFFactoryType::New();
  }

  virtual ~MultiChannelIFFactory(){};

private:
  IFFactoryPointerType   m_Factory;

};
} //end namespace

#endif
