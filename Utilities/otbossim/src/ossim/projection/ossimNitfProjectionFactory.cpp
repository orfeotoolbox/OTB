//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  Matt Revelle
//          David Burken
//
// Description:
//
// Contains class definition for ossimNitfProjectionFactory.
//
// $Id: ossimNitfProjectionFactory.cpp 20457 2012-01-13 20:08:49Z dburken $
//----------------------------------------------------------------------------

#include <ossim/projection/ossimNitfProjectionFactory.h>
#include <ossim/base/ossimDms.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/imaging/ossimNitfTileSource.h>
#include <ossim/projection/ossimBilinearProjection.h>
#include <ossim/projection/ossimEquDistCylProjection.h>
#include <ossim/projection/ossimMgrs.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/projection/ossimUtmProjection.h>
#include <ossim/support_data/ossimNitfBlockaTag.h>
#include <ossim/support_data/ossimNitfFile.h>
#include <ossim/support_data/ossimNitfFileHeader.h>
#include <ossim/support_data/ossimNitfImageHeader.h>
#include <fstream>
#include <cmath>

// Define Trace flags for use within this file:
static ossimTrace traceDebug(ossimString("ossimNitfProjectionFactory:debug"));

ossimNitfProjectionFactory* ossimNitfProjectionFactory::theInstance = 0;

ossimNitfProjectionFactory::ossimNitfProjectionFactory()
{
}

ossimNitfProjectionFactory::~ossimNitfProjectionFactory()
{
}

ossimNitfProjectionFactory* ossimNitfProjectionFactory::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimNitfProjectionFactory();
   }

   return theInstance;
}

ossimProjection*
ossimNitfProjectionFactory::createProjection(const ossimFilename& filename,
                                             ossim_uint32 entryIdx)const
{
   static const char MODULE[]="ossimNitfProjectionFactory::createProjection";
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " DEBUG:"
         << "\nfilename:  " << filename
         << "\nentryIdx:  " << entryIdx
         << std::endl;
   }

   // See if there is an external geomtry.
   ossimProjection* result = createProjectionFromGeometryFile(filename, entryIdx);
   if (result)
   {
      return result;
   }

   if(!isNitf(filename))
   {
      return result; // result is NULL
   }

   ossimRefPtr<ossimNitfFile> nitf = new ossimNitfFile();
   if (!nitf->parseFile(filename))
   {
      return result; // result is NULL
   }

   long imageIndex = static_cast<long>(entryIdx);
   if ( imageIndex > (nitf->getHeader()->getNumberOfImages()-1) )
   {
      return result;
   }

   ossimRefPtr<ossimNitfImageHeader> imageHeader = nitf->getNewImageHeader(imageIndex);
   {
      if (!imageHeader)
      {
         return result;
      }
   }

   result = createProjectionFromHeaders(nitf->getHeader(),
                                        imageHeader.get());
   if (traceDebug())
   {
      ossimString coordinateSystem   = imageHeader->getCoordinateSystem();
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " DEBUG:"
         << "\ncoordinateSysetm:       " << coordinateSystem
         << std::endl;
   }

   return result;
}


ossimProjection*
ossimNitfProjectionFactory::createProjection(const ossimKeywordlist& /* kwl */,
                                             const char* /* prefix */) const
{
   return 0;
}

ossimProjection*
ossimNitfProjectionFactory::createProjection(const ossimString& /* name */) const
{
   return 0;
}

ossimObject*
ossimNitfProjectionFactory::createObject(const ossimString& typeName)const
{
   return (ossimObject*)createProjection(typeName);
}

ossimObject*
ossimNitfProjectionFactory::createObject(const ossimKeywordlist& kwl,
                                         const char* prefix)const
{
   return createProjection(kwl, prefix);
}

void ossimNitfProjectionFactory::getTypeNameList(std::vector<ossimString>& /* typeList */)const
{
   
}

