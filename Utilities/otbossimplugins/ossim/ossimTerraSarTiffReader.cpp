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

// Keyword constants:
static const char PRODUCT_XML_FILE_KW[] = "product_xml_filename";

RTTI_DEF1(ossimTerraSarTiffReader, "ossimTerraSarTiffReader", ossimTiffTileSource)

#ifdef OSSIM_ID_ENABLED
   static const char OSSIM_ID[] = "$Id$";
#endif
   
static ossimTrace traceDebug("ossimTerraSarTiffReader:degug");  

ossimTerraSarTiffReader::ossimTerraSarTiffReader()
   :
   ossimTiffTileSource()
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimTerraSarTiffReader::ossimTerraSarTiffReader entered...\n";
#ifdef OSSIM_ID_ENABLED
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "OSSIM_ID:  " << OSSIM_ID << std::endl;
#endif
   }
}

ossimTerraSarTiffReader::~ossimTerraSarTiffReader()
{
}

bool ossimTerraSarTiffReader::saveState(ossimKeywordlist& kwl,
                                        const char* prefix) const
{
   static const char MODULE[] = "ossimTerraSarTiffReader::saveState";
   
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

bool ossimTerraSarTiffReader::loadState(const ossimKeywordlist& kwl,
                                        const char* prefix)
{
   static const char MODULE[] = "ossimTerraSarTiffReader::loadState";
   
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

ossimString ossimTerraSarTiffReader::getShortName()const
{
   return ossimString("ossim_terrasar_tiff_reader");
}
   
ossimString ossimTerraSarTiffReader::getLongName()const
{
   return ossimString("ossim TerraSAR-X tiff reader");
}

ossimString  ossimTerraSarTiffReader::getClassName()const
{
   return ossimString("ossimTerraSarTiffReader");
}

bool ossimTerraSarTiffReader::open(const ossimFilename& file)
{
   static const char MODULE[] = "ossimTerraSarTiffReader::open";
   
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

void ossimTerraSarTiffReader::close()
{
   theProductXmlFile.clear();
   ossimTiffTileSource::close();
}

bool ossimTerraSarTiffReader::getImageGeometry(ossimKeywordlist& kwl,
                                               const char* prefix)
{
   static const char MODULE[] = "ossimTerraSarTiffReader::getImageGeometry";
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " entered...\n";
   }
   
   if (theGeometryKwl.getSize())
   {
      kwl.add(prefix, theGeometryKwl);
      //kwl = theGeometryKwl;
      return true;
   }
   
   bool result = false;
   ossimRefPtr<ossimProjection> proj = 0;
   ossimFilename filename = getFilenameWithThisExtension(ossimString(".geom"));
   if(!filename.exists())
   {
      // Try tacking on the entry like "foo_e0.hdf".
      filename = getFilenameWithThisExtension(ossimString(".geom"), true);
   }
   if(filename.exists())
   {
      if(kwl.addFile(filename))
      {
         ossimString tempPrefix =
            "image"+ossimString::toString(getCurrentEntry())+".";
         kwl.stripPrefixFromAll(tempPrefix);
         
         if(kwl.find(ossimKeywordNames::TYPE_KW))
         {
            ossimKeywordlist kwlTemp(kwl);
            
            kwlTemp.add(ossimKeywordNames::GEOM_FILE_KW,
                        filename.c_str(),
                        true);

            proj = ossimProjectionFactoryRegistry::instance()->
               createProjection(kwlTemp);
            if(proj.valid())
            {
               result = true;
               kwl.add(prefix, kwlTemp);//kwl = kwlTemp;
            }
         }
      }
   }
   
   if(!result)
   {
      ossimTerraSarModel* model = new ossimTerraSarModel();
      if ( model->open(theProductXmlFile) )
      {
         result = model->saveState(kwl, prefix);
      }

      delete model;
      model = 0;
   }
   
   if (result == true)
   {
      // Capture the geometry for next time.
      theGeometryKwl.clear();
      theGeometryKwl.add(kwl, prefix, true);
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " exit status = " << (result?"true":"false\n")
         << std::endl;
   }
   
   return result;
}

bool ossimTerraSarTiffReader::isTerraSarProductFile(
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

bool ossimTerraSarTiffReader::isTerraSarProductFile(const ossimXmlDocument* xdoc) const
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
