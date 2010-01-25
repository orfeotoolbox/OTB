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
// $Id: ossimRectilinearDataObject.cpp 15833 2009-10-29 01:41:53Z eshirschorn $

#include <ossim/base/ossimRectilinearDataObject.h>
#include <ossim/base/ossimScalarTypeLut.h>

RTTI_DEF1(ossimRectilinearDataObject, "ossimRectilinearDataObject", ossimDataObject);

ossimRectilinearDataObject::ossimRectilinearDataObject()
   : ossimDataObject(),
     theNumberOfDataComponents(0),
     theScalarType(),
     theDataBuffer(),
     theSpatialExtents()
{
}

ossimRectilinearDataObject::ossimRectilinearDataObject(
   const ossimRectilinearDataObject& rhs)
   : ossimDataObject(rhs),
     theNumberOfDataComponents(rhs.theNumberOfDataComponents),
     theScalarType(rhs.theScalarType),
     theDataBuffer(rhs.theDataBuffer),
     theSpatialExtents(rhs.theSpatialExtents)
{
}

ossimRectilinearDataObject::ossimRectilinearDataObject(
   ossim_uint32 numberOfSpatialComponents,
   ossimSource* owner,
   ossim_uint32 numberOfDataComponents,
   ossimScalarType   scalarType,
   ossimDataObjectStatus status)
   :ossimDataObject(owner, OSSIM_NULL),
    theNumberOfDataComponents(numberOfDataComponents),
    theScalarType(scalarType),
    theDataBuffer(0),
    theSpatialExtents(numberOfSpatialComponents)
{
}

ossimRectilinearDataObject::ossimRectilinearDataObject(
   ossimSource* owner,
   ossim_uint32 numberOfDataComponents,
   ossim_uint32 length,
   ossimScalarType   scalarType,
   ossimDataObjectStatus status)
   :ossimDataObject(owner, OSSIM_NULL),
    theNumberOfDataComponents(numberOfDataComponents),
    theScalarType(scalarType),
    theDataBuffer(0),
    theSpatialExtents(1)
{
   theSpatialExtents[0] = length;
}

ossimRectilinearDataObject::ossimRectilinearDataObject(
   ossimSource* owner,
   ossim_uint32 numberOfDataComponents,
   ossim_uint32 width,
   ossim_uint32 height,
   ossimScalarType   scalarType,
   ossimDataObjectStatus status)
   :ossimDataObject(owner, OSSIM_NULL),
    theNumberOfDataComponents(numberOfDataComponents),
    theScalarType(scalarType),
    theDataBuffer(0),
    theSpatialExtents(2)
{
   theSpatialExtents[0] = width;
   theSpatialExtents[1] = height;
}

ossimRectilinearDataObject::ossimRectilinearDataObject(
   ossimSource* owner,
   ossim_uint32 numberOfDataComponents,
   ossim_uint32 width,
   ossim_uint32 height,
   ossim_uint32 depth,
   ossimScalarType   scalarType,
   ossimDataObjectStatus status)
   :ossimDataObject(owner, OSSIM_NULL),
    theNumberOfDataComponents(numberOfDataComponents),
    theScalarType(scalarType),
    theDataBuffer(0),
    theSpatialExtents(3)
{
   theSpatialExtents[0] = width;
   theSpatialExtents[1] = height;
   theSpatialExtents[2] = depth;
}

ossimRectilinearDataObject::~ossimRectilinearDataObject()
{
}

ossim_uint32 ossimRectilinearDataObject::computeSpatialProduct()const
{
   ossim_uint32 spatialProduct = 0;
   for(ossim_uint32 index = 0; index < theSpatialExtents.size(); ++index)
   {
      spatialProduct *= theSpatialExtents[index];
   }
   return spatialProduct;
}

void ossimRectilinearDataObject::setNumberOfDataComponents(ossim_uint32 n)
{
   theNumberOfDataComponents = n;
}

void ossimRectilinearDataObject::setSpatialExtents(ossim_uint32* extents,
                                                   ossim_uint32 size)
{
   if (extents)
   {
      theSpatialExtents.resize(size);
      for(ossim_uint32 i =0; i < size; ++i)
      {
         theSpatialExtents[i] = extents[i];
      }
   }
}

void ossimRectilinearDataObject::setScalarType(ossimScalarType type)
{
   theScalarType = type;
}

ossim_uint32 ossimRectilinearDataObject::getNumberOfDataComponents() const
{
   return theNumberOfDataComponents;
}

ossim_uint32 ossimRectilinearDataObject::getNumberOfSpatialComponents() const
{
   return (ossim_uint32)theSpatialExtents.size();
}

const ossim_uint32* ossimRectilinearDataObject::getSpatialExtents()const
{
   return &(theSpatialExtents.front());
}

ossimScalarType ossimRectilinearDataObject::getScalarType() const
{
   return theScalarType;
}

ossim_uint32 ossimRectilinearDataObject::getScalarSizeInBytes() const
{
   return ossim::scalarSizeInBytes(getScalarType());
}

void* ossimRectilinearDataObject::getBuf()
{
   if (theDataBuffer.size() > 0)
   {
      return static_cast<void*>(&theDataBuffer.front());
   }
   return NULL;
}

const void* ossimRectilinearDataObject::getBuf()const
{
   if (theDataBuffer.size() > 0)
   {
      return static_cast<const void*>(&theDataBuffer.front());
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
         
         theNumberOfDataComponents    = data->theNumberOfDataComponents;
         theScalarType                = data->theScalarType;
         theDataBuffer                = data->theDataBuffer;
         theSpatialExtents            = data->theSpatialExtents;
      }
   }
}

void ossimRectilinearDataObject::initialize()
{
   if (theDataBuffer.size() != getDataSizeInBytes())
   {
      theDataBuffer.resize(getDataSizeInBytes());
      setDataObjectStatus(OSSIM_STATUS_UNKNOWN);
   }
}

ossim_uint32 ossimRectilinearDataObject::getDataSizeInBytes()const
{
   return (ossim_uint32)(getScalarSizeInBytes()*
                         computeSpatialProduct()*
                         theNumberOfDataComponents);
}

std::ostream& ossimRectilinearDataObject::print(std::ostream& out) const
{
   out << "ossimRectilinearDataObject::print:"
       << "\ntheNumberOfDataComponents:     " << theNumberOfDataComponents
       << "\ntheNumberOfSpatialComponents:  " << theSpatialExtents.size()
       << "\ntheScalarType:                 "
       << (ossimScalarTypeLut::instance()->getEntryString(theScalarType))
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
      theNumberOfDataComponents    = rhs.theNumberOfDataComponents;
      theScalarType                = rhs.theScalarType;
      theDataBuffer                = rhs.theDataBuffer;
      theSpatialExtents            = rhs.theSpatialExtents;
   }
   return *this;
}
