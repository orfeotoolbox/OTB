//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: Extensible Metadata Platform (XMP) Info object.
// 
//----------------------------------------------------------------------------
// $Id$

#include <ossim/support_data/ossimXmpInfo.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimXmlDocument.h>
#include <ossim/base/ossimXmlNode.h>
#include <ossim/projection/ossimBilinearProjection.h>
#include <ossim/projection/ossimProjection.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

// Static trace for debugging.
static const ossimTrace traceDebug( ossimString("ossimXmpInfo:debug") );

ossimXmpInfo::ossimXmpInfo()
   : ossimInfoBase(),
     m_file(),
     m_xmpApp1XmlBlock()
{
}

ossimXmpInfo::~ossimXmpInfo()
{
}

bool ossimXmpInfo::open(const ossimFilename& file)
{
   //---
   // NOTE:
   // This parser was written for a specific aerial camera.  If something more generic is
   // needed please provide sample to the group. D. Burken, 17 January 2013.
   //---
   
   bool result = false;

   //---
   // Open the file.
   //---
   std::ifstream str(file.c_str(), std::ios_base::binary|std::ios_base::in);
   if ( str.good() ) 
   {
      // Parse for XMP APP1 XML block. Currently only case.
      std::string xmlString;
      result = getXmpApp1XmlBlock( str, xmlString );
      if ( result )
      {
         m_file = file;
         m_xmpApp1XmlBlock = xmlString;
      }
      else
      {
         m_file.clear();
         m_xmpApp1XmlBlock.clear();
      }
   }

   return result;
}

bool ossimXmpInfo::getXmpApp1XmlBlock( std::ifstream& str, std::string& xmpApp1XmlBlock ) const
{
   bool result = false;

   if ( str.good() )
   {
      ossim_uint8 c;
      
      // Look for Start Of Image (SOI) marker 0xFFD8:
      str.read( (char*)&c, 1);
      if ( c == 0xff )
      {
         str.read( (char*)&c, 1);
         if ( c == 0xd8 )
         {
            // Found SOI marker.
            
            // Now find APP1 marker 0xffe1.  Only do 24 iterations max:
            for ( ossim_uint32 i = 0; i < 24; ++i )
            {
               str.read( (char*)&c, 1);
               if ( c == 0xff )
               {
                  str.read( (char*)&c, 1);
                  if ( c == 0xe1 )
                  {
                     // Found APP1 marker.
                     
                     //---
                     // Next two bytes are the length, MSB first (big endian) per jpeg spec.
                     // length = 2 + 29 + length_of_xmp_packet
                     //---
                     ossim_uint16 xpacketLength = 0;
                     str.read( (char*)&xpacketLength, 2 );
                     
                     // See if system is not big endian and swap if needed.
                     if ( ossim::byteOrder() != OSSIM_BIG_ENDIAN )
                     {
                        ossimEndian swapper;
                        swapper.swap( xpacketLength );
                     }
                     
                     if ( xpacketLength > 31 )
                     {
                        //---
                        // XMP namepsace URI, used as unique ID:
                        // Null terminated ascii string: "http://ns.adobe.com/xap/1.0/"
                        // (29 bytes)
                        // Not using getline until ID verified.
                        //---
                        const ossim_uint32 XMP_ID_SIZE = 29;
                        std::vector<char> v( XMP_ID_SIZE );
                        
                        // Read the next 29 bytes.  (string + null)
                        str.read( &v.front(), XMP_ID_SIZE );
                        
                        // Copy.
                        v.push_back( '\0' ); // Just in case string is bad.
                        std::string s = &v.front();
                        
                        if ( s == "http://ns.adobe.com/xap/1.0/" )
                        {
                           //---
                           // Now read header to determine encoding:
                           // Note: Currently hard coded for UTF-8.
                           //---
                           
                           //---
                           // Look for "<?xpacket begin="
                           // Line has binary data in it or getline could be used.
                           //---
                           const ossim_uint32 HEADER_SIZE = 16;
                           v.resize( HEADER_SIZE );
                           str.read( &v.front(), HEADER_SIZE );
                           
                           v.push_back( '\0' ); // Null terminate.
                           s = &v.front();
                           
                           if ( s == "<?xpacket begin=" )
                           {
                              // Skip the quote which can be single or double.
                              str.seekg( 1, std::ios_base::cur );
                              
                              // Now look for 0xEF, 0xBB, 0xBF which is UTF-8
                              ossim_uint8 encode_bytes[3];
                              str.read( (char*)encode_bytes, 3 );
                              
                              if ( ( encode_bytes[0] == 0xef ) &&
                                   ( encode_bytes[1] == 0xbb ) &&
                                   ( encode_bytes[2] == 0xbf ) )
                              {
                                 // Skip the next six bytes: "' id='"
                                 str.seekg( 6, std::ios_base::cur );
                                 
                                 // Read the ID: W5M0MpCehiHzreSzNTczkc9d
                                 const ossim_uint32 XPACKET_ID_SIZE = 24;
                                 v.resize( XPACKET_ID_SIZE );
                                 str.read ( &v.front(), XPACKET_ID_SIZE );
                                 
                                 v.push_back( '\0' ); // null terminate
                                 s = &v.front();
                                 
                                 if ( s == "W5M0MpCehiHzreSzNTczkc9d" )
                                 {
                                    //---
                                    // Gobble the rest of the line. No more binary characters
                                    // so just use getline.
                                    //---
                                    std::getline( str, s );
                                    
                                    // Read in the xml string (single line):
                                    std::getline( str, xmpApp1XmlBlock );
                                    
                                    if ( xmpApp1XmlBlock.size() )
                                    {
                                       result = true;
                                    }
                                 }
                              }
                              
                           } // Matches: if ( s == "<?xpacket begin=" )
                           
                        } // Matches: if ( s == "http://ns.adobe.com/xap/1.0/" )
                        
                     } // Matches: if ( xpacketLength > 31 )
                  
                  } // Matches: if ( c == 0xd8 )
                  
               } // Matches: if ( c == 0xff )
               
            } // Matches: for ( ossim_uint32 i; i < 24; ++i )
            
         } // Matches: if ( c == 0xd8 )
         
      } // Matches: if ( c == 0xff )
      
   } // Matches: if ( str.good() ) 

   return result;
   
} // End: ossimXmpInfo::getXmpApp1XmlBlock

