//----------------------------------------------------------------------------
//
// File: ossimTiledElevationDatabase.h
// 
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: See description for class below.
//
//----------------------------------------------------------------------------
// $Id$

#ifndef ossimTiledElevationDatabase_HEADER
#define ossimTiledElevationDatabase_HEADER 1

#include <ossim/elevation/ossimElevationDatabase.h>

#include <ossim/base/ossimGrect.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimRtti.h>

#include <vector>

class ossimDblGrid;
class ossimFilename;
class ossimImageData;
class ossimImageGeometry;
class ossimImageHandler;
class ossimProjection;
class ossimSingleImageChain;
class ossimString;

/**
 * @class ossimTiledElevationDatabase
 *
 * Elevation source used for working with generic images opened by an
 * ossimImageHandler.  This class supplies a mapRegion method used to map a
 * region of elevation to a grid.  The grid in turn is used for the
 * getHeightAboveMSL.  This class is for applications that know their region
 * of interest up front and want to bypass the ossimElevManager and grid the
 * elevation prior to processing for speed.  Can work on a file or a
 * directory of files.
 */
class OSSIM_DLL ossimTiledElevationDatabase : public ossimElevationDatabase
{
public:

   /** default constructor */
   ossimTiledElevationDatabase();

   /**
    * @brief Open a connection to a database.
    *
    * @param connectionString File or directory to open.  In most cases this
    * will point to a directory containing DEMs. Satisfies pure virtual
    * ossimElevationDatabase::open().
    *
    * @return true on success, false on error.
    */   
   virtual bool open(const ossimString& connectionString);

   /** @brief close method. Unreferences all data. */
   virtual void close();

   /**
    * @brief Maps elevation data for region to a grid.
    *
    * This uses connectionString passed to open method as starting point.
    */
   void mapRegion(const ossimGrect& region);

   /**
    * @brief Get height above MSL for point.
    *
    * Satisfies pure virtual ossimElevSource::getHeightAboveMSL().
    * 
    * @return Height above MSL.
    */
   virtual double getHeightAboveMSL(const ossimGpt& gpt);

   /**
    * @brief Get height above ellipsoid for point.
    *
    * Satisfies pure virtual ossimElevSource::getHeightAboveMSL().
    * 
    * @return Height above MSL.
    */
   virtual double getHeightAboveEllipsoid(const ossimGpt&);
   
   /**
    * Satisfies pure virtual ossimElevSource::pointHasCoverage
    * 
    * @return true if database has coverage for point.
    */
   virtual bool pointHasCoverage(const ossimGpt& gpt) const;

   /**
    * Statisfies pure virtual ossimElevSource::getAccuracyLE90.
    * @return The vertical accuracy (90% confidence) in the
    * region of gpt:
    */
   virtual double getAccuracyLE90(const ossimGpt& gpt) const;
   
   /**
    * Statisfies pure virtual ossimElevSource::getAccuracyCE90.
    * @return The horizontal accuracy (90% confidence) in the
    * region of gpt.
    */
   virtual double getAccuracyCE90(const ossimGpt& gpt) const;

   /** @brief Initialize from keyword list. */
   virtual bool loadState(const ossimKeywordlist& kwl, const char* prefix=0);

   /** @brief Save the state to a keyword list. */
   virtual bool saveState(ossimKeywordlist& kwl, const char* prefix)const;

   /**
    * @brief ProcessFile method.
    *
    * This method is linked to the ossimFileWalker::walk method via a callback
    * mechanism.  So it is called by the ossimFileWalk (caller).  This class
    * (callee) sets recurse and return flags accordingly to control
    * the ossimFileWalker, e.g. don't recurse, stop altogether.
    * 
    * @param file to process.
    *
    * @param recurseFlag Is set by this method indicating to caller should
    * continue to recurse directories. Typically set to false for directory
    * based images like an RPF a.toc. 
    *
    * @return true Indicates caller should continue to walk files/directories,
    * false indicates caller should stop.
    */
   bool processFile(const ossimFilename& file, bool& recurseFlag);
   
protected:
   /** Protected destructor as this is derived from ossimRefenced. */
   ~ossimTiledElevationDatabase();

private:

   // Private container to hold bounding rect and image handler.
   struct ossimTiledElevationEntry
   {
      /** @brief default constructor */
      ossimTiledElevationEntry();

      /** @brif Constructor that takes rectangle and chain. */
      ossimTiledElevationEntry(const ossimGrect& rect,
                               ossimRefPtr<ossimSingleImageChain> sic );

      /** Bounding rectangle in decimal degrees. */
      ossimGrect m_rect;

      /** Hold pointer to single image chain. */
      ossimRefPtr<ossimSingleImageChain> m_sic;
   };

   /**
    * @brief adds entry to the list checking for duplicates.
    */
   void addEntry(const ossimTiledElevationEntry& entry);

   /**
    * @brief Initializes m_referenceProj from the first entry.
    */
   void initializeReferenceProjection();

   /**
    * @return true if file is a directory based image and the stager should go
    * on to next directory; false if stager should continue with directory.
    */
   bool isDirectoryBasedImage(ossimRefPtr<ossimImageHandler> ih);

   /**
    * @brief Check for match of the following against the first entry of:
    *  bands, projection, scalar type and scale.
    * @returns true if good, false if not the same.
    */
   bool isCompatible(ossimImageHandler* ih,
                     ossimImageGeometry* geom,
                     ossimProjection* proj) const;

   /**
    * @brief Initialize bounding rectangle from image handler.
    *
    * Sets boundingRect to nan if ossimImageGeometry::getCornerGpts returns false.
    * 
    * @param ih Image handler.
    * @param boundingRect Initialized by method.
    */
   void getBoundingRect(ossimRefPtr<ossimImageGeometry> geom, ossimGrect& boundingRect) const;

   /** @brief Loads m_requestedRect into m_grid from m_entries. */
   void mapRegion();

   /** @brief Templated fill grid method. */
   template <class T> void fillGrid(T dummyTemplate, ossimRefPtr<ossimImageData> data);

   /** Hidden from use copy constructor */
   ossimTiledElevationDatabase(const ossimTiledElevationDatabase& copy_this);

   /** Hidden from use assignment operator */
   const ossimTiledElevationDatabase& operator=(const ossimTiledElevationDatabase& rhs);

   std::vector<ossimTiledElevationEntry> m_entries;

   /** Hold region of elevation. */
   ossimDblGrid* m_grid;

   /** Projection of the first entry.  Stored for convenience. */
   ossimRefPtr<ossimProjection> m_referenceProj;

   ossimGrect m_requestedRect;
   ossimGrect m_entryListRect;
   ossimGrect m_mappedRect; // Requested expanded to even post boundary.

   TYPE_DATA 
};

#endif /* ossimTiledElevationDatabase_HEADER */
