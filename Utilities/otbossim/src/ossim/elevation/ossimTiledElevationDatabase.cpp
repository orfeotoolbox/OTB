//----------------------------------------------------------------------------
//
// File: ossimTiledElevationDatabase.cpp
// 
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: See class desciption in header file.
// 
//----------------------------------------------------------------------------
// $Id$

#include <ossim/elevation/ossimTiledElevationDatabase.h>

#include <ossim/base/ossimCallback2wRet.h>
#include <ossim/base/ossimDblGrid.h>
#include <ossim/base/ossimDirectory.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimException.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/imaging/ossimImageGeometry.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/imaging/ossimOrthoImageMosaic.h>
#include <ossim/imaging/ossimSingleImageChain.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/util/ossimFileWalker.h>

#include <cmath>

static ossimTrace traceDebug(ossimString("ossimTiledElevationDatabase:debug"));

RTTI_DEF1(ossimTiledElevationDatabase, "ossimTiledElevationDatabase", ossimElevationDatabase);

//---
// Call back class to register with ossimFileWalker for call to
// ossimTiledElevationDatabase::processFile
//
// Placed here as it is unique to this class.
//---
class ProcessFileCB: public ossimCallback2wRet<const ossimFilename&, bool&, bool>
{
public:
   ProcessFileCB(
      ossimTiledElevationDatabase* obj,
      bool (ossimTiledElevationDatabase::*func)(const ossimFilename&, bool&))
      :
      m_obj(obj),
      m_func(func)
   {}
      
   virtual bool operator()(const ossimFilename& file, bool& flag) const
   {
      return ( m_obj->*m_func)(file, flag);
   }

private:
   ossimTiledElevationDatabase* m_obj;
   bool (ossimTiledElevationDatabase::*m_func)(const ossimFilename& file, bool& flag);
};

ossimTiledElevationDatabase::ossimTiledElevationDatabase()
   :
   ossimElevationDatabase(),
   m_entries(0),
   m_grid(0),
   m_referenceProj(0),
   m_requestedRect(),
   m_entryListRect()
{
   m_requestedRect.makeNan();
   m_entryListRect.makeNan();

   //---
   // Use the ossimElevSource::theGroundRect to hold the mapped rect expanded to even post
   // boundaries.
   //---
   theGroundRect.makeNan();
}

// Protected destructor as this is derived from ossimRefenced.
ossimTiledElevationDatabase::~ossimTiledElevationDatabase()
{
   m_referenceProj = 0;   
   m_entries.clear();
   if ( m_grid )
   {
      delete m_grid;
      m_grid = 0;
   }
}

bool ossimTiledElevationDatabase::open(const ossimString& connectionString)
{
   return false; // tmp drb...
   
   static const char M[] = "ossimTiledElevationDatabase::open";
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << M << " entered...\n"
         << "\nConnection string: " << connectionString << "\n";
   }                   
   
   bool result = false;

   close();

   if ( connectionString.size() )
   {
      m_connectionString = connectionString.c_str();
      result = true;
   }

   if ( m_entries.size() ) result = true;
   
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << M << " result=" << (result?"true\n":"false\n");
   }

   return result;
}

void ossimTiledElevationDatabase::close()
{
   m_entryListRect.makeNan();
   m_requestedRect.makeNan();
   theGroundRect.makeNan();
   m_referenceProj = 0;
   m_entries.clear();
   if ( m_grid )
   {
      delete m_grid;
      m_grid = 0;
   }
   m_meanSpacing = 0.0;
   m_geoid = 0;
   m_connectionString.clear();
}

