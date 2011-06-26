//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Description: Utility class to encapsulate parsing projection info in the xml file.
// 
//----------------------------------------------------------------------------
// $Id: ossimFgdcXmlDoc.cpp 2063 2011-01-19 19:38:12Z ming.su $

#include <ossim/support_data/ossimFgdcXmlDoc.h>
#include <ossim/base/ossimDatum.h>
#include <ossim/base/ossimDatumFactoryRegistry.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimException.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimXmlNode.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/projection/ossimEpsgProjectionFactory.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/projection/ossimUtmProjection.h>

#include <cmath> /* for std::fabs */

// Static trace for debugging
static ossimTrace traceDebug("ossimFgdcXmlDoc:debug");

ossimFgdcXmlDoc::ossimFgdcXmlDoc()
   : m_xmlFilename(ossimFilename::NIL),
     m_xmlDocument(0),
     m_projection(0),
     m_boundInDegree(false)
{
}

ossimFgdcXmlDoc::~ossimFgdcXmlDoc()
{
}

bool ossimFgdcXmlDoc::open(const ossimFilename& xmlFileName)
{
   bool result = false;
   if ( isOpen() )
   {
      close();
   }
   
   if (xmlFileName.size())
   {
      m_xmlDocument = new ossimXmlDocument(xmlFileName);
      if ( m_xmlDocument.valid() )
      {
         // May want to add a test for valid FGDC here??? (drb)
         m_xmlFilename = xmlFileName;
         result = true;
      }
   }
   return result;
}

void ossimFgdcXmlDoc::close()
{
   m_xmlFilename = ossimFilename::NIL;
   m_xmlDocument = 0;
   m_projection  = 0;
}

ossimRefPtr<ossimProjection> ossimFgdcXmlDoc::getProjection()
{
   if ( (m_projection.valid() == false) && isOpen() )
   {
      ossimString projName;
      if ( getProjCsn(projName) )
      {
         if (!projName.empty())
         {
            m_projection = ossimEpsgProjectionFactory::instance()->createProjection(projName);
            
            if ( m_projection.valid() )
            {
               ossimDpt gsd;
               gsd.makeNan();
               ossim_float64 d;
               if ( getXRes(d) )
               {
                  gsd.x = std::fabs(d);
               }
               if ( getYRes(d) )
               {
                  gsd.y = std::fabs(d);
               }
               ossimMapProjection* mapProj = dynamic_cast<ossimMapProjection*>(m_projection.get());
               if (mapProj)
               {
                  ossimDrect rect;
                  getBoundingBox(rect);
                  if (mapProj->isGeographic())
                  {
                     ossimGpt tie(rect.ul().lat, rect.ul().lon);
                     mapProj->setUlTiePoints(tie);
                     mapProj->setDecimalDegreesPerPixel(gsd);
                  }
                  else
                  {
                     ossimDpt tie(rect.ul().x, rect.ul().y);
                     ossimUnitType unitType = getUnitType();
                     if ( unitType == OSSIM_US_SURVEY_FEET)
                     {
                        gsd = gsd * US_METERS_PER_FT;
                        tie = tie * US_METERS_PER_FT;
                     }
                     else if ( unitType == OSSIM_FEET )
                     {
                        gsd = gsd * MTRS_PER_FT;
                        tie = tie * MTRS_PER_FT;
                     }
                        
                     mapProj->setUlTiePoints(tie);
                     mapProj->setMetersPerPixel(gsd);
                  }
               }
            }
         }
      }
   }
   return m_projection;
}

ossimUnitType ossimFgdcXmlDoc::getUnitType() const
{
   ossimUnitType result = OSSIM_METERS; // default
   ossimString units;
   if ( getPlanarDistantUnits(units) )
   {
      units.downcase();
      if ( units.contains("feet") )
      {
         if ( units.contains("international") )
         {
            result = OSSIM_FEET;
         }
         else
         {
            result = OSSIM_US_SURVEY_FEET; // Default for feet.
         }
      }
   }
   return result;
}

