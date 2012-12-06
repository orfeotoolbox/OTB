//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
// Description:
//
//
// Credits:
//        Much of the implementation within this file was taken from gd
//        We can use it but we need to give credit to the people who wrote
//        the algorithms.
//
// COPYRIGHT STATEMENT FOLLOWS THIS LINE

//      Portions copyright 1994, 1995, 1996, 1997, 1998, 1999, 2000, 2001
//      by Cold Spring Harbor Laboratory. Funded under Grant P41-RR02188 by
//      the National Institutes of Health.
     
//      Portions copyright 1996, 1997, 1998, 1999, 2000, 2001 by
//      Boutell.Com, Inc.
     
//      Portions relating to GD2 format copyright 1999, 2000 Philip Warner.
     
//      Portions relating to PNG copyright 1999, 2000 Greg Roelofs.
     
//      Portions relating to libttf copyright 1999, 2000 John Ellson
//      (ellson@lucent.com).
     
//      Portions relating to JPEG and to color quantization copyright 2000,
//      Doug Becker and copyright (C) 1994-1998, Thomas G. Lane. This
//      software is based in part on the work of the Independent JPEG
//      Group. See the file README-JPEG.TXT for more information.
     
//      Portions relating to WBMP copyright 2000 Maurice Szmurlo and Johan
//      Van den Brande.
     
//      _Permission has been granted to copy, distribute and modify gd in
//      any context without fee, including a commercial application,
//      provided that this notice is present in user-accessible supporting
//      documentation._
     
//      This does not affect your ownership of the derived work itself, and
//      the intent is to assure proper credit for the authors of gd, not to
//      interfere with your productive use of gd. If you have questions,
//      ask. "Derived works" includes all programs that utilize the
//      library. Credit must be given in user-accessible documentation.
     
//      _This software is provided "AS IS."_ The copyright holders disclaim
//      all warranties, either express or implied, including but not
//      limited to implied warranties of merchantability and fitness for a
//      particular purpose, with respect to this code and accompanying
//      documentation.
     
//      Although their code does not appear in gd 2.0.1, the authors wish
//      to thank David Koblas, David Rowley, and Hutchison Avenue Software
//      Corporation for their prior contributions.
     
// END OF COPYRIGHT STATEMENT
//*************************************************************************
// $Id: ossimRgbImage.cpp 21184 2012-06-29 15:13:09Z dburken $

#include <cstdlib>
#include <cmath>
#include <ossim/imaging/ossimRgbImage.h>
#include <ossim/base/ossimCommon.h>

// this should be in another file.  This is from gd's gdtable.c file and has
// precomputations for sine and cosine.  It looks like they multiply the cos and
// sign by 1024 and store the rounded int. 
static const int gdCosT[] =
{
  1024,
  1023,
  1023,
  1022,
  1021,
  1020,
  1018,
  1016,
  1014,
  1011,
  1008,
  1005,
  1001,
  997,
  993,
  989,
  984,
  979,
  973,
  968,
  962,
  955,
  949,
  942,
  935,
  928,
  920,
  912,
  904,
  895,
  886,
  877,
  868,
  858,
  848,
  838,
  828,
  817,
  806,
  795,
  784,
  772,
  760,
  748,
  736,
  724,
  711,
  698,
  685,
  671,
  658,
  644,
  630,
  616,
  601,
  587,
  572,
  557,
  542,
  527,
  512,
  496,
  480,
  464,
  448,
  432,
  416,
  400,
  383,
  366,
  350,
  333,
  316,
  299,
  282,
  265,
  247,
  230,
  212,
  195,
  177,
  160,
  142,
  124,
  107,
  89,
  71,
  53,
  35,
  17,
  0,
  -17,
  -35,
  -53,
  -71,
  -89,
  -107,
  -124,
  -142,
  -160,
  -177,
  -195,
  -212,
  -230,
  -247,
  -265,
  -282,
  -299,
  -316,
  -333,
  -350,
  -366,
  -383,
  -400,
  -416,
  -432,
  -448,
  -464,
  -480,
  -496,
  -512,
  -527,
  -542,
  -557,
  -572,
  -587,
  -601,
  -616,
  -630,
  -644,
  -658,
  -671,
  -685,
  -698,
  -711,
  -724,
  -736,
  -748,
  -760,
  -772,
  -784,
  -795,
  -806,
  -817,
  -828,
  -838,
  -848,
  -858,
  -868,
  -877,
  -886,
  -895,
  -904,
  -912,
  -920,
  -928,
  -935,
  -942,
  -949,
  -955,
  -962,
  -968,
  -973,
  -979,
  -984,
  -989,
  -993,
  -997,
  -1001,
  -1005,
  -1008,
  -1011,
  -1014,
  -1016,
  -1018,
  -1020,
  -1021,
  -1022,
  -1023,
  -1023,
  -1024,
  -1023,
  -1023,
  -1022,
  -1021,
  -1020,
  -1018,
  -1016,
  -1014,
  -1011,
  -1008,
  -1005,
  -1001,
  -997,
  -993,
  -989,
  -984,
  -979,
  -973,
  -968,
  -962,
  -955,
  -949,
  -942,
  -935,
  -928,
  -920,
  -912,
  -904,
  -895,
  -886,
  -877,
  -868,
  -858,
  -848,
  -838,
  -828,
  -817,
  -806,
  -795,
  -784,
  -772,
  -760,
  -748,
  -736,
  -724,
  -711,
  -698,
  -685,
  -671,
  -658,
  -644,
  -630,
  -616,
  -601,
  -587,
  -572,
  -557,
  -542,
  -527,
  -512,
  -496,
  -480,
  -464,
  -448,
  -432,
  -416,
  -400,
  -383,
  -366,
  -350,
  -333,
  -316,
  -299,
  -282,
  -265,
  -247,
  -230,
  -212,
  -195,
  -177,
  -160,
  -142,
  -124,
  -107,
  -89,
  -71,
  -53,
  -35,
  -17,
  0,
  17,
  35,
  53,
  71,
  89,
  107,
  124,
  142,
  160,
  177,
  195,
  212,
  230,
  247,
  265,
  282,
  299,
  316,
  333,
  350,
  366,
  383,
  400,
  416,
  432,
  448,
  464,
  480,
  496,
  512,
  527,
  542,
  557,
  572,
  587,
  601,
  616,
  630,
  644,
  658,
  671,
  685,
  698,
  711,
  724,
  736,
  748,
  760,
  772,
  784,
  795,
  806,
  817,
  828,
  838,
  848,
  858,
  868,
  877,
  886,
  895,
  904,
  912,
  920,
  928,
  935,
  942,
  949,
  955,
  962,
  968,
  973,
  979,
  984,
  989,
  993,
  997,
  1001,
  1005,
  1008,
  1011,
  1014,
  1016,
  1018,
  1020,
  1021,
  1022,
  1023,
  1023
};

