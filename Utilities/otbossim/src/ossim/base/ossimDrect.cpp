//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description:
//
// Contains class definition for ossimDrect.
//*******************************************************************
//  $Id: ossimDrect.cpp 9094 2006-06-13 19:12:40Z dburken $

#include <iostream>

#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimPolygon.h>

#define d_MIN(a,b)      (((a)<(b)) ? a : b)
#define d_MAX(a,b)      (((a)>(b)) ? a : b)

static int
clip_1d (double *x0, 
	 double *y0, 
	 double *x1, 
	 double *y1, 
	 double maxdim)
{
  double m;			/* gradient of line */
  if (*x0 < 0)
    {				/* start of line is left of window */
      if (*x1 < 0)		/* as is the end, so the line never cuts the window */
	return 0;
      m = (*y1 - *y0) / (double) (*x1 - *x0);	/* calculate the slope of the line */
      /* adjust x0 to be on the left boundary (ie to be zero), and y0 to match */
      *y0 -= m * *x0;
      *x0 = 0;
      /* now, perhaps, adjust the far end of the line as well */
      if (*x1 > maxdim)
	{
	  *y1 += m * (maxdim - *x1);
	  *x1 = maxdim;
	}
      return 1;
    }
  if (*x0 > maxdim)
    {				/* start of line is right of window -
				   complement of above */
      if (*x1 > maxdim)		/* as is the end, so the line misses the window */
	return 0;
      m = (*y1 - *y0) / (double) (*x1 - *x0);	/* calculate the slope of the line */
      *y0 += m * (maxdim - *x0);	/* adjust so point is on the right
					   boundary */
      *x0 = maxdim;
      /* now, perhaps, adjust the end of the line */
      if (*x1 < 0)
	{
	  *y1 -= m * *x1;
	  *x1 = 0;
	}
      return 1;
    }
  /* the final case - the start of the line is inside the window */
  if (*x1 > maxdim)
    {				/* other end is outside to the right */
      m = (*y1 - *y0) / (double) (*x1 - *x0);	/* calculate the slope of the line */
      *y1 += m * (maxdim - *x1);
      *x1 = maxdim;
      return 1;
    }
  if (*x1 < 0)
    {				/* other end is outside to the left */
      m = (*y1 - *y0) / (double) (*x1 - *x0);	/* calculate the slope of the line */
      *y1 -= m * *x1;
      *x1 = 0;
      return 1;
    }
  /* only get here if both points are inside the window */
  return 1;
}

//*******************************************************************
// Public Constructor: ossimDrect
//
//*******************************************************************
ossimDrect::ossimDrect(const ossimIrect& rect)
   :
      theUlCorner(rect.ul()),
      theUrCorner(rect.ur()),
      theLrCorner(rect.lr()),
      theLlCorner(rect.ll()),
      theOrientMode(rect.orientMode())
{
   if(rect.isNan())
   {
      makeNan();
   }
}


//*****************************************************************************
//  CONSTRUCTOR: ossimDrect(const vector<ossimDpt>& points)
//  
//*****************************************************************************
ossimDrect::ossimDrect(const ossimPolygon& polygon,
                       ossimCoordSysOrientMode mode)
   :
      theOrientMode (mode)
{
   std::vector<ossimDpt> vertices;
   ossimDpt point;
   int index = 0;
   while (polygon.vertex(index, point))
   {
      vertices.push_back(point);
      index++;
   }

   initBoundingRect(vertices);
}

