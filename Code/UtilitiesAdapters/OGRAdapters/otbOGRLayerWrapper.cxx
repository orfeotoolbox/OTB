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
#include "otbOGRLayerWrapper.h"

#include <cassert>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include "ogrsf_frmts.h" // OGRDataSource & OGRLayer

#include "otbOGRDataSourceWrapper.h"

/*===========================================================================*/
/*======================[ Construction & Destruction ]=======================*/
/*===========================================================================*/
namespace  { // Anonymous namespace
  /**\ingroup GeometryInternals
   * Deleter for \c boost::shared_ptr<> that doesn't delete.
   * This is required for \c OGRLayer s that belong to \c OGRDataSource.
   * \internal Unlike OGR, works as a no-op on null geometries.
   */
  struct LeaveAloneDeleter
    {
    void operator()(OGRLayer*) const {}
    };
} // Anonymous namespace


otb::ogr::Layer::Layer(OGRLayer* layer/*, DataSourcePtr datasource*/)
: m_Layer(layer, LeaveAloneDeleter())
#if 0
  , m_DataSource(datasource)
#endif
{
}

otb::ogr::Layer::Layer(OGRLayer* layer, OGRDataSource& sourceInChargeOfLifeTime)
: m_Layer(layer,  boost::bind(&OGRDataSource::ReleaseResultSet, boost::ref(sourceInChargeOfLifeTime), _1))
{
  assert(layer && "A null OGRlayer cannot belong to an OGRDataSource" );
  // OGR always refuses "delete 0". *sigh*
}

/*===========================================================================*/
/*===============================[ Features ]================================*/
/*===========================================================================*/
int otb::ogr::Layer::GetFeatureCount(bool doForceComputation) const
{
  assert(m_Layer && "Can't ask the features count on an invalid layer");
  return m_Layer->GetFeatureCount(doForceComputation);
}

otb::ogr::Feature otb::ogr::Layer::GetNextFeature()
{
  assert(m_Layer && "OGRLayer not initialized");
  OGRFeature * f = m_Layer->GetNextFeature();
  return f;
}

otb::ogr::Layer::iterator otb::ogr::Layer::begin()
{
  assert(m_Layer && "OGRLayer not initialized");
  m_Layer->ResetReading();
  return iterator(*this);
}

otb::ogr::Layer::const_iterator otb::ogr::Layer::cbegin() const
{
  assert(m_Layer && "OGRLayer not initialized");
  m_Layer->ResetReading();
  return const_iterator(*const_cast <Layer*>(this));
}

otb::ogr::Layer::iterator otb::ogr::Layer::start_at(size_t index)
{
  assert(m_Layer && "OGRLayer not initialized");
  m_Layer->SetNextByIndex(index);
  return iterator(*this);
}

otb::ogr::Layer::const_iterator otb::ogr::Layer::cstart_at(size_t index) const
{
  assert(m_Layer && "OGRLayer not initialized");
  m_Layer->SetNextByIndex(index);
  return const_iterator(*const_cast <Layer*>(this));
}

void otb::ogr::Layer::CreateFeature(Feature feature)
{
  assert(m_Layer && "OGRLayer not initialized");
  const OGRErr res = m_Layer->CreateFeature(&feature.ogr());
  if (res != OGRERR_NONE)
    {
    itkGenericExceptionMacro(<< "Cannot create a new feature in the layer <"
      <<GetName()<<">: " << CPLGetLastErrorMsg());
    }
}

void otb::ogr::Layer::DeleteFeature(long nFID)
{
  assert(m_Layer && "OGRLayer not initialized");
  const OGRErr res = m_Layer->DeleteFeature(nFID);
  if (res != OGRERR_NONE)
    {
    itkGenericExceptionMacro(<< "Cannot delete the feature <"<<nFID<<"> in the layer <"
      <<GetName()<<">: " << CPLGetLastErrorMsg());
    }
}

otb::ogr::Feature otb::ogr::Layer::GetFeature(long nFID)
{
  assert(m_Layer && "OGRLayer not initialized");
  if (nFID == OGRNullFID)
    {
    itkGenericExceptionMacro(<< "Invalid feature null id GetFeature() in the layer <"<<GetName()<<">.");
    }
  const Feature feat = m_Layer->GetFeature(nFID);
  return feat;
}

void otb::ogr::Layer::SetFeature(Feature feature)
{
  assert(m_Layer && "OGRLayer not initialized");
  const OGRErr res = m_Layer->SetFeature(&feature.ogr());
  if (res != OGRERR_NONE)
    {
    itkGenericExceptionMacro(<< "Cannot update a feature in the layer <"
      <<GetName()<<">: " << CPLGetLastErrorMsg());
    }
}

