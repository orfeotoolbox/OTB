//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts
// Description:
//
//*************************************************************************
// $Id: ossimUsgsQuad.cpp 12953 2008-06-01 16:24:05Z dburken $

#include <cstdlib>
#include <sstream>
#include <iomanip>

#include <ossim/base/ossimUsgsQuad.h>
#include <ossim/base/ossimDatum.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimNotify.h>

using namespace std;

static const
double QUARTER_QUAD_SEG_SIZE_IN_DEGREES = 0.03125; // 3.75 minutes/60.0/2.0
static const double QUARTER_QUAD_SIZE_IN_DEGREES = 0.0625; // 3.75 minutes/60.0
static const double QUAD_SIZE_IN_DEGREES = 0.125; // 7.5 minutes/60


//***
// Static trace for debugging
//***
static ossimTrace traceDebug("USGS_quad::debug");


ossimUsgsQuad::ossimUsgsQuad(const ossimGpt& lrGpt)
   :
      theQuadLowerRightCorner(0.0, 0.0, 0.0, lrGpt.datum()),
      theQuarterQuadLowerRightCorner(0.0, 0.0, 0.0, lrGpt.datum())
//      thePaddingInDegrees(0.0, 0.0),

{
   const char* MODULE = "ImgOutput::quadBasename";

   //***
   // NOTE:
   // This method computes a file name from the lower right corner of the
   // image.  The format is derived from USGS Digital Raster Graphic(DRG)
   // program for standardized data set names for DRG products.  Due to
   // customer  requirements there is one deviation:  The first digit of the
   // name is converted to a letter with 1 being = A, 2 being = B,
   // 3 being = C, ....
   // This was done to allow the name to be used on a pc.
   //***

   const double QUAD_SIZE_IN_DEGREES = 0.125;

   ossimString          baseString;
   int                  tmpInt;
   double               tmpDbl;
   char quadChar        = 'A';
   char quadNum         = '1';

   ostringstream           os;
   os << setiosflags(ios::right)
      << setiosflags(ios::fixed)
      << setfill('0');

   tmpInt = static_cast<int>(abs(lrGpt.lat)); // Cast away the fractional part.
   os << tmpInt; // latitude

   //***
   // Get the quadrant charactor in the latitude direction. (A - H)
   //***
   tmpDbl = fabs(lrGpt.lat) - (double)tmpInt;
   quadChar += static_cast<int>(tmpDbl / QUAD_SIZE_IN_DEGREES);

   tmpInt = static_cast<int>(abs(lrGpt.lon)); // longitude
   os << setw(3) << tmpInt;

   tmpDbl = fabs(lrGpt.lon) - (double)tmpInt;

   quadNum += static_cast<char>(tmpDbl / QUAD_SIZE_IN_DEGREES);

   os << quadChar << quadNum;

   double latFraction = (lrGpt.lat / QUAD_SIZE_IN_DEGREES) -
                        ossim::round<int>((lrGpt.lat) / QUAD_SIZE_IN_DEGREES);
   double lonFraction = (lrGpt.lon / QUAD_SIZE_IN_DEGREES) -
                        ossim::round<int>((lrGpt.lon) / QUAD_SIZE_IN_DEGREES);

   // Black & White
//   if(theRectsStandardFlag && 
//      theChipSource->radiometry().numberOfBands() == 1)
//   {
//       char quarterQuadChar = '4';
//       if (latFraction && lonFraction)
//       {
//          quarterQuadChar = '1';
//       }
//       else if (latFraction && !lonFraction)
//       {
//          quarterQuadChar = '2';
//       }
//       else if (!latFraction && lonFraction)
//       {
//          quarterQuadChar = '3';
//       }   
//       os << quarterQuadChar << ends;
//    }
//    // Color 
//    else if(theRectsStandardFlag &&
//            theChipSource->radiometry().numberOfBands() > 1)
//    {
//       char quarterQuadChar = '8';
//       if (latFraction && lonFraction)
//       {
//          quarterQuadChar = '5';
//       }
//       else if (latFraction && !lonFraction)
//       {
//          quarterQuadChar = '6';
//       }
//       else if (!latFraction && lonFraction)
//       {
//          quarterQuadChar = '7';
//       }   
//       os << quarterQuadChar << ends;
//    }
//    else
//    {
      char quarterQuadChar = 'D';
      if (latFraction && lonFraction)
      {
         quarterQuadChar = 'A';
      }
      else if (latFraction && !lonFraction)
      {
         quarterQuadChar = 'B';
      }
      else if (!latFraction && lonFraction)
      {
         quarterQuadChar = 'C';
      }   
      os << quarterQuadChar << ends;
//   }

   
   baseString = os.str().c_str();

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << " DEBUG: " << MODULE  
         << "\nbaseString:  " << baseString << std::endl;
   }
   
   setQuadName(baseString);
}


