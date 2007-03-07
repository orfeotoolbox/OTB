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
// $Id: ossimRgbImage.h,v 1.14 2005/05/20 20:22:41 dburken Exp $
#ifndef ossimRgbImage_HEADER
#define ossimRgbImage_HEADER
#include <base/data_types/ossimIpt.h>
#include <imaging/ossimImageData.h>

#include <vector>
using namespace std;

class OSSIM_DLL ossimRgbImage
{
public:
   ossimRgbImage();
   ossimRgbImage(ossimRefPtr<ossimImageData>& currentImageData);
   
   virtual ~ossimRgbImage();

   /**
    * Will construct a new single band image data object.
    */
   virtual void createNewGrey(long width, long height);

   /**
    * Will construct a new 3 band rgb data object.
    */
   virtual void createNewTrueColor(long width, long height);

   /**
    * Will initilize any pre-computations that are needed.
    * Example: setting up the origin for adjustments and
    *          setting up the offsets.
    */
    virtual void initialize();
    
   /**
    * This will plot a pixel and will do inside outside compares.
    * This assumes the pixel has already been put into a relative
    * 0,0 to the upper left corner of the image data.
    *
    * example: if the upperleft corner or origin of the image was
    *          34, 55 then it assumes that the points passed in are
    *          already relative to that corner not absolute.
    */
   inline void slowPlotPixel(long x,
                             long y,
                             unsigned char r,
                             unsigned char g,
                             unsigned char b);

   /**
    * same thing as slowPlotPixel but without the inside outside compare
    */
   inline void fastPlotPixel(long x,
                             long y,
                             unsigned char r,
                             unsigned char g,
                             unsigned char b);
   
   /**
    * All the drawing algorithms have thickness.  So you can draw a line
    * that has thickness of 3 pixels if you want.  Note: the thickness is in
    * pixels
    */
   void setThickness(long thickness){theThickness = thickness;}
                         

   /**
    * Allows you to change the image data that this RgbImage object
    * operates on.  You can specify if the image owns the data or not.
    * if it doesn't own the data then it will not destroy it when the
    * data is changed to a different data object or if this object
    * is deleted.
    */ 
   void setCurrentImageData(ossimRefPtr<ossimImageData>& imageData);
   /**
    * Will return the image data.
    */
   ossimRefPtr<ossimImageData> getImageData();

   /**
    * We will cut and paste the code from
    * drawFilledPolygon(ossimIpt *p, int n).
    */
   void drawFilledPolygon(const vector<ossimDpt>& p);
   /**
    * We will cut and paste the code from
    * drawFilledPolygon(ossimIpt *p, int n).
    */
   void drawFilledPolygon(const vector<ossimIpt>& p);

   
   /**
    * This will draw a polygon and fill it with the specified color.
    */
   void drawFilledPolygon(ossimDpt *p,
                          int n);
   /**
    * This will draw a polygon and fill it with the specified color.
    */
   void drawFilledPolygon(ossimIpt *p,
                          int n);

   /**
    * will draw an arc.  The cx and cy specifies the center point in image
    * space.  The parameters w, h are the width and height of the arc.
    * The s and e are the starting and ending angles.  So if I want to
    * draw a complete circle at point 45, 45 with width 10 and height 23 then
    * we set the angles s =0 and e = 360:
    * drawArc(45, 45, 10, 23, 0, 360).
    *
    */
   void drawArc(double cx,
                double cy,
                double w,
                double h,
                double s,
                double e);

   /**
    * will draw an arc.  The cx and cy specifies the center point in image
    * space.  The parameters w, h are the width and height of the arc.
    * The s and e are the starting and ending angles.  So if I want to
    * draw a complete circle at point 45, 45 with width 10 and height 23 then
    * we set the angles s =0 and e = 360:
    * drawArc(45, 45, 10, 23, 0, 360).
    *
    */
   void drawArc(int cx,
                int cy,
                int w,
                int h,
                int s,
                int e);
   
   /**
    * This will draw a filled arc.  See drawArc for documentation of
    * parameters.
    */
   void drawFilledArc(double cx,
                      double cy,
                      double w,
                      double h,
                      double s,
                      double e);

   /**
    * This will draw a filled arc.  See drawArc for documentation of
    * parameters.
    */
   void drawFilledArc(int cx,
                      int cy,
                      int w,
                      int h,
                      int s,
                      int e);
   
   void drawPolygon(const vector<ossimDpt>& p);
   void drawPolygon(const vector<ossimIpt>& p);
   
   /**
    * Draws a polygon.  The first argument is an array of points.  The second
    * argument is the number of points and the rest are the rgb color values.
    * theColor used is set by the setDrawColor.
    */
   void drawPolygon(ossimDpt *p,
                    int n);