std::ostream& ossimXmpInfo::print(std::ostream& out) const
{
   if ( getXmpApp1XmlBlock().size() )
   {
      std::string prefix = "";
      std::string ext = m_file.ext().downcase().string();
      if ( ( ext == "jpg" ) || ( ext == "jpeg" ) )
      {
         prefix = "jpeg.";
      }
      
      std::string s;

      // Date:
      getDate( s );
      if ( s.size() )
      {
         out << prefix << "acquisitionDate: " << s << std::endl;
      }      
      
      // Mission ID:
      getMissionId( s );
      if ( s.size() )
      {
         out << prefix << "missionId: " << s << std::endl;
      }

      // Sensor ID:
      getSensorId( s );
      if ( s.size() )
      {
         out << prefix << "sensorId: " << s << std::endl;
      }
      
      out << prefix << "xmp.app1.xml: " << getXmpApp1XmlBlock() << std::endl;
      
   }
   return out;

} // End: std::ostream& ossimXmpInfo::print(std::ostream& out) const

const std::string& ossimXmpInfo::getXmpApp1XmlBlock() const
{
   return m_xmpApp1XmlBlock;
}

ossimRefPtr<ossimProjection> ossimXmpInfo::getProjection( const ossimDrect& imageRect ) const
{
   ossimRefPtr<ossimProjection> result = 0;

   if ( getXmpApp1XmlBlock().size() && ( imageRect.hasNans() == false ) )
   {
      // Open a stream to the xml block.
      std::istringstream is;
      is.str( m_xmpApp1XmlBlock );

      ossimXmlDocument xmlDoc;

      // Read the xml document:
      if ( xmlDoc.read( is ) )
      {
         // Find the four corner ground points.
         ossimString s;
         ossimString path;
         ossimGpt ulg;
         ossimGpt urg;
         ossimGpt lrg;
         ossimGpt llg;
         bool foundAll = false;

         while ( 1 )
         {
            // Upper left:
            path = "/x:xmpmeta/rdf:RDF/rdf:Description/dc:UL_Latitude";
            if ( getPath( path, xmlDoc, s ) )
            {
               ulg.lat = s.toFloat64();
            }
            else
            {
               break;
            }
            path = "/x:xmpmeta/rdf:RDF/rdf:Description/dc:UL_Longitude";
            if ( getPath( path, xmlDoc, s ) )
            {
               ulg.lon = s.toFloat64();
            }
            else
            {
               break;
            }

            // Upper right:
            path = "/x:xmpmeta/rdf:RDF/rdf:Description/dc:UR_Latitude";
            if ( getPath( path, xmlDoc, s ) )
            {
               urg.lat = s.toFloat64();
            }
            else
            {
               break;
            }
            path = "/x:xmpmeta/rdf:RDF/rdf:Description/dc:UR_Longitude";
            if ( getPath( path, xmlDoc, s ) )
            {
               urg.lon = s.toFloat64();
            }
            else
            {
               break;
            }
            
            // Lower right:
            path = "/x:xmpmeta/rdf:RDF/rdf:Description/dc:LR_Latitude";
            if ( getPath( path, xmlDoc, s ) )
            {
               lrg.lat = s.toFloat64();
            }
            else
            {
               break;
            }
            path = "/x:xmpmeta/rdf:RDF/rdf:Description/dc:LR_Longitude";
            if ( getPath( path, xmlDoc, s ) )
            {
               lrg.lon = s.toFloat64();
            }
            else
            {
               break;
            }
            
            // Lower left:
            path = "/x:xmpmeta/rdf:RDF/rdf:Description/dc:LL_Latitude";
            if ( getPath( path, xmlDoc, s ) )
            {
               llg.lat = s.toFloat64();
            }
            else
            {
               break;
            }
            path = "/x:xmpmeta/rdf:RDF/rdf:Description/dc:LL_Longitude";
            if ( getPath( path, xmlDoc, s ) )
            {
               llg.lon = s.toFloat64();

               // If we get here mark the status good:
               foundAll = true;
            }
            else
            {
               break;
            }

            //---
            // Final trailing break out of infinite loop.
            // No code past this point if while loop.
            //---
            break;
         }

         if ( foundAll )
         {
            result = new ossimBilinearProjection( imageRect.ul(),
                                                  imageRect.ur(),
                                                  imageRect.lr(),
                                                  imageRect.ll(),
                                                  ulg,
                                                  urg,
                                                  lrg,
                                                  llg );
         }
         
      } // Matches: if ( xmlDoc.read( is ) )
      
   } // Matches: if ( getXmpApp1XmlBlock().size() )
   
   return result;
   
} // End: ossimXmpInfo::getProjection

