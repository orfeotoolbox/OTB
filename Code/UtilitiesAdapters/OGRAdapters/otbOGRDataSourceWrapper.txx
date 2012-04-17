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

/**\ingroup Geometry
 * \file    otbOGRDataSourceWrapper.txx
 * \internal this file is meant to be included by otbOGRDataSourceWrapper.h
 */

#ifndef __otbOGRDataSourceWrapper_txx
#define __otbOGRDataSourceWrapper_txx

/*===========================================================================*/
/*===============================[ Includes ]================================*/
/*===========================================================================*/
#include <cassert>
#include "itkMacro.h" // itk::ExceptionObject
#include "ogrsf_frmts.h" // OGRDataSource & OGRLayer

/*===========================================================================*/
/*==============================[ other stuff ]==============================*/
/*===========================================================================*/

// This implementation is inline so assert() will be expanded according to the
// compilation mode of the client code.
inline
otb::ogr::Layer otb::ogr::DataSource::GetLayer(size_t i)
{
  assert(int(i) < GetLayersCount());
  OGRLayer * layer_ptr = GetLayerUnchecked(i);
  assert(layer_ptr);
  return otb::ogr::Layer(layer_ptr);
}

inline
otb::ogr::Layer const otb::ogr::DataSource::GetLayer(size_t i) const
{
  return const_cast <DataSource*>(this)->GetLayer(i);
}

inline
otb::ogr::Layer const otb::ogr::DataSource::GetLayer(std::string const& name) const
{
  return const_cast <DataSource*>(this)->GetLayer(name);
}


inline
otb::ogr::Layer const otb::ogr::DataSource::GetLayerChecked(size_t i) const
{
  return const_cast <DataSource*>(this)->GetLayerChecked(i);
}

template <class Functor, typename V>
inline
V otb::ogr::DataSource::AccumulateOnLayers(Functor f, V v0) const
{
  assert(m_DataSource && "OGRDataSource not initialized");
  const int nbLayers = this->GetLayersCount();
  for (int i=0; i!=nbLayers; ++i)
    {
    const Layer l = GetLayer(i);
    if (!l)
      {
      itkExceptionMacro(<< "Failed to fetch "<< i <<"th layer from OGRDataSource");
      }
    v0 = f(l, v0);
    }
  return v0;
}

template <class Functor>
inline
void otb::ogr::DataSource::ForEachLayer(Functor f) const
{
  assert(m_DataSource && "OGRDataSource not initialized");
  const int nbLayers = this->GetLayersCount();
  for (int i=0; i!=nbLayers; ++i)
    {
    Layer l = GetLayer(i);
    if (!l)
      {
      itkExceptionMacro(<< "Failed to fetch "<< i <<"th layer from OGRDataSource");
      }
    f(l);
    }
}

#endif // __otbOGRDataSourceWrapper_txx

inline
OGRDataSource & otb::ogr::DataSource::ogr()
{
  assert(m_DataSource && "OGRDataSource not initialized");
  return *m_DataSource;
}