//***************************************************************************
// USGS_quad::USGS_quad(const String& name)
//***************************************************************************
ossimUsgsQuad::ossimUsgsQuad(const ossimString& name,
                             const ossimDatum* datum)
//                             double paddingInMeters)
   :
      theQuadLowerRightCorner(0.0, 0.0, 0.0, datum),
      theQuarterQuadLowerRightCorner(0.0, 0.0, 0.0, datum)
//      thePaddingInDegrees(0.0, 0.0),
{
   static const char MODULE[] = "ossimUsgsQuad::ossimUsgsQuad";

   setQuadName(name);
   if (!datum)
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_FATAL)
            << "FATAL: " << MODULE << "\n"
            << "ossimDatum pointer passed in is null. Returning...\n";
      }
      return;
   }


   //***
   // Initialize the padding.
   //***
//    ossimGpt metersPerDegree = theQuadLowerRightCorner.metersPerDegree();

//    thePaddingInDegrees.u = paddingInMeters / metersPerDegree.u;
//    thePaddingInDegrees.v = paddingInMeters / metersPerDegree.v;

}

void ossimUsgsQuad::setQuadName(const ossimString& name)
{
   const char* MODULE = "ossimUsgsQuad::setQuadName";
   ossimString qqName = name;

   //***
   // Check the length of "name" to see if it's correct.  
   //***
   if (name.length() < 7)
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_FATAL)
            << "FATAL: " << MODULE << "\n"
            << "Quarder quad name length incorrect.\n"
            << "Name:  " << name << "\n"
            << "Expected length:  7 or 8  Name length:  " << name.length()
            << "\nExample of valid quad:  30089C6B\n"
            << "Represents lower right corner of "
            << "30 deg. 15 min. N., 89 deg. 37.5 min. W.\n"
            << "Breakdown:\n"
            << "30  = latitude on even degree boundary "
            << "(Northern Hemisphere implied)\n"
            << "089 = longitude on even degree boundary "
            << "(Western Hemisphere implied)\n"
            << "C   = Third quad section in latitude direction\n"
            << "6   = Sixth quad section in longitude direction\n"
            << "B   = Quarter quad segment within the quad\n"
            << "  (A=upper left, B=upper right, C=lower left, D=lower right)\n"
            << "Quad size 7.5 minute square,  quarter quad size 3.75 "
            << "minute square.\n";
      }
      
      return;
   }

   qqName.upcase();

   char latChars[3];
   char lonChars[4];
   char latQuad;
   char lonQuad;
