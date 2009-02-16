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
#ifndef __otbImageLayer_txx
#define __otbImageLayer_txx


namespace otb
{

template <class TImage>
ImageLayer<TImage>
::ImageLayer()
{
  m_LayerName = "Default";
  m_RedChannelIndex   = 0;
  m_GreenChannelIndex = 0;
  m_BlueChannelIndex  = 0;
  m_HistogramList = HistogramListType::New();
}

template <class TImage>
ImageLayer<TImage>
::~ImageLayer()
{}


template <class TImage>
void
ImageLayer<TImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os<<indent<<"Layer "<<m_LayerName<<std::endl;
}














}

#endif
