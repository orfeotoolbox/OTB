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
// $Id: ossimPolyArea2d.cpp,v 1.20 2005/09/30 19:56:45 gpotts Exp $
#include <list>
#include <sstream>
using namespace std;

#include <base/data_types/ossimPolyArea2d.h>
#include <base/common/ossimKeywordNames.h>

ostream& operator<<(ostream& out, const ossimPolyArea2d& data)
{
   if(!data.thePolygon)
   {
      return out;
   }
   int cnt;
   PLINE *cntr;
   POLYAREA * curpa;
   
   cnt = 0, curpa = data.thePolygon; 
   do
   {
      cnt++;
   } while ( (curpa = curpa->f) != data.thePolygon);
   
   curpa = data.thePolygon; 
   do
   {
      for ( cntr = curpa->contours, cnt = 0; cntr != NULL; cntr = cntr->next, cnt++ )
      {}
      out << cnt << endl;
      
      for ( cntr = curpa->contours; cntr != NULL; cntr = cntr->next )
      {
         VNODE *cur;
         
         out << cntr->Count << endl;
         cur = &cntr->head;
         do
         {
            out << cur->point[0] << " " <<  cur->point[1] << endl;
         } while ( ( cur = cur->next ) != &cntr->head );
      }     
   }while ( (curpa = curpa->f) != data.thePolygon);
   
   return out;
}



ossimPolyArea2d::ossimPolyArea2d()
   :thePolygon(NULL)
{  
}

ossimPolyArea2d::ossimPolyArea2d(POLYAREA* polygon,
                               bool    copyPolygonFlag)
   :thePolygon(NULL)
{
   if(copyPolygonFlag)
   {
      poly_Copy0(&thePolygon, polygon);
   }
   else
   {
      thePolygon = polygon;
   }
}

ossimPolyArea2d::ossimPolyArea2d(const ossimPolyArea2d& rhs)
   :thePolygon(NULL)
{
   poly_Copy0(&thePolygon, rhs.thePolygon);
}

ossimPolyArea2d::ossimPolyArea2d(const vector<ossimDpt>& polygon)
   :thePolygon(NULL)
{
   if(polygon.size() > 0)
   {
      PLINE  *poly = NULL;
      Vector v;
      v[0] = polygon[0].x;
      v[1] = polygon[0].y;
      v[2] = 0;
      poly = poly_NewContour(v);
      for(long index = 1; index < (long)polygon.size(); ++index)
      {

         v[0] = polygon[index].x;
         v[1] = polygon[index].y;
         v[2] = 0.0;
         
         poly_InclVertex(poly->head.prev,
                         poly_CreateNode(v));
         
      }
      poly_PreContour(poly, TRUE);
      
      if ( (poly->Flags & PLF_ORIENT) != PLF_DIR)
      {
         poly_InvContour(poly);
      }
      thePolygon = poly_Create();
      poly_InclContour(thePolygon, poly);
//       if(!poly_Valid(thePolygon))
//       {
//          if(thePolygon)
//          {
//             poly_Free(&thePolygon);
//             thePolygon = NULL;
//          }
//       }
   }   
}

ossimPolyArea2d::ossimPolyArea2d(const vector<ossimGpt>& polygon)
   :thePolygon(NULL)
{
   if(polygon.size() > 0)
   {
      PLINE  *poly = NULL;
      Vector v;
      v[0] = polygon[0].lon;
      v[1] = polygon[0].lat;
      v[2] = 0;
      poly = poly_NewContour(v);
      for(long index = 1; index < (long)polygon.size(); ++index)
      {

         v[0] = polygon[index].lon;
         v[1] = polygon[index].lat;
         v[2] = 0.0;
         
         poly_InclVertex(poly->head.prev,
                         poly_CreateNode(v));
         
      }
      poly_PreContour(poly, TRUE);
      
      if ( (poly->Flags & PLF_ORIENT) != PLF_DIR)
      {
         poly_InvContour(poly);
      }
      thePolygon = poly_Create();
      poly_InclContour(thePolygon, poly);
//       if(!poly_Valid(thePolygon))
//       {
//          if(thePolygon)
//          {
//             poly_Free(&thePolygon);
//             thePolygon = NULL;
//          }
//       }
   }   
}