bool ossimFgdcXmlDoc::isOpen() const
{
   return m_xmlDocument.valid();
}

bool ossimFgdcXmlDoc::getGeoCsn(ossimString& s) const
{
   ossimString xpath = "/metadata/spref/horizsys/cordsysn/geogcsn";
   return getPath(xpath, s);
}

bool ossimFgdcXmlDoc::getProjCsn(ossimString& s) const
{
   ossimString xpath = "/metadata/spref/horizsys/cordsysn/projcsn";
   return getPath(xpath, s);
}

bool ossimFgdcXmlDoc::getAltitudeDistantUnits(ossimString& s) const
{
   ossimString xpath = "/metadata/spref/vertdef/altsys/altunits";
   return getPath(xpath, s);
}

bool ossimFgdcXmlDoc::getGridCoordinateSystem(ossimString& s) const
{
   ossimString xpath = "/metadata/spref/horizsys/planar/gridsys/gridsysn";
   return getPath(xpath, s);
}

ossimRefPtr<ossimProjection> ossimFgdcXmlDoc::getGridCoordSysProjection()
{
   static const char M[] = "ossimFgdcXmlDoc::getGridCoordSysProjection";
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << M << " entered...\n";
   }

   if ( m_projection.valid() == false )
   {
      ossimString s;
      if ( getGridCoordinateSystem(s) )
      {
         ossimString gridsysn = s.downcase();
         if ( getHorizontalDatum(s) )
         {
            ossimString horizdn = s.downcase();
            const ossimDatum* datum = createOssimDatum(s); // throws exception
            
            if ( gridsysn == "universal transverse mercator" )
            {
               // Get the zone:
               if ( getUtmZone(s) )
               {
                  ossim_int32 zone = s.toInt32();

                  //---
                  // Note: Contruct with an origin with our datum.
                  // "proj->setDatum" does not change the origin's datum.
                  // This ensures theossimEpsgProjectionDatabase::findProjectionCode
                  // sets the psc code correctly down the line.
                  //---
                  ossimRefPtr<ossimUtmProjection> utmProj =
                     new ossimUtmProjection( *(datum->ellipsoid()), ossimGpt(0.0,0.0,0.0,datum) );
                  utmProj->setDatum(datum);
                  utmProj->setZone(zone);
                  
                  // Hemisphere( North false easting = 0.0, South = 10000000):
                  bool tmpResult = getUtmFalseNorthing(s);
                  if ( tmpResult && ( s != "0.0" ) )
                  {
                     utmProj->setHemisphere('S');
                  }
                  else
                  {
                     utmProj->setHemisphere('N');
                  }
                  utmProj->setPcsCode(0);

                  ossim_float64 xRes = 0.0;
                  ossim_float64 yRes = 0.0;
                  if (getXRes(xRes) && getYRes(yRes))
                  {
                     ossimDrect rect;
                     getBoundingBox(rect);

                     ossimDpt gsd(std::fabs(xRes), std::fabs(yRes));
                     ossimUnitType unitType = getUnitType();
                   
                     if (m_boundInDegree)
                     {
                        ossimGpt tieg(rect.ul().lat, rect.ul().lon);
                        utmProj->setUlTiePoints(tieg);
                     }
                     else
                     {
                        ossimDpt tie(rect.ul().x, rect.ul().y);
                        if ( unitType == OSSIM_US_SURVEY_FEET)
                        {
                           tie = tie * US_METERS_PER_FT;
                        }
                        else if ( unitType == OSSIM_FEET )
                        {
                           tie = tie * MTRS_PER_FT;
                        }
                        utmProj->setUlTiePoints(tie);
                     }

                     if ( unitType == OSSIM_US_SURVEY_FEET)
                     {
                        gsd = gsd * US_METERS_PER_FT;
                     }
                     else if ( unitType == OSSIM_FEET )
                     {
                        gsd = gsd * MTRS_PER_FT;
                     }
                     utmProj->setMetersPerPixel(gsd);
                  }
                  m_projection = utmProj.get(); // Capture projection.
               }
               else
               {
                  std::string errMsg = M;
                  errMsg += " ERROR: Could not determine utm zone!";
                  throw ossimException(errMsg);
               }
            }
         }
      }
   }
   
   if ( traceDebug() )
   {
      if ( m_projection.valid() )
      {
         m_projection->print(ossimNotify(ossimNotifyLevel_DEBUG));
      }
      ossimNotify(ossimNotifyLevel_DEBUG) << M << " exiting...\n";
   }
   return m_projection;
}


