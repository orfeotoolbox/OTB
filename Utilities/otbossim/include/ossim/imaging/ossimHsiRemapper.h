//*******************************************************************
// Copyright (C) 2002 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description:
//
// Remapper to adjust hue, saturation and intensity.
//
//*************************************************************************
// $Id: ossimHsiRemapper.h 19689 2011-05-31 15:55:21Z gpotts $

#ifndef ossimHsiRemapper_HEADER
#define ossimHsiRemapper_HEADER

#include <ossim/imaging/ossimImageSourceFilter.h>

class OSSIMDLLEXPORT ossimHsiRemapper : public ossimImageSourceFilter
{
public:
   enum
   {
      RED      = 0,
      YELLOW   = 1,
      GREEN    = 2,
      CYAN     = 3,      
      BLUE     = 4,
      MAGENTA  = 5,
      ALL      = 6
   };
   
   ossimHsiRemapper();

   virtual ossimString getLongName()  const;
   virtual ossimString getShortName() const;

   virtual void initialize();
   /*---------------------- PROPERTY INTERFACE ---------------------------*/
   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;
   /*---------------------- END PROPERTY INTERFACE ---------------------------*/
   
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& tile_rect,
                                   ossim_uint32 resLevel=0);

   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;
   
   /*!
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
   
   
   ossim_uint32 getNumberOfOutputBands() const;

   virtual ostream& print(ostream& os) const;

   friend ostream& operator << (ostream& os,  const ossimHsiRemapper& hr);
   
   void setHueOffset               (int color_group, double offset);
   void setHueLowRange             (int color_group, double range);
   void setHueHighRange            (int color_group, double range);
   void setHueBlendRange           (int color_group, double range);
   void setSaturationOffset        (int color_group, double offset);
   void setIntensityOffset         (int color_group, double offset);
   
   void setMasterHueOffset         (double offset);
   void setMasterSaturationOffset  (double offset);
   void setMasterIntensityOffset   (double offset);
   void setMasterIntensityLowClip  (double clip);
   void setMasterIntensityHighClip (double clip);

   void setRedHueOffset            (double offset);
   void setRedHueLowRange          (double range);
   void setRedHueHighRange         (double range);
   void setRedHueBlendRange        (double range);
   void setRedSaturationOffset     (double offset);
   void setRedIntensityOffset      (double offset);

   void setYellowHueOffset         (double offset);
   void setYellowHueLowRange       (double range);
   void setYellowHueHighRange      (double range);
   void setYellowHueBlendRange     (double range);
   void setYellowSaturationOffset  (double offset);
   void setYellowIntensityOffset   (double offset);

   void setGreenHueOffset          (double offset);
   void setGreenHueLowRange        (double range);
   void setGreenHueHighRange       (double range);
   void setGreenHueBlendRange      (double range);
   void setGreenSaturationOffset   (double offset);
   void setGreenIntensityOffset    (double offset);

   void setCyanHueOffset           (double offset);
   void setCyanHueLowRange         (double range);
   void setCyanHueHighRange        (double range);
   void setCyanHueBlendRange       (double range);
   void setCyanSaturationOffset    (double offset);
   void setCyanIntensityOffset     (double offset);

   void setBlueHueOffset           (double offset);
   void setBlueHueLowRange         (double range);
   void setBlueHueHighRange        (double range);
   void setBlueHueBlendRange       (double range);
   void setBlueSaturationOffset    (double offset);
   void setBlueIntensityOffset     (double offset);

   void setMagentaHueOffset        (double offset);
   void setMagentaHueLowRange      (double range);
   void setMagentaHueHighRange     (double range);
   void setMagentaHueBlendRange    (double range);
   void setMagentaSaturationOffset (double offset);
   void setMagentaIntensityOffset  (double offset);

   double getHueOffset             (int color_group) const;
   double getHueLowRange           (int color_group) const;
   double getHueHighRange          (int color_group) const;
   double getHueBlendRange         (int color_group) const;
   double getSaturationOffset      (int color_group) const;
   double getIntensityOffset       (int color_group) const;
   
   double getMasterHueOffset          () const;
   double getMasterSaturationOffset   () const;
   double getMasterIntensityOffset    () const;
   double getMasterIntensityLowClip   () const;
   double getMasterIntensityHighClip  () const;
   
   double getRedHueOffset             () const;
   double getRedHueLowRange           () const;
   double getRedHueHighRange          () const;
   double getRedHueBlendRange         () const;
   double getRedSaturationOffset      () const;
   double getRedIntensityOffset       () const;

   double getYellowHueOffset          () const;
   double getYellowHueLowRange        () const;
   double getYellowHueHighRange       () const;
   double getYellowHueBlendRange      () const;
   double getYellowSaturationOffset   () const;
   double getYellowIntensityOffset    () const;

   double getGreenHueOffset           () const;
   double getGreenHueLowRange         () const;
   double getGreenHueHighRange        () const;
   double getGreenHueBlendRange       () const;
   double getGreenSaturationOffset    () const;
   double getGreenIntensityOffset     () const;

   double getCyanHueOffset            () const;
   double getCyanHueLowRange          () const;
   double getCyanHueHighRange         () const;
   double getCyanHueBlendRange        () const;
   double getCyanSaturationOffset     () const;
   double getCyanIntensityOffset      () const;

   double getBlueHueOffset            () const;
   double getBlueHueLowRange          () const;
   double getBlueHueHighRange         () const;
   double getBlueHueBlendRange        () const;
   double getBlueSaturationOffset     () const;
   double getBlueIntensityOffset      () const;

   double getMagentaHueOffset         () const;
   double getMagentaHueLowRange       () const;
   double getMagentaHueHighRange      () const;
   double getMagentaHueBlendRange     () const;
   double getMagentaSaturationOffset  () const;
   double getMagentaIntensityOffset   () const;

   void   setWhiteObjectClip          (double clip);
   double getWhiteObjectClip          () const;
   void   resetWhiteObjectClip        ();

   void resetGroup(int color_group);
   void resetAll();
   void resetMaster();
   void resetRed();
   void resetYellow();
   void resetGreen();
   void resetCyan();
   void resetBlue();
   void resetMagenta();
   
protected:
   virtual ~ossimHsiRemapper();
   void   allocate(const ossimIrect& rect);
   void   verifyEnabled();
   double calculateMinNormValue();

   bool theValidFlag;
   ossimRefPtr<ossimImageData> theTile;
   double*                     theBuffer;
   double                      theNormalizedMinPix;

   double theMasterHueOffset;
   double theMasterSaturationOffset;
   double theMasterIntensityOffset;
   double theMasterIntensityLowClip;
   double theMasterIntensityHighClip;

   double theRedHueOffset;
   double theRedHueLowRange;
   double theRedHueHighRange;
   double theRedHueBlendRange;
   double theRedSaturationOffset;
   double theRedIntensityOffset;

   double theYellowHueOffset;
   double theYellowHueLowRange;
   double theYellowHueHighRange;
   double theYellowHueBlendRange;
   double theYellowSaturationOffset;
   double theYellowIntensityOffset;

   double theGreenHueOffset;
   double theGreenHueLowRange;
   double theGreenHueHighRange;
   double theGreenHueBlendRange;
   double theGreenSaturationOffset;
   double theGreenIntensityOffset;

   double theCyanHueOffset;
   double theCyanHueLowRange;
   double theCyanHueHighRange;
   double theCyanHueBlendRange;
   double theCyanSaturationOffset;
   double theCyanIntensityOffset;

   double theBlueHueOffset;
   double theBlueHueLowRange;
   double theBlueHueHighRange;
   double theBlueHueBlendRange;
   double theBlueSaturationOffset;
   double theBlueIntensityOffset;

   double theMagentaHueOffset;
   double theMagentaHueLowRange;
   double theMagentaHueHighRange;
   double theMagentaHueBlendRange;
   double theMagentaSaturationOffset;
   double theMagentaIntensityOffset;

   double theWhiteObjectClip;

   TYPE_DATA
};

#endif