ossimProjection* ossimNitfProjectionFactory::createProjection(ossimImageHandler* handler)const
{
   ossimNitfTileSource* nitfTileSource = dynamic_cast<ossimNitfTileSource*> (handler);
   ossimProjection* result = 0;
   if(nitfTileSource)
   {
      if(!result)
      {
         ossimNitfImageHeader* imageHeader = nitfTileSource->getCurrentImageHeader();
         if(imageHeader)
         {
            result = createProjectionFromHeaders(nitfTileSource->getFileHeader(),imageHeader);
         }
      }
   }
   else if(isNitf(handler->getFilename()))
   {
      result =  createProjection(handler->getFilename(), handler->getCurrentEntry());
   }
   return result;
}

bool ossimNitfProjectionFactory::isNitf(const ossimFilename& filename)const
{
   std::ifstream in(filename.c_str(), ios::in|ios::binary);
   
   if(in)
   {
      char nitfFile[4];
      in.read((char*)nitfFile, 4);
      ossimString s(nitfFile, nitfFile+4);
      if ( (s == "NITF") || (s == "NSIF") )
      {
         return true;
      }
   }

   return false;
}

ossimProjection* ossimNitfProjectionFactory::createProjectionFromHeaders(
   ossimNitfFileHeader* fileHeader, ossimNitfImageHeader* imageHeader)const
{
   ossimProjection* result = 0;
   ossimString version = fileHeader->getVersion();
   ossimString coordinateSystem = imageHeader->getCoordinateSystem();

   // Note in version 2.0 ICORDS with 'N' == NONE.  In 2.1 it is UTM North:
   if (coordinateSystem == "G" || coordinateSystem == "D")
   {
      result = makeGeographic(imageHeader, coordinateSystem);
   }
   else if( (coordinateSystem == "S") || (coordinateSystem == "U") || 
            ( version != "02.00" && (coordinateSystem == "N") ) )
   {
      result = makeUtm(imageHeader, coordinateSystem);
   }
   return result;
}

ossimProjection* ossimNitfProjectionFactory::makeGeographic(
   const ossimNitfImageHeader* hdr,
   const ossimString& coordinateSysetm) const
{
   ossimProjection* proj = 0;

   if (hdr)
   {
      // To hold corner points.
      std::vector<ossimGpt> gpts;
      
      //---
      // Get the corner points.
      // 
      // Look for points from the BLOCKA tag.  This may or may not be present.
      // If present since it has six digit precision use it for the points.
      //---
      if ( getBlockaPoints(hdr, gpts) == false )
      {
         ossimString geographicLocation = hdr->getGeographicLocation();

         if ( geographicLocation.size() )
         {
            if (traceDebug())
            {
               ossimNotify(ossimNotifyLevel_DEBUG)
                  << "ossimNitfProjectionFactory::makeGeographic DEBUG:"
                  << "\ngeographicLocation: " << geographicLocation
                  << std::endl;
            }
            
            if (coordinateSysetm == "G")
            {
               //---
               // If coord system is G then format is:
               // Lat = ddmmssX
               //       where d is degrees and m is minutes
               //       and s is seconds and X is either N (North) or S (South).
               // Lon = dddmmssX
               //       where d is degrees and m is minutes
               //       and s is seconds and X is either N (North) or S (South).
               //---
               parseGeographicString(geographicLocation, gpts);
            }
            else if (coordinateSysetm == "D")
            {
               //---
               // If coor system is D then format is:
               // +-dd.ddd +-dd.ddd four times where + is northern hemispher and
               // - is souther hemisphere for lat and longitude
               // + is easting and - is westing.
               //---
               parseDecimalDegreesString(geographicLocation, gpts);
            }
            
         } // matches: if ( geographicLocation.size() )
         
      } // matches: if ( getBlockaPoints(hdr, gpts) == false )
         
      if (gpts.size() == 4)
      {
         if (!isSkewed(gpts))
         {
            proj = makeEuiDistant(hdr, gpts);
         }
         else
         {
            // Image is rotated.  Make a Bilinear.
            proj = makeBilinear(hdr, gpts);
         }
      }

      if (traceDebug() && proj)
      {
		  ossimNotify(ossimNotifyLevel_DEBUG)
			  << "ossimNitfProjectionFactory::makeGeographic DEBUG:"
			  << "\nUpper left corner:   " << gpts[0]
			  << "\nUpper right corner:  " << gpts[1]
			  << "\nLower right corner:  " << gpts[2]
			  << "\nLower left corner:   " << gpts[3] << std::endl;
		  proj->print(ossimNotify(ossimNotifyLevel_DEBUG));
		  ossimNotify(ossimNotifyLevel_DEBUG) << std::endl;
      }
      
   } // matches: if (hdr)

   return proj;
}

