//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimGeoAnnotationMultiPolyLineObject.cpp 13711 2008-10-14 16:49:38Z gpotts $

#include <sstream>

#include <ossim/imaging/ossimGeoAnnotationMultiPolyLineObject.h>
#include <ossim/imaging/ossimAnnotationMultiPolyLineObject.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/projection/ossimImageProjectionModel.h>
#include <ossim/base/ossimException.h>


RTTI_DEF1(ossimGeoAnnotationMultiPolyLineObject,
          "ossimGeoAnnotationMultiPolyLineObject",
          ossimGeoAnnotationObject);

ossimGeoAnnotationMultiPolyLineObject::ossimGeoAnnotationMultiPolyLineObject()
   :
   ossimGeoAnnotationObject(),
   theMultiPolyLine(),
   theBoundingRect(),
   theDatum(ossimDatumFactory::instance()->wgs84()),
   theProjectedPolyLineObject(0)
{
   allocateProjectedPolyLine();   
   theBoundingRect.makeNan();
}

ossimGeoAnnotationMultiPolyLineObject::ossimGeoAnnotationMultiPolyLineObject(
   const vector<ossimPolyLine>& multiPoly,
   ossim_uint8 r,
   ossim_uint8 g,
   ossim_uint8 b,
   ossim_uint8 thickness)
   :
   ossimGeoAnnotationObject(r, g, b, thickness),
   theMultiPolyLine(multiPoly),
   theBoundingRect(),
   theDatum(ossimDatumFactory::instance()->wgs84()),
   theProjectedPolyLineObject(0)
{
   allocateProjectedPolyLine();   
   theBoundingRect.makeNan();
}

ossimGeoAnnotationMultiPolyLineObject::ossimGeoAnnotationMultiPolyLineObject(
   const ossimGeoAnnotationMultiPolyLineObject& rhs)
   :ossimGeoAnnotationObject(rhs),
    theMultiPolyLine(rhs.theMultiPolyLine),
    theBoundingRect(rhs.theBoundingRect),
    theDatum(rhs.theDatum),
    theProjectedPolyLineObject(rhs.theProjectedPolyLineObject?(ossimAnnotationMultiPolyLineObject*)rhs.theProjectedPolyLineObject->dup():0)
{
}

ossimObject* ossimGeoAnnotationMultiPolyLineObject::dup()const
{
   return new ossimGeoAnnotationMultiPolyLineObject(*this);
}

ossimGeoAnnotationMultiPolyLineObject::~ossimGeoAnnotationMultiPolyLineObject()
{
   if(theProjectedPolyLineObject)
   {
      delete theProjectedPolyLineObject;
      theProjectedPolyLineObject = 0;
   }
}

void ossimGeoAnnotationMultiPolyLineObject::applyScale(double x, double y)
{
   ossimNotify(ossimNotifyLevel_NOTICE)
      << "ossimGeoAnnotationPolyLineObject::applyScale NOT IMPLEMENTED!!!!"
      << endl;
}

void ossimGeoAnnotationMultiPolyLineObject::transform(ossimProjection* projection)
{
   if(!projection)
   {
      return;
   }
   
   allocateProjectedPolyLine();

   //---
   // NOTE:
   // allocateProjectedPolygon() will set theProjectedPolyLineObject to 0 if
   // theMultiPolyLine is empty (theMultiPolyLine.size() == 0).  So check
   // before accessing pointer to avoid a core dump.
   //---
   if(!theProjectedPolyLineObject)
   {
      return;
   }

   std::vector<ossimPolyLine>& multiPolyLine =
      theProjectedPolyLineObject->getMultiPolyLine();
   ossimGpt tempPoint(0,0, ossim::nan(), theDatum);
   
   for(std::vector<ossimPolyLine>::size_type polyI = 0;
       polyI < theMultiPolyLine.size();
       ++polyI)
   {
      ossimPolyLine polyLine;
      
      ossim_uint32 numberOfVertices =
         theMultiPolyLine[polyI].getNumberOfVertices();
      for(ossim_uint32 pointI = 0; pointI < numberOfVertices; ++pointI)
      {
         tempPoint.latd(theMultiPolyLine[polyI][pointI].lat);
         tempPoint.lond(theMultiPolyLine[polyI][pointI].lon);
         ossimDpt temp;
         projection->worldToLineSample(tempPoint, temp);
         if(!temp.hasNans())
         {
	    multiPolyLine[polyI].addPoint(temp);
         }
      }
      multiPolyLine[polyI].roundToIntegerBounds(true);
   }
}