static const int gdSinT[] =
{
  0,
  17,
  35,
  53,
  71,
  89,
  107,
  124,
  142,
  160,
  177,
  195,
  212,
  230,
  247,
  265,
  282,
  299,
  316,
  333,
  350,
  366,
  383,
  400,
  416,
  432,
  448,
  464,
  480,
  496,
  512,
  527,
  542,
  557,
  572,
  587,
  601,
  616,
  630,
  644,
  658,
  671,
  685,
  698,
  711,
  724,
  736,
  748,
  760,
  772,
  784,
  795,
  806,
  817,
  828,
  838,
  848,
  858,
  868,
  877,
  886,
  895,
  904,
  912,
  920,
  928,
  935,
  942,
  949,
  955,
  962,
  968,
  973,
  979,
  984,
  989,
  993,
  997,
  1001,
  1005,
  1008,
  1011,
  1014,
  1016,
  1018,
  1020,
  1021,
  1022,
  1023,
  1023,
  1024,
  1023,
  1023,
  1022,
  1021,
  1020,
  1018,
  1016,
  1014,
  1011,
  1008,
  1005,
  1001,
  997,
  993,
  989,
  984,
  979,
  973,
  968,
  962,
  955,
  949,
  942,
  935,
  928,
  920,
  912,
  904,
  895,
  886,
  877,
  868,
  858,
  848,
  838,
  828,
  817,
  806,
  795,
  784,
  772,
  760,
  748,
  736,
  724,
  711,
  698,
  685,
  671,
  658,
  644,
  630,
  616,
  601,
  587,
  572,
  557,
  542,
  527,
  512,
  496,
  480,
  464,
  448,
  432,
  416,
  400,
  383,
  366,
  350,
  333,
  316,
  299,
  282,
  265,
  247,
  230,
  212,
  195,
  177,
  160,
  142,
  124,
  107,
  89,
  71,
  53,
  35,
  17,
  0,
  -17,
  -35,
  -53,
  -71,
  -89,
  -107,
  -124,
  -142,
  -160,
  -177,
  -195,
  -212,
  -230,
  -247,
  -265,
  -282,
  -299,
  -316,
  -333,
  -350,
  -366,
  -383,
  -400,
  -416,
  -432,
  -448,
  -464,
  -480,
  -496,
  -512,
  -527,
  -542,
  -557,
  -572,
  -587,
  -601,
  -616,
  -630,
  -644,
  -658,
  -671,
  -685,
  -698,
  -711,
  -724,
  -736,
  -748,
  -760,
  -772,
  -784,
  -795,
  -806,
  -817,
  -828,
  -838,
  -848,
  -858,
  -868,
  -877,
  -886,
  -895,
  -904,
  -912,
  -920,
  -928,
  -935,
  -942,
  -949,
  -955,
  -962,
  -968,
  -973,
  -979,
  -984,
  -989,
  -993,
  -997,
  -1001,
  -1005,
  -1008,
  -1011,
  -1014,
  -1016,
  -1018,
  -1020,
  -1021,
  -1022,
  -1023,
  -1023,
  -1024,
  -1023,
  -1023,
  -1022,
  -1021,
  -1020,
  -1018,
  -1016,
  -1014,
  -1011,
  -1008,
  -1005,
  -1001,
  -997,
  -993,
  -989,
  -984,
  -979,
  -973,
  -968,
  -962,
  -955,
  -949,
  -942,
  -935,
  -928,
  -920,
  -912,
  -904,
  -895,
  -886,
  -877,
  -868,
  -858,
  -848,
  -838,
  -828,
  -817,
  -806,
  -795,
  -784,
  -772,
  -760,
  -748,
  -736,
  -724,
  -711,
  -698,
  -685,
  -671,
  -658,
  -644,
  -630,
  -616,
  -601,
  -587,
  -572,
  -557,
  -542,
  -527,
  -512,
  -496,
  -480,
  -464,
  -448,
  -432,
  -416,
  -400,
  -383,
  -366,
  -350,
  -333,
  -316,
  -299,
  -282,
  -265,
  -247,
  -230,
  -212,
  -195,
  -177,
  -160,
  -142,
  -124,
  -107,
  -89,
  -71,
  -53,
  -35,
  -17
};

