//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimCadrgProjection.h 17815 2010-08-03 13:23:14Z dburken $
#ifndef ossimCadrgProjection_HEADER
#define ossimCadrgProjection_HEADER
#include <ossim/projection/ossimMapProjection.h>

class OSSIM_DLL ossimCadrgProjection : public ossimMapProjection
{
public:
   ossimCadrgProjection();

                       
   virtual ossimObject *dup()const;
   virtual ossimGpt inverse(const ossimDpt &eastingNorthing)const;
   virtual ossimDpt forward(const ossimGpt &latLon)const;
   
   virtual ossimDpt worldToLineSample(const ossimGpt &worldPoint)    const;
   virtual ossimGpt lineSampleToWorld(const ossimDpt &projectedPoint)const;
   virtual void worldToLineSample(const ossimGpt &worldPoint,
                                  ossimDpt&       lineSample)const;
   virtual void lineSampleToWorld(const ossimDpt &projectedPoint,
                                  ossimGpt& gpt)const;

   double computeXPixConstant(double scale, long zone)const;
   double computeYPixConstant(double scale)const;
   /*!
    * Method to save the state of an object to a keyword list.
    * Return true if ok or false on error.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;

   /*!
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   
   ossimString getProjectionName()const
      {
         return getClassName();
      }
      
   //! Returns TRUE if principal parameters are within epsilon tolerance.
   virtual bool operator==(const ossimProjection& projection) const;

protected:
   virtual ~ossimCadrgProjection();
   /*
    * Are from the ADRG Mil spec for 1:1,000,000 and
    * are scales for each zone.
    */
   static double theCadrgArcA[8];
   

   static double theOldZoneExtents[10];

   static double theNorthLimit;
   static double theSouthLimit;

   ossim_int32   theCadrgZone;

   ossimDpt thePixelConstant;
   double   theMapScale;

   double theWidth;
   double theHeight;

   ossimDpt theUlLineSample;

   ossimGpt theUlGpt;
   ossimGpt theLlGpt;
   ossimGpt theLrGpt;
   ossimGpt theUrGpt;
   
   void computeParameters();
TYPE_DATA
};

#endif