/*===========================================================================*/
/*=================================[ Misc ]==================================*/
/*===========================================================================*/
std::string otb::ogr::Layer::GetName() const
{
  assert(m_Layer && "null layer");
#if GDAL_VERSION_NUM >= 1800
  return m_Layer->GetName();
#else
  return GetLayerDefn().GetName();
#endif
}

OGREnvelope otb::ogr::Layer::GetExtent(bool force/* = false */) const
{
  assert(m_Layer && "OGRLayer not initialized");
  OGREnvelope sExtent;
  const OGRErr res = m_Layer->GetExtent(&sExtent,force);
  if(res != OGRERR_NONE)
    {
    itkGenericExceptionMacro(<< "Cannot retrieve extent of layer <"
      <<GetName()<<">: " << CPLGetLastErrorMsg());
    }
  return sExtent;
}

void otb::ogr::Layer::GetExtent(double& ulx, double& uly, double& lrx, double& lry, bool force) const
{
  const OGREnvelope sExtent = GetExtent(force);
  ulx = sExtent.MinX;
  uly = sExtent.MinY;
  lrx = sExtent.MaxX;
  lry = sExtent.MaxY;
}

OGRLayer & otb::ogr::Layer::ogr()
{
  assert(m_Layer && "OGRLayer not initialized");
  return *m_Layer;
}

void otb::ogr::Layer::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  os << indent << "+";
  if (m_Layer) // in case for odd reason the layer that should exist can't be found
    {
    os << "Layer <" << GetName() << "> of "<< OGRGeometryTypeToName(GetGeomType()) <<"\n";
    indent = indent.GetNextIndent();
    BOOST_FOREACH(Feature f, *this)
      {
      f.PrintSelf(os, indent);
      }
    }
  else
    {
    os << "null Layer\n";
    }
}

/*===========================================================================*/
/*============================[ Spatial Filter ]=============================*/
/*===========================================================================*/

OGRGeometry const* otb::ogr::Layer::GetSpatialFilter() const
{
  assert(m_Layer && "OGRLayer not initialized");
  OGRGeometry* spatialFilter = m_Layer->GetSpatialFilter();
  return spatialFilter;
}

void otb::ogr::Layer::SetSpatialFilter(OGRGeometry const* spatialFilter)
{
  assert(m_Layer && "OGRLayer not initialized");
  // const_cast because OGR is not 100% const-correct
  m_Layer->SetSpatialFilter(const_cast <OGRGeometry*>(spatialFilter));
}

void otb::ogr::Layer::SetSpatialFilterRect(
  double dfMinX, double dfMinY, double dfMaxX, double dfMaxY)
{
  assert(m_Layer && "OGRLayer not initialized");
  m_Layer->SetSpatialFilterRect(dfMinX, dfMinY, dfMaxX, dfMaxY);
}

OGRSpatialReference const* otb::ogr::Layer::GetSpatialRef() const
{
  assert(m_Layer && "OGRLayer not initialized");
  return m_Layer->GetSpatialRef();
}

std::string otb::ogr::Layer::GetProjectionRef() const
{
  assert(m_Layer && "OGRLayer not initialized");
  char * wkt = 0;
  OGRSpatialReference const* srs = GetSpatialRef();
  if(srs)
    {
    const OGRErr res = srs->exportToWkt(&wkt);

    if(res !=  OGRERR_NONE)
      {
      itkGenericExceptionMacro(<< "Cannot convert spatial reference to wkt string for layer <"
        <<GetName()<<">: " << CPLGetLastErrorMsg());
      }

    assert(wkt);
    const std::string stringWkt(wkt);
    // According to documentation, argument of exportToWkt() should be freed
    CPLFree(wkt);
    return stringWkt;
    }

  return "";
}

/*===========================================================================*/
/*==========================[ Feature Definition ]===========================*/
/*===========================================================================*/
OGRFeatureDefn & otb::ogr::Layer::GetLayerDefn() const
{
  assert(m_Layer && "OGRLayer not initialized");
  return *const_cast <OGRLayer*>(m_Layer.get())->GetLayerDefn();
}

void otb::ogr::Layer::CreateField(
  FieldDefn const& field, bool bApproxOK/* = true */)
{
  assert(m_Layer && "OGRLayer not initialized");
  const OGRErr res = m_Layer->CreateField(&field.ogr(), bApproxOK);
  if (res != OGRERR_NONE)
    {
    itkGenericExceptionMacro(<< "Cannot create a field in the layer <"
      <<GetName()<<">: " << CPLGetLastErrorMsg());
    }
}