ossimProjection* ossimNitfProjectionFactory::makeUtm(
   const ossimNitfImageHeader* hdr,
   const ossimString& coordinateSystem) const
{
   ossimProjection* proj = 0;
   if (hdr)
   {
      ossimString geographicLocation = hdr->getGeographicLocation();

      if ( geographicLocation.size() )
      {
         std::vector<ossimDpt> utmPoints;
         ossim_uint32 zone;
         ossimDpt scale;
         char hemisphere = 'N';
         
         bool status = true;
         if ( coordinateSystem == "U")
         {
            // Sets zone, hemisphere and utmPoints. Returns true on success.
            status = parseMgrsString(geographicLocation, zone, hemisphere, utmPoints);
         }
         else
         {
            // Sets zone and utmPoints.  Void return...
            parseUtmString(geographicLocation, zone, utmPoints);
            if(coordinateSystem == "S")
            {
               hemisphere = 'S';
            }
         }
         
         if ( status )
         {
            if ( traceDebug() )
            {
               std::string s;
               s.push_back(hemisphere);
               ossimNotify(ossimNotifyLevel_DEBUG)
                  << "ossimNitfProjectionFactory::makeUtm DEBUG"
                  << "\ngeo string: " << geographicLocation
                  << "\nutm zone:   " << zone
                  << "\nhemisphere: " << hemisphere
                  << std::endl;
               for (ossim_uint32 i=0; i<utmPoints.size(); ++i)
               {
                  ossimNotify(ossimNotifyLevel_DEBUG)
                     << "utmPoints[" << utmPoints[i] << std::endl;
               }
            }
            
            ossimRefPtr<ossimUtmProjection> uproj = new ossimUtmProjection;
            uproj->setHemisphere(hemisphere);
            uproj->setZone(zone);
            
            if(isSkewed(utmPoints))
            {
               std::vector<ossimGpt> gpts;
               
               // Try blocka points first as they are more accurate.
               if ( getBlockaPoints(hdr, gpts) == false )
               {
                  ossimGpt ul = uproj->inverse(utmPoints[0]);
                  ossimGpt ur = uproj->inverse(utmPoints[1]);
                  ossimGpt lr = uproj->inverse(utmPoints[2]);
                  ossimGpt ll = uproj->inverse(utmPoints[3]);
                  gpts.push_back(ul);
                  gpts.push_back(ur);
                  gpts.push_back(lr);
                  gpts.push_back(ll);
               }

               //---
               // Make a bilinear either from our skewed utm points or the points from the blocka
               // tag.
               //---
               proj = makeBilinear(hdr, gpts);
               
               uproj = 0; // Done with utm projeciton
               
            }
            else
            {
               computeScaleInMeters(hdr, utmPoints, scale);
               
               //---
               // Assign our projection to the return "proj".
               // Use ossimRefPtr::release the so we don't delete proj when uproj
               // goes out of scope.
               //---
               proj = uproj.release(); 
            }
            
            if( scale.hasNans() == false )
            {
               //---
               // Get the tie point.
               // 
               // Look for the the BLOCKA tag which may or may not be present.
               // This has six digit precision in decimal degrees which equates to
               // about 0.11 meters (at equator) as compared to 1.0 accuaracy of the
               // IGEOLO field.
               //---
               ossimDpt tie;
               std::vector<ossimGpt> gpts;
               if ( getBlockaPoints(hdr, gpts) )
               {
                  if (traceDebug())
                  {
                     ossimNotify(ossimNotifyLevel_DEBUG)
                        << "ossimNitfProjectionFactory::makeUtm DEBUG:"
                        << "\nTie point from blocka: " << gpts[0]
                        << endl;
                  }
                  
                  tie = proj->forward(gpts[0]);
                  tie.x += scale.x/2.0;
                  tie.y -= scale.y/2.0;
               }
               else
               {
                  tie.x = utmPoints[0].x + scale.x/2.0;
                  tie.y = utmPoints[0].y - scale.y/2.0;
               }
               
               if (traceDebug())
               {
                  ossimNotify(ossimNotifyLevel_DEBUG)
                     << "ossimNitfProjectionFactory::makeUtm DEBUG:"
                     << "\nTie point: " << tie
                     << "\nScale:     " << scale
                     << endl;
               }
               
               // Set the tie and scale.
               ossimMapProjection* mproj = dynamic_cast<ossimMapProjection*>(proj);
               if ( mproj )
               {
                  mproj->setUlEastingNorthing(tie);
                  mproj->setMetersPerPixel(scale);
               }
               else // cannot cast
               {
                  if ( proj )
                  {
                     delete proj;
                     proj = 0;
                  }
               }
            }
            else // Scale has nans
            {
               if ( proj )
               {
                  delete proj;
                  proj = 0;
               }
            }
            
         } // matches: if (status)

      } // matches:  if ( geographicLocation.size() )
         
   } // matches: if (hdr)
   
   return proj;
}