//*****************************************************************************
//  CONSTRUCTOR: ossimDrect(const vector<ossimDpt>& points)
//  
//*****************************************************************************
ossimDrect::ossimDrect(const std::vector<ossimDpt>& points,
                       ossimCoordSysOrientMode mode)
   :
      theOrientMode (mode)
{
  if(points.size())
   {
      unsigned long index;
      double minx, miny;
      double maxx, maxy;
      
      minx = points[0].x;
      miny = points[0].y;
      maxx = points[0].x;
      maxy = points[0].y;
            
      // find the bounds
      for(index = 1; index < points.size();index++)
      {
         
         minx = std::min(minx, points[index].x);
         miny = std::min(miny, points[index].y);
         maxx = std::max(maxx, points[index].x);
         maxy = std::max(maxy, points[index].y);
         
      }
      if(theOrientMode == OSSIM_LEFT_HANDED)
      {
         *this = ossimDrect(minx, miny, maxx, maxy, mode);
      }
      else
      {
         *this = ossimDrect(minx,maxy, maxx, miny, mode);
      }
   }
   else
   {
      makeNan();
   }
}
ossimDrect::ossimDrect(const ossimDpt& p1,
                       const ossimDpt& p2,
                       const ossimDpt& p3,
                       const ossimDpt& p4,
                       ossimCoordSysOrientMode mode)
{
   if(p1.hasNans()||p2.hasNans()||p3.hasNans()||p4.hasNans())
   {
      makeNan();
   }
   else
   {
      double minx, miny;
      double maxx, maxy;
      
      minx = std::min( p1.x, std::min(p2.x, std::min(p3.x, p4.x)));
      miny = std::min( p1.y, std::min(p2.y, std::min(p3.y, p4.y)));
      maxx = std::max( p1.x, std::max(p2.x, std::max(p3.x, p4.x)));
      maxy = std::max( p1.y, std::max(p2.y, std::max(p3.y, p4.y)));
      
      if(mode == OSSIM_LEFT_HANDED)
      {
         *this = ossimDrect(minx, miny, maxx, maxy, mode);
      }
      else
      {            
         *this = ossimDrect(minx,maxy, maxx, miny, mode);
      }
   }
}

ossimDrect::~ossimDrect()
{
}

void ossimDrect::initBoundingRect(const std::vector<ossimDpt>& points)
{
   unsigned long index;

   // initialize everyone to the first point
   if(points.size() > 0)
   {
      theUlCorner.x = points[0].x;
      theUlCorner.y = points[0].y;
      theLrCorner.x = theUlCorner.x;
      theLrCorner.y = theUlCorner.y;
   }
   
   // find the bounds
   for(index = 1; index < points.size();index++)
   {
      // find left most
      if(points[index].x < theUlCorner.x)
         theUlCorner.x = points[index].x;

      // find right most
      else if(points[index].x > theLrCorner.x)
         theLrCorner.x = points[index].x;

      if (theOrientMode == OSSIM_LEFT_HANDED)
      {
         //find top most
         if(points[index].y < theUlCorner.y)
            theUlCorner.y = points[index].y;

         // find bottom most
         else if(points[index].y > theLrCorner.y)
            theLrCorner.y = points[index].y;
      }

      else // right handed coord system
      {
         if(points[index].y > theUlCorner.y)
            theUlCorner.y = points[index].y;

         // find bottom most
         else if(points[index].y < theLrCorner.y)
            theLrCorner.y = points[index].y;
      }
   }

   // now set the other points for the rect.
   theUrCorner.x = theLrCorner.x;
   theUrCorner.y = theUlCorner.y;
   theLlCorner.x = theUlCorner.x;
   theLlCorner.y = theLrCorner.y;
}

//*******************************************************************
// Public Method:
//*******************************************************************
bool ossimDrect::intersects(const ossimDrect& rect) const
{
   if(rect.hasNans() || hasNans())
   {
      return false;
   }
   if (theOrientMode != rect.theOrientMode)
      return false;
   
   ossim_float64  ulx = d_MAX(rect.ul().x,ul().x);
   ossim_float64  lrx = d_MIN(rect.lr().x,lr().x);
   ossim_float64  uly, lry;
   bool rtn;
   
   if (theOrientMode == OSSIM_LEFT_HANDED)
   {
      uly  = d_MAX(rect.ul().y,ul().y);
      lry  = d_MIN(rect.lr().y,lr().y);
      rtn = ((ulx <= lrx) && (uly <= lry));
   }
   else
   {
      uly  = d_MAX(rect.ul().y,ul().y);
      lry  = d_MIN(rect.lr().y,lr().y);
      rtn = ((ulx <= lrx) && (uly >= lry));
   }
      
   return (rtn);
}