   /**
    * Draws a polygon.  The first argument is an array of points.  The second
    * argument is the number of points and the rest are the rgb color values.
    * theColor used is set by the setDrawColor.
    */
   void drawPolygon(ossimIpt *p,
                    int n);
   
   /**
    * About all the draw routines will call draw line.  it takes a start and
    * an end point and a color value.
    * theColor used is set by the setDrawColor.
    */
   void drawLine(double x1,
                 double y1,
                 double x2,
                 double y2);

   /**
    * About all the draw routines will call draw line.  it takes a start and
    * an end point and a color value.
    * theColor used is set by the setDrawColor.
    */
   void drawLine(int x1,
                 int y1,
                 int x2,
                 int y2);

   /**
    * About all the draw routines will call draw line.  it takes a start and
    * an end point and a color value.
    * theColor used is set by the setDrawColor.
    */
   void drawLine(const ossimIpt& start,
                 const ossimIpt& end);
   /**
    * About all the draw routines will call draw line.  it takes a start and
    * an end point and a color value.
    * theColor used is set by the setDrawColor.
    */
   void drawLine(const ossimDpt& start,
                 const ossimDpt& end);


   /**
    * Will draw a rectangle.  The color used is set by setDrawColor.
    */
   void drawRectangle(double x1,
                      double y1,
                      double x2,
                      double y2);

   /**
    * Will draw a rectangle.  The color used is set by setDrawColor.
    */
   void drawRectangle(int x1,
                      int y1,
                      int x2,
                      int y2);
   
   /**
    * Will draw a filled rectangle with the current draw color.
    * Use setDrawColor to set it.
    */
   void drawFilledRectangle(double x1,
                            double y1,
                            double x2,
                            double y2);

   /**
    * Will draw a filled rectangle with the current draw color.
    * Use setDrawColor to set it.
    */
   void drawFilledRectangle(int x1,
                            int y1,
                            int x2,
                            int y2);
   
   /**
    * Will fill the entire data object with the specified color set in
    * setDrawColor.
    */ 
   void fill();

   void setDrawColor(unsigned char r = 255,
                     unsigned char g = 255,
                     unsigned char b = 255);
   /**
    * Will take the point passed in and translate
    * to a 0,0.
    * Example:
    *   Let's say that we have origin 45,34 for the
    *   image's upper left corner. then a call to this
    *   method will translate the point passed in
    *   x-45, y-34
    */
//   inline void translateToOrigin(long& x, long& y);

   /**
    * Will take the point passed in and translate
    * to a 0,0.
    * Example:
    *   Let's say that we have origin 45,34 for the
    *   image's upper left corner. then a call to this
    *   method will translate the point passed in
    *   x-45, y-34
    */
//   inline void translateToOrigin(ossimDpt& pt);
   const ossimRefPtr<ossimImageData> getImageData()const;

protected:

   /**
    * This object operates on the ossimImageData.  Note the
    * ossimImageData is a band separate so the bands follow sequentially:
    * all the reds followed by all the blues followed by all greens
    */
   ossimRefPtr<ossimImageData> theImageData;

   /**
    * This will hold precomputed offsets to the
    * start of each row.  This will get rid of a multiplication
    * per access. Since we will be accessing on a per pixel basis
    * this will be needed.
    */
   long            *theOffsets;

   /**
    * This is a fast access to the start of each band.
    */
   unsigned char*   theBands[3];

   /**
    * This is here so we don't have to call theWidth and theHeight
    * of the ossimImageData.  
    */ 
   long             theWidth;

   /**
    * This is here so we don't have to call theWidth and theHeight
    * of the ossimImageData.  
    */ 
   long             theHeight;

   /**
    * Holds the draw thickness.
    */
   long             theThickness;


   /**
    * The red component of the color used in drawing the shapes
    */
   unsigned char theRed;

   /**
    * The green component of the color used in drawing the shapes
    */
   unsigned char theGreen;

   /**
    * The blue component of the color used in drawing the shapes
    */
   unsigned char theBlue;
};

inline void ossimRgbImage::slowPlotPixel(long x,
                                         long y,
                                         unsigned char r,
                                         unsigned char g,
                                         unsigned char b)
{
   if((x > -1) && (x < theWidth) &&
      (y > -1) && (y < theHeight))
   {
      theBands[0][theOffsets[y]+x] = r;
      theBands[1][theOffsets[y]+x] = g;
      theBands[2][theOffsets[y]+x] = b;
   }
}

inline void ossimRgbImage::fastPlotPixel(long x,
                                         long y,
                                         unsigned char r,
                                         unsigned char g,
                                         unsigned char b)
{
   theBands[0][theOffsets[y]+x] = r;
   theBands[1][theOffsets[y]+x] = g;
   theBands[2][theOffsets[y]+x] = b;
}

#endif
