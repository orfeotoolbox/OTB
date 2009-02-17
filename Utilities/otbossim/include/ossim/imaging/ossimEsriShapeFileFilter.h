//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimEsriShapeFileFilter.h 9094 2006-06-13 19:12:40Z dburken $

#ifndef ossimEsriShapeFileFilter_HEADER
#define ossimEsriShapeFileFilter_HEADER
#include <map>
#include <ossim/imaging/ossimGeoAnnotationSource.h>
#include <ossim/base/ossimViewInterface.h>
#include <ossim/vec/shapefil.h>
#include <ossim/vec/ossimShapeFile.h>
#include <ossim/base/ossimRgbVector.h>

class ossimGeoAnnotationObject;
class ossimAnnotationObject;

/*!
 * class ossimEsriShapeFileFilter
 *
 * This class is used to render shape files.  Iif this filter has
 * an input connection to an ossimImageSourceInterface then it will
 * draw any vectors over the input tile.  If it is not connected
 * it will just render its vector data to a tile and return it.
 * <pre>
 * supported keywords:
 *
 *   max_quadtree_levels:    // number of levels for faster hit testing
 *                           // defaults to 10
 *
 *   fill_flag:              // 1 for true 0 for false.  Default is false
 *                           // and specifies how all closed surfaces are
 *                           // to be drawn
 *
 *   feature_name:           // currently not used.
 *
 *   pen_color:              // specified in Red Green Blue and is the color used
 *                           // if the fill flag is not specified.  Future changes
 *                           // might be to include this as an outline color for
 *                           // filled objects.  Default is white. Example: 255 255 255
 *
 *   brush_color:            // Color used by objects that have the fill flag enabled
 *                           // format is Red Green Blue. Default is white.
 *
 *   line_thickness:         // line drawing thickness in pixels.
 *
 *   border_size:            // can be us feet, feet, meters, degrees,
 *                           // will automatically expand all polygons by the
 *                           // specified size.  Uses the centroid to estimate
 *                           // non degree units when converting to degrees.
 *
 *   point_width_height:     // Specifies the width and height of a point object in
 *                           // pixels.  Default is 1 and has the following format
 *                           // w h.  Example 1 1
 *
 *   filename:               // The esri shape file to be used
 *
 * example Keyword list:  See ossimAnnotationSource for any additional keywords
 *
 *
 * brush_color:  255 255 255
 * feature_name:
 * filename:
 * fill_flag:  0
 * max_quadtree_levels:  10
 * pen_color:  255 255 255
 * point_width_height:  1 1
 * thickness:  1
 * border_size: 25 meters
 * type:  ossimEsriShapeFileFilter
 *
 * </pre>
 */
class ossimEsriShapeFileFilter : public ossimAnnotationSource,
                                 public ossimViewInterface
{
public:
   ossimEsriShapeFileFilter(ossimImageSource* inputSource=NULL);
   ~ossimEsriShapeFileFilter();

   virtual bool setView(ossimObject* baseObject,
                        bool ownsTheView = false);

   virtual ossimObject*       getView();
   virtual const ossimObject* getView()const;

   virtual bool addObject(ossimAnnotationObject* anObject);
   virtual void transformObjects(ossimProjection* projection=0);
   virtual void setProjection(ossimProjection* projection,
                              bool ownsProjectionFlag=false);
   virtual bool getImageGeometry(ossimKeywordlist& kwl,
                                 const char* prefix = 0);
   virtual ossimIrect getBoundingRect(ossim_uint32 resLevel=0)const;
   virtual void computeBoundingRect();

   virtual void drawAnnotations(ossimRefPtr<ossimImageData> tile);
   /*!
    * Will delete the current objects within the layer and add all
    * objects in the passed in file.
    */
   virtual bool loadShapeFile(const ossimFilename& shapeFile);

   virtual ossimFilename getFilename()const
      {
         return theShapeFile.getFilename();
      }
   virtual ossim_int32 getMaxQuadTreeLevels()const
      {
         return theMaxQuadTreeLevels;
      }

   virtual void setMaxQuadTreeLevels(ossim_int32 levels)
      {
         theMaxQuadTreeLevels = (levels>0?levels:10);
      }

   virtual void setBrushColor(const ossimRgbVector& brushColor)
      {
         theBrushColor = brushColor;
      }

   virtual void setPenColor(const ossimRgbVector& penColor)
      {
         thePenColor = penColor;
      }

   virtual ossimRgbVector getPenColor()const
      {
         return thePenColor;
      }

   virtual ossimRgbVector getBrushColor()const
      {
         return theBrushColor;
      }

   virtual bool getFillFlag()const
      {
         return theFillFlag;
      }

   virtual void setFillFlag(bool flag)
      {
         theFillFlag=flag;
      }
   virtual ossimString getFeatureName()const
      {
         return theFeatureName;
      }
   virtual void setFeatureName(const ossimString& name)
      {
         theFeatureName = name;
      }

   virtual void setThickness(ossim_int32 thickness)
      {
         theThickness = thickness >= 0? thickness:1;
      }
   virtual ossim_int32 getThickness()const
      {
         return theThickness;
      }
   virtual void setPointRadius(double r)
      {
         thePointWidthHeight = ossimDpt(fabs(r)*2, fabs(r)*2);
      }
   virtual double getPointRadius()const
      {
         return thePointWidthHeight.x/2.0;
      }

   virtual ossimAnnotationObject* nextObject(bool restart=false)
      {
         if(restart)
         {
            theCurrentObject = theShapeCache.begin();
         }
         else
         {
            if(theCurrentObject != theShapeCache.end())
            {
               ++theCurrentObject;
            }

         }

         if(theCurrentObject == theShapeCache.end())
         {
            return (ossimAnnotationObject*)NULL;
         }
         return (*theCurrentObject).second;
      }


   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=NULL)const;

   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=NULL);
protected:

   /*!
    *  This holds the output projection.  This is used to transform
    * the objects to the projection plane.
    */
   ossimProjection* theViewProjection;

   /*!
    * If the shape file is projected we must know the projector.
    * This will allow us to define the geographic objects correctly.
    */
   ossimProjection* theShapeFileProjection;

   /*!
    * Flag that indicates if we own the view projection.
    */
   bool             theOwnsViewProjectionFlag;

   /*!
    * Specifies if its image/pixel space, Geographic, projected ...etc.
    */
   ossimCoordinateSystemType theCoordinateSystem;

   /*!
    * If its projected this spceifies the type of unit.  We
    * need to know if the projection is in meters, feet, us survey feet
    * ... etc.
    */
   ossimUnitType             theUnitType;

   ossim_SHPTree*                  theTree;

   ossimShapeFile            theShapeFile;

   double                    theMinArray[4];
   double                    theMaxArray[4];

   int                       theMaxQuadTreeLevels;
   ossimRgbVector            thePenColor;
   ossimRgbVector            theBrushColor;
   bool                      theFillFlag;
   ossim_int32               theThickness;
   ossimString               theFeatureName;
   ossimDpt                  thePointWidthHeight;
   double                    theBorderSize;
   ossimUnitType             theBorderSizeUnits;

   mutable std::multimap<int, ossimAnnotationObject*>::iterator theCurrentObject;

   std::multimap<int, ossimAnnotationObject*> theShapeCache;
   ossimDrect theBoundingRect;

   void removeViewProjection();
   void deleteCache();
   void checkAndSetDefaultView();

   virtual void loadPolygon(ossimShapeObject& obj);
   virtual void loadPoint(ossimShapeObject& obj);
   virtual void loadArc(ossimShapeObject& obj);

TYPE_DATA
};

#endif
