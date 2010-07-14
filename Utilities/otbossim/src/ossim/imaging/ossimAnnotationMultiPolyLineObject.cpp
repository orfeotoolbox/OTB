//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimAnnotationMultiPolyLineObject.cpp 17195 2010-04-23 17:32:18Z dburken $
#include <ossim/imaging/ossimAnnotationMultiPolyLineObject.h>
#include <ossim/imaging/ossimAnnotationMultiLineObject.h>
#include <ossim/imaging/ossimRgbImage.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimLine.h>

RTTI_DEF1(ossimAnnotationMultiPolyLineObject, "ossimAnnotationMultiPolyLineObject", ossimAnnotationObject)

ossimAnnotationMultiPolyLineObject::ossimAnnotationMultiPolyLineObject()
   :ossimAnnotationObject()
{
   computeBoundingRect();
}

ossimAnnotationMultiPolyLineObject::ossimAnnotationMultiPolyLineObject(const vector<ossimPolyLine>& multiPoly,
								       unsigned char r,
								       unsigned char g,
								       unsigned char b,
								       long thickness)
   :ossimAnnotationObject(r, g, b, thickness)
{
   theMultiPolyLine = multiPoly;
   computeBoundingRect();
}

ossimAnnotationMultiPolyLineObject::~ossimAnnotationMultiPolyLineObject()
{
}

void ossimAnnotationMultiPolyLineObject::applyScale(double x,
                                                double y)
{
   for(ossim_uint32 i =0; i<theMultiPolyLine.size(); ++i)
   {
      theMultiPolyLine[i] *= ossimDpt(x, y);
   }
   computeBoundingRect();
   // theBoundingRect *= ossimDpt(x,y);
}

bool ossimAnnotationMultiPolyLineObject::intersects(const ossimDrect& rect)const
{
   // do the quick checks first
   //
   if(rect.hasNans()) return false;
   if(!rect.intersects(theBoundingRect)) return false;
   if(theMultiPolyLine.size()<1) return false;
   
   for(ossim_uint32 i =0; i < theMultiPolyLine.size(); ++i)
   {
      vector<ossimPolyLine> result;
      
      if(theMultiPolyLine[i].clipToRect(result, rect))
      {
         return true;
      }
   }

   return false;
}

ossimAnnotationObject* ossimAnnotationMultiPolyLineObject::getNewClippedObject(
   const ossimDrect& /* rect */)const
{
   ossimAnnotationObject* result = (ossimAnnotationObject*)NULL;
   
   ossimNotify(ossimNotifyLevel_WARN)
      << "ossimAnnotationMultiPolyLineObject::getNewClippedObject\n"
      << "Not implemented for " << getClassName() << endl;
   return result;
}

void ossimAnnotationMultiPolyLineObject::draw(ossimRgbImage& anImage)const
{
   if(theMultiPolyLine.size()<1) return;
   if(theBoundingRect.hasNans()) return;
   
   
   anImage.setDrawColor(theRed, theGreen, theBlue);
   anImage.setThickness(theThickness);
   ossimDrect imageRect = anImage.getImageData()->getImageRectangle();
   
   if(theBoundingRect.intersects(imageRect))
   {
      // we need to extend it by a couple of pixels since
      // if a pixel lies on the edge and then another pixel is just off
      // the edge we will get a stair step and so for several pixels
      // the line might be inside the image rectangle but the clip
      // algorithm will only draw 1 pixel since it thinks the first
      // point is inside and the second point is outside and will
      // execute the clip algorithm keeping only the first
      // point.
      ossimDrect clipRect(imageRect.ul().x - 10,
                          imageRect.ul().y - 10,
                          imageRect.lr().x + 10,
                          imageRect.lr().y + 10);
      
      int j = 0;
      for(ossim_uint32 i = 0; i < theMultiPolyLine.size(); ++i)
      {
         const ossimPolyLine& poly = theMultiPolyLine[i];
         int vertexCount            = poly.getNumberOfVertices();

	 if(vertexCount > 1)
	   {

	     ossimDpt start, end;
	     start = poly[0];
	     end   = poly[1];
	     j = 1;
	     do
	       {
		 if(clipRect.clip(start, end))
		   {            
		     anImage.drawLine(ossimIpt(start),
				      ossimIpt(end));
		   }
		 ++j;
		 start = poly[j-1];
		 end   = poly[j];
	       }while(j < vertexCount);
	   }
	 else if(vertexCount == 1)
	   {
		     anImage.drawLine(ossimIpt(poly[0]),
				      ossimIpt(poly[0]));
	   }
      }
   }
}