bool ossimFgdcXmlDoc::getHorizontalDatum(ossimString& s) const
{
   ossimString xpath = "/metadata/spref/horizsys/geodetic/horizdn";
   return getPath(xpath, s);
}

bool ossimFgdcXmlDoc::getPlanarDistantUnits(ossimString& s) const
{
   ossimString xpath = "/metadata/spref/horizsys/planar/planci/plandu";
   return getPath(xpath, s);
}

bool ossimFgdcXmlDoc::getUtmFalseNorthing(ossimString& s) const
{
   ossimString xpath = "/metadata/spref/horizsys/planar/gridsys/utm/transmer/fnorth";
   return getPath(xpath, s); 
}

bool ossimFgdcXmlDoc::getUtmZone(ossimString& s) const
{
   ossimString xpath = "/metadata/spref/horizsys/planar/gridsys/utm/utmzone";
   return getPath(xpath, s); 
}

bool ossimFgdcXmlDoc::getXRes(ossim_float64& v) const
{
   ossimString xpath = "/metadata/spdoinfo/rastinfo/rastxsz";
   if (getPath(xpath, v) == false)
   {
      xpath = "/metadata/spref/horizsys/planar/planci/coordrep/absres";
   }
   return getPath(xpath, v);
}

bool ossimFgdcXmlDoc::getYRes(ossim_float64& v) const
{
   ossimString xpath = "/metadata/spdoinfo/rastinfo/rastysz";
   if (getPath(xpath, v) == false)
   {
      xpath = "/metadata/spref/horizsys/planar/planci/coordrep/ordres";
   }
   return getPath(xpath, v);
}

bool ossimFgdcXmlDoc::getImageSize(ossimIpt& size) const
{
   bool result = false;
   if (  m_xmlDocument.valid() )
   {
      ossimString xpath = "/metadata/spatRepInfo/GridSpatRep/axDimProps/Dimen/dimSize";
      vector<ossimRefPtr<ossimXmlNode> > xml_nodes;
      m_xmlDocument->findNodes(xpath, xml_nodes);
      if (xml_nodes.size() > 1)
      {
         size.x = xml_nodes[0]->getText().toInt32();
         size.y = xml_nodes[1]->getText().toInt32();
         result = true;
      }

      if (result == false)
      {
         xpath = "/metadata/spdoinfo/rastinfo/colcount";
         ossim_float64 x = 0.0;
         result = getPath(xpath, x);
         if (result)
         {
            size.x = (ossim_int32)x;
         }
         xpath = "/metadata/spdoinfo/rastinfo/rowcount";
         ossim_float64 y = 0.0;
         result = getPath(xpath, y);
         if (result)
         {
            size.y = (ossim_int32)y;
         }
      }
   }
   return result;
}

