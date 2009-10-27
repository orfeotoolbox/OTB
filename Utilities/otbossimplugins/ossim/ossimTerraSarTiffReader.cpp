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

#include <cstdlib>

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
      ossimXmlDocument* xdoc = new ossimXmlDocument();
      if ( xdoc->openFile(file) )
      {
         // See if it's a TerraSAR-X product xml file.
         if ( isTerraSarProductFile(xdoc) )
         {
            ossimString s;
            ossimTerraSarProductDoc helper;
            
            if ( helper.getImageFile(xdoc, s) )
            {
               ossimFilename imageFile = file.expand().path();
               imageFile = imageFile.dirCat(s);
               setFilename(imageFile);
               
               result = ossimTiffTileSource::open();

               if (result)
               {
                  theProductXmlFile = file;
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

void ossimplugins::ossimTerraSarTiffReader::close()
{
   theProductXmlFile.clear();
   ossimTiffTileSource::close();
}

ossimImageGeometry* ossimplugins::ossimTerraSarTiffReader::getImageGeometry()
{
   static const char MODULE[] = "ossimplugins::ossimTerraSarTiffReader::getImageGeometry";
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " entered...\n";
   }

   //---
   // Call base class getImageGeometry which will check for external geometry
   // or an already set geometry.
   //---
   ossimImageGeometry* result = ossimImageHandler::getImageGeometry();

   if( result && !result->getProjection() )
   {
      ossimXmlDocument* xdoc = new ossimXmlDocument();
      if ( xdoc->openFile(theProductXmlFile) )
      {
         ossimTerraSarProductDoc helper;
         ossimString s;
         if ( helper.getProjection(xdoc, s) )
         {
            s.upcase();
            if ( s == "GROUNDRANGE" )
            {
               ossimRefPtr<ossimTerraSarModel> model =
                  new ossimTerraSarModel();
               if ( model->open(theProductXmlFile) )
               {
                  // Assign the model to our ossimImageGeometry object.
                  result->setProjection( model.get() );
               }
            }
            else if (s == "MAP")
            {
               
               ossimGeoTiff* gtiff = new ossimGeoTiff( getFilename() );
               if (gtiff->getErrorStatus() == ossimErrorCodes::OSSIM_OK)
               {
                  const char* prefix = 0; // Maybe multi-entry some day???
                  ossimKeywordlist kwl;
                  if (gtiff->addImageGeometry(kwl, prefix))
                  {
                     // Create the projection.
                     ossimRefPtr<ossimProjection> proj =
                        ossimProjectionFactoryRegistry::instance()->
                        createProjection(kwl, prefix);

                     if ( proj.valid() )
                     {
                        // Assign projection to our ossimImageGeometry object.
                        result->setProjection( proj.get() );
                     }
                  }
               }
               delete gtiff;
               gtiff = 0;
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

      delete xdoc;
      xdoc = 0;
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " exited..." << std::endl;
   }
   
   return result;
}

bool ossimplugins::ossimTerraSarTiffReader::isTerraSarProductFile(
   const ossimFilename& file) const
{
   bool result = false;
   
   // Check extension to see if it's xml.
   if ( file.ext().downcase() == "xml" )
   {
      ossimXmlDocument* xdoc = new ossimXmlDocument();

      if ( xdoc->openFile(file) )
      {
         result = isTerraSarProductFile(xdoc);
      }

      delete xdoc;
      xdoc = 0;
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
