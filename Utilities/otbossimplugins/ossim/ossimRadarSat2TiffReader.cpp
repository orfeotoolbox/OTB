//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file
//
// Author:  John Stastny
//
// Description: Image handelr for RadarSAT-2 data that is TIFF based.
//
//----------------------------------------------------------------------------
// $Id$

#include <cstdlib>

#include <ossimRadarSat2TiffReader.h>
#include <ossimRadarSat2ProductDoc.h>
#include <ossimRadarSat2Model.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimXmlDocument.h>
#include <ossim/imaging/ossimImageGeometryRegistry.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>

// Keyword constants:
static const char PRODUCT_XML_FILE_KW[] = "product_xml_filename";

namespace ossimplugins
{
RTTI_DEF1(ossimRadarSat2TiffReader,
          "ossimRadarSat2TiffReader",
          ossimTiffTileSource)
}

#ifdef OSSIM_ID_ENABLED
   static const char OSSIM_ID[] = "$Id$";
#endif
   
static ossimTrace traceDebug("ossimRadarSat2TiffReader:degug");  

ossimplugins::ossimRadarSat2TiffReader::ossimRadarSat2TiffReader()
   :
   ossimTiffTileSource()
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimplugins::ossimRadarSat2TiffReader::ossimRadarSat2TiffReader entered...\n";
#ifdef OSSIM_ID_ENABLED
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "OSSIM_ID:  " << OSSIM_ID << std::endl;
#endif
   }
}

ossimplugins::ossimRadarSat2TiffReader::~ossimRadarSat2TiffReader()
{
}

bool ossimplugins::ossimRadarSat2TiffReader::saveState(
   ossimKeywordlist& kwl, const char* prefix) const
{
   static const char MODULE[] = "ossimplugins::ossimRadarSat2TiffReader::saveState";
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " entered...\n";
   }

   // Save our state:
   kwl.add(prefix, PRODUCT_XML_FILE_KW, theProductXmlFile.c_str());

   // Save the base:
   return ossimTiffTileSource::saveState(kwl, prefix);
}

bool ossimplugins::ossimRadarSat2TiffReader::loadState(
   const ossimKeywordlist& kwl, const char* prefix)
{
   static const char MODULE[] = "ossimplugins::ossimRadarSat2TiffReader::loadState";
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " entered...\n";
   }

   bool result = false;

   // Get the product.xml file name.
   const char* lookup = kwl.find(prefix, PRODUCT_XML_FILE_KW);
   if (lookup)
   {
      theProductXmlFile = lookup;
      if ( isRadarSat2ProductFile(theProductXmlFile) )
      {
         //---
         // Although we can open any tiff here we only do if we have matching
         // RS2 product.xml.
         //---
         result = ossimTiffTileSource::loadState(kwl, prefix);
      }
   }

   if (!result)
   {
      theProductXmlFile.clear();
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " exit status = " << (result?"true":"false\n")
         << std::endl;
   }

   return result;
}

ossimString ossimplugins::ossimRadarSat2TiffReader::getShortName()const
{
   return ossimString("ossim_radarsat2_tiff_reader");
}
   
ossimString ossimplugins::ossimRadarSat2TiffReader::getLongName()const
{
   return ossimString("ossim RadarSAT-2 tiff reader");
}

