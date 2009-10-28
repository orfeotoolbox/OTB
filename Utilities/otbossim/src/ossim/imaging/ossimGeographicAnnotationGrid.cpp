#include <ossim/imaging/ossimGeographicAnnotationGrid.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/imaging/ossimRgbImage.h>
#include <ossim/base/ossimGrect.h>
#include <ossim/imaging/ossimAnnotationLineObject.h>
#include <ossim/imaging/ossimAnnotationGdBitmapFont.h>
#include <ossim/base/ossimDms.h>
#include <ossim/base/ossimTrace.h>


static ossimTrace traceDebug("ossimGeographicAnnotationGrid:debug");

ossimGeographicAnnotationGrid::ossimGeographicAnnotationGrid()
   : ossimAnnotationObject(),
     theViewProjection(NULL),
     theGroundRect(0,0,0,0),
     theDeltaLatSpacing(1.0/60.0), // every minute
     theDeltaLonSpacing(1.0/60.0)
{
}

void ossimGeographicAnnotationGrid::draw(ossimRgbImage& anImage)const

{
   if(!anImage.getImageData())
   {
      return;
   }
   anImage.setDrawColor(theRed, theGreen,theBlue);
   
   if((fabs(theDeltaLatSpacing) <= DBL_EPSILON)||
      (fabs(theDeltaLonSpacing) <= DBL_EPSILON))
   {
      return;
   }

//   ossimDrect imageRect = anImage.getImageData()->getImageRectangle();
   // If the view projection exists we will use it to
   // compute the bounding ground rect and find all lines
   // that overlap that area in the lat and lon direction
   // and draw them.
   //
   // I the projection doesn't exist we will assume that
   // the geographic grid to generate will be 0,0 is origin
   // -180lon and 90lat.
   //
   if(theViewProjection.valid())
   {
      ossimRefPtr<ossimAnnotationLineObject> line=NULL;
      ossimRefPtr<ossimAnnotationGdBitmapFont> font;
      
      line = new ossimAnnotationLineObject();
      font = new ossimAnnotationGdBitmapFont();
      
      ossimDpt start;
      ossimDpt end;
      ossimGpt groundUpper(0,0,0,theGroundRect.ul().datum());
      ossimGpt groundLower(0,0,0,theGroundRect.ul().datum());
      double height = theGroundRect.height();
      double width  = theGroundRect.width();
      double lat = theGroundRect.ul().latd();
//      ossimDrect imageRect = anImage.getImageData()->getImageRectangle();
      double lon;

      // used for the font spacing
      ossimDrect boundingRect;
      
      // we will go across the longitudinal direction first
      for(lon = theGroundRect.ul().lond()+theDeltaLonSpacing;
          lon <= theGroundRect.ur().lond();
          lon += theDeltaLonSpacing)
      {
         groundUpper.lond(lon);
         groundUpper.latd(lat);
         groundLower.lond(lon);
         groundLower.latd(lat-height);

         theViewProjection->worldToLineSample(groundUpper, start);
         theViewProjection->worldToLineSample(groundLower, end);

         line->setLine(start, end);
         line->draw(anImage);
      }
      lon = theGroundRect.ul().lond();

      // now go accross longitude and place the
      // text in.
      for(lon = theGroundRect.ul().lond()+theDeltaLonSpacing;
          lon <= theGroundRect.ur().lond();
          lon += theDeltaLonSpacing)
      {
         groundUpper.lond(lon);
         groundUpper.latd(lat);
         groundLower.lond(lon);
         groundLower.latd(lat-height);

         theViewProjection->worldToLineSample(groundUpper, start);
         theViewProjection->worldToLineSample(groundLower, end);
         font->setText(ossimDms(groundUpper.lond(), false).toString("ddd@mm'ss.ssss\"C"));
         font->getBoundingRect(boundingRect);
         start.y -= boundingRect.height();
         font->setCenterTextPosition(start);
         font->draw(anImage);

         end.y   += boundingRect.height();
         font->setCenterText(end,
                             ossimDms(groundLower.lond(), false).toString("ddd@mm'ss.ssssC"));
         font->draw(anImage);
      }
      lon = theGroundRect.ul().lond();

      ossimGpt groundLeft;
      ossimGpt groundRight;
      
      // Now go down lat direction
      for(lat =  theGroundRect.ul().latd()-theDeltaLatSpacing;
          lat >= theGroundRect.lr().latd();
          lat -= theDeltaLatSpacing)
      {
         groundLeft.lond(lon);
         groundLeft.latd(lat);
         groundRight.lond(lon+width);
         groundRight.latd(lat);

         theViewProjection->worldToLineSample(groundLeft,  start);
         theViewProjection->worldToLineSample(groundRight, end);
         
         line->setLine(start, end);
         line->draw(anImage);
      }

      // now go down lat direction font
      for(lat =  theGroundRect.ul().latd()-theDeltaLatSpacing;
          lat >= theGroundRect.lr().latd();
          lat -= theDeltaLatSpacing)
      {
         groundLeft.lond(lon);
         groundLeft.latd(lat);
         groundRight.lond(lon+width);
         groundRight.latd(lat);

         theViewProjection->worldToLineSample(groundLeft, start);
         theViewProjection->worldToLineSample(groundRight, end);

         
         font->setText(ossimDms(groundLeft.latd()).toString("ddd@mm'ss.ssss\"C"));
         font->getBoundingRect(boundingRect);
         start.x -= boundingRect.width()/2.0-1;
         font->setCenterTextPosition(start);
         font->draw(anImage);

         end.x  += boundingRect.width()/2.0+1;
         font->setCenterText(end,
                             ossimDms(groundRight.latd()).toString("ddd@mm'ss.ssssC"));
         font->draw(anImage);
      }

      line = 0;
      font = 0;
   }
}