void ossimTiledElevationDatabase::mapRegion(const ossimGrect& region)
{
   static const char M[] = "ossimTiledElevationDatabase::mapRegion";
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << M << " entered...\n" << "region: " << region << "\n";
   }
   
   if ( m_connectionString.size() )
   {
      // Put these outside the try block so we can delete if exception thrown.
      ossimFileWalker* fw = 0;
      ossimCallback2wRet<const ossimFilename&, bool&, bool>* cb = 0;

      // Wrap in try catch block as excptions can be thrown under the hood.
      try
      {
         m_requestedRect = region;
         
         ossimFilename f = m_connectionString;
         if ( f.exists() )
         {
            // Walk the directory
            fw = new ossimFileWalker();
            fw->initializeDefaultFilterList();
            cb = new ProcessFileCB(this, &ossimTiledElevationDatabase::processFile);
            fw->registerProcessFileCallback(cb);
            fw->walk(f);

            mapRegion();
         }
         else
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << M << " ERROR: Connection string does not exists: "
               << m_connectionString.c_str()  << endl;
         }
      }
      catch (const ossimException& e)
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "Caught exception: " << e.what() << endl;
         m_entries.clear();
      }

      // cleanup:
      if ( fw )
      {
         delete fw;
         fw = 0;
      }
      if ( cb )
      {
         delete cb;
         cb = 0;
      }
   }

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << M << " exited...\n";
   }
}

bool ossimTiledElevationDatabase::processFile(const ossimFilename& file,
                                              bool& recurseFlag)
{
   static const char M[] = "ossimTiledElevationDatabase::processFile";
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << M << " entered...\n" << "file: " << file << "\n";
   }
   bool continueFlag = true;

   ossimRefPtr<ossimSingleImageChain> sic = new ossimSingleImageChain();
   if ( sic->open(file, false) ) // False for do not open overviews.
   {
      // Set the directory walker flag.
      recurseFlag = !(isDirectoryBasedImage(sic->getImageHandler()));
     
      ossimRefPtr<ossimImageHandler> ih = sic->getImageHandler();
      if ( ih.valid() && (m_requestedRect.isLonLatNan() == false) )
      {
         ossimRefPtr<ossimImageGeometry> geom = ih->getImageGeometry();
         if ( geom.valid() == false )
         {
            std::string errMsg = M;
            errMsg += " ERROR:\nNo image geometry for image: ";
            errMsg += ih->getFilename().string();
            throw ossimException(errMsg);
         }
         
         ossimRefPtr<ossimProjection> proj = geom->getProjection();
         if ( proj.valid() == false )
         {
            std::string errMsg = M;
            errMsg += " ERROR:\nNo image projection for image: ";
            errMsg += ih->getFilename().string();
            throw ossimException(errMsg);
         }
         
         // Get the bounding rect:
         ossimGrect boundingRect;
         getBoundingRect(geom, boundingRect);
         
         if ( boundingRect.isLonLatNan() )
         {
            std::string errMsg = M;
            errMsg += " ERROR:\nBounding rect has nans for image: ";
            errMsg += ih->getFilename().string();
            throw ossimException(errMsg); 
         }
         
         if ( boundingRect.intersects(m_requestedRect) )
         {
            bool addEntryToList = false;
            
            if ( m_entries.size() == 0 ) // First time through.
            {
               addEntryToList = true;
               m_entryListRect = boundingRect;
               m_referenceProj = proj;
               m_meanSpacing = (geom->getMetersPerPixel().x + geom->getMetersPerPixel().y) / 2.0;
            }
            else 
            {
               addEntryToList = isCompatible( ih.get(), geom.get(), proj.get() );
               if ( addEntryToList )
               {
                  // Expand the rectangle.
                  m_entryListRect.combine(boundingRect);
               }
            }
            
            if ( addEntryToList )
            {
               // If we're keeping it add a cache to the chain.
               sic->addCache();
               
               //---
               // Create the entry and give to addEntry which checks for duplicates in case
               // mapRegion was called consecutively.
               //---
               ossimTiledElevationEntry entry(boundingRect, sic);
               addEntry(entry);

               continueFlag = m_requestedRect.completely_within(m_entryListRect);
            }
         }
      }
      else
      {
         std::string errMsg = M;
         errMsg += " ERROR:\nNo image geometry for image: ";
         errMsg += ih->getFilename().string();
         throw ossimException(errMsg);
      }
   }
   
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << M << " recurseFlag=" << recurseFlag
         << " continueFlag=" << continueFlag << "\n";
   }

   return continueFlag;
}

