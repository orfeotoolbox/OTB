//----------------------------------------------------------------------------
//
// File: ossimFileWalker.h
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

#ifndef ossimFileWalker_HEADER
#define ossimFileWalker_HEADER 1

#include <ossim/base/ossimCallback2wRet.h>
#include <ossim/base/ossimConstants.h>
#include <string>
#include <vector>

class ossimFilename;

/**
 * @class ossimFileWalker
 *
 * Utility class to walk through directories and get a list of files to
 * process file. Mechanisms are in place to filter files and  to call back
 * to a processFile method.  Requires a callback method to process files from
 * the user.  Callback is set via registerProcessFileCallback method.
 *
 * Typical usage (snip from ossimTiledElevationDatabase):
 *
 * ossimFileWalker* fw = new ossimFileWalker();
 * fw->initializeDefaultFilterList();
 * ossimCallback2wRet<const ossimFilename&, bool&, bool>* cb =
 * new ProcessFileCB(this, &ossimTiledElevationDatabase::processFile);
 * fw->registerProcessFileCallback(cb);
 * fw->walk(f);
 */
class OSSIM_DLL ossimFileWalker
{
public:
   
   /** default constructor */
   ossimFileWalker();
   
   void walk(const ossimFilename& root) const;

   /**
    * @brief Registers callback method to process a file.
    *
    * @param cb Callback to register.
    *
    * @see m_processFileCallBackPtr for documentation on template interface.
    *
    * @param bool Return flag, true for continue, false for stop.
    
    */   
   void registerProcessFileCallback(ossimCallback2wRet<const ossimFilename&, bool&, bool>* cb);

   /** @return The list of filtered out files. */
   const std::vector<std::string>& getFilteredExtensions() const;

   /**
    * @brief Non const method to allow access for
    * adding or deleting extensions from the list.
    */
   std::vector<std::string>& getFilteredExtensions();

   /**
    * @brief Initializes the filter list with a default set of filtered out
    * file names.
    */
   void initializeDefaultFilterList();
   
private:
   
   /** @return If true walking should continue; else, break. */
   bool walkDir(const ossimFilename& dir) const;
   
   /**
    * @brief Convenience method for file walker code to check file to see is
    * it should be processed.
    *
    * @param f File to check.
    * 
    * @return true f is stagagble, false if not.
    */
   bool isFiltered(const ossimFilename& f) const;
   
   /**
    * @brief isDotFile method.
    * @param f File/directory to check.
    * @return true if file is a dot file.
    */   
   bool isDotFile(const ossimFilename& f) const;

   /**
    * @brief Callback to method to process a file.
    *
    * @param const ossimFilename& First parameter(argument) file to process.
    *
    * @param bool& Recurse flag Indicates should continue to recurse
    * directories. Typically set to false for directory based images like an
    * RPF a.toc.
    *
    * @param bool Return flag, true for continue, false for stop.
    */
   ossimCallback2wRet<const ossimFilename&, bool&, bool>* m_processFileCallBackPtr;
   
   std::vector<std::string> m_filteredExtensions;

};

#endif /* #ifndef ossimFileWalker_HEADER */
