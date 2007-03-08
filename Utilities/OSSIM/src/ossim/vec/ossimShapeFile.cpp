//*******************************************************************
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
// 
// Description: This class extends the stl's string class.
//
//********************************************************************
// $Id: ossimShapeFile.cpp 9963 2006-11-28 21:11:01Z gpotts $
//

#include <iomanip>
#include <ossim/vec/ossimShapeFile.h>



std::ostream& operator<<(std::ostream& out,
                    const ossimShapeObject& rhs)
{
   if(rhs.isLoaded())
   {
      double minx, miny, minz, minm;
      double maxx, maxy, maxz, maxm;
      ossim_uint32 i = 0;
      
      rhs.getBounds(minx, miny, minz, minm,
                    maxx, maxy, maxz, maxm);
      
      out << std::setw(15) << setiosflags(std::ios::left)<<"type:"<<rhs.getTypeByName() << std::endl
          << std::setw(15) << setiosflags(std::ios::left)<<"id:"<<rhs.getId()<<std::endl
          << std::setw(15) << setiosflags(std::ios::left)<<"minx:"<<minx <<std::endl
          << std::setw(15) << setiosflags(std::ios::left)<<"miny:"<<miny <<std::endl
          << std::setw(15) << setiosflags(std::ios::left)<<"minz:"<<minz <<std::endl
          << std::setw(15) << setiosflags(std::ios::left)<<"minm:"<<minm <<std::endl
          << std::setw(15) << setiosflags(std::ios::left)<<"maxx:"<<maxx <<std::endl
          << std::setw(15) << setiosflags(std::ios::left)<<"maxy:"<<maxy <<std::endl
          << std::setw(15) << setiosflags(std::ios::left)<<"maxz:"<<maxz <<std::endl
          << std::setw(15) << setiosflags(std::ios::left)<<"maxm:"<<maxm <<std::endl
          << std::setw(15) << setiosflags(std::ios::left)<<"parts:"<<rhs.getNumberOfParts()<<std::endl
          << std::setw(15) << setiosflags(std::ios::left)<<"vertices:"<<rhs.getNumberOfVertices();

      if(rhs.getNumberOfParts())
      {
         out << std::endl;
         for(i = 0; i < rhs.getNumberOfParts()-1; ++i)
         {
            ossimString s1 = "part start ";
            ossimString s2 = "part type ";
            s1 += (ossimString::toString(i+1)+":");
            s2 += (ossimString::toString(i+1)+":");
            
            out << std::setw(15) << setiosflags(std::ios::left) << s1.c_str() << rhs.theShape->panPartStart[i]<<std::endl;
            out << std::setw(15) << setiosflags(std::ios::left) << s2.c_str() <<  SHPPartTypeName(rhs.theShape->panPartType[i])<<std::endl;
         }
         ossimString s1 = "part start ";
         ossimString s2 = "part type ";
         
         s1 += (ossimString::toString(rhs.getNumberOfParts())+":");
         out << std::setw(15) << setiosflags(std::ios::left) << s1.c_str() <<  rhs.theShape->panPartStart[i]<<std::endl;
         out << std::setw(15) << setiosflags(std::ios::left) << s2.c_str() <<  SHPPartTypeName(rhs.theShape->panPartType[i]);
      }

      out << std::setw(0);
      
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
      out << std::setw(15) << setiosflags(std::ios::left)<<"Shp filename:" << theFilename << std::endl;
      out << std::setw(15) << setiosflags(std::ios::left)<<"Record count:" << theHandle->nRecords << std::endl;
      out << std::setw(15) << setiosflags(std::ios::left)<<"File type:" << getShapeTypeString().c_str() << std::endl;
      out << std::setw(15) << setiosflags(std::ios::left)<<"minx:" << theHandle->adBoundsMin[0] << std::endl;
      out << std::setw(15) << setiosflags(std::ios::left)<<"miny:" << theHandle->adBoundsMin[1] << std::endl;
      out << std::setw(15) << setiosflags(std::ios::left)<<"minz:" << theHandle->adBoundsMin[2] << std::endl;
      out << std::setw(15) << setiosflags(std::ios::left)<<"minm:" << theHandle->adBoundsMin[3] << std::endl;
      out << std::setw(15) << setiosflags(std::ios::left)<<"maxx:" << theHandle->adBoundsMax[0] << std::endl;
      out << std::setw(15) << setiosflags(std::ios::left)<<"maxy:" << theHandle->adBoundsMax[1] << std::endl;
      out << std::setw(15) << setiosflags(std::ios::left)<<"maxz:" << theHandle->adBoundsMax[2] << std::endl;
      out << std::setw(15) << setiosflags(std::ios::left)<<"maxm:" << theHandle->adBoundsMax[3] << std::endl;

      ossimShapeObject shape;

      if(theHandle->nRecords)
      {
         out << std::setw(30) << std::setfill('_') << "" << std::setfill(' ')<<std::endl;
      }
      for(int i=0; i < theHandle->nRecords; ++i)
      {
         if(shape.loadShape(*this, i))
         {
            out << shape << std::endl;
            out << std::setw(30) << std::setfill('_') << ""<<std::setfill(' ')<<std::endl;
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