ossimPolyArea2d::ossimPolyArea2d(const ossimDpt& p1,
                                 const ossimDpt& p2,
                                 const ossimDpt& p3,
                                 const ossimDpt& p4)
   :thePolygon(NULL)
{
   PLINE  *poly = NULL;
   Vector v;
   v[0] = p1.x;
   v[1] = p1.y;
   v[2] = 0;
   poly = poly_NewContour(v);
   poly_InclVertex(poly->head.prev,
                   poly_CreateNode(v));
   v[0] = p2.x;
   v[1] = p2.y;
   v[2] = 0;
   poly_InclVertex(poly->head.prev,
                   poly_CreateNode(v));
   v[0] = p3.x;
   v[1] = p3.y;
   v[2] = 0;
   poly_InclVertex(poly->head.prev,
                   poly_CreateNode(v));
   v[0] = p4.x;
   v[1] = p4.y;
   v[2] = 0;
   poly_InclVertex(poly->head.prev,
                   poly_CreateNode(v));
   
   poly_PreContour(poly, TRUE);
   
   if ( (poly->Flags & PLF_ORIENT) != PLF_DIR)
   {
      poly_InvContour(poly);
   }
   thePolygon = poly_Create();
   poly_InclContour(thePolygon, poly);
//    if(!poly_Valid(thePolygon))
//    {
//       if(thePolygon)
//       {
//          poly_Free(&thePolygon);
//          thePolygon = NULL;
//       }
//    }   
}


ossimPolyArea2d::ossimPolyArea2d(ossimDpt* polygon,
                               long number)
   :thePolygon(NULL)
{
   if(number > 0)
   {
      PLINE  *poly = NULL;
      Vector v;
      v[0] = polygon[0].x;
      v[1] = polygon[0].y;
      v[2] = 0;
      poly = poly_NewContour(v);
      for(long index = 1; index < number; ++index)
      {

         v[0] = polygon[index].x;
         v[1] = polygon[index].y;
         v[2] = 0.0;
         
         poly_InclVertex(poly->head.prev,
                         poly_CreateNode(v));
         
      }
      poly_PreContour(poly, TRUE);
      
      if ( (poly->Flags & PLF_ORIENT) != PLF_DIR)
      {
         poly_InvContour(poly);
      }
      thePolygon = poly_Create();
      poly_InclContour(thePolygon, poly);
//       if(!poly_Valid(thePolygon))
//       {
//          poly_Free(&thePolygon);
//          thePolygon = NULL;
//       }
   }
}

ossimPolyArea2d::ossimPolyArea2d(const ossimPolygon& polygon)
   :thePolygon(NULL)
{
   if(polygon.getVertexCount() > 0)
   {
      PLINE  *poly = NULL;
      Vector v;
      v[0] = polygon[0].x;
      v[1] = polygon[0].y;
      v[2] = 0;
      poly = poly_NewContour(v);
      for(long index = 1; index < (long)polygon.getVertexCount(); ++index)
      {

         v[0] = polygon[index].x;
         v[1] = polygon[index].y;
         v[2] = 0.0;
         
         poly_InclVertex(poly->head.prev,
                         poly_CreateNode(v));
         
      }
      poly_PreContour(poly, TRUE);
      
      // make sure that its clockwise
      if ( (poly->Flags & PLF_ORIENT) != PLF_DIR)
      {
         poly_InvContour(poly);
      }
      thePolygon = poly_Create();
      poly_InclContour(thePolygon, poly);
      if(!poly_Valid(thePolygon))
      {
//         if(thePolygon)
//         {
//            poly_Free(&thePolygon);
//            thePolygon = NULL;
//         }
      }
   }   
}

