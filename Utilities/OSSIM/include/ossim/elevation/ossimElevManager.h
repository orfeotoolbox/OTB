//*****************************************************************************
// FILE: ossimElevManager.h
//
// Copyright (C) 2001 ImageLinks, Inc.
//
// License:  See top level LICENSE.txt file.
//
// DESCRIPTION:
//   Contains declaration of class ossimElevManager. This object provides a
//   single interface to an imaging chain for accessing multiple elevation
//   sources. This object owns one or more elevation sources in an ordered
//   list. When queried for an elevation at a particular point, it searches
//   the available sources for the best result, instantiating new sources if
//   necessary.
//
// SOFTWARE HISTORY:
//>
//   13Apr2001  Oscar Kramer
//              Initial coding.
//<
//*****************************************************************************
#ifndef ossimElevManager_HEADER
#define ossimElevManager_HEADER

#include <vector>
#include <ossim/elevation/ossimElevSource.h>
#include <ossim/base/ossimMutex.h>
#include <ossim/elevation/ossimElevSourceFactory.h>

class ossimDrect;
class ossimKeywordlist;
class ossimElevHandler;
class ossimGeoidManager;

// class ossimDtedManager;

/******************************************************************************
 * CLASS:  ossimElevManager
 *
 *****************************************************************************/
class OSSIMDLLEXPORT ossimElevManager : public ossimElevSource
{
public:
   virtual ~ossimElevManager();
   
   /**
    * METHOD: instance()
    * Implements singelton pattern
    */
   static ossimElevManager* instance();

   /**
    * Height access methods:
    * Return OSSIM_DBL_NAN if point is not found.
    */
   virtual double getHeightAboveMSL(const ossimGpt& gpt);
   virtual double getHeightAboveEllipsoid(const ossimGpt& gpt);

   /**
    * Method to check if the ground point elevation is defined:
    */
   virtual bool pointHasCoverage(const ossimGpt& gpt) const;

   /**
    * Method to load new elevation data given ground rectangle:
    */
   void loadElevData(const ossimDrect& ground_rect);

   /**
    * METHODS: accuracyLE90(), accuracyCE90()
    * Returns the vertical and horizontal accuracy (90% confidence) in the
    * region of gpt:
    */
   virtual double getAccuracyLE90(const ossimGpt& gpt) const;
   virtual double getAccuracyCE90(const ossimGpt& gpt) const;

   /**
    * METHOD: meanSpacingMeters() const
    * Returns the mean spacing in meters between posts in the region of gpt.
    */
   virtual double getMeanSpacingMeters() const;

   /**
    * Method to save the state of an object to a keyword list.
    * Return true if ok or false on error.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0) const;

   /**
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
   
   virtual void addElevSource(ossimElevSource* source);
   virtual void addElevSourceFactory(
      ossimRefPtr<ossimElevSourceFactory> factory);
   
   // Auto loading control...
   virtual void enableAutoLoad();
   virtual void disableAutoLoad();
   virtual bool isAutoLoadEnabled() const;

   /**
    * Auto sorting control...
    * Auto sorting sorts cell on add by mean post spacing putting cells with
    * lowest(best) post spacing at the top.
    */
   virtual void enableAutoSort();
   virtual void disableAutoSort();
   virtual bool isAutoSortEnabled() const;

   /**
    * METHOD:  bool isCellOpen(const ossimFilename& cell) const
    * Returns true if cell is currently opened(in theElevSourceList).
    */
   virtual bool isCellOpen(const ossimFilename& cell) const;

   /**
    * @return Returns the cell file name used for a given point.
    * Will return empty file name if point is not handled by any
    * factories.
    */
   ossimFilename getCellFilenameForPoint(const ossimGpt& gpt) const;

   virtual ossimRefPtr<ossimElevSourceFactory> findFactoryFromDir(
      const ossimFilename& dir);
   virtual const ossimRefPtr<ossimElevSourceFactory> findFactoryFromDir(
      const ossimFilename& dir)const;
   
      
   /**
    * METHOD: bool openDirectory(const ossimFilename& dir)
    * Method to open a directory of elevation cells.
    * Returns true on success, false on error.
    */
   virtual bool openDirectory(const ossimFilename& dir);

   /**
    * METHOD: bool openCell(const ossimFilename& cell)
    * Method to open an elevation cell.
    * Returns true on success, false on error.
    */
   virtual bool openCell(const ossimFilename& cell);

   /**
    * METHOD: bool closeCell(const ossimFilename& cell)
    * Method to close an elevation cell.
    * Returns true on success, false on error.
    */
   virtual bool closeCell(const ossimFilename& cell);

   /**
    * METHOD: void closeAllCells()
    * Method to close all open elevation cells.
    */
   virtual void closeAllCells();

   /**
    * METHOD: bool moveCellUpOne(const ossimFilename& cell)
    * Method to move an elevation cell on the list up by one.
    * Returns true on success, false on error.
    */
   virtual bool moveCellUpOne(const ossimFilename& cell);

   /**
    * METHOD: bool moveCellDownOne(const ossimFilename& cell)
    * Method to move an elevation cell on the list down by one.
    * Returns true on success, false on error.
    */
   virtual bool moveCellDownOne(const ossimFilename& cell);

