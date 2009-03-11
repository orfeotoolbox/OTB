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
#ifndef __otbLayerBasedModel_txx
#define __otbLayerBasedModel_txx

#include "otbLayerBasedModel.h"
#include "otbMacro.h"
#include "itkTimeProbe.h"

namespace otb
{

template <class TLayer>
LayerBasedModel<TLayer>
::LayerBasedModel() :  m_Layers()

{
  // Intializing the layer list
  m_Layers = LayerListType::New();
}

template <class TLayer>
LayerBasedModel<TLayer>
::~LayerBasedModel()
{}

template <class TLayer>
unsigned int
LayerBasedModel<TLayer>
::AddLayer(LayerType * layer)
{
  // Push back and return the size-1
  m_Layers->PushBack(layer);

  // Return the layer index
  return m_Layers->Size()-1;
}

template <class TLayer>
typename LayerBasedModel<TLayer>
::LayerType *
LayerBasedModel<TLayer>
::GetLayer(unsigned int index)
{
  // Check if not out of bound and return the ith element
  if(index >= m_Layers->Size())
    {
    return NULL;
    }
  else
    {
    return m_Layers->GetNthElement(index);
    }
}
template <class TLayer>
bool
LayerBasedModel<TLayer>
::DeleteLayer(unsigned int index)
{
// Check if not out of bound and delete the ith element
  if(index >= m_Layers->Size())
    {
    return false;
    }
  else
    {
    m_Layers->Erase(index);
    return true;
    }
}

template <class TLayer>
typename LayerBasedModel<TLayer>
::LayerType *
LayerBasedModel<TLayer>
::GetLayerByName(std::string name)
{
  LayerType * resp = NULL;
  LayerIteratorType it = m_Layers->Begin();
  bool found  = false;

  // Look for the layer named after name
  while(it!=m_Layers->End() && !found)
    {
    if(it.Get()->GetName() == name)
      {
      resp = it.Get();
      found = true;
      }
    }
  return resp;
}

template <class TLayer>
bool
LayerBasedModel<TLayer>
::DeleteLayerByName(std::string name)
{
  LayerIteratorType it = m_Layers->Begin();
  bool found  = false;
  unsigned int index = 0;

  // Look for the layer named after name
  while(it!=m_Layers->End() && !found)
    {
    if(it.Get()->GetName() == name)
      {
      found = true;
      }
    ++index;
    }
  
  if(found)
    {
    m_Layers->Erase(index-1);
    }
  return found;
}

template <class TLayer>
void
LayerBasedModel<TLayer>
::ClearLayers()
{
  // Clear layers list
  m_Layers->Clear();
}

template <class TLayer>
unsigned int
LayerBasedModel<TLayer>
::GetNumberOfLayers(void)
{
  // return layer list size
  return m_Layers->Size();
}

template <class TLayer>
void
LayerBasedModel<TLayer>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os,indent);
}

} // end namespace otb

#endif 
