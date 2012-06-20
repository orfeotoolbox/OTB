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
#ifndef __otbPixelDescriptionModel_txx
#define __otbPixelDescriptionModel_txx

#include "otbPixelDescriptionModel.h"
#include "otbMacro.h"
#include "otbI18n.h"
#include "itkTimeProbe.h"

namespace otb
{

template <class TOutputImage>
PixelDescriptionModel<TOutputImage>
::PixelDescriptionModel() : m_PixelDescription("")
{
}

template <class TOutputImage>
PixelDescriptionModel<TOutputImage>
::~PixelDescriptionModel()
{}
template <class TOutputImage>
void
PixelDescriptionModel<TOutputImage>
::ClearPixelDescription()
{
  m_PixelDescription = "";
}

template <class TOutputImage>
void
PixelDescriptionModel<TOutputImage>
::UpdatePixelDescription(const IndexType& index, bool usePlaceName)
{
  // The output stringstream
  std::ostringstream oss;
  oss << otbGetTextMacro("Index") << ": " << index << std::endl;
  // Report pixel info for each visible layer
  for (typename Superclass::LayerIteratorType it = this->GetLayers()->Begin();
       it != this->GetLayers()->End(); ++it)
    {
    // If the layer is visible
    if (it.Get()->GetVisible())
      {
      // Get the pixel description
      oss << it.Get()->GetPixelDescription(index, usePlaceName) << std::endl;
      }
    }
  m_PixelDescription = oss.str();

  // Notify all listeners
  this->NotifyAll();
}

template <class TOutputImage>
void
PixelDescriptionModel<TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

template <class TOutputImage>
void
PixelDescriptionModel<TOutputImage>
::NotifyListener(ListenerType * listener)
{
  listener->Notify();
}

} // end namespace otb

#endif
