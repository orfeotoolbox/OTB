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
#include <boost/range/algorithm.hpp>
#include "ogrsf_frmts.h" // OGRDataSource & OGRLayer

/*===========================================================================*/
/*======================[ Construction & Destruction ]=======================*/
/*===========================================================================*/
namespace  { // Anonymous namespace
  /**\ingroup Geometry
   * Deleter for \c boost::shared_ptr<> that doesn't delete.
   * This is required for \c OGRLayer s that belong to \c OGRDataSource.
   * \internal
   */
  struct LeaveAloneDeleter
    {
    void operator()(OGRLayer*) const {}
    };
} // Anonymous namespace


otb::ogr::Layer::Layer(OGRLayer* layer)
: m_Layer(layer, LeaveAloneDeleter())
{
}

otb::ogr::Layer::Layer(OGRLayer* layer, OGRDataSource* sourceInChargeOfLifeTime)
: m_Layer(layer,  boost::bind(&OGRDataSource::ReleaseResultSet, sourceInChargeOfLifeTime, _1))
{
  assert(layer && "A null OGRlayer cannot belong to an OGRDataSource" );
  // OGR always refuses "delete 0". *sigh*
}

/*===========================================================================*/
/*===============================[ Features ]================================*/
/*===========================================================================*/
int otb::ogr::Layer::GetFeatureCount(bool doForceComputation) const
{
  assert(m_Layer);
  return m_Layer->GetFeatureCount(doForceComputation);
}

otb::ogr::Feature otb::ogr::Layer::GetNextFeature()
{
  assert(m_Layer && "OGRLayer not initialized");
  return m_Layer->GetNextFeature();
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

void otb::ogr::Layer::CreateFeature(Feature feature)
{
  assert(m_Layer && "OGRLayer not initialized");
  m_Layer->CreateFeature(&feature.ogr());
}

void otb::ogr::Layer::DeleteFeature(long nFID)
{
  assert(m_Layer && "OGRLayer not initialized");
  m_Layer->DeleteFeature(nFID);
}

otb::ogr::Feature otb::ogr::Layer::GetFeature(long nFID)
{
  assert(m_Layer && "OGRLayer not initialized");
  Feature feat = m_Layer->GetFeature(nFID);
}

void otb::ogr::Layer::SetFeature(Feature feature)
{
  assert(m_Layer && "OGRLayer not initialized");
  m_Layer->SetFeature(&feature.ogr());
}

/*===========================================================================*/
/*=================================[ Misc ]==================================*/
/*===========================================================================*/
std::string otb::ogr::Layer::GetName() const
{
  assert(m_Layer && "null layer");
#if GDAL_VERSION >= 10800
  return m_Layer->GetName();
#else 
  return GetLayerDefn().GetName();
#endif
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
    os << "Layer <" << m_Layer->GetName() << ">\n";
    boost::for_each( // for each feature
      *this,
      boost::bind(&Feature::PrintSelf, _1, boost::ref(os), indent.GetNextIndent()));
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

/*===========================================================================*/
/*==========================[ Feature Definition ]===========================*/
/*===========================================================================*/
OGRFeatureDefn & otb::ogr::Layer::GetLayerDefn() const
{
  assert(m_Layer && "OGRLayer not initialized");
  return *const_cast <OGRLayer*>(m_Layer.get())->GetLayerDefn();
}

void otb::ogr::Layer::CreateField(
  OGRFieldDefn const& field, bool bApproxOK/* = true */)
{
  assert(m_Layer && "OGRLayer not initialized");
  const OGRErr res = m_Layer->CreateField(const_cast <OGRFieldDefn*>(&field), bApproxOK);
  if (res != OGRERR_NONE)
    {
    itkGenericExceptionMacro(<< "Cannot create a new field in the layer <"<<GetName()<<">.");
    }
}

void otb::ogr::Layer::DeleteField(size_t fieldIndex)
{
  assert(m_Layer && "OGRLayer not initialized");
#if GDAL_VERSION < 10900
  itkGenericExceptionMacro("OGRLayer::AlterFieldDefn is not supported by OGR v"
    << GDAL_VERSION << ". Upgrade to a version >= 1.9.0, and recompile OTB.")
#else
  const OGRErr res = m_Layer->DeleteField(int(fieldIndex));
  if (res != OGRERR_NONE)
    {
    itkGenericExceptionMacro(<< "Cannot delete the "<<fieldIndex << "th field in the layer <"
      <<GetName() <<">.");
    }
#endif
}

void otb::ogr::Layer::AlterFieldDefn(
  size_t fieldIndex, OGRFieldDefn& newFieldDefn, int nFlags)
{
  assert(m_Layer && "OGRLayer not initialized");
#if GDAL_VERSION < 10900
  itkGenericExceptionMacro("OGRLayer::AlterFieldDefn is not supported by OGR v"
    << GDAL_VERSION << ". Upgrade to a version >= 1.9.0, and recompile OTB.")
#else
  const OGRErr res = m_Layer->AlterFieldDefn(int(fieldIndex), &newFieldDefn, nFlags);
  if (res != OGRERR_NONE)
    {
    itkGenericExceptionMacro(<< "Cannot alter the "<<fieldIndex << "th field in the layer <"
      <<GetName() <<">.");
    }
#endif
}

void otb::ogr::Layer::ReorderField(size_t oldPos, size_t newPos)
{
  assert(m_Layer && "OGRLayer not initialized");
#if GDAL_VERSION < 10900
  itkGenericExceptionMacro("OGRLayer::ReorderField is not supported by OGR v"
    << GDAL_VERSION << ". Upgrade to a version >= 1.9.0, and recompile OTB.")
#else
  const OGRErr res = m_Layer->ReorderField(int(oldPos), int(newPos));
  if (res != OGRERR_NONE)
    {
    itkGenericExceptionMacro(<< "Cannot move the "<<oldPos << "th field to the "
      << newPos << "th position in the layer <" <<GetName() <<">.");
    }
#endif
}

void otb::ogr::Layer::ReorderFields(int * map)
{
  assert(m_Layer && "OGRLayer not initialized");
#if GDAL_VERSION < 10900
  itkGenericExceptionMacro("OGRLayer::ReorderField is not supported by OGR v"
    << GDAL_VERSION << ". Upgrade to a version >= 1.9.0, and recompile OTB.")
#else
  const OGRErr res = m_Layer->ReorderFields(map);
  if (res != OGRERR_NONE)
    {
    itkGenericExceptionMacro(<< "Cannot reorder the fields of the layer <"
      <<GetName() <<">.");
    }
#endif
}