//   char segment;

   //***
   // Get the latitude.
   //***
   latChars[0] = qqName.c_str()[0];
   latChars[1] = qqName.c_str()[1];
   latChars[2] = '\0';

   //***
   // Get the longitude.
   //***
   lonChars[0] = qqName.c_str()[2];
   lonChars[1] = qqName.c_str()[3];
   lonChars[2] = qqName.c_str()[4];
   lonChars[3] = '\0';

   //***
   // Get the latitude quad.
   //***
   latQuad = qqName.c_str()[5];

   //***
   // Get the longitude quad.
   //***
   lonQuad = qqName.c_str()[6];

   //***
   // Get the segment within the quad.
   //***
   if (name.length() == 8)
   {
      theQuarterQuadSegment = qqName.c_str()[7];
   }
   else
   {
      theQuarterQuadSegment = 'D';
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_NOTICE)
            << MODULE << ":\n"
            << "No quarter quad segment entered.  Defaulting to D segment."
            << std::endl;
      }
   }

   //***
   // Build up the lower right corner from the extracted parts.
   //***

   //***
   // Get the latitude and check the range.
   //***
   double lat = atof(latChars);
   if ( (lat < 0.0) || (lat > 90.0) )
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_FATAL)
            << "FATAL: " << MODULE << "\n"
            << "Latitude range error.  Latitude extracted:  " << lat
            << "\nRange:  0 to 90\n";
      }
      return;
   }

   //***
   // Get the longitude and check the range.
   //***
   double lon = atof(lonChars);
   if ( (lon  < 0.0) || (lon > 180) )
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_FATAL)
            << "FATAL: " << MODULE << "\n"
            << "Longitud range error.  Longitude extracted:  " << lon
            << "\nRange:  0 to 180\n";
      }
      return;
   }

   //***
   // Get the latitude quad and add this to "lat".  The quad are letters from
   // A - H with A being on an even degree boundary, B being 7.5 minute up
   // and so on.
   //***
   double tmp = latQuad - 65;  // Ascii decimal value for A = 65.

   if (tmp)
   {
      //***
      // Check the range.  The are eight quads within one degree.
      //***
      if (tmp > 7.0)
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_FATAL)
               << "FATAL:" << MODULE << "\n"
               << "Latitude quad range error.  Quad extracted:  " << latQuad
               << "\nRange:  A to H\n";
         }
         return;
      }

      tmp *= 7.5;  // Convert the quad to minutes.

      lat += tmp/60.0; // Conver minutes to degrees and add to "lat".
   }
   
   //***
   // Get the longitude quad and add this to "lat".  The quad are letters from
   // A - H with A being on an even degree boundary, B being 7.5 minute up
   // and so on.
   //***
   tmp = lonQuad - 49; // Ascii decimal value for 1 = 49.

   if (tmp)
   {
      //***
      // Check the range.  The are eight quads within one degree.
      //***
      if (tmp > 7.0)
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_FATAL)
               << "FATAL:" << MODULE << "\n"
               << "Longitude quad range error.  Quad extracted:  " << latQuad
               << "\nRange:  1 to 8\n";
         }
         return;
      }

      tmp *= 7.5;  // Convert the quad to minutes.

      lon += tmp/60.0; // Conver minutes to degrees and add to "lon".
   }
   
   //***
   // Currently this naming convention assumes Northern and Western hemisphere.
   //***
   lon = -lon;  // Make negative for Western hemisphere.

   //***
   // Initialize "theQuadLowerRightCorner".
   //***
   theQuadLowerRightCorner.lat = lat;
   theQuadLowerRightCorner.lon = lon;

   //***
   // Get the segment and add to "lat" and "lon".
   //***
   switch (theQuarterQuadSegment)
   {
   case 'A':
      lat += QUARTER_QUAD_SIZE_IN_DEGREES;
      lon -= QUARTER_QUAD_SIZE_IN_DEGREES;
      break;

   case 'B':
      lat += QUARTER_QUAD_SIZE_IN_DEGREES;
      break;

   case 'C':
      lon -= QUARTER_QUAD_SIZE_IN_DEGREES;
      break;

   case 'D':
      break; // At the corner of a quad so nothing to do here.

   default:
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_FATAL)
            << "FATAL:" << MODULE << "\n"
            << "Quarter quad segment range error.\n"
            << "Quarter quad segment extracted:  " << theQuarterQuadSegment
            << "Range:  A to D\n";
      }
      return;
   }

   //***
   // Initialize "theQuarterQuadLowerRightCorner".
   //***
   theQuarterQuadLowerRightCorner.lat = lat;
   theQuarterQuadLowerRightCorner.lon = lon;

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG: " << MODULE << ", "
         << qqName << " lower right quad corner:  "
         << theQuadLowerRightCorner << "\n"
         << "Quarter quad segment:  " << theQuarterQuadSegment << "\n"
         << qqName << " lower right quarter quad corner:  "
         << theQuarterQuadLowerRightCorner << "\n";
   }   
   theName = qqName;
}

void ossimUsgsQuad::getQuadList(vector<ossimUsgsQuad>& result,
                                const ossimGrect& rect)
{
   result.clear();
//   QUAD_SIZE_IN_DEGREES;

   ossimGpt point = rect.ul();

   while(rect.pointWithin(point))
   {
      while(rect.pointWithin(point))
      {
         result.push_back(ossimUsgsQuad(point));

         point.lond(point.lond()+QUAD_SIZE_IN_DEGREES);
      }
      point.lond(rect.ul().lond());
      point.latd(point.latd()-QUAD_SIZE_IN_DEGREES);
   }
}


