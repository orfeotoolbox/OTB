//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file
//
// Author:  David Burken
//
// Description: Image handelr for TerraSar-X data that is TIFF based.
//
//----------------------------------------------------------------------------
// $Id$

#include <ossimTerraSarTiffReader.h>
#include <ossimTerraSarProductDoc.h>
#include <ossimTerraSarModel.h>
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
#include <ossim/support_data/ossimGeoTiff.h>

// Keyword constants:
static const char PRODUCT_XML_FILE_KW[] = "product_xml_filename";

namespace ossimplugins
{
RTTI_DEF1(ossimTerraSarTiffReader,
          "ossimTerraSarTiffReader",
          ossimTiffTileSource)
}

#ifdef OSSIM_ID_ENABLED
   static const char OSSIM_ID[] = "$Id$";
#endif
   
static ossimTrace traceDebug("ossimTerraSarTiffReader:degug");  

ossimplugins::ossimTerraSarTiffReader::ossimTerraSarTiffReader()
   :
   ossimTiffTileSource()
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimplugins::ossimTerraSarTiffReader::ossimTerraSarTiffReader entered...\n";
#ifdef OSSIM_ID_ENABLED
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "OSSIM_ID:  " << OSSIM_ID << std::endl;
#endif
   }
}

ossimplugins::ossimTerraSarTiffReader::~ossimTerraSarTiffReader()
{
}

bool ossimplugins::ossimTerraSarTiffReader::saveState(
   ossimKeywordlist& kwl, const char* prefix) const
{
   static const char MODULE[] = "ossimplugins::ossimTerraSarTiffReader::saveState";
   
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

bool ossimplugins::ossimTerraSarTiffReader::loadState(
   const ossimKeywordlist& kwl, const char* prefix)
{
   static const char MODULE[] = "ossimplugins::ossimTerraSarTiffReader::loadState";
   
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
      if ( isTerraSarProductFile(theProductXmlFile) )
      {
         //---
         // Although we can open any tiff here we only do if we have matching
         // TerraSAR-X product.xml.
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

ossimString ossimplugins::ossimTerraSarTiffReader::getShortName()const
{
   return ossimString("ossim_terrasar_tiff_reader");
}
   
ossimString ossimplugins::ossimTerraSarTiffReader::getLongName()const
{
   return ossimString("ossim TerraSAR-X tiff reader");
}

ossimString  ossimplugins::ossimTerraSarTiffReader::getClassName()const
{
   return ossimString("ossimTerraSarTiffReader");
}

bool ossimplugins::ossimTerraSarTiffReader::open(const ossimFilename& file)
{
   static const char MODULE[] = "ossimplugins::ossimTerraSarTiffReader::open";
   
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
      ossimRefPtr<ossimXmlDocument> xdoc = new ossimXmlDocument();
      if ( xdoc->openFile(file) )
      {
         // See if it's a TerraSAR-X product xml file.
         if ( isTerraSarProductFile( xdoc.get() ) )
         {
            ossimString s;
            ossimTerraSarProductDoc helper;
            
            if ( helper.getImageFile(xdoc.get(), s) )
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
      
   } // matches: if ( file.ext().downcase() == "xml" )

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " exit status = " << (result?"true":"false\n")
         << std::endl;
   }

   return result;
}

void ossimplugins::ossimTerraSarTiffReader::close()
{
   theProductXmlFile.clear();
   ossimTiffTileSource::close();
}

ossimRefPtr<ossimImageGeometry> ossimplugins::ossimTerraSarTiffReader::getImageGeometry()
{
   if ( !theGeometry )
   {
      // Check for external geom:
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
ossimplugins::ossimTerraSarTiffReader::getInternalImageGeometry() const
{
   static const char MODULE[] = "ossimplugins::ossimTerraSarTiffReader::getInternalImageGeometry";
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entered...\n";
   }

   ossimRefPtr<ossimImageGeometry> geom = new ossimImageGeometry();
   
   ossimRefPtr<ossimXmlDocument> xdoc = new ossimXmlDocument();
   if ( xdoc->openFile(theProductXmlFile) )
   {
      ossimTerraSarProductDoc helper;
      ossimString s;
      if ( helper.getProjection(xdoc.get(), s) )
      {
         s.upcase();
         if ( s == "GROUNDRANGE" )
         {
            ossimRefPtr<ossimTerraSarModel> model =
               new ossimTerraSarModel();
            if ( model->open(theProductXmlFile) )
            {
               // Assign the model to our ossimImageGeometry object.
               geom->setProjection( model.get() );
            }
         }
         else if ( (s == "MAP") && theTiffPtr )
         {
            ossimGeoTiff geotiff;

            //---
            // Note: must pass false to readTags so it doesn't close our
            // tiff pointer.
            //---
            geotiff.readTags(theTiffPtr, getCurrentEntry(), false);
            ossimKeywordlist kwl;
            if(geotiff.addImageGeometry(kwl))
            {
               ossimRefPtr<ossimProjection> proj =
                  ossimProjectionFactoryRegistry::instance()->
                     createProjection(kwl);
            
               if ( proj.valid() )
               {
                  // Assign projection to our ossimImageGeometry object.
                  geom->setProjection( proj.get() );
               }
            }
         }
         else
         {
            if (traceDebug())
            {
               ossimNotify(ossimNotifyLevel_DEBUG)
                  << "WARNING: Unhandled projection: " << s << std::endl;
               
            }
         }
      }
      
   } // matches: if ( xdoc->openFile(theProductXmlFile) )
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " exited..." << std::endl;
   }
   
   return geom;
}

bool ossimplugins::ossimTerraSarTiffReader::isTerraSarProductFile(
   const ossimFilename& file) const
{
   bool result = false;
   
   // Check extension to see if it's xml.
   if ( file.ext().downcase() == "xml" )
   {
      ossimRefPtr<ossimXmlDocument> xdoc = new ossimXmlDocument();
      if ( xdoc->openFile(file) )
      {
         result = isTerraSarProductFile( xdoc.get() );
      }
   }
   
   return result;
}

bool ossimplugins::ossimTerraSarTiffReader::isTerraSarProductFile(
   const ossimXmlDocument* xdoc) const
{
   bool result = false;

   if (xdoc)
   {
      ossimTerraSarProductDoc helper;
      
      // See if it's a TerraSAR-X product xml file.
      result = helper.isTerraSarX(xdoc);
   }
   
   return result;
}
