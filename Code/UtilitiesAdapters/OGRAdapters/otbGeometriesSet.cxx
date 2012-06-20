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

/*===========================================================================*/
/*===============================[ Includes ]================================*/
/*===========================================================================*/
#include "otbGeometriesSet.h"
#include <cassert>

/*===========================================================================*/
/*================================[ Helpers ]================================*/
/*===========================================================================*/
template <typename DataType>
inline
DataType & getRef(typename itk::SmartPointer<DataType> const& ds)
{
  assert(ds && "unexpected nil datasource");
  return *ds;
}

#if 0
template <typename DataType>
inline
DataType const& getRef(typename DataType::ConstPointer ds)
{
  assert(ds);
  return *ds;
}
#endif

otb::ogr::Layer const& getRef(otb::ogr::Layer const& layer)
{
  return layer;
}

otb::ogr::Layer & getRef(otb::ogr::Layer & layer)
{
  return layer;
}

/*===========================================================================*/
/*=======================[ Setting and construction ]========================*/
/*===========================================================================*/
otb::GeometriesSet::GeometriesSet()
: m_GeometriesSet(otb::ogr::DataSource::New())
, m_ImageReference(*this)
{
}

otb::GeometriesSet::GeometriesSet(ogr::DataSource::Pointer datasource)
: m_GeometriesSet(datasource)
, m_ImageReference(*this)
{
  assert(datasource && "unexpected nil datasource");
}

otb::GeometriesSet::GeometriesSet(ogr::Layer layer)
: m_GeometriesSet(layer)
, m_ImageReference(*this)
{
}

/*static*/
otb::GeometriesSet::Pointer otb::GeometriesSet::New(ogr::DataSource::Pointer datasource)
{
  Pointer res = new Self(datasource);
  res->UnRegister();
  return res;
}

/*static*/
otb::GeometriesSet::Pointer otb::GeometriesSet::New(ogr::Layer layer)
{
  Pointer res = new Self(layer);
  res->UnRegister();
  return res;
}

/*virtual*/
otb::GeometriesSet::~GeometriesSet()
{
}

void otb::GeometriesSet::Set(ogr::Layer layer)
{
  assert(layer && "unexpected nil layer");
  m_GeometriesSet = layer;
}

void otb::GeometriesSet::Set(ogr::DataSource::Pointer datasource)
{
  assert(datasource && "unexpected nil datasource");
  m_GeometriesSet = datasource;
}

/*===========================================================================*/
/*=================================[ IsSet ]=================================*/
/*===========================================================================*/
struct IsSetTester : boost::static_visitor<bool>
{
  template <typename T>
  bool operator()(T const& gs) const
    {
    return getRef(gs);
    }
};

bool otb::GeometriesSet::IsSet() const
{
  return this->apply(IsSetTester());
}

/*===========================================================================*/
/*===============================[ Printing ]================================*/
/*===========================================================================*/
struct Printer : boost::static_visitor<>
{
  Printer(std::ostream& os, itk::Indent indent)
    : m_os(os), m_indent(indent) {}
  void operator()(otb::ogr::Layer layer) const
    {
    assert(layer && "unexpected nil layer...");
    layer.PrintSelf(m_os, m_indent);
    }
  void operator()(otb::ogr::DataSource::Pointer datasource) const
    {
    assert(datasource && "unexpected nil datasource...");
    datasource->Print(m_os, m_indent);
    }
private:
  std::ostream &m_os;
  itk::Indent   m_indent;
};

void otb::GeometriesSet::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->apply(Printer(os, indent));
}

#if 0
/*===========================================================================*/
/*=========================[ GetMetaDataDictionary ]=========================*/
/*===========================================================================*/
struct MetaDataDictionaryGetter : boost::static_visitor<itk::MetaDataDictionary&>
{
  itk::MetaDataDictionary& operator()(otb::ogr::Layer layer) const
    {
    return layer.GetMetaDataDictionary();
    }
  itk::MetaDataDictionary& operator()(otb::ogr::DataSource::Pointer datasource) const
    {
    return datasource->GetMetaDataDictionary();
    }
};

itk::MetaDataDictionary & otb::GeometriesSet::GetMetaDataDictionary()
{
  return this->apply(MetaDataDictionaryGetter());
}

itk::MetaDataDictionary const& otb::GeometriesSet::GetMetaDataDictionary() const
{
  return const_cast <GeometriesSet *>(this)->GetMetaDataDictionary();
}
#endif
