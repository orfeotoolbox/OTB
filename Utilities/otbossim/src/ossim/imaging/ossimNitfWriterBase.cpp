//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: OSSIM Kakadu based nitf writer.
//
//----------------------------------------------------------------------------
// $Id: ossimNitfWriterBase.cpp 2981 2011-10-10 21:14:02Z david.burken $

#include <ossim/imaging/ossimNitfWriterBase.h>
#include <ossim/base/ossimBooleanProperty.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimPreferences.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/imaging/ossimImageSourceSequencer.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/projection/ossimMapProjectionInfo.h>
#include <ossim/projection/ossimUtmProjection.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/projection/ossimRpcSolver.h>
#include <ossim/support_data/ossimNitfBlockaTag.h>
#include <ossim/support_data/ossimNitfFileHeader.h>
#include <ossim/support_data/ossimNitfFileHeaderV2_X.h>
#include <ossim/support_data/ossimNitfImageHeader.h>
#include <ossim/support_data/ossimNitfImageHeaderV2_X.h>
#include <ossim/support_data/ossimNitfRegisteredTag.h>
#include <ossim/support_data/ossimNitfTagInformation.h>

static const char ENABLE_BLOCKA_KW[] = "enable_blocka_tag";
static const char ENABLE_RPCB_KW[]   = "enable_rpcb_tag";

RTTI_DEF1(ossimNitfWriterBase, "ossimNitfWriterBase", ossimImageFileWriter)
   
static ossimTrace traceDebug(ossimString("ossimNitfWriterBase:debug"));

ossimNitfWriterBase::ossimNitfWriterBase()
   : ossimImageFileWriter(),
     theEnableRpcbTagFlag(false),
     theEnableBlockaTagFlag(true)
{
}

ossimNitfWriterBase::ossimNitfWriterBase(const ossimFilename& filename,
                                         ossimImageSource* inputSource)
   : ossimImageFileWriter(filename, inputSource, 0),
     theEnableRpcbTagFlag(false),
     theEnableBlockaTagFlag(true)
{
}

ossimNitfWriterBase::~ossimNitfWriterBase()
{
}

void ossimNitfWriterBase::setProperty(ossimRefPtr<ossimProperty> property)
{
   if(property.valid())
   {
      ossimString name = property->getName();

      if (name == ENABLE_RPCB_KW)
      {
         theEnableRpcbTagFlag = property->valueToString().toBool();
      }
      else if (name == ENABLE_BLOCKA_KW)
      {
         theEnableBlockaTagFlag = property->valueToString().toBool();
      }
      else
      {
         ossimImageFileWriter::setProperty(property);
      }
   }
}

ossimRefPtr<ossimProperty> ossimNitfWriterBase::getProperty(
   const ossimString& name)const
{
   ossimRefPtr<ossimProperty> result = 0;
   
   if(name == ENABLE_RPCB_KW)
   {
      result = new ossimBooleanProperty(name, theEnableRpcbTagFlag);
   }   
   else if(name == ENABLE_BLOCKA_KW)
   {
      result = new ossimBooleanProperty(name, theEnableBlockaTagFlag);
   }   
   else
   {
      result = ossimImageFileWriter::getProperty(name);
   }

   return result;
}

void ossimNitfWriterBase::getPropertyNames(
   std::vector<ossimString>& propertyNames)const
{
   ossimImageFileWriter::getPropertyNames(propertyNames);

   propertyNames.push_back(ENABLE_BLOCKA_KW);
   propertyNames.push_back(ENABLE_RPCB_KW);
}


bool ossimNitfWriterBase::saveState(ossimKeywordlist& kwl,
                                    const char* prefix) const
{
   kwl.add(prefix, ENABLE_RPCB_KW, theEnableRpcbTagFlag, true);
   kwl.add(prefix, ENABLE_BLOCKA_KW, theEnableBlockaTagFlag, true);

   return ossimImageFileWriter::saveState(kwl, prefix);
}