void ossimGeographicAnnotationGrid::setViewProjectionInformation(ossimMapProjection* projection,
                                                                 const ossimGrect& boundingGroundRect)
{
   static const char* MODULE = "ossimGeographicAnnotationGrid::setViewProjectionInformation";
   theViewProjection = projection;

   // set the ground and then stretch it
   // to cover which even degree grid we are currently
   // in.
   theGroundRect     = boundingGroundRect;

   // find the even degree grid.
   ossimGrect rect(ossimGpt(90, -180),
                   ossimGpt(-90, 180));

   // make sure that it is within the range of the
   // geographic grid.
   ossimGrect clipRect = rect.clipToRect(boundingGroundRect);

   // for easier math we will shift the points so they
   // are between 0..360Lon and 0..180Lat.  This way
   // we are working with just positive numbers
   double upperLeftLonShift   = clipRect.ul().lond() + 180;
   double upperLeftLatShift   = clipRect.ul().latd() + 90;
   double lowerRightLonShift   = clipRect.lr().lond() + 180;
   double lowerRightLatShift   = clipRect.lr().latd() + 90;

   // now find the even boundaries
   double upperLeftLonCell = floor(upperLeftLonShift/theDeltaLonSpacing)*
                             theDeltaLonSpacing;
   double upperLeftLatCell  = floor(upperLeftLatShift/theDeltaLatSpacing)*
                              theDeltaLatSpacing;
   double lowerRightLonCell = floor(lowerRightLonShift/theDeltaLonSpacing)*
                              theDeltaLonSpacing;
   double lowerRightLatCell  = floor(lowerRightLatShift/theDeltaLatSpacing)*
                               theDeltaLatSpacing;

   // now adjust them by 1 boundary distance.
   upperLeftLonCell  -= theDeltaLonSpacing;
   upperLeftLatCell  += theDeltaLatSpacing;
   lowerRightLonCell += theDeltaLonSpacing;
   lowerRightLatCell -= theDeltaLatSpacing;

   // now clamp to the range of the lat and lon
   upperLeftLonCell  = (upperLeftLonCell<0?0:upperLeftLonCell);
   upperLeftLatCell  = (upperLeftLatCell>180?180:upperLeftLatCell);
   lowerRightLonCell = (lowerRightLonCell>360?360:lowerRightLonCell);
   lowerRightLatCell = (lowerRightLatCell<0?0:lowerRightLatCell);

   // now shift them back into range
   upperLeftLonCell  -= 180;
   upperLeftLatCell  -= 90;
   lowerRightLonCell -= 180;
   lowerRightLatCell -= 90;
   
   const ossimDatum* datum = theGroundRect.ul().datum();
   theGroundRect = ossimGrect(upperLeftLatCell, upperLeftLonCell,
                              lowerRightLatCell, lowerRightLonCell,
                              datum);

   if(traceDebug())
   {
      CLOG << "Ground Rect: " << theGroundRect << endl;
   }
   computeBoundingRect();
}

void ossimGeographicAnnotationGrid::getBoundingRect(ossimDrect& rect)const
{
   rect = theBoundingRect;
}

void ossimGeographicAnnotationGrid::computeBoundingRect()
{
   static const char* MODULE = "ossimGeographicAnnotationGrid::computeBoundingRect";
   
   if(theViewProjection.valid())
   {
      vector<ossimDpt> points(4);

      theViewProjection->worldToLineSample(theGroundRect.ul(), points[0]);
      theViewProjection->worldToLineSample(theGroundRect.ll(), points[1]);
      theViewProjection->worldToLineSample(theGroundRect.lr(), points[2]);
      theViewProjection->worldToLineSample(theGroundRect.ur(), points[3]);

      // now solve the bounding rect in view space.
      theBoundingRect = ossimDrect(points);

      // for now we will add a border for the labelling
      // of lat lon readouts.  We need a better border
      // computation that checks exactly what we need
      // based on font sizes.
      //
      ossimDpt ul = theBoundingRect.ul();
      ossimDpt lr = theBoundingRect.lr();

      
      ossimRefPtr<ossimAnnotationGdBitmapFont> font = new ossimAnnotationGdBitmapFont();;
      font->setCenterText(ossimDpt(0,0),"ddd@mm'ss.ssssC");
      ossimDrect boundingRect;
      font->getBoundingRect(boundingRect);
      font = 0;
      
      theBoundingRect = ossimDrect(ul.x - boundingRect.width(),
                                   ul.y - boundingRect.height(),
                                   lr.x + boundingRect.width(),
                                   lr.y + boundingRect.height());

      if(traceDebug())
      {
         CLOG << " bounding rect: " << theBoundingRect << endl;
      }
   }
}

std::ostream& ossimGeographicAnnotationGrid::print(std::ostream& out)const
{
   return out;
}