bool ossimplugins::ossimRadarSat2TiffReader::open(const ossimFilename& file)
{
   static const char MODULE[] = "ossimplugins::ossimRadarSat2TiffReader::open";
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " entered...\n"
         << "file: " << file << "\n";
   }

   bool result = false;
   
   if ( isOpen() )
   {
      close();
   }

   // Check extension to see if it's xml.
   if ( file.ext().downcase() == "xml" )
   {
      ossimXmlDocument* xdoc = new ossimXmlDocument();
      if ( xdoc->openFile(file) )
      {
         // See if it's a TerraSAR-X product xml file.
         if ( isRadarSat2ProductFile(xdoc) )
         {
            ossimString s;
            ossimRadarSat2ProductDoc helper;
            
            if ( helper.getImageFile(xdoc, s) )
            {
               ossimFilename imageFile = file.expand().path();
               imageFile = imageFile.dirCat(s);

               setFilename(imageFile);

               result = ossimTiffTileSource::open();
               if (result)
               {
                  theProductXmlFile = file;
                  completeOpen();
               }
            }
         }
      }
      delete xdoc;
      xdoc = 0;
      
   } // matches: if ( file.ext().downcase() == "xml" )

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " exit status = " << (result?"true":"false\n")
         << std::endl;
   }

   return result;
}

void ossimplugins::ossimRadarSat2TiffReader::close()
{
   theProductXmlFile.clear();
   ossimTiffTileSource::close();
}

ossimRefPtr<ossimImageGeometry> ossimplugins::ossimRadarSat2TiffReader::getImageGeometry()
{
   if ( !theGeometry )
   {
      //---
      // Check for external geom:
      //---
      theGeometry = getExternalImageGeometry();
      
      if ( !theGeometry )
      {
         // Check the internal geometry first to avoid a factory call.
         theGeometry = getInternalImageGeometry();

         // At this point it is assured theGeometry is set.
         
         //---
         // WARNING:
         // Must create/set theGeometry at this point or the next call to
         // ossimImageGeometryRegistry::extendGeometry will put us in an infinite loop
         // as it does a recursive call back to ossimImageHandler::getImageGeometry().
         //---         

         // Check for set projection.
         if ( !theGeometry->getProjection() )
         {
            // Try factories for projection.
            ossimImageGeometryRegistry::instance()->extendGeometry(this);
         }
      }

      // Set image things the geometry object should know about.
      initImageParameters( theGeometry.get() );
   }

   return theGeometry;
}

ossimRefPtr<ossimImageGeometry>
ossimplugins::ossimRadarSat2TiffReader::getInternalImageGeometry() const
{
   static const char MODULE[] = "ossimplugins::ossimRadarSat2TiffReader::getInternalImageGeometry";
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entered...\n";
   }

   ossimRefPtr<ossimImageGeometry> geom = new ossimImageGeometry();
   
   ossimXmlDocument* xdoc = new ossimXmlDocument();
   if ( xdoc->openFile(theProductXmlFile) )
   {
      ossimRefPtr<ossimRadarSat2Model> model = new ossimRadarSat2Model();
            
      if ( model->open(theProductXmlFile) )
      {     
         // Assign the model to our ossimImageGeometry object.        
         geom->setProjection( model.get() );      
      } 
      else    
      {       
         if (traceDebug())    
         {
            ossimNotify(ossimNotifyLevel_DEBUG) << "WARNING: Unhandled projection: " << std::endl;
         }
      }
   } // matches: if ( xdoc->openFile(theProductXmlFile) )
   
   delete xdoc;
   xdoc = 0;

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " exited..." << std::endl;
   }
   
   return geom;
}

bool ossimplugins::ossimRadarSat2TiffReader::isRadarSat2ProductFile(
   const ossimFilename& file) const
{
   bool result = false;
   
   // Check extension to see if it's xml.
   if ( file.ext().downcase() == "xml" )
   {
      ossimXmlDocument* xdoc = new ossimXmlDocument();

      if ( xdoc->openFile(file) )
      {
         result = isRadarSat2ProductFile(xdoc);
      }

      delete xdoc;
      xdoc = 0;
   }
   
   return result;
}

bool ossimplugins::ossimRadarSat2TiffReader::isRadarSat2ProductFile(
   const ossimXmlDocument* xdoc) const
{
   bool result = false;

   if (xdoc)
   {
      ossimRadarSat2ProductDoc helper;
      
      // See if it's a TerraSAR-X product xml file.
      result = helper.isRadarSat2(xdoc);
   }
   
   return result;
}