ossimRgbImage::ossimRgbImage()
   : ossimReferenced(),
     theImageData(0),
     theOffsets(0),
     theThickness(1),
     theRed(255),
     theGreen(255),
     theBlue(255)
{
}

ossimRgbImage::ossimRgbImage(ossimRefPtr<ossimImageData>& currentImageData)
   : ossimReferenced(),
     theImageData(0),
     theOffsets(0),
     theThickness(1),
     theRed(255),
     theGreen(255),
     theBlue(255)
{
   setCurrentImageData(currentImageData);
}


ossimRgbImage::~ossimRgbImage()
{
   if(theOffsets)
   {
      delete [] theOffsets;
      theOffsets = 0;
   }
}

const ossimRefPtr<ossimImageData> ossimRgbImage::getImageData()const
{
   return theImageData;
}

void ossimRgbImage::createNewGrey(ossim_int32 width, ossim_int32 height)
{
   theImageData = new ossimImageData(0,
                                     OSSIM_UCHAR,
                                     1,
                                     width,
                                     height);
   theImageData->initialize();

   setCurrentImageData(theImageData);

}

void ossimRgbImage::createNewTrueColor(ossim_int32 width, ossim_int32 height)
{
   theImageData = new ossimImageData(0,
                                     OSSIM_UCHAR,
                                     3,
                                     width,
                                     height);
   theImageData->initialize();   
}

void ossimRgbImage::initialize()
{
   if(!theImageData)
   {
      return;
   }
   if(theOffsets)
   {
      delete [] theOffsets;
      theOffsets = 0;
   }
   theWidth  = theImageData->getWidth();
   theHeight = theImageData->getHeight();
   
   theOffsets = new ossim_int32[theHeight];
   for(ossim_int32 row = 0; row < theHeight; ++row)
   {
      theOffsets[row] = row*theWidth;
   }
   
   // make it work for 1, 2, or 3 band data.
   // we assume the fourth band is for the alpha channel
   if(theImageData->getNumberOfBands() >= 3)
   { // point each band to r, g, b.
      theBands[0] = static_cast<ossim_uint8*>(theImageData->getBuf(0));
      theBands[1] = static_cast<ossim_uint8*>(theImageData->getBuf(1));
      theBands[2] = static_cast<ossim_uint8*>(theImageData->getBuf(2));
   }
   else if(theImageData->getNumberOfBands() == 2)
   {//point r and g to the same place
      theBands[0] = static_cast<ossim_uint8*>(theImageData->getBuf(0));
      theBands[1] = static_cast<ossim_uint8*>(theImageData->getBuf(0));
      theBands[2] = static_cast<ossim_uint8*>(theImageData->getBuf(1));
   }
   else if(theImageData->getNumberOfBands() == 1)
   {// point all band s to the first band
      theBands[0] = static_cast<ossim_uint8*>(theImageData->getBuf(0));
      theBands[1] = static_cast<ossim_uint8*>(theImageData->getBuf(0));
      theBands[2] = static_cast<ossim_uint8*>(theImageData->getBuf(0));
   }
}

void ossimRgbImage::setCurrentImageData(ossimRefPtr<ossimImageData>& imageData)
{
   if(imageData.valid())
   {
      if((imageData->getScalarType() == OSSIM_UCHAR)&&
         (imageData->getDataObjectStatus()!=OSSIM_NULL))
      {
         theImageData = imageData;
         initialize();
      }
   }
}

ossimRefPtr<ossimImageData> ossimRgbImage::getImageData()
{
   return theImageData;
}


void ossimRgbImage::drawPolygon(ossimIpt *p,
                                int n)
{
   if(!theImageData)
   {
      return;
   }
   int i;
   int lx, ly;
   
   if (!n)
   {
      return;
   }
   lx = p->x;
   ly = p->y;
   drawLine (lx, ly, p[n - 1].x, p[n - 1].y);
   
   for (i = 1; (i < n); i++)
   {
      p++;
      drawLine (lx, ly, p->x, p->y);
      lx = p->x;
      ly = p->y;
   }
}

void ossimRgbImage::drawPolygon(ossimDpt *p,
                                int n)
{
   if(!theImageData)
   {
      return;
   }
   int i;
   double lx, ly;
   
   if (!n)
   {
      return;
   }
   lx = p->x;
   ly = p->y;
   drawLine (lx, ly, p[n - 1].x, p[n - 1].y);
   
   for (i = 1; (i < n); i++)
   {
      p++;
      drawLine (lx, ly, p->x, p->y);
      lx = p->x;
      ly = p->y;
   }
}

void ossimRgbImage::drawPolygon(const std::vector<ossimIpt> &p)
{
   if(!theImageData)
   {
      return;
   }
   int n = (int)p.size();
   
   int i;
   int lx, ly;
   
   if (!n)
   {
      return;
   }
   lx = p[0].x;
   ly = p[0].y;
   drawLine (lx, ly, p[n - 1].x, p[n - 1].y);
   
   for (i = 1; (i < n); i++)
   {
      drawLine (lx, ly, p[i].x, p[i].y);
      lx = p[i].x;
      ly = p[i].y;
   }
}

