//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description:
//
// Utility/support data class to extract the projection from a dot aux dot xml
// file commonly found with mrsid data.
// 
//----------------------------------------------------------------------------
// $Id$

#include <ossim/support_data/ossimAuxXmlSupportData.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimXmlAttribute.h>
#include <ossim/base/ossimXmlDocument.h>
#include <ossim/base/ossimXmlNode.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/projection/ossimProjection.h>
#include <fstream>
#include <ostream>

// Static trace for debugging.
static const ossimTrace traceDebug( ossimString("ossimAuxXmlSupportData:debug") );

ossimAuxXmlSupportData::ossimAuxXmlSupportData()
{
}

ossimAuxXmlSupportData::~ossimAuxXmlSupportData()
{
}

ossimRefPtr<ossimProjection> ossimAuxXmlSupportData::getProjection(const ossimFilename& file) const
{
   ossimRefPtr<ossimProjection> result = 0;

   std::ifstream is(file.c_str(), std::ios_base::binary|std::ios_base::in);

   if ( is.good() )
   {
      // Read the xml document:
      ossimXmlDocument xdoc;
      if ( xdoc.read( is ) )
      {
         // Get the WKT string
         ossimString wkt;
         ossimString path = "/PAMDataset/Metadata/GeodataXform/SpatialReference/WKT";
         if ( getPath( path, xdoc, wkt ) )
         {
            if ( wkt.size() )
            {
               // Substitute "&quot;":
               ossimString tmpWkt = wkt.substitute( ossimString("&quot;"), ossimString(""), true );
               
               ossimString name;
               if ( getProjcsName( tmpWkt.string(), name.string() ) )
               {
                  result = ossimProjectionFactoryRegistry::instance()->createProjection( name );
                  if ( result.valid() )
                  {
                     if ( initializeProjection( xdoc, tmpWkt.string(), result.get() ) == false )
                     {
                        // Zero out the result if tie or scale not set.
                        result = 0;
                     }
                  }
               }
            }
         }
      }  
   }
   
   return result;
   
} // End: ossimAuxXmlSupportData::getProjection

bool ossimAuxXmlSupportData::initializeProjection( const ossimXmlDocument xdoc,
                                                   const std::string& wkt, 
                                                   ossimProjection* proj ) const
{
   bool result = false;

   ossimRefPtr<ossimMapProjection> mapProj = dynamic_cast<ossimMapProjection*>( proj );
   if ( mapProj.valid() )
   {
      // Find the tie and scale.
      ossimString path = "/PAMDataset/Metadata/MDI";
      std::vector<ossimRefPtr<ossimXmlNode> > xnodes;
      xdoc.findNodes(path, xnodes);
      if ( xnodes.size() )
      {
         ossimDpt tie;
         ossimDpt scale;
         tie.makeNan();
         scale.makeNan();
         
         for ( ossim_uint32 i = 0; i < xnodes.size(); ++i )
         {
            if ( xnodes[i].valid() )
            {
               ossimString value;
               ossimString attrName = "key";
               
               ossimRefPtr<ossimXmlAttribute> attr = xnodes[i]->findAttribute( attrName );
               if ( attr.valid() )
               {
                  if (attr->getValue() == "IMAGE__XY_ORIGIN" )
                  {
                     value = xnodes[i]->getText();
                     if ( value.size() )
                     {
                        // Split it:
                        std::vector<ossimString> list;
                        value.split( list, ossimString(","), true );
                        if ( list.size() == 2 )
                        {
                           if ( list[0].size() )
                           {
                              tie.x = list[0].toFloat64();
                           }
                           if ( list[1].size() )
                           {
                              tie.y = list[1].toFloat64();
                           }
                        }
                     }
                  }
                  else if (attr->getValue() == "IMAGE__X_RESOLUTION" )
                  {
                     value = xnodes[i]->getText();
                     if ( value.size() )
                     {
                        scale.x = value.toFloat64();
                     }
                  }
                  else if (attr->getValue() == "IMAGE__Y_RESOLUTION" )
                  {
                     value = xnodes[i]->getText();
                     if ( value.size() )
                     {
                        scale.y = value.toFloat64();
                     }
                  }
               }
               
            } // Matches: if ( xnodes[i].valid() )
            
         } // Matches: for ( ossim_uint32 i = 0; i < xnodes.size(); ++i )

         if ( !tie.hasNans() && !scale.hasNans() )
         {
            if ( mapProj->isGeographic() )
            {
               // Assuming tie and scale in decimal degrees:
               mapProj->setDecimalDegreesPerPixel( scale );
               ossimGpt gpt(tie.y, tie.x, 0.0);
               mapProj->setUlTiePoints( ossimGpt( gpt ) );
               result = true;
            }
            else
            {
               // Get the units:
               ossimUnitType units = getUnits( wkt );

               // Convert to meters:
               result = true;
               if ( units != OSSIM_METERS )
               {
                  if ( units == OSSIM_FEET )
                  {
                     tie.x = tie.x * MTRS_PER_FT;
                     tie.y = tie.y * MTRS_PER_FT;
                     scale.x = scale.x * MTRS_PER_FT;
                     scale.y = scale.y * MTRS_PER_FT;                     
                  }
                  else if ( units == OSSIM_US_SURVEY_FEET)
                  {
                     tie.x = tie.x * US_METERS_PER_FT;
                     tie.y = tie.y * US_METERS_PER_FT;
                     scale.x = scale.x * OSSIM_US_SURVEY_FEET;
                     scale.y = scale.y * OSSIM_US_SURVEY_FEET;
                  }
                  else
                  {
                     ossimNotify(ossimNotifyLevel_WARN)
                        << "ossimAuxXmlSupportData::initializeProjection WARNING: "
                        << "Unhandled unit type: " << units << std::endl;
                     result = false;
                  }
               }

               if ( result )
               {
                  mapProj->setMetersPerPixel( scale );
                  mapProj->setUlTiePoints( tie );
               }
            }
         }
         
      } // Matches: if ( xnodes.size() ) 
   }

   return result;
   
} // ossimAuxXmlSupportData::initializeProjection