bool ossimNitfProjectionFactory::parseMgrsString(const ossimString& mgrsLocationString,
                                                 ossim_uint32& zone,
                                                 char& hemisphere,
                                                 std::vector<ossimDpt>& utmPoints)const
{
   bool result = false; // Start false.
   
   //---
   // From spec:
   // UTM expressed in MGRS use the format zzBJKeeeeennnnn (15 characters).
   // Assumption Zone and hemisphere same for all corners.
   //---
   if ( mgrsLocationString.size() >= 60 )
   {
      // Split the location string into four separate ones.
      std::vector<std::string> mgrsStr(4); // Corner strings.
      mgrsStr[0] = mgrsLocationString.substr(0, 15);
      mgrsStr[1] = mgrsLocationString.substr(15, 15);
      mgrsStr[2] = mgrsLocationString.substr(30, 15);
      mgrsStr[3] = mgrsLocationString.substr(45, 15);
      
      utmPoints.resize(4);
      long z = 0;
      ossim_float64 e=0.0;
      ossim_float64 n=0.0;
      
      result = true; // Set to true.

      //---
      // Convert each string to Easting Northing.  This also sets zone hemisphere.
      // Method takes long for zone.
      //---
      for (ossim_uint32 i = 0; i < 4; ++i)
      {
         if ( Convert_OSSIM_MGRS_To_UTM(mgrsStr[i].c_str(), &z, &hemisphere, &e, &n) == 0 )
         {
            utmPoints[i].x = e;
            utmPoints[i].y = n;
         }
         else
         {
            result = false; // Geotrans code errored on string.
            break;
         }
      }
      if (result) zone = static_cast<ossim_uint32>(z); // Set the zone.
   }
   return result;
}

ossimProjection* ossimNitfProjectionFactory::makeEuiDistant(
   const ossimNitfImageHeader* hdr,
   const std::vector<ossimGpt>& gpts) const
{
   ossimEquDistCylProjection* proj = 0;

   // Get the scale.
   ossimDpt scale;
   computeScaleInDecimalDegrees(hdr, gpts, scale);

   if (scale.hasNans())
   {
      return proj;
   }
   
   // Make the projection.
   proj = new ossimEquDistCylProjection();

   if ( scale.x )
   {
      ossimGpt origin = proj->getOrigin();
      
      //---
      // ossimEquDistCylProjection uses the origin_latitude for meters per pixel (gsd)
      // computation.  So is not set in tiff tags, compute to achieve the proper
      // horizontal scaling.
      //---
      origin.lat = ossim::acosd(scale.y/scale.x);

      proj->setOrigin(origin);
   }

   // Shift the tie to the center of the pixel.
   ossimGpt tiePoint;
   tiePoint.latd(gpts[0].latd() - (scale.y/2.0));
   tiePoint.lond(gpts[0].lond() + (scale.x/2.0));

   // Set the tie points.
   proj->setUlTiePoints(tiePoint);

   // Set the scale.
   proj->setDecimalDegreesPerPixel(scale);

   return proj;
}