void otb::ogr::Layer::DeleteField(size_t fieldIndex)
{
  assert(m_Layer && "OGRLayer not initialized");
#if GDAL_VERSION_NUM < 1900
  itkGenericExceptionMacro("OGRLayer::DeleteField is not supported by OGR v"
    << GDAL_VERSION_NUM << ". Upgrade to a version >= 1.9.0, and recompile OTB.")
#else
  const OGRErr res = m_Layer->DeleteField(int(fieldIndex));
  if (res != OGRERR_NONE)
    {
    itkGenericExceptionMacro(<< "Cannot delete the "<<fieldIndex << "th field in the layer <"
      <<GetName() <<">: " << CPLGetLastErrorMsg());
    }
#endif
}

void otb::ogr::Layer::AlterFieldDefn(
  size_t fieldIndex, FieldDefn const& newFieldDefn, int nFlags)
{
  assert(m_Layer && "OGRLayer not initialized");
#if GDAL_VERSION_NUM < 1900
  itkGenericExceptionMacro("OGRLayer::AlterFieldDefn is not supported by OGR v"
    << GDAL_VERSION_NUM << ". Upgrade to a version >= 1.9.0, and recompile OTB.")
#else
  const OGRErr res = m_Layer->AlterFieldDefn(
    int(fieldIndex),
    &newFieldDefn.ogr(),
    nFlags);
  if (res != OGRERR_NONE)
    {
    itkGenericExceptionMacro(<< "Cannot alter the "<<fieldIndex << "th field in the layer <"
      <<GetName() <<">: " << CPLGetLastErrorMsg());
    }
#endif
}

void otb::ogr::Layer::ReorderField(size_t oldPos, size_t newPos)
{
  assert(m_Layer && "OGRLayer not initialized");
#if GDAL_VERSION_NUM < 1900
  itkGenericExceptionMacro("OGRLayer::ReorderField is not supported by OGR v"
    << GDAL_VERSION_NUM << ". Upgrade to a version >= 1.9.0, and recompile OTB.")
#else
  const OGRErr res = m_Layer->ReorderField(int(oldPos), int(newPos));
  if (res != OGRERR_NONE)
    {
    itkGenericExceptionMacro(<< "Cannot move the "<<oldPos << "th field to the "
      << newPos << "th position in the layer <" <<GetName() <<">: " << CPLGetLastErrorMsg());
    }
#endif
}

void otb::ogr::Layer::ReorderFields(int * map)
{
  assert(m_Layer && "OGRLayer not initialized");
#if GDAL_VERSION_NUM < 1900
  itkGenericExceptionMacro("OGRLayer::ReorderField is not supported by OGR v"
    << GDAL_VERSION_NUM << ". Upgrade to a version >= 1.9.0, and recompile OTB.")
#else
  const OGRErr res = m_Layer->ReorderFields(map);
  if (res != OGRERR_NONE)
    {
    itkGenericExceptionMacro(<< "Cannot reorder the fields of the layer <"
      <<GetName() <<">: " << CPLGetLastErrorMsg());
    }
#endif
}

void otb::ogr::Layer::SetIgnoredFields(char const** fieldNames)
{
  assert(m_Layer && "OGRLayer not initialized");
#if GDAL_VERSION_NUM >= 1900
  const OGRErr res = m_Layer->SetIgnoredFields(fieldNames);
  if (res != OGRERR_NONE)
    {
    itkGenericExceptionMacro(<< "Cannot set fields to ignore on the layer <"
      <<GetName() <<">: " << CPLGetLastErrorMsg());
    }
#else
  itkGenericExceptionMacro("OGRLayer::SetIgnoredFields is not supported by OGR v"
    << GDAL_VERSION_NUM << ". Upgrade to a version >= 1.9.0, and recompile OTB.")
#endif
}

OGRwkbGeometryType otb::ogr::Layer::GetGeomType() const
{
  assert(m_Layer && "OGRLayer not initialized");
#if GDAL_VERSION_NUM < 1800
  return GetLayerDefn().GetGeomType();
#else
  return m_Layer->GetGeomType();
#endif
}

bool otb::ogr::operator==(Layer const& lhs, Layer const& rhs)
{
  const bool equal = lhs.m_Layer.get() == rhs.m_Layer.get();
  return equal;
}