std::ostream& ossimAnnotationMultiPolyLineObject::print(std::ostream& out)const
{
   ossimAnnotationObject::print(out);
   out << endl;
   out << setw(15)<<setiosflags(ios::left)<<"type:"<<getClassName() << endl
       << setw(15)<<setiosflags(ios::left)<<"polylines:"<<theMultiPolyLine.size()<<endl;
   
   for(ossim_uint32 i = 0; i < theMultiPolyLine.size(); ++i)
   {
      out << "____________________________________________________"<<endl
          << theMultiPolyLine[i] << endl;
   }
   out << "____________________________________________________"<<endl;
   return out;
}

void ossimAnnotationMultiPolyLineObject::getBoundingRect(ossimDrect& rect)const
{   
   rect = theBoundingRect;
}

void ossimAnnotationMultiPolyLineObject::computeBoundingRect()
{
   if(theMultiPolyLine.size())
   {
      ossimDrect rect(theMultiPolyLine[0].getVertexList());
      
      for(ossim_uint32 i = 1; i < theMultiPolyLine.size(); ++i)
      {
         ossimDrect rect2(theMultiPolyLine[i].getVertexList());

         if(rect.hasNans())
         {
            rect = rect2;
         }
         else
         {
            if(!rect2.hasNans())
            {
               rect = rect.combine(rect2);
            }
         }
      }
      
      theBoundingRect = rect;
   }
   else
   {
      theBoundingRect.makeNan();
   }
   if(!theBoundingRect.hasNans())
   {
      ossimIpt origin = theBoundingRect.ul();
      theBoundingRect = ossimDrect(origin.x - theThickness/2,
                                   origin.y - theThickness/2,
                                   origin.x + (theBoundingRect.width()-1) + theThickness/2,
                                   origin.y + (theBoundingRect.height()-1) + theThickness/2);
   }
}

bool ossimAnnotationMultiPolyLineObject::isPointWithin(const ossimDpt& imagePoint)const
{
//    for(ossim_uint32 i = 0; i < theMultiPolyLine.size(); ++i)
//    {
//       if(theMultiPolyLine[i].isPointWithin(imagePoint))
//       {
//          return true;
//       }
//   }

  return theBoundingRect.pointWithin(imagePoint);
}

ossimAnnotationMultiPolyLineObject::ossimAnnotationMultiPolyLineObject(const ossimAnnotationMultiPolyLineObject& rhs)
      : ossimAnnotationObject(rhs),
        theMultiPolyLine(rhs.theMultiPolyLine),
        theBoundingRect(rhs.theBoundingRect)
{
}

ossimObject* ossimAnnotationMultiPolyLineObject::dup()const
{
   return new ossimAnnotationMultiPolyLineObject(*this);
}

void ossimAnnotationMultiPolyLineObject::addPolyLine(const ossimPolyLine& poly)
{
   theMultiPolyLine.push_back(poly);
}

void ossimAnnotationMultiPolyLineObject::addPoint(ossim_uint32 polygonIndex,
                                                  const ossimDpt& pt)
{
   if(polygonIndex < theMultiPolyLine.size())
   {
      theMultiPolyLine[polygonIndex].addPoint(pt);
   }
}

void ossimAnnotationMultiPolyLineObject::setMultiPolyLine(const vector<ossimPolyLine>& multiPoly)
{
   theMultiPolyLine = multiPoly;
   computeBoundingRect();
}

const std::vector<ossimPolyLine>& ossimAnnotationMultiPolyLineObject::getMultiPolyLine()const
{
   return theMultiPolyLine;
}

std::vector<ossimPolyLine>& ossimAnnotationMultiPolyLineObject::getMultiPolyLine()
{
   return theMultiPolyLine;
}

