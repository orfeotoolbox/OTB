//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimRectilinearDataObject.h 19931 2011-08-10 11:53:25Z gpotts $
#ifndef ossimRectilinearDataObject_HEADER
#define ossimRectilinearDataObject_HEADER
#include <ossim/base/ossimDataObject.h>

class OSSIMDLLEXPORT ossimRectilinearDataObject : public ossimDataObject
{
public:

   /** default constructor */
   ossimRectilinearDataObject();

   ossimRectilinearDataObject(const ossimRectilinearDataObject&rhs);
      
   ossimRectilinearDataObject(ossim_uint32 numberOfSpatialComponents,
                              ossimSource* owner,
                              ossim_uint32 numberOfDataComponents,
                              ossimScalarType scalarType=OSSIM_SCALAR_UNKNOWN,
                              ossimDataObjectStatus status=OSSIM_NULL);
   
   /**
    * This is a helper constructor that allows one to instantiate a one
    * dimensional Spatial component with N number of data components.
    * It will internally allocate theSpatialExtent to 1 and set the
    * contents equal to the value passed in to length.
    */
   ossimRectilinearDataObject(ossimSource* owner,
                              ossim_uint32 numberOfDataComponents,
                              ossim_uint32 length,
                              ossimScalarType scalarType=OSSIM_SCALAR_UNKNOWN,
                              ossimDataObjectStatus status=OSSIM_NULL);

   /**
    * This is a helper constructor that allows one to instantiate a two
    * dimensional Spatial component (WidthxHeight) with N number of
    * data components.  It will internally allocate theSpatialExtent
    * to 2 and set the contents equal to the value passed in to width, and
    * height.
    */
   ossimRectilinearDataObject(ossimSource* owner,
                              ossim_uint32 numberOfDataComponents,
                              ossim_uint32 width,
                              ossim_uint32 height,
                              ossimScalarType scalarType=OSSIM_SCALAR_UNKNOWN,
                              ossimDataObjectStatus status=OSSIM_NULL);

   /**
    * This is a helper constructor that allows one to instantiate a two
    * dimensional Spatial component (WidthxHeightxDepth) with N number of
    * data components.  It will internally allocate theSpatialExtent
    * to 3 and set the contents equal to the value passed in to width,
    * height, and depth.
    */
   ossimRectilinearDataObject(ossimSource* owner,
                              ossim_uint32 numberOfDataComponents,
                              ossim_uint32 width,
                              ossim_uint32 height,
                              ossim_uint32 depth,
                              ossimScalarType   scalarType=OSSIM_SCALAR_UNKNOWN,
                              ossimDataObjectStatus status=OSSIM_NULL);
   

   virtual ~ossimRectilinearDataObject();
   /**
    * How many components make up this data object.  For
    * example:  If this were an RGB image data object then
    * the number of components would be set to 3 and the
    * RGB would be seen as a single data object being
    * passed through.
    */
   virtual void setNumberOfDataComponents(ossim_uint32 n);
   
   virtual void setSpatialExtents(ossim_uint32* extents, ossim_uint32  size);

   /**
    *  See ossimScalarType in ossimConstants for a full list
    *
    *  OSSIM_SCALAR_UNKNOWN
    *  OSSIM_UCHAR          Unsigned char
    *  OSSIM_USHORT16       16bit unsigned short
    *  OSSIM_SSHORT16       16bit signed short
    *  OSSIM_USHORT11       11bit unsigned short
    *  OSSIM_FLOAT          float
    *  OSSIM_NORMALIZED_DOUBLE normalized 0 to 1 data
    */
   virtual void setScalarType(ossimScalarType type);

   virtual ossim_uint32 getNumberOfDataComponents() const;

   virtual ossim_uint32 getNumberOfSpatialComponents() const;

   virtual const ossim_uint32* getSpatialExtents() const;

   virtual ossimScalarType getScalarType() const;

   virtual ossim_uint32 getScalarSizeInBytes()const;

   virtual void*   getBuf();

   virtual const void* getBuf() const;

   virtual void assign(const ossimRectilinearDataObject* data);

   /**
    *  Calls deInitialize() which deletes theDataBuffer and sets object status
    *  to OSSIM_NULL, then initializes theDataBuffer.
    *
    *  @see deInitialize
    *  
    *  @note This method is destructive in that if the buffer existed of
    *  the correct size it will delete it, then reallocate.
    */
   virtual void initialize();

   virtual ossim_uint32 computeSpatialProduct()const;

   virtual ossim_uint32 getDataSizeInBytes()const;

   virtual std::ostream& print(std::ostream& out) const;

   /**
    * @brief assignment operator=
    * @param rhs The data to assign from.
    * @param A reference to this object.
    */
   virtual const ossimRectilinearDataObject& operator=(
      const ossimRectilinearDataObject& rhs);

   virtual bool saveState(ossimKeywordlist& kwl, const char* prefix=0)const;
   virtual bool loadState(const ossimKeywordlist& kwl, const char* prefix=0);

protected:
   ossim_uint32              m_numberOfDataComponents;
   ossimScalarType           m_scalarType;
   std::vector<ossim_uint8>  m_dataBuffer;
   std::vector<ossim_uint32> m_spatialExtents;
   
TYPE_DATA
};

#endif