void ossimTiledElevationDatabase::addEntry(const ossimTiledElevationEntry& entry)
{
   bool result = true;
   std::vector<ossimTiledElevationEntry>::const_iterator i = m_entries.begin();
   while ( i != m_entries.end() )
   {
      if ( (*i).m_sic->getFilename() == entry.m_sic->getFilename() )
      {
         result = false; // In list already.
         break;
      }
      ++i;
   }
   if ( result )
   {
      m_entries.push_back(entry); // Add to list.
   }
}

void ossimTiledElevationDatabase::initializeReferenceProjection()
{
   if ( m_entries.size() )
   {
      ossimRefPtr<ossimImageHandler> ih = m_entries[0].m_sic->getImageHandler();
      if ( ih.valid() )
      {
         ossimRefPtr<ossimImageGeometry> geom = ih->getImageGeometry();
         if ( geom.valid() )
         {
            m_referenceProj = geom->getProjection();
         }
         // else throw exception ??? (drb)
      }
   }
}

double ossimTiledElevationDatabase::getHeightAboveMSL(const ossimGpt& gpt)
{
   if ( m_grid )
   {
      return (*m_grid)(gpt.lon, gpt.lat);
   }
   return ossim::nan();
}

double ossimTiledElevationDatabase::getHeightAboveEllipsoid(const ossimGpt& gpt)
{
   double h = getHeightAboveMSL(gpt);
   if(h != ossim::nan())
   {
      h += getOffsetFromEllipsoid(gpt);
   }
   return h;
}
 
bool ossimTiledElevationDatabase::pointHasCoverage(const ossimGpt& gpt) const
{
   if ( theGroundRect.isLonLatNan() == false)
   {
      return theGroundRect.pointWithin(gpt);
   }
   return false;
}

double ossimTiledElevationDatabase::getAccuracyLE90(const ossimGpt& /* gpt */) const
{
   return 0;
}

double ossimTiledElevationDatabase::getAccuracyCE90(const ossimGpt& /* gpt */) const
{
   return 0;
}

bool ossimTiledElevationDatabase::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
   static const char M[] = "ossimTiledElevationDatabase::loadState";
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << M << " entered..." << "\nkwl:\n" << kwl << "\n";
   }     
   bool result = false;
   const char* lookup = kwl.find(prefix, "type");
   if ( lookup )
   {
      std::string type = lookup;
      if ( ( type == "image_directory" ) || ( type == "ossimTiledElevationDatabase" ) )
      {
         result = ossimElevationDatabase::loadState(kwl, prefix);
      }
   }

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << M << " result=" << (result?"true\n":"false\n");
   }

   return result;
}

bool ossimTiledElevationDatabase::saveState(ossimKeywordlist& kwl, const char* prefix) const
{
   return ossimElevationDatabase::saveState(kwl, prefix);
}

// Private method:
bool ossimTiledElevationDatabase::isDirectoryBasedImage(ossimRefPtr<ossimImageHandler> ih)
{
   bool result = false;
   if ( ih.valid() )
   {
      // Get the image handler name.
      ossimString imageHandlerName = ih->getClassName();
      if ( (imageHandlerName == "ossimAdrgTileSource") ||
           (imageHandlerName == "ossimCibCadrgTileSource") )  
      {
         result = true;
      }
   }
   return result;
}