//*******************************************************************
// Public Method: ossimDrect::completely_within
//*******************************************************************
bool ossimDrect::completely_within(const ossimDrect& rect) const
{
   if(hasNans() || rect.hasNans())
   {
      return false;
   }
   if (theOrientMode != rect.theOrientMode)
      return false;
   
   /*  --------------
       |     1      |
       | ---------- |
       | |        | |
       | |        | |
       | |   2    | |
       | |        | |
       | |        | |
       | ---------- |
       |            |
       --------------  */

   bool rtn = true;
   
   if (theUlCorner.x < rect.ul().x)
      rtn = false;
   
   else if (theLrCorner.x > rect.lr().x)
      rtn = false;
   
   else if (theOrientMode == OSSIM_LEFT_HANDED)
   {
      if (theUlCorner.y < rect.ul().y)
         rtn = false;
   
      else if (theLrCorner.y > rect.lr().y)
         rtn = false;
   }
   
   else
   {
      if (theUlCorner.y > rect.ul().y)
         rtn = false;
   
      else if (theLrCorner.y < rect.lr().y)
         rtn = false;
   }

   return rtn;
}

//*******************************************************************
// Public Method: ossimDrect::stretchOut
//*******************************************************************
void ossimDrect::stretchOut()
{
   set_ulx(floor(theUlCorner.x));
   set_lrx(ceil(theLrCorner.x));

   if (theOrientMode == OSSIM_LEFT_HANDED)
   {
      set_uly(floor(theUlCorner.y));
      set_lry(ceil(theLrCorner.y));
   }
   else
   {
      set_uly(ceil(theUlCorner.y));
      set_lry(floor(theLrCorner.y));
   }
}

void ossimDrect::stretchToTileBoundary(const ossimDpt& widthHeight)
{
   ossimDpt ul;
   ossimDpt lr;
   ossim_int32 evenDivision=0;

   if(theOrientMode == OSSIM_LEFT_HANDED)
   {
      ul.x = theUlCorner.x;
      if( fmod(theUlCorner.x, widthHeight.x) != 0)
      {
         ul.x = ((long)(ul.x / widthHeight.x))*widthHeight.x;
         if(ul.x > theUlCorner.x)
         {
            ul.x -= widthHeight.x;
         }
      }
      ul.y = theUlCorner.y;
      if( fmod(theUlCorner.y, widthHeight.y) != 0)
      {
         ul.y = ((long)(ul.y / widthHeight.y))*widthHeight.y;
         if(ul.y > theUlCorner.y)
         {
            ul.y -= widthHeight.y;
         }
      }
      
      evenDivision = fmod(theLrCorner.x, widthHeight.x) == 0;
      lr.x = theLrCorner.x;
      if(!evenDivision)
      {
         lr.x = ((long)((lr.x)/widthHeight.x)) * widthHeight.x;
         if(lr.x < theLrCorner.x)
         {
            lr.x += widthHeight.x;
         }
      }

      evenDivision = fmod(theLrCorner.y, widthHeight.y) == 0;
      lr.y = theLrCorner.y;
      if(!evenDivision)
      {
         lr.y = ((long)(lr.y/widthHeight.y)) * widthHeight.y;
         if(lr.y < theLrCorner.y)
         {
            lr.y += widthHeight.y;
         }
      }
   }
   else
   {
      ul.x = theUlCorner.x;
      ul.y = theUlCorner.y;
      if( !equals(fmod(theUlCorner.x, widthHeight.x), 0.0))
      {
         ul.x = ((long)(ul.x/ widthHeight.x))*widthHeight.x;
         if(ul.x > theUlCorner.x)
         {
            ul.x -= widthHeight.x;
         }
      }
      if( !equals((double)fmod(theUlCorner.y, widthHeight.y), (double)0.0) )
      {
         ul.y = ((long)(ul.y / widthHeight.y))*widthHeight.y;
         if(ul.y < theUlCorner.y)
         {
            ul.y += widthHeight.y;
         }
      }
      
      evenDivision = equals( fmod(theLrCorner.x, widthHeight.x), (double)0.0);
      lr.x = theLrCorner.x;
      if(!evenDivision)
      {
         lr.x = ((long)(lr.x/widthHeight.x)) * widthHeight.x;
         if(lr.x < theLrCorner.x)
         {
            lr.x += widthHeight.x;
         }
      }

      evenDivision = equals(fmod(theLrCorner.y, widthHeight.y), 0);
      lr.y = theLrCorner.y;
      if(!evenDivision)
      {
         lr.y = ((long)(lr.y/widthHeight.y)) * widthHeight.y;

         if(lr.y > theLrCorner.y)
         {
            lr.y -= widthHeight.y;
         }
      }
  }

   *this = ossimDrect(ul, lr, theOrientMode);
}