bool ossimNitfWriterBase::loadState(const ossimKeywordlist& kwl,
                                    const char* prefix)
{
   // Look for the rpcb enable flag keyword.
   const char* lookup = kwl.find(prefix, ENABLE_RPCB_KW);
   if(lookup)
   {
      ossimString os = lookup;
      theEnableRpcbTagFlag = os.toBool();
   }

   // Look for the blocka enable flag keyword.
   lookup = kwl.find(prefix, ENABLE_BLOCKA_KW);
   if(lookup)
   {
      ossimString os = lookup;
      theEnableBlockaTagFlag = os.toBool();
   }

   return ossimImageFileWriter::loadState(kwl, prefix);
}

void ossimNitfWriterBase::writeGeometry(ossimNitfImageHeaderV2_X* hdr,
                                        ossimImageSourceSequencer* seq)
{
   if (hdr && seq)
   {
      ossimRefPtr<ossimImageGeometry> geom = theInputConnection->getImageGeometry();
      ossimKeywordlist kwl;

      if (geom.valid()&&geom->hasProjection())
      {
         // Get the requested bounding rectangles.
         ossimIrect rect = seq->getBoundingRect();
         
         // See if it's a map projection; else, a sensor model.
         ossimMapProjection* mapProj =
            PTR_CAST(ossimMapProjection, geom->getProjection());
         if (mapProj)
         {
            // Use map info to get the corners.
            ossimMapProjectionInfo mapInfo(mapProj, rect);
            mapInfo.setPixelType(OSSIM_PIXEL_IS_AREA);
            
            // See if it's utm.
            ossimUtmProjection* utmProj = PTR_CAST(ossimUtmProjection,
                                                   mapProj);
            if(utmProj)
            {
               ossimDpt ul = mapInfo.ulEastingNorthingPt();
               ossimDpt ur = mapInfo.urEastingNorthingPt();
               ossimDpt lr = mapInfo.lrEastingNorthingPt();
               ossimDpt ll = mapInfo.llEastingNorthingPt();
               
               if(utmProj->getHemisphere() == 'N')
               {
                  hdr->setUtmNorth(utmProj->getZone(), ul, ur, lr, ll);
               }
               else
               {
                  hdr->setUtmSouth(utmProj->getZone(), ul, ur, lr, ll);
               }
            }
            else
            {
               ossimGpt ul = mapInfo.ulGroundPt();
               ossimGpt ur = mapInfo.urGroundPt();
               ossimGpt lr = mapInfo.lrGroundPt();
               ossimGpt ll = mapInfo.llGroundPt();
               hdr->setGeographicLocationDms(ul, ur, lr, ll);
            }

            if (theEnableBlockaTagFlag)
            {
               addBlockaTag(mapInfo, hdr);
            }
         }
         
         if (theEnableRpcbTagFlag)
         {
            addRpcbTag(rect, geom->getProjection(), hdr);
         }
         
      } // matches:  if (proj.valid())
      
   } // matches: if (hdr && seq)
}