//***************************************************************************
// ossimUsgsQuad::quarterQuadSegRect()
//***************************************************************************
ossimGrect ossimUsgsQuad::quarterQuadSegRect(char seg)
{
   static const char MODULE[] = "ossimUsgsQuad::quarterQuadSegRect";

   double startLat=0.0;
   double startLon=0.0;
   double stopLat=0.0;
   double stopLon=0.0;

   //***
   // Get the segment and add to "lat" and "lon".
   //***
   switch (seg)
   {
   case 'A':  // Upper left segment of quarter quad.
      startLat = theQuarterQuadLowerRightCorner.lat +
                 QUARTER_QUAD_SEG_SIZE_IN_DEGREES;// -
//                 thePaddingInDegrees.u;

      startLon = theQuarterQuadLowerRightCorner.lon -
                 QUARTER_QUAD_SIZE_IN_DEGREES;// -
                 //thePaddingInDegrees.v;
                 
      stopLat = theQuarterQuadLowerRightCorner.lat +
                QUARTER_QUAD_SIZE_IN_DEGREES;// +
                                             //thePaddingInDegrees.u;

      stopLon =  theQuarterQuadLowerRightCorner.lon -
                 QUARTER_QUAD_SEG_SIZE_IN_DEGREES; // +
                                                  //thePaddingInDegrees.v;
      break;

   case 'B': // Upper right segment of the quarter quad.
      startLat = theQuarterQuadLowerRightCorner.lat +
                 QUARTER_QUAD_SEG_SIZE_IN_DEGREES; //-
//                 thePaddingInDegrees.u;

      startLon = theQuarterQuadLowerRightCorner.lon -
                 QUARTER_QUAD_SEG_SIZE_IN_DEGREES;// -
//                 thePaddingInDegrees.v;
                 
      stopLat = theQuarterQuadLowerRightCorner.lat +
                QUARTER_QUAD_SIZE_IN_DEGREES;// +
//                thePaddingInDegrees.u;

      stopLon =  theQuarterQuadLowerRightCorner.lon;// +
//                 thePaddingInDegrees.v;
      break;

   case 'C': // Lower left segment of the quarter quad.
      startLat = theQuarterQuadLowerRightCorner.lat;// -
//                 thePaddingInDegrees.u;

      startLon = theQuarterQuadLowerRightCorner.lon -
                 QUARTER_QUAD_SIZE_IN_DEGREES;// -
//                 thePaddingInDegrees.v;
                 
      stopLat = theQuarterQuadLowerRightCorner.lat +
                QUARTER_QUAD_SEG_SIZE_IN_DEGREES;// +
//                thePaddingInDegrees.u;

      stopLon =  theQuarterQuadLowerRightCorner.lon -
                 QUARTER_QUAD_SEG_SIZE_IN_DEGREES;// +
//                 thePaddingInDegrees.v;

      break;

   case 'D':
      startLat = theQuarterQuadLowerRightCorner.lat;// -
//                 thePaddingInDegrees.u;

      startLon = theQuarterQuadLowerRightCorner.lon -
                 QUARTER_QUAD_SEG_SIZE_IN_DEGREES;// -
//                 thePaddingInDegrees.v;
                 
      stopLat = theQuarterQuadLowerRightCorner.lat +
                QUARTER_QUAD_SEG_SIZE_IN_DEGREES;// +
//                thePaddingInDegrees.u;

      stopLon =  theQuarterQuadLowerRightCorner.lon;// +
//                 thePaddingInDegrees.v;
      break; // At the corner of a quad so nothing to do here.

   default:
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_FATAL)
            << "FATAL:" << MODULE << "\n"
            << "Quarter quad segment range error.\n"
            << "Quarter quad segment extracted:  " << seg
            << "Range:  A to D\n";
      }
      break;
   }

   ossimGrect
      rect(ossimGpt(startLat,
                    startLon,
                    theQuarterQuadLowerRightCorner.height(),
                    theQuarterQuadLowerRightCorner.datum()),
           ossimGpt(stopLat,
                    stopLon,
                    theQuarterQuadLowerRightCorner.height(),
                    theQuarterQuadLowerRightCorner.datum()));

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG: " << MODULE
         << "\nQuarter quad segment " << seg << " rect:  " << rect << "\n";
   }

   return rect;
}

//***************************************************************************
// ossimUsgsQuad::quarterQuadRect()
//***************************************************************************
ossimGrect ossimUsgsQuad::quarterQuadRect() const
{
   static const char MODULE[] = "ossimUsgsQuad::quarterQuadRect";

   double startLat = theQuarterQuadLowerRightCorner.lat;// -
//                     thePaddingInDegrees.u;  // SouthWest corner lat.

   double startLon = theQuarterQuadLowerRightCorner.lon -
                     QUARTER_QUAD_SIZE_IN_DEGREES;// -
//                     thePaddingInDegrees.v; // SouthWest corner lon.
   
   double stopLat = theQuarterQuadLowerRightCorner.lat +
                     QUARTER_QUAD_SIZE_IN_DEGREES;// +
//                    thePaddingInDegrees.u; // NorthEast corner lat.
   
   double stopLon  = theQuarterQuadLowerRightCorner.lon;// +
//                     thePaddingInDegrees.v; // NorthEast corner lon.

   ossimGrect
      rect(ossimGpt(startLat,
                    startLon,
                    theQuarterQuadLowerRightCorner.height(),
                    theQuarterQuadLowerRightCorner.datum()),
           ossimGpt(stopLat,
                    stopLon,
                    theQuarterQuadLowerRightCorner.height(),
                    theQuarterQuadLowerRightCorner.datum()));

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG: " << MODULE
         << "\nQuarter quad rect:  " << rect <<"\n";
   }


   return rect;
}