//*******************************************************************
// Public Method: ossimDrect::print
//*******************************************************************
void ossimDrect::print(std::ostream& os) const
{
   os << (theOrientMode==OSSIM_LEFT_HANDED?"left handed: ":"right handed: ") << theUlCorner << theLrCorner;
}

//*******************************************************************
// friend function: operator<<
//*******************************************************************
std::ostream& operator<<(std::ostream& os, const ossimDrect& rect)
{
   rect.print(os);

   return os;
}

//*******************************************************************
// Public Method: ossimDrect::clip
//*******************************************************************
bool ossimDrect::clip(ossimDpt &p1, ossimDpt &p2)const
{
   if(p1.isNan() || p2.isNan())
   {
      return false;
   }
   ossimDpt shift(-theUlCorner.x,
		  -theUlCorner.y);

   ossimDpt tempShiftP1 = p1+shift;
   ossimDpt tempShiftP2 = p2+shift;
   double maxW = width()-1;
   double maxH = height()-1;
  if (clip_1d (&tempShiftP1.x, &tempShiftP1.y, 
	       &tempShiftP2.x, &tempShiftP2.y, 
	       maxW) == 0)
    {
      return false;
    }
  if(clip_1d (&tempShiftP1.y, 
	      &tempShiftP1.x, 
	      &tempShiftP2.y, 
	      &tempShiftP2.x, maxH) == 0)
    {
      return false;
    }
  p1 = tempShiftP1-shift;
  p2 = tempShiftP2-shift;
  return true;
//   bool done    = false;
   
//    long p1Code = getCode(p1, (*this));
//    long p2Code = getCode(p2, (*this));
//    long codeOut;

//    do{

//       if((!p1Code) && (!p2Code))
//       {
//          visible = true;
//          done    = true;
//       }
//       else if(p1Code&p2Code)
//       {
//          done    = true;
//       }
//       else
//       {
//          if(p1Code)
//          {
//             codeOut = p1Code;
//          }
//          else
//          {
//             codeOut = p2Code;
//          }
        
//          if(codeOut&TOP) // divide rect at the top
//          {
//             tempPt.x = p1.x + (p2.x - p1.x)*
//                        ((*this).ul().y - p1.y)/(p2.y - p1.y);
//             tempPt.y = (*this).ul().y;
//          }
//          else if(codeOut&BOTTOM)
//          {
//             tempPt.x = p1.x + (p2.x - p1.x)*
//                        ((*this).lr().y - p1.y)/(p2.y - p1.y);
//             tempPt.y = (*this).lr().y;
//          }
//          else if(codeOut&RIGHT)
//          {
//             tempPt.y = p1.y + (p2.y - p1.y)*
//                        ((*this).lr().x - p1.x)/(p2.x - p1.x);
//             tempPt.x = (*this).lr().x;
//          }
//          else if(codeOut&LEFT)
//          {
//             tempPt.y = p1.y + (p2.y - p1.y)*
//                        ((*this).ul().x - p1.x)/(p2.x - p1.x);
//             tempPt.x = (*this).ul().x;
//          }
//          if(codeOut == p1Code)
//          {
//             p1.x = tempPt.x;
//             p1.y = tempPt.y;
//             p1Code = getCode(p1, (*this));
//          }
//          else
//          {
//             p2.x = tempPt.x;
//             p2.y = tempPt.y;
//             p2Code = getCode(p2, (*this));
//          }
//       }
//    }while(!done);
//    return visible;
}

