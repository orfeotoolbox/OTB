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


/**\ingroup Geometry
 * \file    otbOGRDataSourceWrapper.hxx
 * \internal this file is meant to be included by otbOGRDataSourceWrapper.h
 */

#ifndef otbOGRDataSourceWrapper_hxx
#define otbOGRDataSourceWrapper_hxx

#include "otbOGRDataSourceWrapper.h"

/*===========================================================================*/
/*===============================[ Includes ]================================*/
/*===========================================================================*/
#include <cassert>
#include "itkMacro.h" // itk::ExceptionObject
#include "ogrsf_frmts.h" // OGRDataSource & OGRLayer

/*===========================================================================*/
/*=========================[ otb::ogr::DataSource ]==========================*/
/*===========================================================================*/
// These function definitions are inline so assert() will be expanded according
// to the compilation mode of the client code.

inline
GDALDataset & otb::ogr::DataSource::ogr()
{
  assert(m_DataSource && "GDALDataset not initialized");
  return *m_DataSource;
}

inline
otb::ogr::Layer otb::ogr::DataSource::GetLayer(size_t i)
{
  assert(int(i) < GetLayersCount() && "Out-of-range index");
  OGRLayer * layer_ptr = GetLayerUnchecked(i);
  assert(layer_ptr && "No layer returned by OGR");
  return otb::ogr::Layer(layer_ptr, IsLayerModifiable(i));
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

inline
OGRLayer* otb::ogr::DataSource::GetLayerUnchecked(size_t i) const
{
  return const_cast <DataSource*>(this)->GetLayerUnchecked(i);
}

/*===========================================================================*/
/*====================[ otb::ogr::DataSource/iterators ]=====================*/
/*===========================================================================*/
template <class Value>
otb::ogr::DataSource::layer_iter<Value>::layer_iter(container_type & datasource, size_t index)
: m_DataSource(&datasource), m_index(index)
{}

template <class Value>
otb::ogr::DataSource::layer_iter<Value>::layer_iter()
: m_DataSource(0), m_index(0)
{}

template <class Value>
template <class OtherValue> otb::ogr::DataSource::layer_iter<Value>::layer_iter(
  otb::ogr::DataSource::layer_iter<OtherValue> const& other,
  typename boost::enable_if<boost::is_convertible<OtherValue*,Value*>
  , enabler
  >::type
)
: m_DataSource(other.m_DataSource), m_index(other.m_index)
{}

template <class Value>
template <class OtherValue>
bool otb::ogr::DataSource::layer_iter<Value>::equal(layer_iter<OtherValue> const& other) const
{
  return m_DataSource == other.m_DataSource && other.m_index == m_index;
}

template <class Value>
void otb::ogr::DataSource::layer_iter<Value>::increment()
{
  assert(m_DataSource
    && int(m_index) < m_DataSource->GetLayersCount()
    && "cannot increment past end()");
  ++m_index;
}

template <class Value>
Value otb::ogr::DataSource::layer_iter<Value>::dereference() const
{
  assert(m_DataSource
    && int(m_index) < m_DataSource->GetLayersCount()
    && "cannot dereference past end()");
  return Value(m_DataSource->GetLayerUnchecked(m_index), m_DataSource->IsLayerModifiable(m_index));
}


#endif // otbOGRDataSourceWrapper_hxx