bool ossimTiledElevationDatabase::isCompatible(ossimImageHandler* ih,
                                               ossimImageGeometry* geom,
                                               ossimProjection* proj) const
{
   //---
   // Check for match of the following against the first entry:
   // bands
   // projection
   // scalar type
   // scale
   //---
   bool result = false;

   if ( m_entries.size() && ih && geom && proj )
   {
      ossimRefPtr<ossimImageHandler> entry0_ih = m_entries[0].m_sic->getImageHandler();
      
      // Check scalar type and bands.
      if ( (entry0_ih->getOutputScalarType() == ih->getOutputScalarType()) &&
           (entry0_ih->getNumberOfOutputBands() == ih->getNumberOfOutputBands()) )
      {
         // Check the scale.
         ossimRefPtr<const ossimMapProjection> mapProj =
            dynamic_cast<const ossimMapProjection*>(proj);
         if ( mapProj.valid() )
         {
            ossimRefPtr<const ossimMapProjection> refMapProj =
               dynamic_cast<const ossimMapProjection*>(m_referenceProj.get());
            
            if ( mapProj->isGeographic() )
            {
               if ( refMapProj->getDecimalDegreesPerPixel() ==
                    mapProj->getDecimalDegreesPerPixel() )
               {
                  result = true;
               }
               else if ( refMapProj->getMetersPerPixel() ==
                         mapProj->getMetersPerPixel() )
               {
                  result = true;
               }
            }
         }
         else // sensor model
         {
            if ( proj->getMetersPerPixel() == m_referenceProj->getMetersPerPixel() )
            {
               result = true;
            }
         }
      }
   }
   
   return result;
}

void ossimTiledElevationDatabase::getBoundingRect(
   ossimRefPtr<ossimImageGeometry> geom, ossimGrect& boundingRect) const
{
   if ( geom.valid() )
   {
      std::vector<ossimGpt> corner(4);
      if ( geom->getCornerGpts(corner[0], corner[1], corner[2], corner[3]) )
      {
         ossimGpt ulGpt(corner[0]);
         ossimGpt lrGpt(corner[0]);
         for ( ossim_uint32 i = 1; i < 4; ++i )
         {
            if ( corner[i].lon < ulGpt.lon ) ulGpt.lon = corner[i].lon;
            if ( corner[i].lat > ulGpt.lat ) ulGpt.lat = corner[i].lat;
            if ( corner[i].lon > lrGpt.lon ) lrGpt.lon = corner[i].lon;
            if ( corner[i].lat < lrGpt.lat ) lrGpt.lat = corner[i].lat;
         }
         boundingRect = ossimGrect(ulGpt, lrGpt);
      }
      else
      {
         boundingRect.makeNan();
      }
   }
}
void ossimTiledElevationDatabase::mapRegion()
{
   static const char M[] = "ossimTiledElevationDatabase::mapRegion";
   
   if ( m_entries.size() && ( m_requestedRect.isLonLatNan() == false ) )
   {
      ossimRefPtr<ossimOrthoImageMosaic> mosaic = new ossimOrthoImageMosaic();
      std::vector<ossimTiledElevationEntry>::iterator i = m_entries.begin();
      while ( i != m_entries.end() )
      {
         mosaic->connectMyInputTo( (*i).m_sic.get() );
         ++i;
      }

      // Compute the requested rectangle in view space.
      ossimRefPtr<ossimImageGeometry> geom = mosaic->getImageGeometry();
      if ( geom.valid() )
      {
         ossimDpt ulDpt;
         ossimDpt lrDpt;
         geom->worldToLocal(m_requestedRect.ul(), ulDpt);
         geom->worldToLocal(m_requestedRect.lr(), lrDpt);

         // Expand out to fall on even view coordinates.
         ulDpt.x = std::floor(ulDpt.x);
         ulDpt.y = std::floor(ulDpt.y);
         lrDpt.x = std::ceil(lrDpt.x);
         lrDpt.y = std::floor(lrDpt.y);

         // Get new(expanded) corners in ground space.
         ossimGpt ulGpt;
         ossimGpt lrGpt;
         geom->localToWorld(ulDpt, ulGpt);
         geom->localToWorld(lrDpt, lrGpt);
         theGroundRect = ossimGrect(ulGpt, lrGpt);
            
         // Expanded requested rect in view space.
         ossimIpt ulIpt = ulDpt;
         ossimIpt lrIpt = lrDpt;
         const ossimIrect VIEW_RECT(ulIpt, lrIpt);

         // Get the data.
         ossimRefPtr<ossimImageData> data = mosaic->getTile(VIEW_RECT, 0);
         if ( data.valid() )
         {
            // Initialize the grid:
            const ossimIpt SIZE( data->getWidth(), data->getHeight() );
            const ossimDpt ORIGIN(ulGpt.lon, lrGpt.lat); // SouthWest corner
            const ossimDpt SPACING( (lrGpt.lon-ulGpt.lon)/(SIZE.x-1),
                                    (ulGpt.lat-lrGpt.lat)/(SIZE.y-1) );
            if ( !m_grid ) m_grid = new ossimDblGrid();
            m_grid->initialize(SIZE, ORIGIN, SPACING, ossim::nan());

            if ( traceDebug() )
            {
               ossimNotify(ossimNotifyLevel_DEBUG)
                  << M
                  << "\nrequested view rect: " << VIEW_RECT
                  << "\nexpanded ground rect: " << theGroundRect
                  << "\norigin:  " << ORIGIN
                  << "\nsize:    " << SIZE
                  << "\nspacing: " << SPACING << std::endl;
            }

            // Fill the grid:
            switch( data->getScalarType() )
            {
               case OSSIM_SINT16:
               {
                  fillGrid(ossim_sint16(0), data);
                  break;
               }
               case OSSIM_SINT32:
               {
                  fillGrid(ossim_sint32(0), data);
                  break;
               }
               case OSSIM_FLOAT32:
               {
                  fillGrid(ossim_float32(0), data);
                  break;
               }
               case OSSIM_FLOAT64:
               {
                  fillGrid(ossim_float64(0), data);
                  break;
               }
               case OSSIM_UINT8:
               case OSSIM_SINT8:
               case OSSIM_USHORT11:
               case OSSIM_UINT16:
               case OSSIM_UINT32:
               case OSSIM_NORMALIZED_DOUBLE:
               case OSSIM_NORMALIZED_FLOAT:
               case OSSIM_SCALAR_UNKNOWN:
               default:
               {
                  std::string errMsg = M;
                  errMsg += " ERROR:\nUnhandled scalar type: ";
                  errMsg += data->getScalarTypeAsString().string();
                  throw ossimException(errMsg);
               }
            }
            
         } // if ( data.valid() )

      } // if ( geom.valid() )

   } // if ( m_entries.size() && ...
}