ossimPolyArea2d::~ossimPolyArea2d()
{
   if(thePolygon)
   {
      poly_Free(&thePolygon);
      thePolygon = NULL;
   }
}
const ossimPolyArea2d& ossimPolyArea2d::operator *=(const ossimDpt& scale)
{
   if(!thePolygon) return *this;
   
   PLINE *cntr     = (PLINE*)NULL;
   POLYAREA *curpa = (POLYAREA*)NULL;
   
   curpa           = thePolygon;

   do{
      for ( cntr = curpa->contours; cntr != NULL; cntr = cntr->next )
      {
         VNODE *cur = &cntr->head;
         double xmin, ymin;
         double xmax, ymax;
         xmin = xmax = cur->point[0];
         ymin = ymax = cur->point[1];
         do
         {
            cur->point[0] *= scale.x;
            cur->point[1] *= scale.y;
            xmin = std::min(cur->point[0], xmin);
            xmax = std::max(cur->point[0], xmax);
            ymin = std::min(cur->point[1], ymin);
            ymax = std::max(cur->point[1], ymax);
         } while ( ( cur = cur->next ) != &cntr->head );
      }
   }
   while( (curpa = curpa->f) != thePolygon);
   
   return *this;
}

ossimPolyArea2d ossimPolyArea2d::operator *(const ossimDpt& scale)const
{
   if(!thePolygon) return *this;

   ossimPolyArea2d result(*this);
   PLINE *cntr     = (PLINE*)NULL;
   POLYAREA *curpa = (POLYAREA*)NULL;
   
   curpa           = result.thePolygon;

   do{
      for ( cntr = curpa->contours; cntr != NULL; cntr = cntr->next )
      {
         VNODE *cur = &cntr->head;
         double xmin, ymin;
         double xmax, ymax;
         xmin = xmax = cur->point[0];
         ymin = ymax = cur->point[1];
         
         do
         {
            cur->point[0] *= scale.x;
            cur->point[1] *= scale.y;
            xmin = std::min(cur->point[0], xmin);
            xmax = std::max(cur->point[0], xmax);
            ymin = std::min(cur->point[1], ymin);
            ymax = std::max(cur->point[1], ymax);
         } while ( ( cur = cur->next ) != &cntr->head );

         cntr->xmin = xmin;
         cntr->ymin = ymin;
         cntr->xmax = xmax;
         cntr->ymax = ymax;
      }
   }
   while( (curpa = curpa->f) != result.thePolygon);
   
   return result;
}

const ossimPolyArea2d& ossimPolyArea2d::operator *=(double scale)
{
   return ((*this)*=ossimDpt(scale, scale));
}

ossimPolyArea2d ossimPolyArea2d::operator *(double scale)const
{
   return ((*this)*ossimDpt(scale, scale));
}

/*!
 * Assignment operator.  Allows you to assign
 * or copy one polygon to another.
 */
const ossimPolyArea2d& ossimPolyArea2d::operator =(const ossimPolyArea2d& rhs)
{
   if(this != &rhs)
   {
      if(thePolygon)
      {
         poly_Free(&thePolygon);
         thePolygon = NULL;
      }
      if(rhs.thePolygon)
      {
         poly_Copy0(&thePolygon, rhs.thePolygon);
      }
   }

   return *this;
}

const ossimPolyArea2d& ossimPolyArea2d::operator =(const ossimIrect& rect)
{
   if(thePolygon)
   {
      poly_Free(&thePolygon);
      thePolygon = NULL;
   }
   
   PLINE  *poly = NULL;
   Vector v;
   v[0] = rect.ul().x;
   v[1] = rect.ul().y;
   v[2] = 0;
   poly = poly_NewContour(v);
   poly_InclVertex(poly->head.prev,
                   poly_CreateNode(v));
   v[0] = rect.ur().x;
   v[1] = rect.ur().y;
   v[2] = 0;
   poly_InclVertex(poly->head.prev,
                   poly_CreateNode(v));
   v[0] = rect.lr().x;
   v[1] = rect.lr().y;
   v[2] = 0;
   poly_InclVertex(poly->head.prev,
                   poly_CreateNode(v));
   v[0] = rect.ll().x;
   v[1] = rect.ll().y;
   v[2] = 0;
   poly_InclVertex(poly->head.prev,
                   poly_CreateNode(v));
   
   poly_PreContour(poly, TRUE);
   
   if ( (poly->Flags & PLF_ORIENT) != PLF_DIR)
   {
      poly_InvContour(poly);
   }
   thePolygon = poly_Create();
   poly_InclContour(thePolygon, poly);
//    if(!poly_Valid(thePolygon))
//    {
//       if(thePolygon)
//       {
//          poly_Free(&thePolygon);
//          thePolygon = NULL;
//       }
//    }

   return *this;
}

