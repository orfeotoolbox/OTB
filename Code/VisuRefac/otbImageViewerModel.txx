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
#ifndef __otbImageViewerModel_txx
#define __otbImageViewerModel_txx

#include "otbImageViewerModel.h"

namespace otb
{

template <class TOutputImage>
ImageViewerModel<TOutputImage>
::ImageViewerModel()
{

}

template <class TOutputImage>
ImageViewerModel<TOutputImage>
::~ImageViewerModel()
{

}

template <class TOutputImage>
unsigned int
ImageViewerModel<TOutputImage>
::AddLayer(LayerType * layer)
{

}

template <class TOutputImage>
typename ImageViewerModel<TOutputImage>
::LayerType *
ImageViewerModel<TOutputImage>
::GetLayer(unsigned int index)
{

}
template <class TOutputImage>
bool
ImageViewerModel<TOutputImage>
::DeleteLayer(unsigned int index)
{

}

template <class TOutputImage>
typename ImageViewerModel<TOutputImage>
::LayerType *
ImageViewerModel<TOutputImage>
::GetLayerByName(std::string name)
{

}

template <class TOutputImage>
bool
ImageViewerModel<TOutputImage>
::DeleteLayerByName(std::string name)
{

}

template <class TOutputImage>
void
ImageViewerModel<TOutputImage>
::ClearLayers()
{

}

template <class TOutputImage>
unsigned int
ImageViewerModel<TOutputImage>
::GetNumberOfLayers(void)
{

}

template <class TOutputImage>
void
ImageViewerModel<TOutputImage>
::Update()
{

}

template <class TOutputImage>
void
ImageViewerModel<TOutputImage>
::RenderVisibleLayers()
{

}

template <class TOutputImage>
void
ImageViewerModel<TOutputImage>
::RasterizeVisibleLayers()
{

}

template <class TOutputImage>
void
ImageViewerModel<TOutputImage>
::Notify(ListenerType * listener)
{

}

template <class TOutputImage>
void
ImageViewerModel<TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os,indent);
}

} // end namespace otb

#endif 