ossimProjection* ossimNitfProjectionFactory::makeBilinear(
   const ossimNitfImageHeader* hdr,
   const std::vector<ossimGpt>& gpts) const
{
   double rows = hdr->getNumberOfRows();
   double cols = hdr->getNumberOfCols();
   
   ossimDpt ul(0.0, 0.0);
   ossimDpt ur(cols-1.0, 0.0);   
   ossimDpt lr(cols-1.0, rows-1.0);
   ossimDpt ll(0.0, rows-1.0);

   ossimRefPtr<ossimBilinearProjection> proj;
   try
   {
     proj = new ossimBilinearProjection(ul,
                                      ur,
                                      lr,
                                      ll,
                                      gpts[0],
                                      gpts[1],
                                      gpts[2],
                                      gpts[3]);
   }
   catch(...)
   {
      proj = 0;
   }
   
   return proj.release();
}

bool ossimNitfProjectionFactory::isSkewed(
   const std::vector<ossimGpt>& gpts) const
{

   return !( (gpts[0].latd() == gpts[1].latd()) &&
             (gpts[2].latd() == gpts[3].latd()) &&
             (gpts[0].lond() == gpts[3].lond()) &&
             (gpts[1].lond() == gpts[2].lond()) );
   
}

bool ossimNitfProjectionFactory::isSkewed(
   const std::vector<ossimDpt>& dpts) const
{
   return !( (dpts[0].y == dpts[1].y) &&
             (dpts[2].y == dpts[3].y) &&
             (dpts[0].x == dpts[3].x) &&
             (dpts[1].x == dpts[2].x) );
   
}

bool ossimNitfProjectionFactory::getBlockaPoints(
   const ossimNitfImageHeader* hdr,
   std::vector<ossimGpt>& gpts) const
{
   if (!hdr)
   {
      return false;
   }

   ossimRefPtr<ossimNitfRegisteredTag> tag =
      hdr->getTagData(ossimString("BLOCKA"));

   if (!tag)
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimNitfProjectionFactory::getBlockaPoints DEBUG:"
            << "\nBLOCKA tag not found."
            << std::endl;
      }
      return false;
   }

   if (gpts.size())
   {
      gpts.clear();
   }

   ossimNitfBlockaTag* blockaTag = PTR_CAST(ossimNitfBlockaTag, tag.get());
   if (!blockaTag)
   {
      return false;
   }

   ossimDpt dpt;
   ossimGpt gpt;

   // Get the upper left or first row first column.
   blockaTag->getFrfcLoc(dpt);
   gpt.latd(dpt.y);
   gpt.lond(dpt.x);
   gpts.push_back(gpt);

   // Get the upper right or first row last column.
   blockaTag->getFrlcLoc(dpt);
   gpt.latd(dpt.y);
   gpt.lond(dpt.x);
   gpts.push_back(gpt);

   // Get the lower right or last row last column.
   blockaTag->getLrlcLoc(dpt);
   gpt.latd(dpt.y);
   gpt.lond(dpt.x);
   gpts.push_back(gpt);

   // Get the lower left or last row first column.
   blockaTag->getLrfcLoc(dpt);
   gpt.latd(dpt.y);
   gpt.lond(dpt.x);
   gpts.push_back(gpt);

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimNitfProjectionFactory::getBlockaPoints DEBUG:"
         << std::endl;
      for (int i=0; i<4; ++i)
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "gpt[" << i << "] " << gpts[i] << std::endl;
      }
   }
   
   return true;
}

void ossimNitfProjectionFactory::computeScaleInDecimalDegrees(
   const ossimNitfImageHeader* hdr,
   const std::vector<ossimGpt>& gpts,
   ossimDpt& scale) const
{
   if ( !hdr || isSkewed(gpts))
   {
      scale.makeNan();
      return;
   }
   ossimIrect imageRect = hdr->getImageRect();

   //---
   // Calculate the scale.  This assumes that the corner points are for the
   // edge of the corner pixels, not the center of the corner pixels.
   //---
   double longitudeSize  = 0.0;
   double latitudeSize = 0.0;
   if ( (gpts[1].lond() < 0.0) && (gpts[0].lond() >= 0) )
   {
      //---
      // Upper right negative(Western), upper left positive (Eastern).
      // Crossing date line maybe???
      //---
      longitudeSize = (gpts[1].lond() + 360.0) - gpts[0].lond();
   }
   else
   {
      longitudeSize = gpts[1].lond() - gpts[0].lond();
   }

   latitudeSize = gpts[0].latd() - gpts[2].latd();

   double rows = imageRect.height();
   double cols = imageRect.width();
//   double rows = hdr->getNumberOfRows();
//   double cols = hdr->getNumberOfCols();
       
   if (!rows || !cols)
   {
      scale.makeNan();
      return;
   }
   scale.y = latitudeSize  / rows;
   scale.x = longitudeSize / cols;
}

