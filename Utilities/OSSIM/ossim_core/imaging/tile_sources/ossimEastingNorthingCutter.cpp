//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
// $Id: ossimEastingNorthingCutter.cpp,v 1.5 2005/10/03 13:16:40 gpotts Exp $
//----------------------------------------------------------------------------

#include <sstream>
#include <imaging/tile_sources/ossimEastingNorthingCutter.h>
#include <projections/map_projections/ossimMapProjection.h>
#include <base/common/ossimKeywordNames.h>
#include <base/common/ossimTrace.h>

RTTI_DEF2(ossimEastingNorthingCutter, "ossimEastingNorthingCutter", ossimRectangleCutFilter, ossimViewInterface);

static ossimTrace traceDebug(ossimString("ossimEastingNorthingCutter:debug"));


ossimEastingNorthingCutter::ossimEastingNorthingCutter(ossimObject* owner,
                                                       ossimImageSource* inputSource)
   :ossimRectangleCutFilter(owner,
                            inputSource),
    ossimViewInterface(0),
    theViewProjection(0)
{
   theUlEastingNorthing.makeNan();
   theLrEastingNorthing.makeNan();
   theCutType = OSSIM_RECTANGLE_NULL_OUTSIDE;
}

ossimEastingNorthingCutter::ossimEastingNorthingCutter(ossimImageSource* inputSource)
   :ossimRectangleCutFilter(inputSource),
    ossimViewInterface(0),
    theViewProjection(0)
{
   theUlEastingNorthing.makeNan();
   theLrEastingNorthing.makeNan();
   theCutType = OSSIM_RECTANGLE_NULL_OUTSIDE;
}

ossimEastingNorthingCutter::~ossimEastingNorthingCutter()
{
   if(theViewProjection)
   {
      delete theViewProjection;
      theViewProjection = 0;
   }
}

void ossimEastingNorthingCutter::setEastingNorthingRectangle(const ossimDpt& ul,
                                                             const ossimDpt& lr)
{
   theUlEastingNorthing = ul;
   theLrEastingNorthing = lr;

   transformVertices();
}

ossimDpt ossimEastingNorthingCutter::getUlEastingNorthing()const
{
   return theUlEastingNorthing;
}

ossimDpt ossimEastingNorthingCutter::getLrEastingNorthing()const
{
   return theLrEastingNorthing;
}


void ossimEastingNorthingCutter::initialize()
{
   ossimRectangleCutFilter::initialize();
   transformVertices();
}

bool ossimEastingNorthingCutter::saveState(ossimKeywordlist& kwl,
                                           const char* prefix)const
{
   kwl.add(prefix,
           "ul_en",
           ossimString::toString(theUlEastingNorthing.x) + " " +
           ossimString::toString(theUlEastingNorthing.y),
           true);
   kwl.add(prefix,
           "lr_en",
           ossimString::toString(theLrEastingNorthing.x) + " " +
           ossimString::toString(theLrEastingNorthing.y),
           true);

   return ossimRectangleCutFilter::saveState(kwl, prefix);
}

bool ossimEastingNorthingCutter::loadState(const ossimKeywordlist& kwl,
                                           const char* prefix)
{
   const char *ul = kwl.find(prefix,
                             "ul_en");
   const char *lr = kwl.find(prefix,
                             "lr_en");

   theUlEastingNorthing.makeNan();
   theLrEastingNorthing.makeNan();
   
   if(ul)
   {
      istringstream in(ul);

      in >> theUlEastingNorthing.x >> theUlEastingNorthing.y;
   }
   
   if(lr)
   {
      istringstream in(lr);

      in >> theLrEastingNorthing.x >> theLrEastingNorthing.y;
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimEastingNorthingCutter::loadState DEBUG:"
         << "\ntheUlEastingNorthing:  " << theUlEastingNorthing
         << "\ntheLrEastingNorthing:  " << theLrEastingNorthing
         << endl;
   }

   return ossimRectangleCutFilter::loadState(kwl, prefix);
}

bool ossimEastingNorthingCutter::setView(ossimObject* baseObject,
                                         bool ownsTheView)
{
   ossimProjection* tempProj = PTR_CAST(ossimProjection,
                                        baseObject);
   if(!tempProj)
   {
      return false;
   }
   
   if(theViewProjection)
   {
      delete theViewProjection;
      theViewProjection = 0;
   }

   if(ownsTheView)
   {
      theViewProjection = tempProj;
   }
   else
   {
      theViewProjection = (ossimProjection*)tempProj->dup();
   }

   transformVertices();

   return true;
}

ossimObject* ossimEastingNorthingCutter::getView()
{
   return theViewProjection;
}

const ossimObject* ossimEastingNorthingCutter::getView()const
{
   return theViewProjection;
}

void ossimEastingNorthingCutter::transformVertices()
{
   theRectangle.makeNan();

   if(theUlEastingNorthing.hasNans()||
      theLrEastingNorthing.hasNans()||
      (!theViewProjection))
   {
      return;
   }

   ossimMapProjection* mapProj = PTR_CAST(ossimMapProjection,
                                          theViewProjection);
   ossimDpt ul;
   ossimDpt lr;
   ossimDpt easting;
   ossimDpt northing;
   if(mapProj)
   {
      mapProj->eastingNorthingToLineSample(theUlEastingNorthing, ul);
      mapProj->eastingNorthingToLineSample(theLrEastingNorthing, lr);

      ossimDrect drect(ul.x, ul.y, lr.x, lr.y);

      theRectangle = drect;

      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimEastingNorthingCutter::transformVertices DEBUG:"
            << "original drect:  " << drect
            << "\ntheRectangle:  " << theRectangle
            << endl;
      }
   }
}
