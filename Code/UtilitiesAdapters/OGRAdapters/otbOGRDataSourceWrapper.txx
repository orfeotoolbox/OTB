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
/*=========================[ otb::ogr::DataSource ]==========================*/
/*===========================================================================*/
// These function definitions are inline so assert() will be expanded according
// to the compilation mode of the client code.

inline
OGRDataSource & otb::ogr::DataSource::ogr()
{
  assert(m_DataSource && "OGRDataSource not initialized");
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


#endif // __otbOGRDataSourceWrapper_txx