//***************************************************************************
// ossimUsgsQuad::quadRect()
//***************************************************************************
ossimGrect ossimUsgsQuad::quadRect() const
{
   static const char MODULE[] = "ossimUsgsQuad::quadRect";

   double startLat = theQuadLowerRightCorner.lat;// -
//                     thePaddingInDegrees.u;  // SouthWest corner lat.

   double startLon = theQuadLowerRightCorner.lon -
                     QUAD_SIZE_IN_DEGREES;// -
//                     thePaddingInDegrees.v; // SouthWest corner lon.
   
   double stopLat = theQuadLowerRightCorner.lat +
                     QUAD_SIZE_IN_DEGREES;// +
//                    thePaddingInDegrees.u; // NorthEast corner lat.
   
   double stopLon  = theQuadLowerRightCorner.lon;// +
//                     thePaddingInDegrees.v; // NorthEast corner lon.

   ossimGrect
      rect(ossimGpt(startLat,
                    startLon,
                    theQuarterQuadLowerRightCorner.height(),
                    theQuarterQuadLowerRightCorner.datum()),
           ossimGpt(stopLat,
                    stopLon,
                    theQuarterQuadLowerRightCorner.height(),
                    theQuarterQuadLowerRightCorner.datum()));

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG: " << MODULE
         << "\nQuarter quad rect:  " << rect << "\n";
   }

   return rect;
}

//***************************************************************************
// ossimUsgsQuad::quarterQuadSegKwRect(char seg)
//***************************************************************************
ossimString ossimUsgsQuad::quarterQuadSegKwRect(char seg)
{
   static const char MODULE[] = "ossimUsgsQuad::quarterQuadSegKwRect";

   ossimString tmp;

   //***
   // Check the range of the segment, must be A, B, C or D.
   //***
   if ( (seg < 65) || (seg > 68) )
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_FATAL)
            << "FATAL:" << MODULE << "\n"
            << seg << " out of range!\n"
            << "Possible values:  A, B, C, or D\n";
      }
      return tmp;
   }

   ossimGrect rect = quarterQuadSegRect(seg);

   ostringstream os;

   os << setprecision(12)
      << rect.ll().lat  << " "
      << rect.ul().lon << " "
      << rect.ul().lat << " "
      << rect.ur().lon  << ends;
//      << rect.stop.lat  << " "
//      << rect.start.lon << " "
//      << rect.start.lat << " "
//      << rect.stop.lon  << ends;

   tmp = os.str().c_str();

   return tmp;
}

//***************************************************************************
// ossimUsgsQuad::quarterQuadKwRect()
//***************************************************************************
ossimString ossimUsgsQuad::quarterQuadKwRect() const
{
//   static const char MODULE[] = "ossimUsgsQuad::quarterQuadKwRect";

   ossimGrect rect = quarterQuadRect();

   ostringstream os;

   os << setprecision(12)
      << rect.ll().lat  << " "
      << rect.ul().lon << " "
      << rect.ul().lat << " "
      << rect.ur().lon  << ends;
//    os << setprecision(12)
//       << rect.stop.lat  << " "
//       << rect.start.lon << " "
//       << rect.start.lat << " "
//       << rect.stop.lon  << ends;

   ossimString tmp = os.str().c_str();



   return tmp;
}

//***************************************************************************
// ossimUsgsQuad::quadKwRect()
//***************************************************************************
ossimString ossimUsgsQuad::quadKwRect() const
{
//   static const char MODULE[] = "ossimUsgsQuad::quadKwRect";

   ossimGrect rect = quadRect();

   ostringstream os;

   os << setprecision(12)
      << rect.ll().lat  << " "
      << rect.ul().lon << " "
      << rect.ul().lat << " "
      << rect.ur().lon  << ends;
//    os << setprecision(12)
//       << rect.stop.lat  << " "
//       << rect.start.lon << " "
//       << rect.start.lat << " "
//       << rect.stop.lon  << ends;

   ossimString tmp = os.str().c_str();

   return tmp;
}