   /**
    * METHOD: bool moveCellToTop(const ossimFilename& cell)
    * Method to move an elevation cell to the top of the list.
    * Returns true on success, false on error.
    */
   virtual bool moveCellToTop(const ossimFilename& cell);

   /**
    * METHOD: bool moveCellToBottomconst ossimFilename& cell)
    * Method to move an elevation cell to the bottom of the list.
    * Returns true on success, false on error.
    */
   virtual bool moveCellToBottom(const ossimFilename& cell);

   /**
    * @param list List to initialize with open cells.
    *
    * @note list is clear prior to stuffing.
    */
   void getOpenCellList(std::vector<ossimFilename>& list) const;

   ossimFilename getDefaultElevationPath() const;

   void setDefaultElevationPath(const ossimFilename& path);

   ossim_uint32 getNumberOfFactories()const;
   const ossimRefPtr<ossimElevSourceFactory> getFactory(ossim_uint32 idx)const;
   
protected:
   typedef std::vector<ossimRefPtr<ossimElevSourceFactory> >::iterator ossimElevSourceFactoryIterator;
   typedef std::vector<ossimRefPtr<ossimElevSourceFactory> >::const_iterator ossimElevSourceFactoryConstIterator;

   typedef std::vector<ossimRefPtr<ossimElevSource> >::iterator ossimElevSourceListIterator;
   typedef std::vector<ossimRefPtr<ossimElevSource> >::const_iterator ossimElevSourceListConstIterator;
   
   /** Protected constructor. */
   ossimElevManager();

   void updateMinMax();

   /**
    * Searches keyword list for "elevation_directory" keywords and adds
    * factories if found.  Will look for key words in this order.
    *
    * 1) "elevation_directory"
    * 2) "elevation_directory0"
    * 3) "elevation_directory1"
    * ...
    * ...
    * Until count of all elevation_directory keywords is reached.
    *
    * @param kwl Keyword list to search.
    *
    * @prefix Prefix for keys.
    */
   void addElevFactories(const ossimKeywordlist& kwl, const char* prefix=0);

   /**
    * Searches keyword list for "dted_directory" keywords and adds factories
    * if found.  Will look for key words in this order.
    *
    * 1) "dted_directory"
    * 2) "dted_directory0"
    * 3) "dted_directory1"
    * ...
    * ...
    * Until count of all dted_directory keywords is reached.
    *
    * @param kwl Keyword list to search.
    *
    * @prefix Prefix for keys.
    */
   void addDtedFactories(const ossimKeywordlist& kwl, const char* prefix=0);

   /**
    * Searches keyword list for "srtm_directory" keywords and adds factories
    * if found.  Will look for key words in this order.
    *
    * 1) "srtm_directory"
    * 2) "srtm_directory0"
    * 3) "srtm_directory1"
    * ...
    * ...
    * Until count of all srtm_directory keywords is reached.
    *
    * @param kwl Keyword list to search.
    *
    * @prefix Prefix for keys.
    */
   void addSrtmFactories(const ossimKeywordlist& kwl, const char* prefix=0);
   
   /**
    * Searches keyword list for "elevation_cell" keywords and adds
    * factories if found.  Will look for key words in this order.
    *
    * 1) "elevation_cell"
    * 2) "elevation_cell0"
    * 3) "elevation_cell1"
    * ...
    * ...
    * Until count of all elevation_cell keywords is reached.
    *
    * @param kwl Keyword list to search.
    *
    * @prefix Prefix for keys.
    */
   void addElevCells(const ossimKeywordlist& kwl, const char* prefix=0);

   /**
    * Method to determine if a file name is of dted format.
    *
    * @param f File to look at.
    *
    * @return true if dted format, false if not.
    */
   bool isDtedFilenameFormat(const ossimFilename& file) const;

   /**
    * Method to determine if a file name is of srtm format.
    *
    * @param f File to look at.
    *
    * @return true if dted format, false if not.
    */
   bool isSrtmFilenameFormat(const ossimFilename& file) const;

   /**
    * Method to determine if a directory is of dted structure.
    *
    * @param dir Directory to look at.
    *
    * @return true if dted directory structure, false if not.
    */
   bool isDtedDirectory(const ossimFilename& dir) const;

   /**
    * Method to determine if a directory looks like srtm.
    *
    * @param dir Directory to look at.
    *
    * @return true if srtm directory structure, false if not.
    */
   bool isSrtmDirectory(const ossimFilename& dir) const;

   bool isGeneralRaster(const ossimFilename& file)const;
   
   
   bool openDtedCell(const ossimFilename& file);

   bool openSrtmCell(const ossimFilename& file);

   bool openGeneralRasterCell(const ossimFilename& file);
   
   void addDtedFactory(const ossimFilename& file);

   void addSrtmFactory(const ossimFilename& file);

   void addGeneralRasterElevFactory(const ossimFilename& file);

   void loadStandardElevationPaths();
   /**
    * Data members:
    */
   static ossimElevManager*              theInstance;
   std::vector<ossimRefPtr<ossimElevSource> >        theElevSourceList;
   std::vector<ossimRefPtr<ossimElevSourceFactory> > theElevSourceFactoryList;
   ossimGeoidManager*                    theGeoidManager;
   bool                                  theAutoLoadFlag;
   bool                                  theAutoSortFlag;
   ossimFilename                         theDefaultElevationPath;
   mutable ossimMutex                            theMutex;
TYPE_DATA
};

#endif