template <class T> void ossimTiledElevationDatabase::fillGrid(T /* dummyTemplate */,
                                                              ossimRefPtr<ossimImageData> data)
{
   if ( data.valid() )
   {
      // Copy to grid reversing the lines as the ossimDblGrid's origin is the SW corner.
      const ossim_float64 NP  = data->getNullPix(0);
      const T* buf = static_cast<T*>(data->getBuf(0));
      if ( buf )
      {
         const ossimIpt SIZE( data->getWidth(), data->getHeight() );
         ossim_int32 bufIdx = (SIZE.y-1) * data->getWidth();
         ossim_int32 grdY = 0;
         for (ossim_int32 y = SIZE.y-1; y >= 0; --y)
         {
            for (ossim_int32 x = 0; x < SIZE.x; ++ x)
            {
               ossim_float64 v = static_cast<ossim_float64>(buf[bufIdx+x]);
               m_grid->setNode(x, grdY, (v!=NP?v:ossim::nan()) );
            }
            bufIdx -= data->getWidth();
            ++grdY;
         }
      }
   }
}

// Hidden from use...
ossimTiledElevationDatabase::ossimTiledElevationDatabase(
   const ossimTiledElevationDatabase& /* copy_this */)
{
}

// Hidden from ues...
const ossimTiledElevationDatabase& ossimTiledElevationDatabase::operator=(
   const ossimTiledElevationDatabase& /* rhs */)
{
   return *this;
}

// Private container class:
ossimTiledElevationDatabase::ossimTiledElevationEntry::ossimTiledElevationEntry()
   : m_rect(),
     m_sic(0)
{}
                                                   
ossimTiledElevationDatabase::ossimTiledElevationEntry::ossimTiledElevationEntry(
   const ossimGrect& rect, ossimRefPtr<ossimSingleImageChain> sic )
   : m_rect(rect),
     m_sic(sic)
{}