//*******************************************************************
// Public Method: ossimDrect::getCode
//*******************************************************************
long ossimDrect::getCode(const ossimDpt& aPoint,
                         const ossimDrect& clipRect)
{
   long result=NONE; // initialize to inside rect
   
   if( (aPoint.x > clipRect.lr().x) )
      result |= RIGHT;
   else if( (aPoint.x < clipRect.ul().x) )
      result |= LEFT;

   if (clipRect.theOrientMode == OSSIM_LEFT_HANDED)
   {
      if( (aPoint.y < clipRect.ul().y) )
         result |= TOP;
      else if( (aPoint.y > clipRect.lr().y) )
         result |= BOTTOM;
   }
   else
   {
      if( (aPoint.y > clipRect.ul().y) )
         result |= TOP;
      else if( (aPoint.y < clipRect.lr().y) )
         result |= BOTTOM;
   }
      
   return result;
}


void ossimDrect::splitToQuad(ossimDrect& ulRect,
                             ossimDrect& urRect,
                             ossimDrect& lrRect,
                             ossimDrect& llRect)
{
   ossimDpt ulPt  = this->ul();
   ossimDpt urPt  = this->ur();
   ossimDpt lrPt  = this->lr();
   ossimDpt llPt  = this->ll();
   ossimIpt midPt = this->midPoint();
   
   ulRect = ossimDrect(ulPt.x,
		       ulPt.y,
		       midPt.x,
		       midPt.y,
		       theOrientMode);
   
   urRect = ossimDrect(midPt.x+DBL_EPSILON,
		       ulPt.y,
		       urPt.x,
		       midPt.y,
		       theOrientMode);

   if(theOrientMode  == OSSIM_LEFT_HANDED)
     {
       lrRect = ossimDrect(midPt.x+DBL_EPSILON,
			   midPt.y+DBL_EPSILON,
			   lrPt.x,
			   theOrientMode);
     }
   else
     {       
       lrRect = ossimDrect(midPt.x+DBL_EPSILON,
			   midPt.y-DBL_EPSILON,
			   lrPt.x,
			   theOrientMode);
     }

   if(theOrientMode  == OSSIM_LEFT_HANDED)
     {
       llRect = ossimDrect(ulPt.x,
			   midPt.y+DBL_EPSILON,
			   midPt.x,
			   llPt.y,
			   theOrientMode);
     }
   else
     {
       llRect = ossimDrect(ulPt.x,
			   midPt.y-DBL_EPSILON,
			   midPt.x,
			   llPt.y,
			   theOrientMode);       
     }
}

//*******************************************************************
// Public Method: ossimDrect::clipToRect
//*******************************************************************
ossimDrect ossimDrect::clipToRect(const ossimDrect& rect)const
{
   if(rect.hasNans() || hasNans())
   {
      ossimDrect result;
      result.makeNan();

      return result;
   }
   
   if (theOrientMode != rect.theOrientMode)
      return (*this);

   double x0 = d_MAX(rect.ul().x, ul().x);
   double x1 = d_MIN(rect.lr().x, lr().x);
   double y0, y1;

   if(!this->intersects(rect))
   {
      return ossimDrect(OSSIM_DBL_NAN,
                        OSSIM_DBL_NAN,
                        OSSIM_DBL_NAN,
                        OSSIM_DBL_NAN);
   }
   if (theOrientMode == OSSIM_LEFT_HANDED)
   {
      y0 = d_MAX(rect.ul().y, ul().y);
      y1 = d_MIN(rect.lr().y, lr().y);

      if( (x1 < x0) || (y1 < y0) )
         return ossimDrect(ossimDpt(0,0), ossimDpt(0,0), theOrientMode);
      else
         return ossimDrect(x0, y0, x1, y1, theOrientMode);
   }
   else
   {
      y1 = d_MIN(rect.ul().y,ul().y);
      y0 = d_MAX(rect.lr().y,lr().y);

      if((x1 < x0) || (y1 < y0))
         return ossimDrect(ossimDpt(0,0), ossimDpt(0,0), theOrientMode);
      else
         return ossimDrect(x0, y1, x1, y0, theOrientMode);
   }
}

const ossimDrect& ossimDrect::operator=(const ossimIrect& rect)
{
   if(rect.isNan())
   {
      makeNan();
   }
   else
   {
      theUlCorner   = rect.ul();
      theUrCorner   = rect.ur();
      theLrCorner   = rect.lr();
      theLlCorner   = rect.ll();
      theOrientMode = rect.orientMode();
   }
   
   return *this;
}