void ossimRgbImage::drawPolygon(const std::vector<ossimDpt> &p)
{
   if(!theImageData)
   {
      return;
   }
   int n = (int)p.size();
   
   int i;
   double lx, ly;
   
   if (!n)
   {
      return;
   }
   lx = p[0].x;
   ly = p[0].y;
   drawLine (lx, ly, p[n - 1].x, p[n - 1].y);
   
   for (i = 1; (i < n); i++)
   {
      drawLine (lx, ly, p[i].x, p[i].y);
      lx = p[i].x;
      ly = p[i].y;
   }
}

inline int gdCompareInt (const void *a, const void *b)
{
  return (*(const int *) a) - (*(const int *) b);
}

void ossimRgbImage::drawFilledArc (double cx,
                                   double cy,
                                   double w,
                                   double h,
                                   double s,
                                   double e)
{
//    drawFilledArc((int)ossim::round<int>(cx), // center x
//                  (int)ossim::round<int>(cy), // center y
//                  (int)ossim::round<int>(w), // width
//                  (int)ossim::round<int>(h), // height
//                  (int)ossim::round<int>(s), // start angle
//                  (int)ossim::round<int>(e)); // end angle
   drawFilledArc((int)(cx), // center x
                 (int)(cy), // center y
                 (int)(w), // width
                 (int)(h), // height
                 (int)(s), // start angle
                 (int)(e)); // end angle
}

void ossimRgbImage::drawFilledArc (int cx,
                                   int cy,
                                   int w,
                                   int h,
                                   int s,
                                   int e)
{
   if(!theImageData)
   {
      return;
   }
   ossimIpt pts[3];
   int i;
   int lx = 0, ly = 0;
   // int fx, fy;
   int w2, h2;
   w2 = w / 2;
   h2 = h / 2;
   while (e < s)
   {
      e += 360;
   }
   for (i = s; (i <= e); i++)
   {
      int x, y;
      x = ((ossim_int32) gdCosT[i % 360] * (ossim_int32) w2 / 1024) + cx;
      y = ((ossim_int32) gdSinT[i % 360] * (ossim_int32) h2 / 1024) + cy;
      if (i != s)
      {
         /* This is expensive! */
         pts[0].x = lx;
         pts[0].y = ly;
         pts[1].x = x;
         pts[1].y = y;
         pts[2].x = cx;
         pts[2].y = cy;
         drawFilledPolygon (pts, 3);
      }
      // else
      // {
      //   fx = x;
      //   fy = y;
      // }
      lx = x;
      ly = y;
   }
}

void ossimRgbImage::drawArc (int cx,
                             int cy,
                             int w,
                             int h,
                             int s,
                             int e)
{
   if(!theImageData)
   {
      return;
   }
   int i;
   int lx = 0, ly = 0;
   // int fx, fy;
   int w2, h2;
   w2 = w / 2;
   h2 = h / 2;
   while (e < s)
   {
      e += 360;
   }
   for (i = s; (i <= e); i++)
   {
      int x, y;
      x = ((ossim_int32) gdCosT[i % 360] * (ossim_int32) w2 / 1024) + cx;
      y = ((ossim_int32) gdSinT[i % 360] * (ossim_int32) h2 / 1024) + cy;
      if (i != s)
      {
         drawLine (lx, ly, x, y);
      }
      // else
      //       {
      //          fx = x;
      //          fy = y;
      //       }
      lx = x;
      ly = y;
   }
}

void ossimRgbImage::drawFilledEllipse (int cx,
                                       int cy,
                                       int axisMinor,
                                       int axisMajor,
                                       double rot)
{
   if(!theImageData)
   {
      return;
   }
   
   int lx = 0;
   int ly = 0;
   int w2 = axisMinor/2;
   int h2 = axisMajor/2;
   ossimIpt pts[3];
   
   float sinRot = sin(rot);
   float cosRot = cos(rot);
   
   for (int i = 0; i <= 360; ++i)
   {
      int x = ((ossim_int32) gdCosT[i % 360] * (ossim_int32) w2 / 1024);
      int y = ((ossim_int32) gdSinT[i % 360] * (ossim_int32) h2 / 1024);
      
      int rx = (ossim_int32)((float)x*cosRot - (float)y*sinRot + cx);
      int ry = (ossim_int32)((float)y*cosRot + (float)x*sinRot + cy);
      
      if (i != 0)
      {
         /* This is expensive! */
         pts[0].x = lx;
         pts[0].y = ly;
         pts[1].x = rx;
         pts[1].y = ry;
         pts[2].x = cx;
         pts[2].y = cy;
         
         drawFilledPolygon (pts, 3);
      }
      lx = rx;
      ly = ry;
   }
}

