//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:
// 
// Image handler class for a raster files with an ENVI header file.
//
//----------------------------------------------------------------------------
// $Id$

#include <ossim/imaging/ossimEnviTileSource.h>
#include <ossim/base/ossimAffineTransform.h>
#include <ossim/base/ossimStringProperty.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/imaging/ossimImageGeometryRegistry.h>
#include <ossim/projection/ossimAlphaSensorHRI.h>
#include <ossim/projection/ossimAlphaSensorHSI.h>
#include <ossim/support_data/ossimAlphaSensorSupportData.h>
#include <ossim/support_data/ossimWavelength.h>


RTTI_DEF1(ossimEnviTileSource,
          "ossimEnviTileSource",
          ossimGeneralRasterTileSource)

static ossimTrace traceDebug("ossimEnviTileSource:debug");

ossimEnviTileSource::ossimEnviTileSource()
   :
   ossimGeneralRasterTileSource(),
   m_enviHdr()
{
}

ossimEnviTileSource::~ossimEnviTileSource()
{
}

bool ossimEnviTileSource::open()
{
   static const char MODULE[] = "ossimEnviTileSource::open";
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " entered..."
         << "\nimage file: " << theImageFile << std::endl;
   }

   bool result = false;
   
   if(isOpen())
   {
      close();
   }

   // Look for a header file:
   ossimFilename hdr = theImageFile;
   hdr.setExtension("hdr"); // image.hdr
   if ( !hdr.exists() )
   {
      hdr = theImageFile;
      hdr.string() += ".hdr"; // image.ras.hdr
   }

   if ( hdr.exists() )
   {
      if ( traceDebug() )
      {
         ossimNotify(ossimNotifyLevel_DEBUG) << "header file: " << hdr << std::endl;
      }

      if ( m_enviHdr.open( hdr ) )
      {
         if ( m_rasterInfo.initializeFromEnviHdr( m_enviHdr ) )
         {
            // Set image file for initializeHandler method.
            m_rasterInfo.setImageFile( theImageFile );
            
            // Look for an omd file:
            ossimFilename omd = theImageFile;
            omd.setExtension("omd"); // image.omd
            if ( !omd.exists() )
            {
               omd.setExtension("kwl"); // image.kwl
            }
            
            if ( omd.exists() )
            {
               if ( traceDebug() )
               {
                  ossimNotify(ossimNotifyLevel_DEBUG) << "omd file: " << omd << std::endl;
               }

               // Pick up adjusted min / max values if present.
               ossimKeywordlist kwl( omd );
               m_rasterInfo.getImageMetaData().updateMetaData( kwl, std::string("") );
            }
           
            theMetaData = m_rasterInfo.getImageMetaData();
            
            result = initializeHandler();
            if ( result )
            {
               completeOpen();
               
               //---
               // This will set default output band list if we are a band selector and 
               // "default bands" key is found in the envi header.  If "default bands"
               // is not found it will set to identity(input = output).
               //---
               setDefaultBandList();
            }
         }
      }
   }
   
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " Exit status: " << (result?"true":"false") << std::endl;
   }
   
   return result;
}
   
//************************************************************************************************
//! Returns the image geometry object associated with this tile source or NULL if non defined.
//! The geometry contains full-to-local image transform as well as projection (image-to-world)
//************************************************************************************************
ossimRefPtr<ossimImageGeometry> ossimEnviTileSource::getImageGeometry()
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
         if ( theGeometry.valid()&&!theGeometry->getProjection() )
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

ossimRefPtr<ossimImageGeometry> ossimEnviTileSource::getInternalImageGeometry() const
{
   static const char MODULE[] = "ossimEnviTileSource::getInternalImageGeometry";
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entered...\n";
   }
   ossimRefPtr<ossimImageGeometry> geom;

// debugging to turn off model load
//   return geom;

   geom  = new ossimImageGeometry();

    ossimRefPtr<ossimAlphaSensorSupportData> supData = new ossimAlphaSensorSupportData();
    if ( supData->readSupportFiles( m_enviHdr ) )
    {
       if (supData->isHSI())
       {
          ossimRefPtr<ossimAlphaSensorHSI> sensor = new ossimAlphaSensorHSI();
          if ( sensor->initialize( *(supData.get()) ) )
          {
             geom->setProjection( sensor.get() );
          }
       }
       else
       {
          ossimRefPtr<ossimAlphaSensorHRI> sensor = new ossimAlphaSensorHRI();
          if ( sensor->initialize( *(supData.get()) ) )
          {
             geom->setProjection( sensor.get() );
          }
       }
       
    } // Matches: if ( supData->readSupportFiles( m_enviHdr ) )
    else
    {
      geom = 0;
    }

    return geom;
}

bool ossimEnviTileSource::saveState(ossimKeywordlist& kwl,
                                    const char* prefix)const
{
   return ossimGeneralRasterTileSource::saveState(kwl,prefix);
}

bool ossimEnviTileSource::loadState(const ossimKeywordlist& kwl,
                                    const char* prefix)
{
   return ossimGeneralRasterTileSource::loadState(kwl, prefix);
}

ossimRefPtr<ossimProperty> ossimEnviTileSource::getProperty(
   const ossimString& name)const
{
   if(name == "file_type")
   {
      return new ossimStringProperty(name, getShortName());
   }
   
   return ossimImageHandler::getProperty(name);
}

void ossimEnviTileSource::getPropertyNames(
   std::vector<ossimString>& propertyNames)const
{
   ossimImageHandler::getPropertyNames(propertyNames);
   propertyNames.push_back("file_type");
}

ossimString ossimEnviTileSource::getShortName() const
{
   return ossimString("ENVI");
}
   
ossimString ossimEnviTileSource::getLongName() const
{
   return ossimString("ENVI reader");
}
 

void ossimEnviTileSource::setDefaultBandList()
{
   if ( isBandSelector() )
   {
      // Look in ENVI header for "default bands":
      std::vector<ossim_uint32> bands;
      m_enviHdr.getDefaultBands( bands );

      if ( !bands.size() && ( getNumberOfInputBands() > 2 ) )
      {
         // Try to derive RGB from wavelengths if found in ENVI header.
         ossimWavelength wl;
         if ( wl.initialize( m_enviHdr ) )
         {
            wl.getRgbBands( bands );
         }
      }
      
      if ( bands.size() )
      {            
         ossimImageHandler::setOutputBandList(bands, m_outputBandList);
      }
   }
   
   if ( m_outputBandList.empty() )
   {
      // Initialized to identity (input = output):
      ossimImageSource::getOutputBandList( m_outputBandList );
   }
}