const ossimPolyArea2d& ossimPolyArea2d::operator =(const ossimDrect& rect)
{
   if(thePolygon)
   {
      poly_Free(&thePolygon);
      thePolygon = NULL;
   }
   
   PLINE  *poly = NULL;
   Vector v;
   v[0] = rect.ul().x;
   v[1] = rect.ul().y;
   v[2] = 0;
   poly = poly_NewContour(v);
   poly_InclVertex(poly->head.prev,
                   poly_CreateNode(v));
   v[0] = rect.ur().x;
   v[1] = rect.ur().y;
   v[2] = 0;
   poly_InclVertex(poly->head.prev,
                   poly_CreateNode(v));
   v[0] = rect.lr().x;
   v[1] = rect.lr().y;
   v[2] = 0;
   poly_InclVertex(poly->head.prev,
                   poly_CreateNode(v));
   v[0] = rect.ll().x;
   v[1] = rect.ll().y;
   v[2] = 0;
   poly_InclVertex(poly->head.prev,
                   poly_CreateNode(v));
   
   poly_PreContour(poly, TRUE);
   
   if ( (poly->Flags & PLF_ORIENT) != PLF_DIR)
   {
      poly_InvContour(poly);
   }
   thePolygon = poly_Create();
   poly_InclContour(thePolygon, poly);
//    if(!poly_Valid(thePolygon))
//    {
//       if(thePolygon)
//       {
//          poly_Free(&thePolygon);
//          thePolygon = NULL;
//       }
//    }
   
   return *this;
}

ossimPolyArea2d ossimPolyArea2d::operator -(const ossimPolyArea2d& rhs)const
{
   if(this == &rhs)
   {
      return ossimPolyArea2d();
   }
   POLYAREA* result=NULL;

   if (poly_Boolean(this->thePolygon,
                    rhs.thePolygon,
                    &result,
                    PBO_SUB) == err_ok) 
   {
      return ossimPolyArea2d(result, false);
   }

   // if there were errors then we will just return an
   // empty polygon.
   //
   return ossimPolyArea2d();
}

ossimPolyArea2d& ossimPolyArea2d::operator -=(const ossimPolyArea2d& rhs)
{
   if(this == &rhs)
   {
      poly_Free(&thePolygon);
      thePolygon = NULL;
   }
   else
   {
      POLYAREA* result=NULL;
      
      if (poly_Boolean(this->thePolygon,
                       rhs.thePolygon,
                       &result,
                       PBO_SUB) == err_ok) 
      {
         poly_Free(&thePolygon);
         thePolygon = result;
      }
      else
      {
         // errors so make it empty
         poly_Free(&thePolygon);
         thePolygon = NULL;
      }
   }

   return *this;
}

ossimPolyArea2d ossimPolyArea2d::operator +(const ossimPolyArea2d& rhs)const
{
   if(this == &rhs)
   {
      return *this;
   }
   POLYAREA* result=NULL;

   if (poly_Boolean(this->thePolygon,
                    rhs.thePolygon,
                    &result,
                    PBO_UNITE) == err_ok) 
   {
      return ossimPolyArea2d(result, false);
   }

   // if there were errors then we will just return an
   // empty polygon.
   //
   return ossimPolyArea2d();
}

ossimPolyArea2d& ossimPolyArea2d::operator +=(const ossimPolyArea2d& rhs)
{
   if(this == &rhs)
   {
      poly_Free(&thePolygon);
      thePolygon = NULL;
   }
   else
   {
      POLYAREA* result=NULL;
      if (poly_Boolean(this->thePolygon,
                       rhs.thePolygon,
                       &result,
                       PBO_UNITE) == err_ok) 
      {
         poly_Free(&thePolygon);
         thePolygon = result;
      }
      else
      {
         // errors so make it empty
         poly_Free(&thePolygon);
         thePolygon = NULL;
      }
   }

   return *this;
}