void ossimRgbImage::drawEllipse (int cx,
                                 int cy,
                                 int axisMinor,
                                 int axisMajor,
                                 double rot,
                                 bool drawAxes)
{
   if(!theImageData)
   {
      return;
   }

   // Range check rotation.
   if ( (rot < -TWO_PI) || (rot > TWO_PI) )
   {
      rot = 0.0; // throw exception or continue???
   }

   int lx = 0;
   int ly = 0;
   int w2 = axisMinor/2;
   int h2 = axisMajor/2;
   
   float sinRot = sin(rot);
   float cosRot = cos(rot);
   
   int xMaj[2];
   int yMaj[2];
   int xMin[2];
   int yMin[2];
   xMin[0] = 0;
   xMin[1] = 0;
   yMin[0] = 0;
   yMin[1] = 0;
   xMaj[0] = 0;
   xMaj[1] = 0;   
   yMaj[0] = 0;
   yMaj[1] = 0;

   for (int i = 0; i <= 360; i++)
   {
      // Compute current coordinates
      int x = ((ossim_int32) gdCosT[i % 360] * (ossim_int32) w2 / 1024);
      int y = ((ossim_int32) gdSinT[i % 360] * (ossim_int32) h2 / 1024);
      
      // Rotate through orientation angle
      int rx = (ossim_int32)((float)x*cosRot - (float)y*sinRot + cx);
      int ry = (ossim_int32)((float)y*cosRot + (float)x*sinRot + cy);
      
      // Draw the 1-degree segment
      if (i != 0)
      {
         drawLine (lx, ly, rx, ry);
      }
      lx = rx;
      ly = ry;

      if (drawAxes)
      {
         // Save axes end-points
         switch (i)
         {
            case 0:
               xMin[0] = rx;
               yMin[0] = ry;
               break;
            case 90:
               xMaj[0] = rx;
               yMaj[0] = ry;
               break;
            case 180:
               xMin[1] = rx;
               yMin[1] = ry;
               break;
            case 270:
               xMaj[1] = rx;
               yMaj[1] = ry;
               break;
            default:
               break;
         }
      }
   }
   
   // Draw axes if selected
   if (drawAxes)
   {
      ossim_uint8 rCurr;
      ossim_uint8 gCurr;
      ossim_uint8 bCurr;
      getDrawColor(rCurr, gCurr, bCurr);
      
      ossim_int32 thickCurr = theThickness;
      
      ossim_uint8 mask = 0xff;

      // Draw the minor axis first so it is on the bottom.
      drawLine (xMin[0], yMin[0], xMin[1], yMin[1]);

      //set major axis to thicker yellow
      setDrawColor((rCurr^mask),(gCurr^mask),(bCurr^mask));
      // setDrawColor(255,255,1);
      theThickness += 1;
      drawLine (xMaj[0], yMaj[0], xMaj[1], yMaj[1]);
      theThickness = thickCurr;
      setDrawColor(rCurr,gCurr,bCurr); //set back
   }
}

void ossimRgbImage::drawFilledPolygon(ossimIpt *p,
                                      int n)
{
   if(!theImageData)
   {
      return;
   }
   int i;
   int y;
   int miny, maxy;
   int minx, maxx;
   int x1, y1;
   int x2, y2;
   int ind1, ind2;
   int ints;
   int *polyInts = 0;
   
   if(!n)
   {
      return;
   }
   polyInts = new int[n];
   
   miny = p[0].y;
   maxy = p[0].y;
   minx = p[0].x;
   maxx = p[0].x;
   for (i = 1; (i < n); i++)
   {
      miny = std::min((int)p[i].y, miny);
      minx = std::min((int)p[i].x, minx);
      maxy = std::max((int)p[i].y, maxy);
      maxx = std::max((int)p[i].x, maxx);
   }
   /* Fix in 1.3: count a vertex only once */
   for (y = miny; (y <= maxy); y++)
   {
      ints = 0;
      for (i = 0; (i < n); i++)
      {
         if (!i)
         {
            ind1 = n - 1;
            ind2 = 0;
         }
         else
         {
            ind1 = i - 1;
            ind2 = i;
         }
         y1 = p[ind1].y;
         y2 = p[ind2].y;
         if (y1 < y2)
         {
            x1 = p[ind1].x;
            x2 = p[ind2].x;
         }
         else if (y1 > y2)
         {
            y2 = p[ind1].y;
            y1 = p[ind2].y;
            x2 = p[ind1].x;
            x1 = p[ind2].x;
         }
         else
         {
            continue;
         }
         if ((y >= y1) && (y < y2))
         {
            polyInts[ints++] = (y - y1) * (x2 - x1) / (y2 - y1) + x1;
         }
         else if ((y == maxy) && (y > y1) && (y <= y2))
         {
            polyInts[ints++] = (y - y1) * (x2 - x1) / (y2 - y1) + x1;
         }
      }
      qsort (polyInts, ints, sizeof (int), gdCompareInt);

      if(ints)
      {
         for (i = 0; (i < (ints)); i += 2)
         {
            drawLine (polyInts[i], y,
                      polyInts[i + 1], y);
         }
      }
      else
      {
         if((miny == maxy) && (minx==maxx))
         {
            drawLine((int)p[0].x, (int)p[0].y, (int)p[0].x, (int)p[0].y);
         }
      }
   }

   if (polyInts) delete [] polyInts;
}

