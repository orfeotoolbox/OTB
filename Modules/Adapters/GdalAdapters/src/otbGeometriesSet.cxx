/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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


/*===========================================================================*/
/*===============================[ Includes ]================================*/
/*===========================================================================*/
#include "otbGeometriesSet.h"
#include <cassert>

/*===========================================================================*/
/*================================[ Helpers ]================================*/
/*===========================================================================*/
namespace otb { namespace ogr { namespace internal {

/**\ingroup GeometryInternals
 * Access shim: itk smart-pointer to reference.
 * \param[in] ds  itk smart pointer
 * \return a reference to the pointee.
 * \throw None
 * \pre <tt>ds != 0</tt>
 * \since OTB v 3.14.0
 */
template <typename DataType> inline
  DataType & getRef(typename itk::SmartPointer<DataType> const& ds)
    {
    assert(ds && "unexpected nil datasource");
    return *ds;
    }

/**\ingroup GeometryInternals
 * Access shim: const layer to reference.
 * \param[in] layer layer proxy
 * \return the layer proxy
 * \throw None
 * \since OTB v 3.14.0
 */
inline
otb::ogr::Layer const& getRef(otb::ogr::Layer const& layer)
  {
  return layer;
  }

/**\ingroup GeometryInternals
 * Access shim: layer to reference.
 * \param[in] layer layer proxy
 * \return the layer proxy
 * \throw None
 * \since OTB v 3.14.0
 */
inline
otb::ogr::Layer & getRef(otb::ogr::Layer & layer)
  {
  return layer;
  }
} } } // otb::ogr::internal namespaces


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

void otb::GeometriesSet::CopyInformation(const DataObject *data)
{
  const otb::GeometriesSet * geom =
    dynamic_cast<const otb::GeometriesSet*>(data);
  if (geom)
    {
    this->SetLargestPossibleRegion(geom->GetLargestPossibleRegion());
    }
}

void otb::GeometriesSet::SetRequestedRegionToLargestPossibleRegion()
{
  this->SetRequestedRegion(this->GetLargestPossibleRegion());
}

void otb::GeometriesSet::UpdateOutputInformation()
{
  Superclass::UpdateOutputInformation();
  if( !this->GetSource() && !m_GeometriesSet.empty())
    {
    ogr::Layer layer(ITK_NULLPTR,true);
    switch(m_GeometriesSet.which())
      {
      case 0: // OGRDataSource
        {
        ogr::DataSource::Pointer & ds = boost::get<ogr::DataSource::Pointer>(m_GeometriesSet);
        layer = ds->GetLayer(0);
        break;
        }
      case 1: // OGRLayer
        {
        layer = boost::get<ogr::Layer>(m_GeometriesSet);
        break;
        }
      default:
        break;
      }
    switch(m_LargestPossibleRegion.GetMode())
      {
      case otb::GeometriesRegion::RANGE:
        {
        int size = layer.GetFeatureCount(false);
        if (size < 0)
          size = layer.GetFeatureCount(true);
        assert((size >= 0) && "Unable to compute number of features");
        m_LargestPossibleRegion.SetStartId(0);
        m_LargestPossibleRegion.SetCount(static_cast<unsigned long>(size));
        break;
        }
      case otb::GeometriesRegion::SPATIAL:
        {
        double ulx=0.0,uly=0.0,lrx=0.0,lry=0.0;
        bool extentAvailable = true;
        try
          {
          layer.GetExtent(ulx,uly,lrx,lry,false);
          }
        catch(const itk::ExceptionObject&)
          {
          extentAvailable = false;
          }
        if (!extentAvailable)
          {
          try
            {
            layer.GetExtent(ulx,uly,lrx,lry,true);
            extentAvailable = true;
            }
          catch (const itk::ExceptionObject&)
            {}
          }
        assert(extentAvailable && "Unable to compute extent");
        m_LargestPossibleRegion.SetStartX(ulx);
        m_LargestPossibleRegion.SetStartY(uly);
        m_LargestPossibleRegion.SetEndX(lrx);
        m_LargestPossibleRegion.SetEndY(lry);
        break;
        }
      default:
        break;
      }
    }
}

void
otb::GeometriesSet::SetLargestPossibleRegion(const GeometriesRegion & region)
{
  if (m_LargestPossibleRegion != region)
    {
    m_LargestPossibleRegion = region;
    this->Modified();
    }
}

const otb::GeometriesRegion &
otb::GeometriesSet::GetLargestPossibleRegion() const
{
  return m_LargestPossibleRegion;
}

void
otb::GeometriesSet::SetBufferedRegion(const GeometriesRegion & region)
{
  if (m_BufferedRegion != region)
    {
    m_BufferedRegion = region;
    this->Modified();
    }
}

const otb::GeometriesRegion &
otb::GeometriesSet::GetBufferedRegion() const
{
  return m_BufferedRegion;
}

void
otb::GeometriesSet::SetRequestedRegion(const GeometriesRegion & region)
{
  if (m_RequestedRegion != region)
    {
    m_RequestedRegion = region;
    this->Modified();
    }
}

const otb::GeometriesRegion &
otb::GeometriesSet::GetRequestedRegion() const
{
  return m_RequestedRegion;
}

void
otb::GeometriesSet::SetRequestedRegion( const DataObject *data )
{
  const otb::GeometriesSet * const geometries =
    dynamic_cast<const otb::GeometriesSet *>(data);
  if (geometries != ITK_NULLPTR)
    {
    this->SetRequestedRegion(geometries->GetRequestedRegion());
    }
}

/*===========================================================================*/
/*=================================[ IsSet ]=================================*/
/*===========================================================================*/
namespace otb { namespace ogr { namespace internal {
/**\ingroup GeometryInternals
 * Visitor (functor) to test whether a geometries set is non-null.
 * \param[in] gs geometries set
 * \throw None
 * \since OTB v 3.14.0
 */
struct IsSetTester : boost::static_visitor<bool>
{
  template <typename T>
    bool operator()(T const& gs) const
      {
      return getRef(gs);
      }
};
} } } // otb::ogr::internal namespaces

bool otb::GeometriesSet::IsSet() const
{
  return this->apply(ogr::internal::IsSetTester());
}

/*===========================================================================*/
/*===============================[ Printing ]================================*/
/*===========================================================================*/
namespace otb { namespace ogr { namespace internal {
/**\ingroup GeometryInternals
 * Visitor (functor) to print a geometries set.
 * \param[in] gs geometries set
 * \throw None
 * \since OTB v 3.14.0
 */
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
} } } // otb::ogr::internal namespaces

void otb::GeometriesSet::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->apply(ogr::internal::Printer(os, indent));
}
