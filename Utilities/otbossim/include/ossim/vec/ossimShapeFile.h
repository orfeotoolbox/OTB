//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimShapeFile.h 11424 2007-07-27 17:01:23Z dburken $

#ifndef ossimShapeFile_HEADER
#define ossimShapeFile_HEADER
#include <iostream>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimDrect.h>

#include <ossim/vec/shapefil.h>

class ossimShapeFile;
class ossimAnnotationObject;

class OSSIMDLLEXPORT ossimShapeObject
{
public:
   friend std::ostream& operator<<(std::ostream& out,
                              const ossimShapeObject& rhs);
   
   ossimShapeObject()
      :theShape((ossim_SHPObject*)NULL),
       theIndex(-1)
      {
      }

   ~ossimShapeObject()
      {
         if(theShape)
         {
            ossim_SHPDestroyObject(theShape);
            theShape = NULL;
         }
      }
   void setShape(ossim_SHPObject* obj);
   bool isLoaded()const
      {
         return (theShape!=NULL);
      }
   long getIndex()const
      {
         return theIndex;
      }
   long getId()const
      {
         if(theShape)
         {
            return theShape->nShapeId;
         }
         
         return -1;
      }
   
   bool loadShape(const ossimShapeFile& shapeFile,
                  long shapeRecord);
   
   ossim_uint32 getNumberOfParts()const
      {
         if(theShape)
         {
            return theShape->nParts;
         }

         return 0;
      }
   ossim_uint32 getNumberOfVertices()const
      {
         if(theShape)
         {
            return theShape->nVertices;
         }
         
         return 0;
      }

   void getBounds(double& minX, double& minY, double& minZ, double& minM,
                  double& maxX, double& maxY, double& maxZ, double& maxM)const
      {
         if(theShape)
         {
            minX = theShape->dfXMin;
            minY = theShape->dfYMin;
            minZ = theShape->dfZMin;
            minM = theShape->dfMMin;
            maxX = theShape->dfXMax;
            maxY = theShape->dfYMax;
            maxZ = theShape->dfZMax;
            maxM = theShape->dfMMax;
         }
         else
         {
            minX = minY = minZ = minM =
            maxX = maxY = maxZ = maxM = ossim::nan();
         }
      }

   void getBounds(double& minX, double& minY,
                  double& maxX, double& maxY)const
      {
         if(theShape)
         {
            minX = theShape->dfXMin;
            minY = theShape->dfYMin;
            maxX = theShape->dfXMax;
            maxY = theShape->dfYMax;
         }
         else
         {
            minX = minY = 
            maxX = maxY = ossim::nan();
         }
      }

   void getBoundingRect(ossimDrect& result,
                        ossimCoordSysOrientMode orient = OSSIM_RIGHT_HANDED)const;

   ossimDrect getBoundingRect(ossimCoordSysOrientMode orient = OSSIM_RIGHT_HANDED)const
      {
         ossimDrect result;
         
         getBoundingRect(result, orient);

         return result;
      }

   int getType()const
      {
         if(theShape)
         {
            return theShape->nSHPType;
         }
         return ossim_SHPT_NULL;
      }
   ossimString getTypeByName()const
      {
         if(theShape)
         {
            return ossimString(ossim_SHPTypeName(theShape->nSHPType));
         }
         return "unknown";
      }
   int getPartType(ossim_uint32 partIndex)const
      {
         if((partIndex > getNumberOfParts())||
            (!theShape))
         {
            return -1;
         }

         return theShape->panPartType[partIndex];
      }
   ossimString getPartByName(ossim_uint32 partIndex)const
      {
         if((partIndex > getNumberOfParts())||
            (!theShape))
         {
         }
         switch(theShape->panPartType[partIndex])
         {
         case ossim_SHPP_TRISTRIP:
         {
            ossimString("tristrip");
            break;
         }
         case ossim_SHPP_TRIFAN:
         {
            ossimString("trifan");
            break;
         }
         case ossim_SHPP_OUTERRING:
         {
            ossimString("outerring");
            break;
         }
         case ossim_SHPP_INNERRING:
         {
            ossimString("innerring");
            break;
         }
         case ossim_SHPP_RING:
         {
            ossimString("ring");
            break;
         }
         }
         return ossimString("unknown");
      }
   ossim_SHPObject* getShapeObject()
      {
         return theShape;
      }
   const ossim_SHPObject* getShapeObject()const
      {
         return theShape;
      }

   
protected:
   ossim_SHPObject* theShape;
   long       theIndex;
};

class OSSIMDLLEXPORT ossimShapeFile
{
public:
   friend std::ostream& operator <<(std::ostream& out, const ossimShapeFile& rhs)
      {
         rhs.print(out);
         
         return out;
      }
   
   ossimShapeFile();
   virtual ~ossimShapeFile();
   
   virtual bool open(const ossimFilename& file,
                     const ossimString& flags=ossimString("rb"));
   
   virtual void close();
   bool isOpen()const
      {
         return (theHandle!=NULL);
      }
   virtual ossim_SHPHandle getHandle()
      {
         return theHandle;
      }

   virtual const ossim_SHPHandle getHandle()const
      {
         return theHandle;
      }
   
   virtual void print(std::ostream& out)const;
   virtual ossimString getShapeTypeString()const;

   
   virtual long getNumberOfShapes()const
      {
         if(theHandle)
         {
            return theHandle->nRecords;
         }
         return 0;
      }
   void getBounds(double& minX, double& minY, double& minZ, double& minM,
                  double& maxX, double& maxY, double& maxZ, double& maxM)const
      {
         if(theHandle)
         {
            minX = theHandle->adBoundsMin[0];
            minY = theHandle->adBoundsMin[1];
            minZ = theHandle->adBoundsMin[2];
            minM = theHandle->adBoundsMin[3];
            maxX = theHandle->adBoundsMax[0];
            maxY = theHandle->adBoundsMax[1];
            maxZ = theHandle->adBoundsMax[2];
            maxM = theHandle->adBoundsMax[3];
         }
         else
         {
            minX = minY = minZ = minM =
            maxX = maxY = maxZ = maxM = ossim::nan();
         }
      }
   
   void getBounds(double& minX, double& minY,
                  double& maxX, double& maxY)const
      {
         if(theHandle)
         {
            minX = theHandle->adBoundsMin[0];
            minY = theHandle->adBoundsMin[1];
            maxX = theHandle->adBoundsMax[0];
            maxY = theHandle->adBoundsMax[1];
         }
         else
         {
            minX = minY = maxX = maxY = ossim::nan();
         }
      }

   void getBoundingRect(ossimDrect& result,
                        ossimCoordSysOrientMode orient = OSSIM_RIGHT_HANDED)const;
   
   ossimDrect getBoundingRect(ossimCoordSysOrientMode orient = OSSIM_RIGHT_HANDED)const
      {
         ossimDrect result;
         
         getBoundingRect(result, orient);
         
         return result;
      }
   const ossimFilename& getFilename()const
      {
         return theFilename;
      }
protected:
   ossim_SHPHandle	 theHandle;
   ossimFilename theFilename;

};

#endif