void ossimRgbImage::drawFilledPolygon(ossimDpt *p,
                                      int n)
{
   if(!theImageData)
   {
      return;
   }
   int i;
   int y;
   int miny, maxy;
   int x1, y1;
   int x2, y2;
   int ind1, ind2;
   int ints;
   int *polyInts = 0;
   
   if(!n)
   {
      return;
   }
   polyInts = new int[n];
   
//   miny = ossim::round<int>(p[0].y);
//   maxy = ossim::round<int>(p[0].y);
   miny = (int)(p[0].y);
   maxy = (int)(p[0].y);
   for (i = 1; (i < n); i++)
   {
//      int testPy = ossim::round<int>(p[i].y);
      int testPy = (int)(p[i].y);
      if (testPy < miny)
      {
         miny = testPy;
      }
      if (testPy > maxy)
      {
         maxy = testPy;
      }
   }
   /* Fix in 1.3: count a vertex only once */
   for (y = miny; (y <= maxy); y++)
   {
      ints = 0;
      for (i = 0; (i < n); i++)
      {
         if (!i)
         {
            ind1 = n - 1;
            ind2 = 0;
         }
         else
         {
            ind1 = i - 1;
            ind2 = i;
         }
//         y1 = ossim::round<int>(p[ind1].y);
//         y2 = ossim::round<int>(p[ind2].y);
         y1 = (int)(p[ind1].y);
         y2 = (int)(p[ind2].y);
         if (y1 < y2)
         {
            x1 = ossim::round<int>(p[ind1].x);
            x2 = ossim::round<int>(p[ind2].x);
         }
         else if (y1 > y2)
         {
//            y2 = ossim::round<int>(p[ind1].y);
//            y1 = ossim::round<int>(p[ind2].y);
//            x2 = ossim::round<int>(p[ind1].x);
//            x1 = ossim::round<int>(p[ind2].x);
            y2 = (int)(p[ind1].y);
            y1 = (int)(p[ind2].y);
            x2 = (int)(p[ind1].x);
            x1 = (int)(p[ind2].x);
         }
         else
         {
            continue;
         }
         if ((y >= y1) && (y < y2))
         {
            polyInts[ints++] = (y - y1) * (x2 - x1) / (y2 - y1) + x1;
         }
         else if ((y == maxy) && (y > y1) && (y <= y2))
         {
            polyInts[ints++] = (y - y1) * (x2 - x1) / (y2 - y1) + x1;
         }
      }
      qsort (polyInts, ints, sizeof (int), gdCompareInt);
      
      for (i = 0; (i < (ints)); i += 2)
      {
         drawLine (polyInts[i], y,
                   polyInts[i + 1], y);
      }
   }

   if (polyInts) delete [] polyInts;
}

void ossimRgbImage::drawFilledPolygon(const std::vector<ossimIpt> &p)
{
   if(!theImageData)
   {
      return;
   }
   int n = (int)p.size();
   int i;
   int y;
   int miny, maxy;
   int x1, y1;
   int x2, y2;
   int ind1, ind2;
   int ints;
   int *polyInts = 0;
   
   if(!n)
   {
      return;
   }
   polyInts = new int[n];
   
   miny = p[0].y;
   maxy = p[0].y;
   for (i = 1; (i < n); i++)
   {
      if (p[i].y < miny)
      {
         miny = p[i].y;
      }
      if (p[i].y > maxy)
      {
         maxy = p[i].y;
      }
   }
   /* Fix in 1.3: count a vertex only once */
   for (y = miny; (y <= maxy); y++)
   {
      ints = 0;
      for (i = 0; (i < n); i++)
      {
         if (!i)
         {
            ind1 = n - 1;
            ind2 = 0;
         }
         else
         {
            ind1 = i - 1;
            ind2 = i;
         }
         y1 = p[ind1].y;
         y2 = p[ind2].y;
         if (y1 < y2)
         {
            x1 = p[ind1].x;
            x2 = p[ind2].x;
         }
         else if (y1 > y2)
         {
            y2 = p[ind1].y;
            y1 = p[ind2].y;
            x2 = p[ind1].x;
            x1 = p[ind2].x;
         }
         else
         {
            continue;
         }
         if ((y >= y1) && (y < y2))
         {
            polyInts[ints++] = (y - y1) * (x2 - x1) / (y2 - y1) + x1;
         }
         else if ((y == maxy) && (y > y1) && (y <= y2))
         {
            polyInts[ints++] = (y - y1) * (x2 - x1) / (y2 - y1) + x1;
         }
      }
      qsort (polyInts, ints, sizeof (int), gdCompareInt);
      
      for (i = 0; (i < (ints)); i += 2)
      {
         drawLine (polyInts[i], y,
                   polyInts[i + 1], y);
      }
   }

   if (polyInts) delete [] polyInts;
}

void ossimRgbImage::drawFilledPolygon(const std::vector<ossimDpt> &p)
{
   if(!theImageData)
   {
      return;
   }
   int n = (int)p.size();
   int i;
   int y;
   int miny, maxy;
   int x1, y1;
   int x2, y2;
   int ind1, ind2;
   int ints;
   int *polyInts = 0;
   
   if(!n)
   {
      return;
   }
   polyInts = new int[n];
   
   miny = (int)p[0].y;//ossim::round<int>(p[0].y);
   maxy = (int)p[0].y;//ossim::round<int>(p[0].y);
   for (i = 1; (i < n); i++)
   {
      int testPy = (int)p[i].y;//ossim::round<int>(p[i].y);
      if (testPy < miny)
      {
         miny = testPy;
      }
      if (testPy > maxy)
      {
         maxy = testPy;
      }
   }
   /* Fix in 1.3: count a vertex only once */
   for (y = miny; (y <= maxy); y++)
   {
      ints = 0;
      for (i = 0; (i < n); i++)
      {
         if (!i)
         {
            ind1 = n - 1;
            ind2 = 0;
         }
         else
         {
            ind1 = i - 1;
            ind2 = i;
         }
//          y1 = ossim::round<int>(p[ind1].y);
//          y2 = ossim::round<int>(p[ind2].y);
         y1 = (int)(p[ind1].y);
         y2 = (int)(p[ind2].y);
         if (y1 < y2)
         {
//            x1 = ossim::round<int>(p[ind1].x);
//            x2 = ossim::round<int>(p[ind2].x);
            x1 = (int)(p[ind1].x);
            x2 = (int)(p[ind2].x);
         }
         else if (y1 > y2)
         {
//             y2 = ossim::round<int>(p[ind1].y);
//             y1 = ossim::round<int>(p[ind2].y);
//             x2 = ossim::round<int>(p[ind1].x);
//             x1 = ossim::round<int>(p[ind2].x);
            y2 = (int)(p[ind1].y);
            y1 = (int)(p[ind2].y);
            x2 = (int)(p[ind1].x);
            x1 = (int)(p[ind2].x);
         }
         else
         {
            continue;
         }
         if ((y >= y1) && (y < y2))
         {
            polyInts[ints++] = (y - y1) * (x2 - x1) / (y2 - y1) + x1;
         }
         else if ((y == maxy) && (y > y1) && (y <= y2))
         {
            polyInts[ints++] = (y - y1) * (x2 - x1) / (y2 - y1) + x1;
         }
      }
      qsort (polyInts, ints, sizeof (int), gdCompareInt);
      
      for (i = 0; (i < (ints)); i += 2)
      {
         drawLine (polyInts[i], y,
                   polyInts[i + 1], y);
      }
   }

   if (polyInts) delete [] polyInts;
}

