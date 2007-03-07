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
// $Id: ossimShapeFile.cpp,v 1.7 2004/04/08 13:49:55 gpotts Exp $
//
#include <vec/shape/ossimShapeFile.h>



ostream& operator<<(ostream& out,
                    const ossimShapeObject& rhs)
{
   if(rhs.isLoaded())
   {
      double minx, miny, minz, minm;
      double maxx, maxy, maxz, maxm;
      ossim_uint32 i = 0;
      
      rhs.getBounds(minx, miny, minz, minm,
                    maxx, maxy, maxz, maxm);
      
      out << setw(15) << setiosflags(ios::left)<<"type:"<<rhs.getTypeByName() << endl
          << setw(15) << setiosflags(ios::left)<<"id:"<<rhs.getId()<<endl
          << setw(15) << setiosflags(ios::left)<<"minx:"<<minx <<endl
          << setw(15) << setiosflags(ios::left)<<"miny:"<<miny <<endl
          << setw(15) << setiosflags(ios::left)<<"minz:"<<minz <<endl
          << setw(15) << setiosflags(ios::left)<<"minm:"<<minm <<endl
          << setw(15) << setiosflags(ios::left)<<"maxx:"<<maxx <<endl
          << setw(15) << setiosflags(ios::left)<<"maxy:"<<maxy <<endl
          << setw(15) << setiosflags(ios::left)<<"maxz:"<<maxz <<endl
          << setw(15) << setiosflags(ios::left)<<"maxm:"<<maxm <<endl
          << setw(15) << setiosflags(ios::left)<<"parts:"<<rhs.getNumberOfParts()<<endl
          << setw(15) << setiosflags(ios::left)<<"vertices:"<<rhs.getNumberOfVertices();

      if(rhs.getNumberOfParts())
      {
         out << endl;
         for(i = 0; i < rhs.getNumberOfParts()-1; ++i)
         {
            ossimString s1 = "part start ";
            ossimString s2 = "part type ";
            s1 += (ossimString::toString(i+1)+":");
            s2 += (ossimString::toString(i+1)+":");
            
            out << setw(15) << setiosflags(ios::left) << s1.c_str() << rhs.theShape->panPartStart[i]<<endl;
            out << setw(15) << setiosflags(ios::left) << s2.c_str() <<  SHPPartTypeName(rhs.theShape->panPartType[i])<<endl;
         }
         ossimString s1 = "part start ";
         ossimString s2 = "part type ";
         
         s1 += (ossimString::toString(rhs.getNumberOfParts())+":");
         out << setw(15) << setiosflags(ios::left) << s1.c_str() <<  rhs.theShape->panPartStart[i]<<endl;
         out << setw(15) << setiosflags(ios::left) << s2.c_str() <<  SHPPartTypeName(rhs.theShape->panPartType[i]);
      }

      out << setw(0);
      
   }
   return out;   
}

void ossimShapeObject::getBoundingRect(ossimDrect& result,
                                       ossimCoordSysOrientMode orient)const
{
   double minx, miny, maxx, maxy;

   if(theShape)
   {
      getBounds(minx, miny, maxx, maxy);
      
      if(orient == OSSIM_RIGHT_HANDED)
      {
         result = ossimDrect(minx, maxy, maxx, miny, orient);
      }
      else
      {
         result = ossimDrect(minx, miny, maxx, maxy, orient);         
      }
   }
   else
   {
      result = ossimDrect(0,0,0,0,orient);
      result.makeNan();
   }
}

void ossimShapeObject::setShape(SHPObject* obj)
{
   if(theShape)
   {
      SHPDestroyObject(theShape);
      theShape = NULL;            
   }
   
   theShape = obj;
}

bool ossimShapeObject::loadShape(const ossimShapeFile& shapeFile,
                                 long shapeRecord)
{
   if(theShape)
   {
      SHPDestroyObject(theShape);
      theShape = NULL;            
   }
   if(shapeFile.isOpen())
   {
      theShape = SHPReadObject(shapeFile.getHandle(),
                               shapeRecord);
      theIndex = shapeRecord;
   }
   else
   {
      theIndex = -1;
   }
   return (theShape != (SHPObject*)NULL);
}


ossimShapeFile::ossimShapeFile()
   :theHandle(NULL)
{
}

ossimShapeFile::~ossimShapeFile()
{
   close();
}

void ossimShapeFile::close()
{
   if(theHandle)
   {
      SHPClose(theHandle);
      theHandle = NULL;
   }
}

bool ossimShapeFile::open(const ossimFilename& file,
                          const ossimString& flags)
{
   if(isOpen()) close();
   
   theHandle = SHPOpen( file.c_str(),
                        flags.c_str());
   if(isOpen())
   {
      theFilename = file;
   }
   
   return (theHandle != NULL);
}



void ossimShapeFile::print(std::ostream& out)const
{
   if(isOpen())
   {
      out << setw(15) << setiosflags(ios::left)<<"Shp filename:" << theFilename << endl;
      out << setw(15) << setiosflags(ios::left)<<"Record count:" << theHandle->nRecords << endl;
      out << setw(15) << setiosflags(ios::left)<<"File type:" << getShapeTypeString().c_str() << endl;
      out << setw(15) << setiosflags(ios::left)<<"minx:" << theHandle->adBoundsMin[0] << endl;
      out << setw(15) << setiosflags(ios::left)<<"miny:" << theHandle->adBoundsMin[1] << endl;
      out << setw(15) << setiosflags(ios::left)<<"minz:" << theHandle->adBoundsMin[2] << endl;
      out << setw(15) << setiosflags(ios::left)<<"minm:" << theHandle->adBoundsMin[3] << endl;
      out << setw(15) << setiosflags(ios::left)<<"maxx:" << theHandle->adBoundsMax[0] << endl;
      out << setw(15) << setiosflags(ios::left)<<"maxy:" << theHandle->adBoundsMax[1] << endl;
      out << setw(15) << setiosflags(ios::left)<<"maxz:" << theHandle->adBoundsMax[2] << endl;
      out << setw(15) << setiosflags(ios::left)<<"maxm:" << theHandle->adBoundsMax[3] << endl;

      ossimShapeObject shape;

      if(theHandle->nRecords)
      {
         out << setw(30) << setfill('_') << "" << setfill(' ')<<endl;
      }
      for(int i=0; i < theHandle->nRecords; ++i)
      {
         if(shape.loadShape(*this, i))
         {
            out << shape << endl;
            out << setw(30) << setfill('_') << ""<<setfill(' ')<<endl;
         }
      }
   }
}


ossimString ossimShapeFile::getShapeTypeString()const
{
   if(theHandle)
   {
      return SHPTypeName(theHandle->nShapeType);
   }
   return "";
}

void ossimShapeFile::getBoundingRect(ossimDrect& result,
                                     ossimCoordSysOrientMode orient)const
{
   double minx, miny, maxx, maxy;
   
   getBounds(minx, miny, maxx, maxy);
   if(orient == OSSIM_RIGHT_HANDED)
   {
      result = ossimDrect(minx, maxy, maxx, miny, orient);
   }
   else
   {
      result = ossimDrect(minx, miny, maxx, maxy, orient);         
   }
}