bool ossimAuxXmlSupportData::getPath(
   const ossimString& path, const ossimXmlDocument& xdoc, ossimString& s) const
{
   bool result = false;

   std::vector<ossimRefPtr<ossimXmlNode> > xnodes;
   xdoc.findNodes(path, xnodes);
   if ( xnodes.size() == 1 ) // Error if more than one.
   {
      if ( xnodes[0].valid() )
      {
         s = xnodes[0]->getText();
         result = true;
      }
      else if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimAuxXmlSupportData::getPath ERROR:\n"
            << "Node not found: " << path
            << std::endl;
      }
   }
   else if ( xnodes.size() == 0 )
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimAuxXmlSupportData::getPath ERROR:\n"
            << "Node not found: " << path
            << std::endl;
      }
   }
   else
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimAuxXmlSupportData::getPath ERROR:\n"
            << "Multiple nodes found: " << path
            << std::endl;
      }
   }

   if (!result)
   {
      s.clear();
   }
   return result;
   
} // bool ossimAuxXmlSupportData::getPath



bool ossimAuxXmlSupportData::getProjcsName( const std::string& wkt,
                                            std::string& name ) const
{
   bool result = false;
   
   if ( wkt.size() )
   {
      // Find "PROJCS":
      std::string::size_type startPos = wkt.find( std::string("PROJCS[") );
      if ( startPos != std::string::npos )
      {
         startPos += 7; // Move to end of projcs[ string.
         
         // Find first comma ',':
         std::string::size_type endPos = wkt.find( ',', startPos+1 );
         if ( endPos != std::string::npos )
         {
            if ( startPos < endPos )
            {
               name = wkt.substr( startPos, endPos-startPos );
               if ( name.size() )
               {
                  result = true;
               }
            }
         }
      }
   }
   
   return result;
                  
} // End: ossimAuxXmlSupportData::getProgcsName

ossimUnitType ossimAuxXmlSupportData::getUnits( const std::string& wkt ) const
{
   ossimUnitType units = OSSIM_UNIT_UNKNOWN;
   if ( wkt.size() )
   {
      // Note this is order dependent, i.e. you have to look for "Foot_US" before "Foot".
      ossimString os = wkt;
      os.downcase();
      if ( os.contains("feet_intl") )
      {
         units = OSSIM_FEET;
      }
      else if ( os.contains("foot_us") )
      {
         units = OSSIM_US_SURVEY_FEET;
      }
      else if ( os.contains("feet") || os.contains("foot") )
      {
         units = OSSIM_FEET;
      }
      //---
      // There can be two "unit" blocks.  One for PROJCS and one for GEOGCS.
      // Do this last as the GEOGCS block will have Degree in it. May need to split
      // the two blocks and then search if this doesn't work.
      //---
      else if ( os.contains("degree") )
      {
         units = OSSIM_DEGREES;
      }
   }

   return units;
   
} // End: ossimAuxXmlSupportData::getUnits
