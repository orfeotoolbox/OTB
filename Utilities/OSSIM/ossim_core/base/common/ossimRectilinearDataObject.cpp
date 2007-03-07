//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimRectilinearDataObject.cpp,v 1.12 2005/05/09 12:39:38 dburken Exp $

#include <base/common/ossimRectilinearDataObject.h>
#include <base/misc/lookup_tables/ossimScalarTypeLut.h>

RTTI_DEF1(ossimRectilinearDataObject, "ossimRectilinearDataObject", ossimDataObject);

ossimRectilinearDataObject::ossimRectilinearDataObject(const ossimRectilinearDataObject& rhs)
   : ossimDataObject(rhs),
     theNumberOfDataComponents(rhs.theNumberOfDataComponents),
     theNumberOfSpatialComponents(rhs.theNumberOfSpatialComponents),
     theScalarType(rhs.theScalarType),
     theDataBuffer(NULL),
     theSpatialExtents(NULL)
{
   if(rhs.theDataBuffer)
   {
      theDataBuffer = new ossim_uint8[rhs.getDataSizeInBytes()];
      if(theDataBuffer)
      {
         memcpy(theDataBuffer, rhs.theDataBuffer, rhs.getDataSizeInBytes());
      }
   }
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
    theDataBuffer(NULL),
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
    theDataBuffer(NULL),
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
    theDataBuffer(NULL),
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
    theDataBuffer(NULL),
    theSpatialExtents(NULL)
{
   
   theSpatialExtents = new ossim_uint32[3];
   theSpatialExtents[0] = width;
   theSpatialExtents[1] = height;
   theSpatialExtents[2] = depth;
}

ossimRectilinearDataObject::~ossimRectilinearDataObject()
{
   if(theDataBuffer)
   {
      delete [] theDataBuffer;
      theDataBuffer = NULL;
   }
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
   return static_cast<void*>(theDataBuffer);
}

const void* ossimRectilinearDataObject::getBuf()const
{
   return static_cast<const void*>(theDataBuffer);
}

void ossimRectilinearDataObject::assign(const ossimRectilinearDataObject* data)
{
   if(!data) return;
   ossimDataObject::assign(data);
   ossim_uint32 savedSize = getDataSizeInBytes();
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
   if(data->theDataBuffer)
   {
      //***
      // Make sure this buffer is the same size; if not, delete.
      // This could be left alone if bigger than the data buffer;
      // however, deleting for now...
      //***
      ossim_uint32 source_size = data->getDataSizeInBytes();
      ossim_uint32 newSize     = getDataSizeInBytes();
      if(!theDataBuffer)
      {
         theDataBuffer = new ossim_uint8[source_size];
      }
      else if(savedSize != newSize)
      {
         if(theDataBuffer)
         {
            delete [] theDataBuffer;
            theDataBuffer = NULL;
            theDataBuffer = new ossim_uint8[newSize];
         }
      }

      // Copy the image data...
      memcpy(theDataBuffer, data->theDataBuffer, source_size);
   }
   if(!theDataBuffer) setDataObjectStatus(OSSIM_NULL);
}

void ossimRectilinearDataObject::initialize()
{
   deInitialize();

   ossim_uint32 size = getDataSizeInBytes();
   if(size)
   {
      theDataBuffer = new ossim_uint8[size];
   }   
}

void ossimRectilinearDataObject::deInitialize()
{
   if(theDataBuffer)
   {
      delete [] theDataBuffer;
      theDataBuffer = NULL;
      setDataObjectStatus(OSSIM_NULL);
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
