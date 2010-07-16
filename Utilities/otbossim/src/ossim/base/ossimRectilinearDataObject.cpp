//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
// Contributor: David A. Horner (DAH) - http://dave.thehorners.com
//
//*************************************************************************
// $Id: ossimRectilinearDataObject.cpp 17195 2010-04-23 17:32:18Z dburken $

#include <ossim/base/ossimRectilinearDataObject.h>
#include <ossim/base/ossimScalarTypeLut.h>

RTTI_DEF1(ossimRectilinearDataObject, "ossimRectilinearDataObject", ossimDataObject);

ossimRectilinearDataObject::ossimRectilinearDataObject()
   : ossimDataObject(),
     m_numberOfDataComponents(0),
     m_scalarType(),
     m_dataBuffer(),
     m_spatialExtents()
{
}

ossimRectilinearDataObject::ossimRectilinearDataObject(
   const ossimRectilinearDataObject& rhs)
   : ossimDataObject(rhs),
     m_numberOfDataComponents(rhs.m_numberOfDataComponents),
     m_scalarType(rhs.m_scalarType),
     m_dataBuffer(rhs.m_dataBuffer),
     m_spatialExtents(rhs.m_spatialExtents)
{
}

ossimRectilinearDataObject::ossimRectilinearDataObject(
   ossim_uint32 numberOfSpatialComponents,
   ossimSource* owner,
   ossim_uint32 numberOfDataComponents,
   ossimScalarType   scalarType,
   ossimDataObjectStatus /* status */)
   :ossimDataObject(owner, OSSIM_NULL),
    m_numberOfDataComponents(numberOfDataComponents),
    m_scalarType(scalarType),
    m_dataBuffer(0),
    m_spatialExtents(numberOfSpatialComponents)
{
}

ossimRectilinearDataObject::ossimRectilinearDataObject(
   ossimSource* owner,
   ossim_uint32 numberOfDataComponents,
   ossim_uint32 length,
   ossimScalarType   scalarType,
   ossimDataObjectStatus /* status */ )
   :ossimDataObject(owner, OSSIM_NULL),
    m_numberOfDataComponents(numberOfDataComponents),
    m_scalarType(scalarType),
    m_dataBuffer(0),
    m_spatialExtents(1)
{
   m_spatialExtents[0] = length;
}

ossimRectilinearDataObject::ossimRectilinearDataObject(
   ossimSource* owner,
   ossim_uint32 numberOfDataComponents,
   ossim_uint32 width,
   ossim_uint32 height,
   ossimScalarType   scalarType,
   ossimDataObjectStatus /* status */)
   :ossimDataObject(owner, OSSIM_NULL),
    m_numberOfDataComponents(numberOfDataComponents),
    m_scalarType(scalarType),
    m_dataBuffer(0),
    m_spatialExtents(2)
{
   m_spatialExtents[0] = width;
   m_spatialExtents[1] = height;
}

ossimRectilinearDataObject::ossimRectilinearDataObject(
   ossimSource* owner,
   ossim_uint32 numberOfDataComponents,
   ossim_uint32 width,
   ossim_uint32 height,
   ossim_uint32 depth,
   ossimScalarType   scalarType,
   ossimDataObjectStatus /* status */)
   :ossimDataObject(owner, OSSIM_NULL),
    m_numberOfDataComponents(numberOfDataComponents),
    m_scalarType(scalarType),
    m_dataBuffer(0),
    m_spatialExtents(3)
{
   m_spatialExtents[0] = width;
   m_spatialExtents[1] = height;
   m_spatialExtents[2] = depth;
}

ossimRectilinearDataObject::~ossimRectilinearDataObject()
{
}

ossim_uint32 ossimRectilinearDataObject::computeSpatialProduct()const
{
   ossim_uint32 spatialProduct = 0;
   for(ossim_uint32 index = 0; index < m_spatialExtents.size(); ++index)
   {
      spatialProduct *= m_spatialExtents[index];
   }
   return spatialProduct;
}

void ossimRectilinearDataObject::setNumberOfDataComponents(ossim_uint32 n)
{
   m_numberOfDataComponents = n;
}

void ossimRectilinearDataObject::setSpatialExtents(ossim_uint32* extents,
                                                   ossim_uint32 size)
{
   if (extents)
   {
      m_spatialExtents.resize(size);
      for(ossim_uint32 i =0; i < size; ++i)
      {
         m_spatialExtents[i] = extents[i];
      }
   }
}

void ossimRectilinearDataObject::setScalarType(ossimScalarType type)
{
   m_scalarType = type;
}

ossim_uint32 ossimRectilinearDataObject::getNumberOfDataComponents() const
{
   return m_numberOfDataComponents;
}

ossim_uint32 ossimRectilinearDataObject::getNumberOfSpatialComponents() const
{
   return (ossim_uint32)m_spatialExtents.size();
}

const ossim_uint32* ossimRectilinearDataObject::getSpatialExtents()const
{
   return &(m_spatialExtents.front());
}

ossimScalarType ossimRectilinearDataObject::getScalarType() const
{
   return m_scalarType;
}

ossim_uint32 ossimRectilinearDataObject::getScalarSizeInBytes() const
{
   return ossim::scalarSizeInBytes(getScalarType());
}

void* ossimRectilinearDataObject::getBuf()
{
   if (m_dataBuffer.size() > 0)
   {
      return static_cast<void*>(&m_dataBuffer.front());
   }
   return NULL;
}

const void* ossimRectilinearDataObject::getBuf()const
{
   if (m_dataBuffer.size() > 0)
   {
      return static_cast<const void*>(&m_dataBuffer.front());
   }
   return NULL;
}

void ossimRectilinearDataObject::assign(const ossimRectilinearDataObject* data)
{
   if(data)
   {
      if (this != data)
      {
         ossimDataObject::assign(data);
         
         m_numberOfDataComponents    = data->m_numberOfDataComponents;
         m_scalarType                = data->m_scalarType;
         m_dataBuffer                = data->m_dataBuffer;
         m_spatialExtents            = data->m_spatialExtents;
      }
   }
}

void ossimRectilinearDataObject::initialize()
{
   if (m_dataBuffer.size() != getDataSizeInBytes())
   {
      m_dataBuffer.resize(getDataSizeInBytes());
      setDataObjectStatus(OSSIM_STATUS_UNKNOWN);
   }
}

ossim_uint32 ossimRectilinearDataObject::getDataSizeInBytes()const
{
   return (ossim_uint32)(getScalarSizeInBytes()*
                         computeSpatialProduct()*
                         m_numberOfDataComponents);
}

std::ostream& ossimRectilinearDataObject::print(std::ostream& out) const
{
   out << "ossimRectilinearDataObject::print:"
       << "\nm_numberOfDataComponents:     " << m_numberOfDataComponents
       << "\ntheNumberOfSpatialComponents:  " << m_spatialExtents.size()
       << "\nm_scalarType:                 "
       << (ossimScalarTypeLut::instance()->getEntryString(m_scalarType))
       << endl;
   
   return ossimDataObject::print(out);
}

const ossimRectilinearDataObject& ossimRectilinearDataObject::operator=(
   const ossimRectilinearDataObject& rhs)
{
   if (this != &rhs)
   {
      // ossimDataObject initialization:
      ossimDataObject::operator=(rhs);

      // ossimRectilinearDataObject (this) initialization:
      m_numberOfDataComponents    = rhs.m_numberOfDataComponents;
      m_scalarType                = rhs.m_scalarType;
      m_dataBuffer                = rhs.m_dataBuffer;
      m_spatialExtents            = rhs.m_spatialExtents;
   }
   return *this;
}