void ossimRgbImage::drawLine(const ossimIpt& start,
                             const ossimIpt& end)
{
   drawLine((int)start.x,
            (int)start.y,
            (int)end.x,
            (int)end.y);
   
}

void ossimRgbImage::drawLine(const ossimDpt& start,
                             const ossimDpt& end)
{
//    drawLine((int)ossim::round<int>(start.x),
//             (int)ossim::round<int>(start.y),
//             (int)ossim::round<int>(end.x),
//             (int)ossim::round<int>(end.y));

   drawLine((int)(start.x),
            (int)(start.y),
            (int)(end.x),
            (int)(end.y));
   
}

void ossimRgbImage::drawLine(double x1,
                             double y1,
                             double x2,
                             double y2)
{
//    drawLine((int)ossim::round<int>(x1),
//             (int)ossim::round<int>(y1),
//             (int)ossim::round<int>(x2),
//             (int)ossim::round<int>(y2));
   drawLine((int)(x1),
            (int)(y1),
            (int)(x2),
            (int)(y2));
   
}
void ossimRgbImage::drawLine(int x1,
                             int y1,
                             int x2,
                             int y2)
{
   if(!theImageData)
   {
      return;
   }

   // we need to shift x and y relative to the image data
   // origin.  That way we are doing the algorithm relative to
   // the 0,0
   //
   ossimIpt origin = ossimDpt(theImageData->getOrigin());
   x1 += (-origin.x);
   y1 += (-origin.y);
   x2 += (-origin.x);
   y2 += (-origin.y);

//   ossimDpt start(x1, y1);
//   ossimDpt end(x2,y2);
//   ossimDrect bounds(0,0,theWidth-1, theHeight-1);

//   if(bounds.clip(start, end)) return;

//   x1 = (int)start.x;
//   y1 = (int)start.y;
//   x2 = (int)end.x;
//   y2 = (int)end.y;
   
   int dx, dy, incr1, incr2, d, x, y, xend, yend, xdirflag, ydirflag;
   int wid;
   int w, wstart;
   int thick = theThickness;
   
  dx = abs (x2 - x1);
  dy = abs (y2 - y1);
  if (dy <= dx)
    {
      /* More-or-less horizontal. use wid for vertical stroke */
      /* Doug Claar: watch out for NaN in atan2 (2.0.5) */
      if ((dx == 0) && (dy == 0))
	{
	  wid = 1;
	}
      else
	{
	  /* 2.0.12: Michael Schwartz: divide rather than multiply;
	     TBB: but watch out for /0! */
	  double ac = cos (atan2 ((double)dy, (double)dx));
	  if (ac != 0)
	    {
	      wid = ossim::round<int>(thick / ac);
	    }
	  else
	    {
	      wid = 1;
	    }
	  if (wid == 0)
	    {
	      wid = 1;
	    }
	}
      d = 2 * dy - dx;
      incr1 = 2 * dy;
      incr2 = 2 * (dy - dx);
      if (x1 > x2)
	{
	  x = x2;
	  y = y2;
	  ydirflag = (-1);
	  xend = x1;
	}
      else
	{
	  x = x1;
	  y = y1;
	  ydirflag = 1;
	  xend = x2;
	}

      /* Set up line thickness */
      wstart = y - wid / 2;
      for (w = wstart; w < wstart + wid; w++)
	slowPlotPixel(x, w, theRed, theGreen, theBlue);

      if (((y2 - y1) * ydirflag) > 0)
	{
	  while (x < xend)
	    {
	      x++;
	      if (d < 0)
		{
		  d += incr1;
		}
	      else
		{
		  y++;
		  d += incr2;
		}
	      wstart = y - wid / 2;
	      for (w = wstart; w < wstart + wid; w++)
		slowPlotPixel(x, w, theRed, theGreen, theBlue);
	    }
	}
      else
	{
	  while (x < xend)
	    {
	      x++;
	      if (d < 0)
		{
		  d += incr1;
		}
	      else
		{
		  y--;
		  d += incr2;
		}
	      wstart = y - wid / 2;
	      for (w = wstart; w < wstart + wid; w++)
		slowPlotPixel(x, w, theRed, theGreen, theBlue);
	    }
	}
    }
  else
    {
      /* More-or-less vertical. use wid for horizontal stroke */
      /* 2.0.12: Michael Schwartz: divide rather than multiply;
         TBB: but watch out for /0! */
      double as = sin (atan2 ((double)dy, (double)dx));
      if (as != 0)
	{
	  wid = ossim::round<int>(thick / as);
	}
      else
	{
	  wid = 1;
	}
      if (wid == 0)
	wid = 1;

      d = 2 * dx - dy;
      incr1 = 2 * dx;
      incr2 = 2 * (dx - dy);
      if (y1 > y2)
	{
	  y = y2;
	  x = x2;
	  yend = y1;
	  xdirflag = (-1);
	}
      else
	{
	  y = y1;
	  x = x1;
	  yend = y2;
	  xdirflag = 1;
	}

      /* Set up line thickness */
      wstart = x - wid / 2;
      for (w = wstart; w < wstart + wid; w++)
	slowPlotPixel(w, y, theRed, theGreen, theBlue);

      if (((x2 - x1) * xdirflag) > 0)
	{
	  while (y < yend)
	    {
	      y++;
	      if (d < 0)
		{
		  d += incr1;
		}
	      else
		{
		  x++;
		  d += incr2;
		}
	      wstart = x - wid / 2;
	      for (w = wstart; w < wstart + wid; w++)
		slowPlotPixel(w, y, theRed, theGreen, theBlue);
	    }
	}
      else
	{
	  while (y < yend)
	    {
	      y++;
	      if (d < 0)
		{
		  d += incr1;
		}
	      else
		{
		  x--;
		  d += incr2;
		}
	      wstart = x - wid / 2;
	      for (w = wstart; w < wstart + wid; w++)
		slowPlotPixel(w, y, theRed, theGreen, theBlue);
	    }
	}
    }
}
void ossimRgbImage::drawRectangle(double x1,
                                  double y1,
                                  double x2,
                                  double y2)
{
//    drawRectangle(ossim::round<int>(x1),
//                  ossim::round<int>(y1),
//                  ossim::round<int>(x2),
//                  ossim::round<int>(y2));
   drawRectangle((int)(x1),
                 (int)(y1),
                 (int)(x2),
                 (int)(y2));
}

