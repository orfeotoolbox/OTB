//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimAnnotationMultiLineObject.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimAnnotationMultiLineObject_HEADER
#define ossimAnnotationMultiLineObject_HEADER
#include <ossim/imaging/ossimAnnotationObject.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimPolyLine.h>

class OSSIMDLLEXPORT ossimAnnotationMultiLineObject : public ossimAnnotationObject
{
public:
   ossimAnnotationMultiLineObject();
   ossimAnnotationMultiLineObject(const std::vector<ossimPolyLine>& lineList,
                                  unsigned char r=255,
                                  unsigned char g=255,
                                  unsigned char b=255,
                                  long thickness=1)
      :ossimAnnotationObject(r, g, b, thickness),
       thePolyLineList(lineList)
      {
         computeBoundingRect();
      }

   ossimAnnotationMultiLineObject(const ossimPolyLine& lineList,
                                  unsigned char r=255,
                                  unsigned char g=255,
                                  unsigned char b=255,
                                  long thickness=1)
      :ossimAnnotationObject(r, g, b, thickness)
      {
         thePolyLineList.push_back(lineList);
         computeBoundingRect();
      }
   

   ossimAnnotationMultiLineObject(const ossimAnnotationMultiLineObject& rhs)
      :ossimAnnotationObject(rhs),
       thePolyLineList(rhs.thePolyLineList),
       theBoundingRect(rhs.theBoundingRect)
      {
      }
       
   virtual ossimObject* dup()const
      {
         return new ossimAnnotationMultiLineObject(*this);
      }
   
   virtual void applyScale(double x,
                           double y);
   virtual ossimAnnotationObject* getNewClippedObject(const ossimDrect& rect)const;
   virtual bool intersects(const ossimDrect& rect)const;   
   virtual void draw(ossimRgbImage& anImage)const;
   virtual std::ostream& print(std::ostream& out)const;
   virtual void getBoundingRect(ossimDrect& rect)const
      {
         rect = theBoundingRect;
      }
   virtual bool isPointWithin(const ossimDpt& imagePoint)const;
   virtual void computeBoundingRect();

   virtual const vector<ossimPolyLine>& getPolyLineList()const
      {
         return thePolyLineList;
      }

   virtual vector<ossimPolyLine>& getPolyLineList()
      {
         return thePolyLineList;
      }
   virtual void setPolyLine(const ossimPolyLine& line)
      {
         thePolyLineList.clear();
         thePolyLineList.push_back(line);
      }
protected:
   vector<ossimPolyLine> thePolyLineList;
   ossimDrect            theBoundingRect;

TYPE_DATA
};

#endif