void ossimGeoAnnotationMultiPolyLineObject::transform(
   const ossimImageProjectionModel& model, ossim_uint32 rrds)
{
   const ossimProjection* projection = model.getProjection();
   if (projection)
   {
      allocateProjectedPolyLine();

      //---
      // NOTE:
      // allocateProjectedPolygon() will set theProjectedPolyLineObject to 0 if
      // theMultiPolyLine is empty (theMultiPolyLine.size() == 0).  So check
      // before accessing pointer to avoid a core dump.
      //---
      if(theProjectedPolyLineObject)
      {
         std::vector<ossimPolyLine>& multiPolyLine =
            theProjectedPolyLineObject->getMultiPolyLine();
         ossimGpt tempPoint(0,0, ossim::nan(), theDatum);
   
         for(vector<ossimPolyLine>::size_type polyI = 0;
             polyI < theMultiPolyLine.size();
             ++polyI)
         {
            ossimPolyLine polyLine;
            
            ossim_uint32 numberOfVertices =
               theMultiPolyLine[polyI].getNumberOfVertices();
            for(ossim_uint32 pointI = 0; pointI < numberOfVertices; ++pointI)
            {
               tempPoint.latd(theMultiPolyLine[polyI][pointI].lat);
               tempPoint.lond(theMultiPolyLine[polyI][pointI].lon);
               ossimDpt r0Pt;
               projection->worldToLineSample(tempPoint, r0Pt);
               if ( !r0Pt.hasNans() )
               {
                  if (rrds)
                  {
                     // Transform r0 point to new rrds level.
                     try
                     {
                        ossimDpt rnPt;
                        model.r0ToRn(rrds, r0Pt, rnPt);
                        multiPolyLine[polyI].addPoint(rnPt);
                     }
                     catch (const ossimException& e)
                     {
                        ossimNotify(ossimNotifyLevel_WARN)
                           << e.what() << std::endl;
                     } 
                  }
                  else
                  {
                     multiPolyLine[polyI].addPoint(r0Pt);
                  }
               }
               
            } // End point loop
            
            multiPolyLine[polyI].roundToIntegerBounds(true);
            
         } // End poly line loop
         
      } // End if (theProjectedPolyLineObject)
      
   } // End if (projection)
}

std::ostream& ossimGeoAnnotationMultiPolyLineObject::print(std::ostream& out)const
{
   ossimNotify(ossimNotifyLevel_INFO)
      << "ossimGeoAnnotationMultiPolyLineObject::print\n"
      << "Poly line size: " << theMultiPolyLine.size() << endl;
   return out;
}

bool ossimGeoAnnotationMultiPolyLineObject::intersects(const ossimDrect& rect)const
{
   ossimNotify(ossimNotifyLevel_NOTICE)
      << "ossimGeoAnnotationMultiPolyObject::intersects NOT IMPLEMENTED"
      << endl;
  return false;
}

ossimAnnotationObject* ossimGeoAnnotationMultiPolyLineObject::getNewClippedObject(const ossimDrect& rect)const
{
   ossimNotify(ossimNotifyLevel_NOTICE)
      << "ossimGeoAnnotationMultiPolyObject::getNewClippedObject "
      << "NOT IMPLEMENTED" << endl;

   return (ossimAnnotationObject*)(dup());
}

void ossimGeoAnnotationMultiPolyLineObject::draw(ossimRgbImage& anImage)const
{
   if(theProjectedPolyLineObject)
   {
      theProjectedPolyLineObject->draw(anImage);
   }
}

void ossimGeoAnnotationMultiPolyLineObject::getBoundingRect(ossimDrect& rect)const
{
   rect = theBoundingRect;
}

void ossimGeoAnnotationMultiPolyLineObject::addPoint(ossim_uint32 polygonIndex,
                                                     const ossimGpt& pt)
{
   if(polygonIndex < theMultiPolyLine.size())
   {
      theMultiPolyLine[polygonIndex].addPoint(pt);
      
      // we will have to reset the projected polygon
      if(theProjectedPolyLineObject)
      {
         delete theProjectedPolyLineObject;
         theProjectedPolyLineObject = 0;
      }
   }
}

void ossimGeoAnnotationMultiPolyLineObject::setMultiPolyLine(
   const vector<ossimPolyLine>& multiPoly)
{
   theMultiPolyLine = multiPoly;
   if(theProjectedPolyLineObject)
   {
      delete theProjectedPolyLineObject;
      theProjectedPolyLineObject = 0;
   }
}