void  ossimXmpInfo::getDate( std::string& date ) const
{
   date.clear();
   
   if ( getXmpApp1XmlBlock().size() )
   {
      // Open a stream to the xml block.
      std::istringstream is;
      is.str( m_xmpApp1XmlBlock );
      
      ossimXmlDocument xmlDoc;
      
      // Read the xml document:
      if ( xmlDoc.read( is ) )
      {
         //---
         // ISO8601 date in the form of: YYYY-MM-DDThh:mm:ss.sssZ
         // Find the four corner ground points.
         //---
         ossimString s;
         ossimString path;
         std::string year;
         std::string month;
         std::string dayOfMonth;
         std::string hour;
         std::string minute;
         std::string second;
         std::string milliSecond;
         
         bool foundAll = false;

         while ( 1 )
         {
            // year:
            path = "/x:xmpmeta/rdf:RDF/rdf:Description/dc:IRIG_Year";
            if ( getPath( path, xmlDoc, s ) )
            {
               year = s.string();
            }
            else
            {
               break;
            }
            // month:
            path = "/x:xmpmeta/rdf:RDF/rdf:Description/dc:IRIG_Month";
            if ( getPath( path, xmlDoc, s ) )
            {
               month = s.string();
            }
            else
            {
               break;
            }
            
            // day of month:
            path = "/x:xmpmeta/rdf:RDF/rdf:Description/dc:IRIG_Day_of_Month";
            if ( getPath( path, xmlDoc, s ) )
            {
               dayOfMonth = s.string();
            }
            else
            {
               break;
            }
            
            // hour:
            path = "/x:xmpmeta/rdf:RDF/rdf:Description/dc:IRIG_Hour";
            if ( getPath( path, xmlDoc, s ) )
            {
               hour = s.string();
            }
            else
            {
               break;
            }
            
            // minute:
            path = "/x:xmpmeta/rdf:RDF/rdf:Description/dc:IRIG_Min";
            if ( getPath( path, xmlDoc, s ) )
            {
               minute = s.string();
            }
            else
            {
               break;
            }
            
            // second:
            path = "/x:xmpmeta/rdf:RDF/rdf:Description/dc:IRIG_Sec";
            if ( getPath( path, xmlDoc, s ) )
            {
               second = s.string();
            }
            else
            {
               break;
            }
            
            // millisecond:
            path = "/x:xmpmeta/rdf:RDF/rdf:Description/dc:IRIG_Msec";
            if ( getPath( path, xmlDoc, s ) )
            {
               milliSecond = s.string();
            }
            else
            {
               break;
            }

            // If we get here mark the status good for downstream code.
            foundAll = true;
            
            //---
            // Final trailing break out of infinite loop.
            // No code past this point if while loop.
            //---
            break;

         } // End: while ( 1 )
         
         if ( foundAll )
         {
            date = year + "-"; // Assuming year in 4 digit form, e.g. 2013.

            // Month two digit wide.
            std::ostringstream os1;
            os1 << std::setiosflags(std::ios_base::fixed|std::ios_base::right) 
                << std::setfill('0')
                << std::setw(2)
                << month;
            date += os1.str() + "-";

            // Day two digit wide.
            std::ostringstream os2;
            os2 << std::setiosflags(std::ios_base::fixed|std::ios_base::right) 
                << std::setfill('0')
                << std::setw(2)
                << dayOfMonth;
            date += os2.str() + "T";

            // Hour two digit wide.
            std::ostringstream os3;
            os3 << std::setiosflags(std::ios_base::fixed|std::ios_base::right) 
                << std::setfill('0')
                << std::setw(2)
                << hour;
            date += os3.str() + ":";

            // Minute two digit wide.
            std::ostringstream os4;
            os4 << std::setiosflags(std::ios_base::fixed|std::ios_base::right) 
                << std::setfill('0')
                << std::setw(2)
                << minute;
            date += os4.str() + ":";

            // Second two digit wide.
            std::ostringstream os5;
            os5 << std::setiosflags(std::ios_base::fixed|std::ios_base::right) 
                << std::setfill('0')
                << std::setw(2)
                << second;
            date += os5.str() + ".";

             // Milli second three digit wide.
            std::ostringstream os6;
            os6 << std::setiosflags(std::ios_base::fixed|std::ios_base::right) 
                << std::setfill('0')
                << std::setw(3)
                << milliSecond;
            date += os6.str() + "Z"; // Assumption...
         }
         
      } // Matches: if ( xmlDoc.read( is ) )
      
   } // Matches: if ( getXmpApp1XmlBlock().size() )
   
} // End: void  ossimXmpInfo::getDate( std::string& date ) const


void ossimXmpInfo::getMissionId( std::string& mission ) const
{
   // Currently nothing in sample data to grab so using mission "x".
   mission = "x";
}

void ossimXmpInfo::getSensorId( std::string& sensor ) const
{
   // Currently nothing in sample data to grab so using sensor "x".
   sensor = "x";
}

bool ossimXmpInfo::getPath(
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
            << "ossimXmpInfo::getPath ERROR:\n"
            << "Node not found: " << path
            << std::endl;
      }
   }
   else if ( xnodes.size() == 0 )
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimXmpInfo::getPath ERROR:\n"
            << "Node not found: " << path
            << std::endl;
      }
   }
   else
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimXmpInfo::getPath ERROR:\n"
            << "Multiple nodes found: " << path
            << std::endl;
      }
   }

   if (!result)
   {
      s.clear();
   }
   return result;
   
} // bool ossimXmpInfo::getPath

