//----------------------------------------------------------------------------
//
// License:  ?
//
// Author:  Chia Aik Song & Yin Tiangang
//
// Description: Image handler for ALOS PALSAR data.
//
//----------------------------------------------------------------------------
// $Id$

#include <cstdlib>

#include <ossimTerraSarTiffReader.h>
#include <ossimAlosPalsarReader.h>
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

RTTI_DEF1(ossimAlosPalsarReader, "ossimAlosPalsarReader", ossimTiffTileSource)

#ifdef OSSIM_ID_ENABLED
   static const char OSSIM_ID[] = "$Id$";
#endif
   
static ossimTrace traceDebug("ossimAlosPalsarReader:debug");

ossimAlosPalsarReader::ossimAlosPalsarReader()
   :
   ossimTiffTileSource()
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimAlosPalsarReader::ossimAlosPalsarReader entered...\n";
#ifdef OSSIM_ID_ENABLED
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "OSSIM_ID:  " << OSSIM_ID << std::endl;
#endif
   }
}

ossimAlosPalsarReader::~ossimAlosPalsarReader()
{
}

bool ossimAlosPalsarReader::saveState(ossimKeywordlist& kwl,
                                        const char* prefix) const
{
   static const char MODULE[] = "ossimAlosPalsarReader::saveState";
   
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

bool ossimAlosPalsarReader::loadState(const ossimKeywordlist& kwl,
                                        const char* prefix)
{
   static const char MODULE[] = "ossimAlosPalsarReader::loadState";
   
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

ossimString ossimAlosPalsarReader::getShortName()const
{
   return ossimString("ossim_alospalsar_reader");
}
   
ossimString ossimAlosPalsarReader::getLongName()const
{
   return ossimString("ossim ALOS PALSAR reader");
}

ossimString  ossimAlosPalsarReader::getClassName()const
{
   return ossimString("ossimAlosPalsarReader");
}

bool ossimAlosPalsarReader::open(const ossimFilename& file)
{
   static const char MODULE[] = "ossimAlosPalsarReader::open";
   
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

void ossimAlosPalsarReader::close()
{
   theProductXmlFile.clear();
   ossimTiffTileSource::close();
}

bool ossimAlosPalsarReader::getImageGeometry(ossimKeywordlist& kwl,
                                               const char* prefix)
{
   static const char MODULE[] = "ossimAlosPalsarReader::getImageGeometry";
   
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

bool ossimAlosPalsarReader::isTerraSarProductFile(
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

bool ossimAlosPalsarReader::isTerraSarProductFile(const ossimXmlDocument* xdoc) const
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