void ossimFgdcXmlDoc::getBoundingBox(ossimDrect& rect) const
{
   rect.makeNan();
   
   if (isOpen())
   {
      double ll_lat = 0.0;
      double ll_lon = 0.0;
      double lr_lat = 0.0;
      double lr_lon = 0.0;
      double ul_lat = 0.0;
      double ul_lon = 0.0;
      double ur_lat = 0.0;
      double ur_lon = 0.0;
     
      ossimString xpath = "/metadata/idinfo/spdom/lboundng/leftbc";
      bool result = getPath(xpath, ul_lon);
      if (!result)
      {
         xpath = "/metadata/idinfo/spdom/bounding/westbc";
         result = getPath(xpath, ul_lon);
         m_boundInDegree = true;
      }
      if (result)
      {
         ll_lon = ul_lon;
      }
      
      xpath = "/metadata/idinfo/spdom/lboundng/rightbc";
      result = getPath(xpath, ur_lon);
      if (!result)
      {
         xpath = "/metadata/idinfo/spdom/bounding/eastbc";
         result = getPath(xpath, ur_lon);
         m_boundInDegree = true;
      }
      if (result)
      {
         lr_lon = ur_lon;
      }

      xpath = "/metadata/idinfo/spdom/lboundng/bottombc";
      result = getPath(xpath, ll_lat);
      if (!result)
      {
         xpath = "/metadata/idinfo/spdom/bounding/southbc";
         result = getPath(xpath, ll_lat);
         m_boundInDegree = true;
      }
      if (result)
      {
         lr_lat = ll_lat;
      }

      xpath = "/metadata/idinfo/spdom/lboundng/topbc";
      result = getPath(xpath, ul_lat);
      if (!result)
      {
         xpath = "/metadata/idinfo/spdom/bounding/northbc";
         result = getPath(xpath, ul_lat);
         m_boundInDegree = true;
      }
      if (result)
      {
         ur_lat = ul_lat;
      }
      
      rect = ossimDrect(ossimDpt(ul_lon, ul_lat),
                        ossimDpt(ur_lon, ur_lat),
                        ossimDpt(lr_lon, lr_lat),
                        ossimDpt(ll_lon, ll_lat), OSSIM_RIGHT_HANDED); 
   }
}

ossim_uint32 ossimFgdcXmlDoc::getNumberOfBands()
{
   ossim_uint32 numOfBands = 0;
   ossimString s;
   ossimString xpath = "/metadata/spdoinfo/rastinfo/vrtcount";
   if (getPath(xpath, s))
   {
      numOfBands = s.toInt();
   }
   return numOfBands;
}

bool ossimFgdcXmlDoc::getPath(const ossimString& path, ossimString& s) const
{
   static const char M[] = "ossimFgdcXmlDoc::getPath";
   
   bool result = false;
   if ( m_xmlDocument.valid() )
   {
      std::vector<ossimRefPtr<ossimXmlNode> > xnodes;
      m_xmlDocument->findNodes(path, xnodes);
      if ( xnodes.size() == 1 ) // Error if more than one.
      {
         if ( xnodes[0].valid() )
         {
            s = xnodes[0]->getText();
            result = true;
         }
         else
         {
            if(traceDebug())
            {
               
               ossimNotify(ossimNotifyLevel_WARN)
                  << M << " ERROR:\n"
                  << "Node not found: " << path
                  << std::endl;
            }
         }
      }
      else if ( xnodes.size() == 0 )
      {
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_WARN) << M << " ERROR:\nNode not found: " << path
               << std::endl;
         }
      }
      else
      {
         if(traceDebug())
         {
            
            ossimNotify(ossimNotifyLevel_WARN)
               << M << " ERROR:\nMultiple nodes found: " << path << std::endl;
         }
      }
   }
   if (!result)
   {
      s.clear();
   }
   return result;      
}
   
bool ossimFgdcXmlDoc::getPath(const ossimString& path, ossim_float64& v) const
{
   ossimString s;
   bool result = getPath(path, s);
   if ( result )
   {
      v = s.toFloat64();
   }
   return result;
}

const ossimDatum* ossimFgdcXmlDoc::createOssimDatum(const ossimString& s) const
{
   ossimString horizdn = s.downcase();
   ossimString datumCode;
   if ( horizdn == "north american datum of 1983" )
   {
      datumCode = "NAR-C";
   }
   else if ( horizdn == "north american datum of 1927" )
   {
      datumCode = "NAS-C";
   }
   else if ( horizdn == "wgs84")
   {
      datumCode = "WGE";
   }
   else
   {
      std::string errMsg = "ossimFgdcXmlDoc::createOssimDatum ERROR: Unhandled datum: ";
      errMsg += horizdn.string();
      throw ossimException(errMsg);
   }
   return ossimDatumFactoryRegistry::instance()->create(datumCode);   
}