ossimPolyArea2d ossimPolyArea2d::operator &(const ossimPolyArea2d& rhs)const
{
   if(this == &rhs)
   {
      return ossimPolyArea2d(*this);
   }
   if(thePolygon&&rhs.thePolygon)
   {
      POLYAREA* result=NULL;
      
      if (poly_Boolean(this->thePolygon,
                       rhs.thePolygon,
                       &result,
                       PBO_ISECT) == err_ok) 
      {
         return ossimPolyArea2d(result, false);
      }
   }
   
   // if there were errors then we will just return an
   // empty polygon.
   //
   return ossimPolyArea2d();
}

ossimPolyArea2d& ossimPolyArea2d::operator &=(const ossimPolyArea2d& rhs)
{
   if(this != &rhs)
   {
      POLYAREA* result=NULL;
      if (poly_Boolean(this->thePolygon,
                       rhs.thePolygon,
                       &result,
                       PBO_ISECT) == err_ok) 
      {
         poly_Free(&thePolygon);
         thePolygon = result;
      }
      else
      {
         // errors so make it empty
         poly_Free(&thePolygon);
         thePolygon = NULL;
      }
   }

   return *this;
}

ossimPolyArea2d ossimPolyArea2d::operator ^(const ossimPolyArea2d& rhs)const
{
   if(this != &rhs)
   {
      POLYAREA* result=NULL;
      
      if (poly_Boolean(this->thePolygon,
                       rhs.thePolygon,
                       &result,
                       PBO_XOR) == err_ok) 
      {
         return ossimPolyArea2d(result, false);
      }
      
   }
   
   return ossimPolyArea2d();
}

ossimPolyArea2d& ossimPolyArea2d::operator ^=(const ossimPolyArea2d& rhs)
{
   if(this == &rhs)
   {
      poly_Free(&thePolygon);
      thePolygon = NULL;
   }
   else
   {
      POLYAREA* result=NULL;
      if (poly_Boolean(this->thePolygon,
                       rhs.thePolygon,
                       &result,
                       PBO_XOR) == err_ok) 
      {
         poly_Free(&thePolygon);
         thePolygon = result;
      }
      else
      {
         // errors so make it empty
         poly_Free(&thePolygon);
         thePolygon = NULL;
      }
   }
   
   return *this;
}

bool ossimPolyArea2d::isPointWithin(const ossimDpt& point)const
{
   if(!thePolygon)
   {
      return false;
   }
   Vector v;
   v[0] = point.x;
   v[1] = point.y;
   v[2] = 0;
   
   return (poly_CheckInside(thePolygon, v)==TRUE);
  
}

bool ossimPolyArea2d::isPointWithin(double x, double y)const
{
   if(!thePolygon)
   {
      return false;
   }
   Vector v;
   v[0] = x;
   v[1] = y;
   v[2] = 0;
   
   return (poly_CheckInside(thePolygon, v)==TRUE);
}

void ossimPolyArea2d::getBoundingRect(ossimDrect& rect)
{
   if(thePolygon)
   {
      PLINE*    cntr  = NULL;
      POLYAREA* curpa = NULL;
      
      curpa = thePolygon;
      rect = ossimDrect(curpa->contours->xmin,
                        curpa->contours->ymin,
                        curpa->contours->xmax,
                        curpa->contours->ymax);
      do
      {        
         for ( cntr = curpa->contours->next; cntr != NULL; cntr = cntr->next )
         {
            ossimDrect rect2 = ossimDrect(curpa->contours->xmin,
                                          curpa->contours->ymin,
                                          curpa->contours->xmax,
                                          curpa->contours->ymax);
            if(rect2.width() != 1 &&
               rect2.height() != 1)
            {
               rect = rect.combine(rect2);
            }
         }     
      }while ( (curpa = curpa->f) != thePolygon);
   }
   else
   {
      rect = ossimDrect(0,0,0,0);
   }
}

