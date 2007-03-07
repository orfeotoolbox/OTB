//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License 
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author: Garrett Potts (gpotts@imagelinks)
// Description:
//
//*************************************************************************
// $Id: ossimShapeFile.h,v 1.12 2004/04/08 13:49:55 gpotts Exp $

#ifndef ossimShapeFile_HEADER
#define ossimShapeFile_HEADER
#include <iostream>
#include <base/common/ossimConstants.h>
#include <base/data_types/ossimFilename.h>
#include <base/data_types/ossimString.h>
#include <base/data_types/ossimDrect.h>

#include "shapefil.h"

class ossimShapeFile;
class ossimAnnotationObject;

class OSSIMDLLEXPORT ossimShapeObject
{
public:
   friend ostream& operator<<(ostream& out,
                              const ossimShapeObject& rhs);
   
   ossimShapeObject()
      :theShape((SHPObject*)NULL),
       theIndex(-1)
      {
      }

   ~ossimShapeObject()
      {
         if(theShape)
         {
            SHPDestroyObject(theShape);
            theShape = NULL;
         }
      }
   void setShape(SHPObject* obj);
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
            maxX = maxY = maxZ = maxM = OSSIM_DBL_NAN;
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
            maxX = maxY = OSSIM_DBL_NAN;
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
         return SHPT_NULL;
      }
   ossimString getTypeByName()const
      {
         if(theShape)
         {
            return ossimString(SHPTypeName(theShape->nSHPType));
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
         case SHPP_TRISTRIP:
         {
            ossimString("tristrip");
            break;
         }
         case SHPP_TRIFAN:
         {
            ossimString("trifan");
            break;
         }
         case SHPP_OUTERRING:
         {
            ossimString("outerring");
            break;
         }
         case SHPP_INNERRING:
         {
            ossimString("innerring");
            break;
         }
         case SHPP_RING:
         {
            ossimString("ring");
            break;
         }
         }
         return ossimString("unknown");
      }
   SHPObject* getShapeObject()
      {
         return theShape;
      }
   const SHPObject* getShapeObject()const
      {
         return theShape;
      }

   
protected:
   SHPObject* theShape;
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
   virtual SHPHandle getHandle()
      {
         return theHandle;
      }

   virtual const SHPHandle getHandle()const
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
            maxX = maxY = maxZ = maxM = OSSIM_DBL_NAN;
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
            minX = minY = maxX = maxY = OSSIM_DBL_NAN;
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
   SHPHandle	 theHandle;
   ossimFilename theFilename;

};

#endif
