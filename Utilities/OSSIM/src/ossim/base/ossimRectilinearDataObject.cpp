//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
// Contributor: David A. Horner (DAH) - http://dave.thehorners.com
//
//*************************************************************************
// $Id: ossimRectilinearDataObject.cpp 10483 2007-02-13 04:34:48Z dburken $

#include <ossim/base/ossimRectilinearDataObject.h>
#include <ossim/base/ossimScalarTypeLut.h>

RTTI_DEF1(ossimRectilinearDataObject, "ossimRectilinearDataObject", ossimDataObject);

ossimRectilinearDataObject::ossimRectilinearDataObject(const ossimRectilinearDataObject& rhs)
   : ossimDataObject(rhs),
     theNumberOfDataComponents(rhs.theNumberOfDataComponents),
     theNumberOfSpatialComponents(rhs.theNumberOfSpatialComponents),
     theScalarType(rhs.theScalarType),
     theDataBuffer(0),
     theSpatialExtents(NULL)
{
   theDataBuffer = rhs.theDataBuffer;

   if(rhs.theSpatialExtents)
   {
      theSpatialExtents = new ossim_uint32[rhs.theNumberOfSpatialComponents];
      memcpy(theSpatialExtents, rhs.theSpatialExtents, sizeof(ossim_uint32)*theNumberOfSpatialComponents);
   }
}

ossimRectilinearDataObject::ossimRectilinearDataObject(ossim_uint32 numberOfSpatialComponents,
                                                       ossimSource* owner,
                                                       ossim_uint32 numberOfDataComponents,
                                                       ossimScalarType   scalarType,
                                                       ossimDataObjectStatus status)
   :ossimDataObject(owner, OSSIM_NULL),
    theNumberOfDataComponents(numberOfDataComponents),
    theNumberOfSpatialComponents(numberOfSpatialComponents),
    theScalarType(scalarType),
    theDataBuffer(0),
    theSpatialExtents(NULL)
{
   if(theNumberOfSpatialComponents)
   {
      theSpatialExtents = new ossim_uint32[theNumberOfSpatialComponents];
      memset(theSpatialExtents, 1, theNumberOfSpatialComponents);
   }
}

ossimRectilinearDataObject::ossimRectilinearDataObject(ossimSource* owner,
                                                       ossim_uint32 numberOfDataComponents,
                                                       ossim_uint32 length,
                                                       ossimScalarType   scalarType,
                                                       ossimDataObjectStatus status)
   :ossimDataObject(owner, OSSIM_NULL),
    theNumberOfDataComponents(numberOfDataComponents),
    theNumberOfSpatialComponents(1),
    theScalarType(scalarType),
    theDataBuffer(0),
    theSpatialExtents(NULL)
{
   
   theSpatialExtents = new ossim_uint32[1];
   theSpatialExtents[0] = length;
}

ossimRectilinearDataObject::ossimRectilinearDataObject(ossimSource* owner,
                                                       ossim_uint32 numberOfDataComponents,
                                                       ossim_uint32 width,
                                                       ossim_uint32 height,
                                                       ossimScalarType   scalarType,
                                                       ossimDataObjectStatus status)
   :ossimDataObject(owner, OSSIM_NULL),
    theNumberOfDataComponents(numberOfDataComponents),
    theNumberOfSpatialComponents(2),
    theScalarType(scalarType),
    theDataBuffer(0),
    theSpatialExtents(NULL)
{
   
   theSpatialExtents = new ossim_uint32[2];
   theSpatialExtents[0] = width;
   theSpatialExtents[1] = height;
}

ossimRectilinearDataObject::ossimRectilinearDataObject(ossimSource* owner,
                                                       ossim_uint32 numberOfDataComponents,
                                                       ossim_uint32 width,
                                                       ossim_uint32 height,
                                                       ossim_uint32 depth,
                                                       ossimScalarType   scalarType,
                                                       ossimDataObjectStatus status)
   :ossimDataObject(owner, OSSIM_NULL),
    theNumberOfDataComponents(numberOfDataComponents),
    theNumberOfSpatialComponents(3),
    theScalarType(scalarType),
    theDataBuffer(0),
    theSpatialExtents(NULL)
{
   
   theSpatialExtents = new ossim_uint32[3];
   theSpatialExtents[0] = width;
   theSpatialExtents[1] = height;
   theSpatialExtents[2] = depth;
}

ossimRectilinearDataObject::~ossimRectilinearDataObject()
{
   if(theSpatialExtents)
   {
      delete [] theSpatialExtents;
      theSpatialExtents = NULL;
   }
   theNumberOfDataComponents    = 0;
   theNumberOfSpatialComponents = 0;
}

ossim_uint32 ossimRectilinearDataObject::computeSpatialProduct()const
{
   if(!theSpatialExtents) return 0;
   ossim_uint32 spatialProduct = 1;
   for(ossim_uint32 index = 0; index < theNumberOfSpatialComponents; ++index)
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
   if(!extents) return;
   if((size != theNumberOfSpatialComponents)&&size)
   {
      if(theSpatialExtents)
      {
         delete [] theSpatialExtents;
         theSpatialExtents = NULL;
      }
      theSpatialExtents = new ossim_uint32[size];
   }
   int i = 0;
   for(i =0; i < (int)size; ++i)
   {
      theSpatialExtents[i] = extents[i];
   }
   theNumberOfSpatialComponents = size;
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
   return theNumberOfSpatialComponents;
}

const ossim_uint32* ossimRectilinearDataObject::getSpatialExtents()const
{
   return theSpatialExtents;
}

ossimScalarType ossimRectilinearDataObject::getScalarType() const
{
   return theScalarType;
}

ossim_uint32 ossimRectilinearDataObject::getScalarSizeInBytes() const
{
   return ossimGetScalarSizeInBytes(getScalarType());
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
   if(!data) return;
   ossimDataObject::assign(data);

   //***
   // This method assumes contiguous buffer for "theData".  Override if
   // needed.
   //***
   
   if(theNumberOfSpatialComponents != data->theNumberOfSpatialComponents)
   {
      delete [] theSpatialExtents;
      if(data->theNumberOfSpatialComponents)
      {
         theSpatialExtents = new ossim_uint32[data->theNumberOfSpatialComponents];
      }
      else
      {
         theSpatialExtents = NULL;
      }
   }
   for(ossim_uint32 index = 0; index < theNumberOfSpatialComponents; ++index)
   {
      theSpatialExtents[index] = data->theSpatialExtents[index];
   }
   
   theNumberOfSpatialComponents = data->theNumberOfSpatialComponents;
   theNumberOfDataComponents    = data->theNumberOfDataComponents;
   theScalarType                = data->theScalarType;
   //***
   // First copy the buffer if there is one.
   //***
   theDataBuffer = data->theDataBuffer;
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
      << "\ntheNumberOfSpatialComponents:  " << theNumberOfSpatialComponents;
   
   out << "\ntheScalarType:                 "
       << (ossimScalarTypeLut::instance()->getEntryString(theScalarType))
       << endl;

   return ossimDataObject::print(out);
}