void ossimNitfWriterBase::addBlockaTag(ossimMapProjectionInfo& mapInfo,
                                       ossimNitfImageHeaderV2_X* hdr)
{
   if (hdr)
   {
      // Capture the current pixel type.
      ossimPixelType originalPixelType = mapInfo.getPixelType();
      
      // This tag wants corners as area:
      mapInfo.setPixelType(OSSIM_PIXEL_IS_AREA);
      
      // Stuff the blocka tag which has six digit precision.
      ossimNitfBlockaTag* blockaTag = new ossimNitfBlockaTag();
      
      // Set the block number.
      blockaTag->setBlockInstance(1);
      
      // Set the number of lines.
      blockaTag->setLLines(mapInfo.linesPerImage());
      
      // Set first row, first column.
      blockaTag->setFrfcLoc(ossimDpt(mapInfo.ulGroundPt()));
      
      // Set first row, last column.
      blockaTag->setFrlcLoc(ossimDpt(mapInfo.urGroundPt()));
      
      // Set last row, last column.
      blockaTag->setLrlcLoc(ossimDpt(mapInfo.lrGroundPt()));
      
      // Set last row, first column.
      blockaTag->setLrfcLoc(ossimDpt(mapInfo.llGroundPt()));
      
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimNitfWriterBase::addBlockaTag DEBUG:"
            << "\nBLOCKA Tag:" << *((ossimObject*)(blockaTag))
            << std::endl;
      }
      
      // Add the tag to the header.
      ossimRefPtr<ossimNitfRegisteredTag> blockaTagRp = blockaTag;
      ossimNitfTagInformation blockaTagInfo(blockaTagRp);
      hdr->addTag(blockaTagInfo);
      
      // Reset the pixel type to original value
      mapInfo.setPixelType(originalPixelType);
      
   } // matches: if (hdr)
}

void ossimNitfWriterBase::addRpcbTag(const ossimIrect& rect,
                                     ossimProjection* proj,
                                     ossimNitfImageHeaderV2_X* hdr)
{
   if (proj && hdr)
   {
      bool useElevation = false;
      
      if (PTR_CAST(ossimMapProjection, proj))
      {
         // If we're already map projected turn the elevation off.
         useElevation = false;
      }
      
      // Make an rpc solver.
      ossimRefPtr<ossimRpcSolver> rs = new ossimRpcSolver(useElevation);
      
      // Compute the coefficients.
      rs->solveCoefficients(ossimDrect(rect), proj, 64, 64);
      
      // Add the tag.
      ossimRefPtr<ossimNitfRegisteredTag> tag = rs->getNitfRpcBTag();
      ossimNitfTagInformation tagInfo(tag);
      hdr->addTag(tagInfo);
      
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimNitfWriterBase::addRpcbTag DEBUG:"
            << "\nRPCB Tag:" << *((ossimObject*)(tag.get()))
            << "\nProjection:\n";
         
         proj->print(ossimNotify(ossimNotifyLevel_DEBUG));
         
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "\nRect: " << rect << std::endl;
      }
      
   } // matches: if (proj && hdr)
}

void ossimNitfWriterBase::setComplexityLevel(std::streamoff endPosition,
                                             ossimNitfFileHeaderV2_X* hdr)
{
   if (hdr)
   {
      //---
      // See MIL-STD-2500C, Table A-10:
      //
      // Lots of rules here, but for now we will key off of file size.
      //---
      const std::streamoff MB   = 1024 * 1024;
      const std::streamoff MB50 = 50   * MB;
      const std::streamoff GIG  = 1000 * MB;
      const std::streamoff GIG2 = 2    * GIG;
      
      ossimString complexity = "03"; // Less than 50 mb.
      
      if ( (endPosition >= MB50) && (endPosition < GIG) )
      {
         complexity = "05";
      }
      else if ( (endPosition >= GIG) && (endPosition < GIG2) )
      {
         complexity = "06";
      }
      else if (endPosition >= GIG2)
      {
         complexity = "07";
      }
      
      hdr->setComplexityLevel(complexity);
   }
}

ossimString ossimNitfWriterBase::getExtension() const
{
   return ossimString("ntf");
}

void ossimNitfWriterBase::initializeDefaultsFromConfigFile( ossimNitfFileHeaderV2_X* fileHdr,
                                                            ossimNitfImageHeaderV2_X* imgHdr )
{
   // Look in prefs for site configuration file:
   const char* lookup = ossimPreferences::instance()->
      findPreference("nitf_writer.site_configuration_file");
   if ( lookup && fileHdr && imgHdr )
   {
      ossimKeywordlist kwl;
      if ( kwl.addFile( lookup ) )
      {
         fileHdr->loadState( kwl, "nitf.file." );
         imgHdr->loadState( kwl, "nitf.image." );
      }
   }
}