void ossimNitfProjectionFactory::computeScaleInMeters(
   const ossimNitfImageHeader* hdr,
   const std::vector<ossimDpt>& dpts,
   ossimDpt& scale) const
{
   if ( !hdr || isSkewed(dpts))
   {
      scale.makeNan();
      return;
   }
   ossimIrect imageRect = hdr->getImageRect();

   //---
   // Calculate the scale.  This assumes that the corner points are for the
   // edge of the corner pixels, not the center of the corner pixels.
   //---
   double eastingSize  = 0.0;
   double northingSize = 0.0;
   
   eastingSize  = fabs(dpts[1].x - dpts[0].x);
   northingSize = fabs(dpts[0].y - dpts[3].y);

   double rows = imageRect.height();//hdr->getNumberOfRows();
   double cols = imageRect.width();//hdr->getNumberOfCols();

   if (!rows || !cols)
   {
      scale.makeNan();
      return;
   }
   scale.y = northingSize / rows;
   scale.x = eastingSize  / cols;
}

void ossimNitfProjectionFactory::parseUtmString(const ossimString& utmLocation,
                                                ossim_uint32& zone,
                                                std::vector<ossimDpt>& utmPoints)const
{
   ossim_uint32 idx = 0;
   ossimString z;
   ossimString east;
   ossimString north;

   
   z    = ossimString(utmLocation.begin() + idx,
                   utmLocation.begin() + idx + 2);
   idx += 2;
   east = ossimString(utmLocation.begin() + idx,
                      utmLocation.begin() + idx + 6);
   idx += 6;
   north = ossimString(utmLocation.begin() + idx,
                       utmLocation.begin() + idx + 7);
   idx += 7;

   utmPoints.push_back(ossimDpt(east.toDouble(),
                                north.toDouble()));
   
   z    = ossimString(utmLocation.begin() + idx,
                   utmLocation.begin() + idx + 2);
   idx += 2;
   east = ossimString(utmLocation.begin() + idx,
                      utmLocation.begin() + idx + 6);
   idx += 6;
   north = ossimString(utmLocation.begin() + idx,
                       utmLocation.begin() + idx + 7);
   idx += 7;
   utmPoints.push_back(ossimDpt(east.toDouble(),
                                north.toDouble()));

   z    = ossimString(utmLocation.begin() + idx,
                   utmLocation.begin() + idx + 2);
   idx += 2;
   east = ossimString(utmLocation.begin() + idx,
                      utmLocation.begin() + idx + 6);
   idx += 6;
   north = ossimString(utmLocation.begin() + idx,
                       utmLocation.begin() + idx + 7);
   idx += 7;
   utmPoints.push_back(ossimDpt(east.toDouble(),
                                north.toDouble()));

   z    = ossimString(utmLocation.begin() + idx,
                   utmLocation.begin() + idx + 2);
   idx += 2;
   east = ossimString(utmLocation.begin() + idx,
                      utmLocation.begin() + idx + 6);
   idx += 6;
   north = ossimString(utmLocation.begin() + idx,
                       utmLocation.begin() + idx + 7);
   idx += 7;
   utmPoints.push_back(ossimDpt(east.toDouble(),
                                north.toDouble()));

   zone = z.toUInt32();
}