void ossimGeoAnnotationMultiPolyLineObject::computeBoundingRect()
{
   theBoundingRect.makeNan();
   if(theProjectedPolyLineObject)
   {
      theProjectedPolyLineObject->computeBoundingRect();
      theProjectedPolyLineObject->getBoundingRect(theBoundingRect);
   }
   
}

bool ossimGeoAnnotationMultiPolyLineObject::isPointWithin(const ossimDpt& imagePoint)const
{
   if(theProjectedPolyLineObject)
   {
      return theProjectedPolyLineObject->isPointWithin(imagePoint);
   }
   
   return false;
}

void ossimGeoAnnotationMultiPolyLineObject::setColor(ossim_uint8 r,
                                                     ossim_uint8 g,
                                                     ossim_uint8 b)
{
   ossimAnnotationObject::setColor(r, g, b);
   if(theProjectedPolyLineObject)
   {
      theProjectedPolyLineObject->setColor(r, g, b);
   }
}

void ossimGeoAnnotationMultiPolyLineObject::setThickness(ossim_uint8 thickness)
{
   ossimAnnotationObject::setThickness(thickness);
   if(theProjectedPolyLineObject)
   {
      theProjectedPolyLineObject->setThickness(thickness);
   }
}
   
void ossimGeoAnnotationMultiPolyLineObject::setDatum(const ossimDatum* datum)
{
   theDatum = datum;
}

const ossimDatum* ossimGeoAnnotationMultiPolyLineObject::getDatum()const
{
   return theDatum;
}

const std::vector<ossimPolyLine>&
ossimGeoAnnotationMultiPolyLineObject::getMultiPolyLine() const
{
   return theMultiPolyLine;
}

std::vector<ossimPolyLine>&
ossimGeoAnnotationMultiPolyLineObject::getMultiPolyLine()
{
   return theMultiPolyLine;
}

bool ossimGeoAnnotationMultiPolyLineObject::saveState(ossimKeywordlist& kwl,
                                                      const char* prefix) const
{
   ossim_uint32 vIdx = 0;
   
   for(ossim_uint32 i = 0; i < theMultiPolyLine.size(); ++i)
   {
      ossimPolyLine pl = theMultiPolyLine[i];
      for(ossim_uint32 j = 0; j < pl.getNumberOfVertices(); ++j)
      {
         ossimDpt dpt = pl[j];
         std::ostringstream os;
         os << setprecision(15) << dpt.x << " " << dpt.y << endl;
         
         ossimString key = "v";
         key += ossimString::toString(vIdx);
         kwl.add(prefix, key.c_str(), os.str().c_str());
         ++vIdx;
      }
   }      
      
   return ossimGeoAnnotationObject::saveState(kwl, prefix);
}

bool ossimGeoAnnotationMultiPolyLineObject::loadState(
   const ossimKeywordlist& kwl, const char* prefix)
{
   //---
   // Base class state must be called first to pick up colors...
   //---
   bool status =  ossimGeoAnnotationObject::loadState(kwl, prefix);

   ossim_uint32 index = 0;
   ossimString copyPrefix = prefix;
   ossim_uint32 count = kwl.getNumberOfSubstringKeys(copyPrefix+
                                                      "v[0-9]");
   
   ossim_uint32 numberOfMatches = 0;
   const ossim_uint32 MAX_INDEX = count + 100;
   ossimPolyLine pl;
   
   while(numberOfMatches < count)
   {
      ossimString key = "v";
      key += ossimString::toString(index);

      const char* lookup = kwl.find(prefix, key.c_str());
      if (lookup)
      {
         ++numberOfMatches;
         ossimDpt dpt;
         ossimString x,y;
         std::istringstream is(lookup);
         is >> x >> y;
         pl.addPoint(ossimDpt(x.toDouble(), y.toDouble()));
      }

      if (pl.size() == 2)
      {
         theMultiPolyLine.push_back(pl);
         pl.clear();
      }

      ++index;
      
      if (index > MAX_INDEX) // Avoid infinite loop...
      {
         break;
      }
   }

   return status;
}

void ossimGeoAnnotationMultiPolyLineObject::allocateProjectedPolyLine()
{
   if(theProjectedPolyLineObject)
   {
      delete theProjectedPolyLineObject;
      theProjectedPolyLineObject = 0;
   }
   
   if(theMultiPolyLine.size())
   {
      vector<ossimPolyLine> polyList(theMultiPolyLine.size());
      theProjectedPolyLineObject =
         new ossimAnnotationMultiPolyLineObject(polyList,
                                                theRed,
                                                theGreen,
                                                theBlue,
                                                theThickness);
   }
}
