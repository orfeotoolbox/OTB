//*******************************************************************
//
// License:  See LICENSE.txt file in the top level directory.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimAnnotationEllipseObject.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimAnnotationEllipseObject_HEADER
#define ossimAnnotationEllipseObject_HEADER

#include <ossim/imaging/ossimAnnotationObject.h>

class OSSIMDLLEXPORT ossimAnnotationEllipseObject : public ossimAnnotationObject
{
public:
   ossimAnnotationEllipseObject(const ossimDpt& center = ossimDpt(0,0),
                                const ossimDpt& widthHeight = ossimDpt(1,1),
                                ossim_float64 azimuthInDegrees=0.0,
                                bool enableFill = false,
                                ossim_uint8 r = 255,
                                ossim_uint8 g = 255,
                                ossim_uint8 b = 255,
                                ossim_uint8 thickness = 1);
   ossimAnnotationEllipseObject(const ossimAnnotationEllipseObject& rhs);

   virtual ossimObject* dup()const;


   virtual void applyScale(double x, double y);

   virtual void draw(ossimRgbImage& anImage)const;

   virtual bool intersects(const ossimDrect& rect)const;

   virtual ossimAnnotationObject* getNewClippedObject(
      const ossimDrect& rect)const;
   
   virtual std::ostream& print(std::ostream& out)const;
   virtual void getBoundingRect(ossimDrect& rect)const;
   virtual void computeBoundingRect();
  /*!
   */
   virtual bool isPointWithin(const ossimDpt& imagePoint)const;
   virtual void setCenterWidthHeight(const ossimDpt& center,
                                     const ossimDpt& widthHeight);

   /**
    * @brief Sets the azimuth.
    * @param azimuth in degrees.
    */
   virtual void setAzimuth(ossim_float64 azimuth);

   /**
    * @brief Gets the azimuth.
    * @return The azimuth in decimal degrees.
    */
   virtual ossim_float64 getAzimuth() const;
   
   virtual void setFillFlag(bool enabled);

   /** @return theFillEnabled flag. */
   virtual bool getFillFlag() const;

   /**
    * @brief Turns on and off drawing of axes on the ellipse.
    * This flag is only used if fill is turned off.
    *
    * @param flag true to enable, false to disable.
    */
   virtual void setDrawAxesFlag(bool flag);

   /** @return The draw axes flag. */
   virtual bool getDrawAxesFlag() const;

   /**
    * Saves the current state of this object.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;

   /**
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
   
protected:
   virtual ~ossimAnnotationEllipseObject();
   ossimDpt      theCenter;
   ossimDpt      theWidthHeight;
   ossim_float64 theAzimuthInDegrees;
   bool          theFillEnabled;
   bool          theDrawAxesFlag;
   ossimDrect    theBoundingRect;

TYPE_DATA
};

#endif