void ossimNitfProjectionFactory::parseGeographicString(
   const ossimString& geographicLocation, std::vector<ossimGpt>& gpts) const
{
   gpts.clear();
   
   if (geographicLocation.size() != 60)
   {
      return;
   }

   std::string::size_type geo_index = 0;
   for (int i=0; i<4; ++i)
   {
      //---
      // We have to split up the geographicLocation string for the dms class.
      // 
      // geographicLocation = ddmmssXdddmmssX (four times).
      // "dd mm ss X" has a string length of 10
      // "ddd mm ss X" has a string length of 11
      //---
      std::string::size_type lat_index = 0;
      std::string::size_type lon_index = 0;
      const char SPACE = ' ';
      ossimString latString(10, SPACE);
      ossimString lonString(11, SPACE);

      // degrees latitude
      latString[lat_index++] = geographicLocation[geo_index++];
      latString[lat_index++] = geographicLocation[geo_index++];
      ++lat_index;

      // minutes latitude
      latString[lat_index++] = geographicLocation[geo_index++];
      latString[lat_index++] = geographicLocation[geo_index++];
      ++lat_index;
      
      // seconds latitude
      latString[lat_index++] = geographicLocation[geo_index++];
      latString[lat_index++] = geographicLocation[geo_index++];
      ++lat_index;

      // hemisphere
      latString[lat_index++] = geographicLocation[geo_index++];
 
      // degrees longitude
      lonString[lon_index++] = geographicLocation[geo_index++];
      lonString[lon_index++] = geographicLocation[geo_index++];
      lonString[lon_index++] = geographicLocation[geo_index++];
      ++lon_index;

      // minutes longitude
      lonString[lon_index++] = geographicLocation[geo_index++];
      lonString[lon_index++] = geographicLocation[geo_index++];
      ++lon_index;

      // seconds longitude
      lonString[lon_index++] = geographicLocation[geo_index++];
      lonString[lon_index++] = geographicLocation[geo_index++];
      ++lon_index;

      // hemisphere
      lonString[lon_index++] = geographicLocation[geo_index++];

      // Convert to decimal degrees using the dms class.
      ossimGpt gpt;
      ossimDms dms(0.0);
      dms.setLatFlag(true);
      if ( ! dms.setDegrees(latString.c_str()) )
      {
         gpts.clear();
         return;
      }
      gpt.latd(dms.getDegrees());

      dms.setLatFlag(false);
      if ( ! dms.setDegrees(lonString.c_str()) )
      {
         gpts.clear();
         return;
      }
      gpt.lond(dms.getDegrees());

      gpts.push_back(gpt);

      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimNitfProjectionFactory::parseGeographicString DEBUG:"
            << "\nground point[" << i << "]:  " << gpt
            << std::endl;
      }
   }
}

void ossimNitfProjectionFactory::parseDecimalDegreesString(const ossimString& geographicLocation,
                                                           std::vector<ossimGpt>& gpts) const
{
   const char* bufPtr = geographicLocation.c_str();

   
   ossimString ulLat(bufPtr,
                     bufPtr + 7);
   bufPtr+=7;
   ossimString ulLon(bufPtr,
                     bufPtr+8);
   bufPtr+=8;
   ossimString urLat(bufPtr,
                     bufPtr + 7);
   bufPtr+=7;
   ossimString urLon(bufPtr,
                     bufPtr+8);
   bufPtr+=8;
   ossimString lrLat(bufPtr,
                     bufPtr + 7);
   bufPtr+=7;
   ossimString lrLon(bufPtr,
                     bufPtr+8);
   bufPtr+=8;
   ossimString llLat(bufPtr,
                     bufPtr + 7);
   bufPtr+=7;
   ossimString llLon(bufPtr,
                     bufPtr+8);

   gpts.push_back(ossimGpt(ulLat.toDouble(), ulLon.toDouble()));
   gpts.push_back(ossimGpt(urLat.toDouble(), urLon.toDouble()));
   gpts.push_back(ossimGpt(lrLat.toDouble(), lrLon.toDouble()));
   gpts.push_back(ossimGpt(llLat.toDouble(), llLon.toDouble()));
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimNitfProjectionFactory::parseDecimalDegreesString DEBUG:"
         << "\nground point[" << 0 << "]:  " << gpts[0]
         << "\nground point[" << 1 << "]:  " << gpts[1]
         << "\nground point[" << 2 << "]:  " << gpts[2]
         << "\nground point[" << 3 << "]:  " << gpts[3]
         << std::endl;
   }
}

ossimNitfProjectionFactory::ossimNitfProjectionFactory(const ossimNitfProjectionFactory&)
{
}

ossimNitfProjectionFactory& ossimNitfProjectionFactory::operator=(const ossimNitfProjectionFactory&)
{
   return *this;
}