void ossimRgbImage::drawRectangle(int x1,
                                  int y1,
                                  int x2,
                                  int y2)
{
  if(x1 > x2)
  {
     swap(x1, x2);
  }
  if(y1 > y2)
  {
     swap(y1, y2);
  }
  int x1h = x1,
      x1v = x1,
      y1h = y1,
      y1v = y1,
      x2h = x2,
      x2v = x2,
      y2h = y2,
      y2v = y2;
  
  int thick = theThickness;
  
  
  if (thick > 1)
    {
      int half = thick / 2;
      int half1 = thick - half;

      if (y1 < y2)
	{
	  y1v = y1h - half;
	  y2v = y2h + half1 - 1;
	}
      else
	{
	  y1v = y1h + half1 - 1;
	  y2v = y2h - half;
	}
    }

  drawLine (x1h, y1h, x2h, y1h);
  drawLine (x1h, y2h, x2h, y2h);
  drawLine (x1v, y1v, x1v, y2v);
  drawLine (x2v, y1v, x2v, y2v);
}


void ossimRgbImage::drawFilledRectangle(double x1,
                                        double y1,
                                        double x2,
                                        double y2)
{
//   drawFilledRectangle((int)ossim::round<int>(x1),
//                       (int)ossim::round<int>(y1),
//                       (int)ossim::round<int>(x2),
//                       (int)ossim::round<int>(y2));
   drawFilledRectangle((int)(x1),
                       (int)(y1),
                       (int)(x2),
                       (int)(y2));
}

void ossimRgbImage::drawFilledRectangle(int x1,
                                        int y1,
                                        int x2,
                                        int y2)
{
   int x, y;

   if(x1 > x2)
   {
      swap(x1, x2);
   }
   if(y1 > y2)
   {
      swap(y1, y2);
   }
   for (y = y1; (y <= y2); y++)
   {
      for (x = x1; (x <= x2); x++)
      {
         slowPlotPixel (x, y, theRed, theGreen, theBlue);
      }
   }
}

void ossimRgbImage::fill()
{
   if(!theImageData)
   {
      return;
   }
   ossim_int32 width = theWidth;
   ossim_int32 height = theHeight;
   
   for(ossim_int32 row = 0; row < height; ++row)
   {
      for(ossim_int32 col = 0; col < width; ++col)
      {
         fastPlotPixel(col, row,
                       theRed, theGreen, theBlue);
      }
   }
}

void ossimRgbImage::setDrawColor(ossim_uint8 r,
                                 ossim_uint8 g,
                                 ossim_uint8 b)
{
   theRed   = r;
   theGreen = g;
   theBlue  = b;
}

void ossimRgbImage::getDrawColor(ossim_uint8& rCurr,
                                 ossim_uint8& gCurr,
                                 ossim_uint8& bCurr)
{
   rCurr = theRed;
   gCurr = theGreen;
   bCurr = theBlue;
}

void ossimRgbImage::setThickness(ossim_int32 thickness)
{
   theThickness = thickness;
}

ossim_int32 ossimRgbImage::getThickness() const
{
   return theThickness;
}