bool ossimPolyArea2d::saveState(ossimKeywordlist& kwl,
                               const char* prefix)const
{
   kwl.add(prefix,
           ossimKeywordNames::TYPE_KW,
           "ossimPolyArea2d",
           true);
           
   if(!thePolygon)
   {
      return true;
   }
   int cnt;
   PLINE *cntr;
   POLYAREA * curpa;
   
   cnt = 0, curpa = thePolygon; 
   do
   {
      cnt++;
   } while ( (curpa = curpa->f) != thePolygon);
   
   curpa = thePolygon; 
   do
   {
      cnt = 1;
      for ( cntr = curpa->contours; cntr != NULL; cntr = cntr->next )
      {
         VNODE *cur;
         ossimString newPrefix = prefix;
         newPrefix+="contour";
         newPrefix += ossimString::toString(cnt);
         newPrefix += ".";
         kwl.add(newPrefix.c_str(),
                 "flags",
                 (int)cntr->Flags,
                 true);
         
         cur = &cntr->head;
         int pointCount = 1;
         do
         {
            ossimString pointString = "point";
            pointString += ossimString::toString(pointCount);
            ossimString points;
            points = ossimString::toString(cur->point[0]);
            points += " ";
            points += ossimString::toString(cur->point[1]);
            kwl.add(newPrefix.c_str(),
                    pointString,
                    points.c_str(),
                    true);
            ++pointCount;
         } while ( ( cur = cur->next ) != &cntr->head );
         ++cnt;
      }     
   }while ( (curpa = curpa->f) != thePolygon);
   
   return true;
}

bool ossimPolyArea2d::loadState(const ossimKeywordlist& kwl,
                                const char* prefix)
{
   
   vector<ossimString> contourList = kwl.getSubstringKeyList(ossimString(prefix) + "contour[0-9]+\\.");
   if(thePolygon)
   {
      poly_Free(&thePolygon);
      thePolygon = NULL;
   }
   if(contourList.size() > 0)
   {
      thePolygon = poly_Create();
      vector<ossimString>::iterator currentContour = contourList.begin();
      
      Vector v;
      PLINE  *poly = NULL;
      ossim_uint32 currentContourIndex = 0;
      while(currentContour != contourList.end())
      {
         ossimString copyPrefix = *currentContour;
         ossim_int32 contourFlags = 0;
         
         const char* lookupFlags = kwl.find((*currentContour).c_str(),
                                             "flags");
         if(lookupFlags)
         {
            contourFlags = ossimString(lookupFlags).toInt32();
         }
         ossim_uint32 numberOfPoints = kwl.getNumberOfSubstringKeys(copyPrefix + "point[0-9]");
         ossim_uint32 numberOfMatches = 0;
         ossim_uint32 currentPoint = 0;
         while(numberOfMatches < numberOfPoints)
         {
            ossimString pointString = "point";
            pointString += ossimString::toString(currentPoint);
            const char* pointLookup = kwl.find((*currentContour).c_str(),
                                               pointString);
            if(pointLookup)
            {
               istringstream s(pointLookup);

               s >> v[0] >> v[1];
               v[2] = 0;

               // initialize the poly if we are on the
               // first match point else we just add it.
               if(!numberOfMatches)
               {
                  poly = poly_NewContour(v);
               }
               else
               {
                  poly_InclVertex(poly->head.prev,
                                  poly_CreateNode(v));
               }
               
               ++numberOfMatches;
            }
            ++currentPoint;
         }
         poly->Flags = contourFlags;
         poly_PreContour(poly, TRUE);
         if ( (poly->Flags & PLF_ORIENT) !=
              (currentContourIndex ? PLF_INV : PLF_DIR) )
         {
            poly_InvContour(poly);
         }
         if(!poly_InclContour(thePolygon, poly))
         {
            poly_DelContour(&poly);
         }
         poly = NULL;
         ++currentContour;
         ++currentContourIndex;
      }
   }
   return true;
}

bool ossimPolyArea2d::getAllVisiblePolygons(vector<ossimPolygon>& polyList)const
{
   if(!thePolygon) return false;
   if(!poly_Valid(thePolygon))
   {
      return false;
   }
   polyList.clear();
   POLYAREA* curpa = thePolygon;
   PLINE*    cntr;
   int index = 0;
   do
   {
      for ( cntr = curpa->contours; cntr != NULL; cntr = cntr->next )
      {
         if(cntr->Flags & PLF_DIR)
         {
            polyList.push_back(ossimPolygon());
            VNODE* cur = &cntr->head;
            do
            {   
               polyList[index].addPoint(cur->point[0],
                                        cur->point[1]);
               
            } while ( ( cur = cur->next ) != &cntr->head );
            ++index;
         }
      }
   }while ( (curpa = curpa->f) != thePolygon);

   return (polyList.size()>0);
}
