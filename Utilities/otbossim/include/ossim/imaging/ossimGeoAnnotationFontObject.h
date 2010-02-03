//*************************************************************************
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimGeoAnnotationFontObject.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimGeoAnnotationFontObject_HEADER
#define ossimGeoAnnotationFontObject_HEADER
#include <ossim/imaging/ossimGeoAnnotationObject.h>
#include <ossim/imaging/ossimAnnotationFontObject.h>

class ossimFont;

class ossimGeoAnnotationFontObject : public ossimGeoAnnotationObject
{
public:
   ossimGeoAnnotationFontObject();
   
   
   ossimGeoAnnotationFontObject(const ossimGpt& location,
                                const ossimString& s,
                                const ossimIpt& pixelSize = ossimIpt(12, 12),
                                double rotation = 0.0,
                                const ossimDpt& scale = ossimDpt(1.0, 1.0),
                                const ossimDpt& shear = ossimDpt(0.0,0.0),
                                ossim_uint8 r=255,
                                ossim_uint8 g=255,
                                ossim_uint8 b=255);
   ossimGeoAnnotationFontObject(const ossimGeoAnnotationFontObject& rhs);
   
   virtual ossimObject* dup()const;

   ossimAnnotationFontObject* getFontObject();

   void setFont(ossimFont* font);

   ossimFont* getFont();

   virtual void setPointSize(const ossimIpt& size);

   virtual void setRotation(double rotation);

   virtual void setScale(const ossimDpt& scale);

   virtual void setShear(const ossimDpt& shear);
   
   virtual void setColor(ossim_uint8 r,
                         ossim_uint8 g,
                         ossim_uint8 b);

   virtual void transform(ossimImageGeometry* projection);

   virtual void setCenterGround(const ossimGpt& gpt);
   
   virtual void draw(ossimRgbImage& anImage)const;
   
   virtual void computeBoundingRect();
   
   virtual void getBoundingRect(ossimDrect& rect)const;
   
   virtual ossimAnnotationObject* getNewClippedObject(
      const ossimDrect& rect)const;
   
   virtual bool intersects(const ossimDrect& rect)const;
   
   virtual void applyScale(double x, double y);
   
   /**
    * Saves the current state of this object.
    *
    * @see loadState for keywords.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;
   
   /**
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    *
    * Keywords:
    *
    * For center format is: ( latitude, longitude, height, datum_code )
    * center: ( 42.01, -90.71, 0.0, WGE )
    *
    * For point_size format is: "( x, y )"
    * point_size:  ( 18, 18 )
    *
    * For scale format is: "( x, y )"
    * scale: ( 1.0, 1.0 )
    *
    * For shear format is: "( x, y )"
    * shear: ( 0.0, 0.0 )
    *
    * text: Airport
    *
    * USE ONE OF family_name:
    * family_name: URW Palladio L
    * family_name: gd sans
    * family_name: URW Bookman L
    * family_name: Nimbus Roman No9 L
    * family_name: Nimbus Sans L
    * family_name: URW Palladio L
    * family_name: Century Schoolbook L
    * family_name: Nimbus Mono L
    * family_name: Nimbus Sans L Condensed
    *
    * USE ONE OF style_name:
    * style_name:  normal
    * style_name:  bold
    * style_name:  italic
    * style_name:  bold italic
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
   
protected:
   virtual ~ossimGeoAnnotationFontObject();
   
   ossimGpt                   theCenterGround;
   ossimRefPtr<ossimFont>     theFont;
   ossimFontInformation       theFontInfo;
   ossimRefPtr<ossimAnnotationFontObject> theAnnotationFontObject;

TYPE_DATA
};

#endif
