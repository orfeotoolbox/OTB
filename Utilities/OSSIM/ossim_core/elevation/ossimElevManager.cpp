//**************************************************************************
// FILE: ossimElevManager.cc
//
// Copyright (C) 2001 ImageLinks, Inc.
//
// License:  See top level LICENSE.txt file.
//
// DESCRIPTION:
//   Contains implementation of class ossimElevManager. This object 
//   provides a single interface for accessing multiple elevation
//   sources. This object owns one or more elevation sources in an ordered
//   list. When queried for an elevation at a particular point, it searches
//   the available sources for the best result, instantiating new sources if
//   necessary.
//
// SOFTWARE HISTORY:
//>
//   23Apr2001  Oscar Kramer
//              Initial coding.
//<
//**************************************************************************
// $Id: ossimElevManager.cpp,v 1.46 2005/12/21 21:25:21 dburken Exp $

#include <algorithm>

#include <elevation/ossimElevManager.h>
#include <elevation/ossimElevLess.h>
#include <elevation/formats/dted_cell/ossimDtedFactory.h>
#include <elevation/formats/dted_cell/ossimDtedHandler.h>
#include <elevation/formats/srtm/ossimSrtmFactory.h>
#include <elevation/formats/srtm/ossimSrtmHandler.h>
#include <elevation/formats/ossimElevCellHandler.h>
#include <base/data_types/geoid/ossimGeoidManager.h>
#include <base/data_types/ossimKeywordlist.h>
#include <base/data_types/ossimKeyword.h>
#include <base/data_types/ossimDirectory.h>
#include <base/common/ossimKeywordNames.h>
#include <base/common/ossimTrace.h>
#include <base/common/ossimScopedLock.h>
#include <base/common/events/ossimElevationManagerEvent.h>
#include <base/context/ossimNotifyContext.h>


RTTI_DEF1(ossimElevManager, "ossimElevManager" , ossimElevSource)

//***
// Define Trace flags for use within this file:
//***
static ossimTrace traceDebug ("ossimElevManager:debug");

#ifdef OSSIM_ID_ENABLED
static const char OSSIM_ID[] = "$Id: ossimElevManager.cpp,v 1.46 2005/12/21 21:25:21 dburken Exp $";
#endif

ossimElevManager* ossimElevManager::theInstance = NULL;

ossimElevManager* ossimElevManager::instance()
{
   if (!theInstance)
      theInstance = new ossimElevManager();
   
   return theInstance;
}

ossimElevManager::ossimElevManager()
   :
      ossimElevSource(),
      theElevSourceList(0),
      theElevSourceFactoryList(0),
      theGeoidManager(ossimGeoidManager::instance()),
      theAutoLoadFlag(true),
      theAutoSortFlag(true),
      theDefaultElevationPath(ossimFilename::NIL)
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimElevManager::ossimElevManager() entered..."
         << std::endl;
#if OSSIM_ID_ENABLED
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "OSSIM_ID:  "
         << OSSIM_ID
         << std::endl;
#endif      
   }

   if (!theInstance) theInstance = this;
}

ossimElevManager::~ossimElevManager()
{
   theInstance = NULL;

   // Delete the list of elevation sources and clear the list.
//    vector<ossimElevSource*>::iterator s = theElevSourceList.begin();
//    while (s != theElevSourceList.end())
//    {
//       delete *s;
//       ++s;
//    }
   theElevSourceList.clear();

   // Delete the list of elevation factories and clear the list.
//    vector<const ossimElevSourceFactory*>::iterator f
//       = theElevSourceFactoryList.begin();
//    while (f != theElevSourceFactoryList.end())
//    {
//       delete *f;
//       ++f;
//    }
   theElevSourceFactoryList.clear();
}

bool ossimElevManager::loadState(const ossimKeywordlist& kwl,
                                 const char* prefix)
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG ossimElevManager::loadState: Entered..."
         << std::endl;
   }
   ossimString copyPrefix = prefix;

   //---
   // Call the base class loadState.  This will pick up the enabled keyword.
   // Note we need to tack on the "elevation." prefix so the lookup of
   // "elevation.enabled" occurs.
   //---
   ossimString elev_prefix;
   if (prefix)
   {
      elev_prefix = prefix;
   }
   elev_prefix += "elevation.";
   ossimSource::loadState(kwl, elev_prefix);
   
   const char* lookup; // used throughout

   // Check for the elevation.autoload_dted.enabled keyword.
   lookup = kwl.find(prefix, "elevation.auto_load_dted.enabled");
   if (lookup)
   {
      theAutoLoadFlag = ossimString(lookup).toBool();
   }

   // Check for the autosort_elevation keyword.
   lookup = kwl.find(prefix, "elevation.auto_sort.enabled");
   if (lookup)
   {
      theAutoSortFlag = ossimString(lookup).toBool();
   }

   // Check for a default elevation path.
   const char DEFAULT_ELEVATION_PATH_KW[] = "default_elevation_path";
   lookup = kwl.find(DEFAULT_ELEVATION_PATH_KW);
   if(lookup)
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            <<  "DEBUG ossimElevManager::loadState: "
            << DEFAULT_ELEVATION_PATH_KW
            << " lookup:  " << lookup << std::endl;
      }
      theDefaultElevationPath = lookup;
   }
   ossimString regExpression =  ossimString("^(") + copyPrefix + "elevation_source[0-9]+.)";
   vector<ossimString> keys =
      kwl.getSubstringKeyList( regExpression );
   long numberOfSources = keys.size();
   ossim_uint32 offset = (copyPrefix+"elevation_source").size();
   ossim_uint32 idx = 0;
   std::vector<int> theNumberList(numberOfSources);
   for(idx = 0; idx < theNumberList.size();++idx)
   {
      ossimString numberStr(keys[idx].begin() + offset,
                            keys[idx].end());
      theNumberList[idx] = numberStr.toInt();
   }
   std::sort(theNumberList.begin(), theNumberList.end());
   
   for(idx=0;idx < theNumberList.size();++idx)
   {
      ossimString newPrefix = copyPrefix;
      newPrefix += ossimString("elevation_source");
      newPrefix += ossimString::toString(theNumberList[idx]);
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG ossimElevManager::loadState:"
            << "\nLooking for key:  " << newPrefix
            << std::endl;
      }
      ossimString fileKey = newPrefix;
      fileKey += ".";
      fileKey += ossimKeywordNames::FILENAME_KW;
      lookup = kwl.find(prefix, fileKey.c_str());
      if (lookup)
      {
         ossimFilename file = lookup;
         
         // Lookup the type.
         ossimString typeKey = newPrefix;
         typeKey += ".";
         typeKey += ossimKeywordNames::TYPE_KW;
         
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "DEBUG ossimElevManager::loadState:"
               << "\nLooking for key:  " << typeKey
               << std::endl;
         }
         
         lookup = kwl.find(prefix, typeKey.c_str());
         if (lookup)
         {
            ossimString type = lookup;
            if (type == "dted_cell")
            {
               openDtedCell(file);
            }
            else if (type == "srtm_cell")
            {
               openSrtmCell(file);
            }
            else if (type == "dted_directory")
            {
               addDtedFactory(file);
            }
            else if (type == "srtm_directory")
            {
               addSrtmFactory(file);
            }
         }
         else // No "type" specified, try to auto detect.
         {
            if(!file.isDir())
            {
               openCell(file);
            }
            else
            {
               if ( isDtedDirectory(file) )
               {
                  addDtedFactory(file);
               }
               
               else if ( isSrtmDirectory(file) )
               {
                  addSrtmFactory(file);
               }
            }
//             ossimString tempFile = file;
//             if(!file.isFile())
//             {
//                tempFile += ".gz";
//             }
//             if (tempFile.isFile())
//             {
//                if ( isSrtmFilenameFormat(tempFile) )
//                {
//                   openSrtmCell(file);
//                }
//                else if ( isDtedFilenameFormat(file) )
//                {
//                   openDtedCell(file);
//                }
//             }
//             else // Directory...
//             {
//                if ( isDtedDirectory(file) )
//                {
//                   addDtedFactory(file);
//                }
               
//                else if ( isSrtmDirectory(file) )
//                {
//                   addSrtmFactory(file);
//                }
//             }
         }
      } // end if lookup
   } // end for loop
#if 0      
   // Find any cells using "elevation_source" keyword.
    count =  kwl.numberOf(ossimKeywordNames::ELEVATION_SOURCE_KW);
    if (count)
    {
      //---
      // Look for numbered keyword like "elevation_source1",
      // "elevation_source2" and so on.
      //---
      ossim_uint32 sourceNumber = 0;
      ossim_uint32 found = 0;
      while ((found < count) && (sourceNumber < 10000) )
      {
         ossimString key = ossimKeywordNames::ELEVATION_SOURCE_KW;
         key += ossimString::toString(sourceNumber);
         
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "DEBUG ossimElevManager::loadState:"
               << "\nLooking for key:  " << key
               << std::endl;
         }

         ossimString fileKey = key;
         fileKey += ".";
         fileKey += ossimKeywordNames::FILENAME_KW;

         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "DEBUG ossimElevManager::loadState:"
               << "\nLooking for key:  " << fileKey
               << std::endl;
         }
         
         lookup = kwl.find(prefix, fileKey.c_str());
         if (lookup)
         {
            ++found;
            ossimFilename file = lookup;

            // Lookup the type.
            ossimString typeKey = key;
            typeKey += ".";
            typeKey += ossimKeywordNames::TYPE_KW;

            if (traceDebug())
            {
               ossimNotify(ossimNotifyLevel_DEBUG)
                  << "DEBUG ossimElevManager::loadState:"
                  << "\nLooking for key:  " << typeKey
                  << std::endl;
            }

            lookup = kwl.find(prefix, typeKey.c_str());
            if (lookup)
            {
               ++found;
               
               ossimString type = lookup;
               if (type == "dted_cell")
               {
                  openDtedCell(file);
               }
               else if (type == "srtm_cell")
               {
                  openSrtmCell(file);
               }
               else if (type == "dted_directory")
               {
                  addDtedFactory(file);
               }
               else if (type == "srtm_directory")
               {
                  addSrtmFactory(file);
               }
            }
            else // No "type" specified, try to auto detect.
            {
               if (file.isFile())
               {
                  if ( isDtedFilenameFormat(file) )
                  {
                     openSrtmCell(file);
                  }
                  else if ( isSrtmFilenameFormat(file) )
                  {
                     openDtedCell(file);
                  }
               }
               else // Directory...
               {
                  if ( isDtedDirectory(file) )
                  {
                     addDtedFactory(file);
                  }
                     
                  else if ( isSrtmDirectory(file) )
                  {
                     addSrtmFactory(file);
                  }
               }
            }
         }
         
         ++sourceNumber;
         
      } // End of "while ((found < count) && (sourceNumber < 10000) )"
    }// End of "if (count)" ELEVATION_SOURCE_KW
#endif
    //---
    // Below is only here for backwards compatibility.
   
   //***
   // Find any dted cells.
   //***
   const char DTED_CELL_KW[] = "dted_cell";
   
   // int number_of_cells = kwl.numberOf(prefix, DTED_CELL_KW);
   int number_of_cells = kwl.numberOf(DTED_CELL_KW);

   if (number_of_cells)
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_FATAL)
            << "DEBUG ossimElevManager::loadState: "
            << "\nNumber of cells = " << number_of_cells
            << std::endl;
      }
      
      int count = 0;
   
      // Look for just the keyword "dted_cell" with no number appended to it.
      lookup = kwl.find(prefix, DTED_CELL_KW);
      if (lookup)
      {
         ++count;
         ossimFilename f = lookup;
         openDtedCell(f);
      }
   
      // Look for numbered keyword like "dted_cell1".
      int cell_number = 0;
      while ((count < number_of_cells) &&
             (cell_number < 1000) )
      {
         ossimString key = DTED_CELL_KW;
         key += ossimString::toString(cell_number);
         
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "DEBUG ossimElevManager::loadState:"
               << "\nLooking for key:  " << key
               << std::endl;
         }
         
         lookup = kwl.find(prefix, key);
         if (lookup)
         {
            ++count;
            
            ossimFilename f = lookup;
            openDtedCell(f);
         }
      
         ++cell_number;
         
      }  // End of "while (count < number_of_cells)"
   }
   else
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG ossimElevManager::loadState:"
            << "\nNo dted_cell found in the keyword list."
            << std::endl;
      }
   }
   
   // Add any factories for dted_directory  keyword:
   addDtedFactories(kwl, prefix);

   // Add any factories for srtm_directory keyword:
   addSrtmFactories(kwl, prefix);

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG ossimElevManager::loadState:"
         << "\nElevation is:  "
         << (isSourceEnabled()?"enabled":"disabled")
         << "\nAuto elevation loading is:  "
         << (isAutoLoadEnabled()?"enabled":"disabled")
         << "\nAuto sorting of elevation is:  "
         << (isAutoSortEnabled()?"enabled":"disabled")
         << "\ntheDefaultElevationPath = "
         << theDefaultElevationPath
         << "DEBUG ossimElevManager::loadState: Exited..."
         << std::endl;
   }
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG ossimElevManager::loadState: Exited..." << std::endl;
   }
   
   return true;
}

bool ossimElevManager::saveState(ossimKeywordlist& kwl,
                                 const char* prefix) const
{
   ossim_uint32 sourceNumber = 1;

   // Save the cells.
   ossimElevSourceListConstIterator s = theElevSourceList.begin();
   while (s != theElevSourceList.end())
   {
      ossimString keywordBase = ossimKeywordNames::ELEVATION_SOURCE_KW;
      keywordBase += ossimString::toString(sourceNumber);
      
      ossimString typeKw = keywordBase;
      typeKw += ".";
      typeKw += ossimKeywordNames::TYPE_KW;
      
      ossimString filenameKw = keywordBase;
      filenameKw += ".";
      filenameKw += ossimKeywordNames::FILENAME_KW;
      
      kwl.add(prefix, filenameKw.c_str(), (*s)->getFilename());
      if ((*s)->getClassName() == "ossimDtedHandler")
      {
         kwl.add(prefix, typeKw.c_str(), "dted_cell");
      }
      else if ((*s)->getClassName() == "ossimSrtmHandler")
      {
         kwl.add(prefix, typeKw.c_str(), "strm_cell");
      }
      ++s;
      ++sourceNumber;
   }

   // Save the factories.
   ossimElevSourceFactoryConstIterator f = theElevSourceFactoryList.begin();

   while (f != theElevSourceFactoryList.end())
   {
      ossimString keywordBase = ossimKeywordNames::ELEVATION_SOURCE_KW;
      keywordBase += ossimString::toString(sourceNumber);
      
      ossimString typeKw = keywordBase;
      typeKw += ".";
      typeKw += ossimKeywordNames::TYPE_KW;
      
      ossimString filenameKw = keywordBase;
      filenameKw += ".";
      filenameKw += ossimKeywordNames::FILENAME_KW;

      kwl.add(prefix, filenameKw.c_str(), (*f)->getDirectory());
      
      if ((*f)->getClassName() == "ossimDtedFactory")
      {
         kwl.add(prefix, typeKw.c_str(), "dted_directory");
      }
      else if ((*f)->getClassName() == "ossimSrtmFactory")
      {
         kwl.add(prefix, typeKw.c_str(), "strm_directory");
      }
      
      ++f;
      ++sourceNumber;
   }
   
   return true;
}

void ossimElevManager::loadElevData(const ossimDrect& ground_rect)
{
   //***
   // NOT YET IMPLEMENTED. DEM Loading will occur when height is requested.
   // list until 
   //***
}

double ossimElevManager::getHeightAboveMSL(const ossimGpt& gpt)
{
   
   if (theEnableFlag)
   {
      ossimScopedLock<ossimMutex> scopedLock(theMutex);
      //***
      // Search through the list of elevation sources for a valid elevation
      // post.
      //***
      ossimElevSourceListIterator s = theElevSourceList.begin();
      while (s != theElevSourceList.end())
      {
         double d = (*s)->getHeightAboveMSL(gpt);
         if (d != (*s)->getNullHeightValue())
         {
            return  d;
         }
         (*s)->close();  // Close to keep file descriptors down.
         ++s;
      }
      
      if (theAutoLoadFlag)
      {
         //
         // If we got here there were no sources in the list that contained
         // coverage so go through the list of managers and see if they have
         // coverage.
         //
         ossimElevSourceFactoryConstIterator sf =
            theElevSourceFactoryList.begin();

         while (sf != theElevSourceFactoryList.end())
         {
            ossimElevSource* source = (*sf)->getNewElevSource(gpt);
            if (source)
            {
               theMutex.unlock();
               // Add it to the list.
               addElevSource(source);
               
               theMutex.lock();
               
               double d = source->getHeightAboveMSL(gpt);
               if (d != source->getNullHeightValue())
               {
                  return d;
               }
               else
               {
                  return theNullHeightValue;
               }
            }
            ++sf;
         }
      }
      
   }  // End of "if (isSourceEnabled())"
   
   // If we get here return a null height value.
   return theNullHeightValue;
}

double ossimElevManager::getHeightAboveEllipsoid(const ossimGpt& gpt)
{
   if (theEnableFlag)
   {
      // Fetch the geoid offset first:
      double d = theGeoidManager->offsetFromEllipsoid(gpt);
      
      if (d != OSSIM_DBL_NAN)
      {
         double h = getHeightAboveMSL(gpt);
         if (h != theNullHeightValue)
         {
            return  (h + d);
         }
         else
         {
            return d;
         }
      }
   }

   return theNullHeightValue;
}

double ossimElevManager::getAccuracyCE90(const ossimGpt& gpt) const
{
   // return theDtedManager->getAccuracyCE90(gpt);
   return 0.0;
}

double ossimElevManager::getAccuracyLE90(const ossimGpt& gpt) const
{
   // return theDtedManager->getAccuracyLE90(gpt);
   return 0.0;
}

double ossimElevManager::getMeanSpacingMeters() const
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_FATAL)
         << "FATAL ossimElevManager::getMeanSpacingMeters: not implemented..."
         << std::endl;
   }
   return 0.0;
}

bool ossimElevManager::pointHasCoverage(const ossimGpt& gpt) const
{
   // return theDtedManager->pointHasCoverage(gpt);
   return 0.0;
}

void ossimElevManager::addElevSource(ossimElevSource* source)
{
   // Add it to the list...
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_INFO)
         << "INFO ossimElevManager::addElevSource: Opened cell "
         << source->getFilename().c_str() << std::endl;
   }
   
   theElevSourceList.push_back(source);
   
   if (theAutoSortFlag)
   {
      // Sort by highest resolution.
      sort(theElevSourceList.begin(),
           theElevSourceList.end(),
           ossimElevLess());
   }

   // Update the min / max values.
   updateMinMax();
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG ossimElevManager::addElevSource Sorted elevation sources:"
         << std::endl;
      
      int index = 0;
      ossimElevSourceListConstIterator s = theElevSourceList.begin();
      while (s != theElevSourceList.end())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "Elevation source[" << index << "]:  "
            << (*s)->getFilename().c_str() << std::endl;
         ++s;
         ++index;
      }
   }

   ossimElevationManagerEvent event( this );

   fireEvent( event );
}

void ossimElevManager::updateMinMax()
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG ossimElevManager::updateMinMax: "
         << "\nInitial min height above MSL:  " << theMinHeightAboveMSL
         << "\nInitial max height above MSL:  " << theMaxHeightAboveMSL
         << std::endl;
   }

   double BOGUS_MIN = 100000.0;
   double BOGUS_MAX = -100000.0;
   double min_value = BOGUS_MIN;
   double max_value = BOGUS_MAX;
   
   ossimElevSourceListConstIterator s = theElevSourceList.begin();
   while (s != theElevSourceList.end())
   {
      double s_min = (*s)->getMinHeightAboveMSL();
      double s_max = (*s)->getMaxHeightAboveMSL();

      if (s_min < min_value) min_value = s_min;
      if (s_max > max_value) max_value = s_max;
      ++s;
   }

   // Don't asign if unless something was found...
   if (min_value != BOGUS_MIN) theMinHeightAboveMSL = min_value;
   if (max_value != BOGUS_MAX) theMaxHeightAboveMSL = max_value;

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG ossimElevManager::updateMinMax:\n"
         << "Updated min height above MSL:  " << theMinHeightAboveMSL
         << "\nUpdated max height above MSL:  " << theMaxHeightAboveMSL
         << std::endl;
   }
}

void ossimElevManager::addElevSourceFactory(ossimRefPtr<ossimElevSourceFactory> source)
{
   theElevSourceFactoryList.push_back(source);
}

void ossimElevManager::enableAutoLoad()
{
   theAutoLoadFlag = true;
}

void ossimElevManager::disableAutoLoad()
{
   theAutoLoadFlag = false;
}

bool ossimElevManager::isAutoLoadEnabled() const
{
   return theAutoLoadFlag;
}

void ossimElevManager::enableAutoSort()
{
   theAutoSortFlag = true;
}

void ossimElevManager::disableAutoSort()
{
   theAutoSortFlag = false;
}

bool ossimElevManager::isAutoSortEnabled() const
{
   return theAutoSortFlag;
}

ossimRefPtr<ossimElevSourceFactory> ossimElevManager::findFactoryFromDir(const ossimFilename& dir)
{
   ossim_uint32 idx = 0;
   for(idx = 0; idx < theElevSourceFactoryList.size(); ++idx)
   {
      if(theElevSourceFactoryList[idx]->getDirectory() == dir)
      {
         return theElevSourceFactoryList[idx];
      }
   }
   
   return ossimRefPtr<ossimElevSourceFactory>();
}

const ossimRefPtr<ossimElevSourceFactory> ossimElevManager::findFactoryFromDir(const ossimFilename& dir)const
{
   ossim_uint32 idx = 0;

   for(idx = 0; idx < theElevSourceFactoryList.size(); ++idx)
   {
      if(theElevSourceFactoryList[idx]->getDirectory() == dir)
      {
         return theElevSourceFactoryList[idx];
      }
   }
   
   return ossimRefPtr<ossimElevSourceFactory>();
}


bool ossimElevManager::isCellOpen(const ossimFilename& cell) const
{
   bool result = false;

   if (cell == ossimFilename::NIL) return result;
   
   ossimElevSourceListConstIterator s = theElevSourceList.begin();
   while (s != theElevSourceList.end())
   {
      if (cell == (*s)->getFilename())
      {
         result = true;
         break;
      }
      ++s;
   }
   
   return result;
}

bool ossimElevManager::openCell(const ossimFilename& cell)
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG ossimElevManager::openCell: Entered..."
         << "\nCell:  " << cell << std::endl;
   }

   if (cell == ossimFilename::NIL) return false;

   ossimFilename tempFile = cell;
   if(!tempFile.isFile())
   {
      tempFile += ".gz";
   }
   if(!tempFile.isFile()) return false;
   
   if (isDtedFilenameFormat(tempFile))
   {
      return openDtedCell(tempFile);
   }
   else if (isSrtmFilenameFormat(tempFile))
   {
      return openSrtmCell(tempFile);
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG ossimElevManager::openCell:\n"
         << "Could not open:  " << cell 
         << std::endl;
   }
   
   return false;
}

bool ossimElevManager::openDirectory(const ossimFilename& dir)
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG  ossimElevManager::openDirectory Entered..."
         << "\nDirectory:  " << dir << std::endl;
   }

   if (dir == ossimFilename::NIL || !dir.isDir()) return false;

   ossimDirectory od(dir);

   if (od.isOpened() == false) return false;

   ossimFilename f;

   bool result = false;
   
   if (od.getFirst(f, ossimDirectory::OSSIM_DIR_FILES))
   {
      do
      {
         if (openCell(f) == true)
         {
            result = true;
         }
      } while (od.getNext(f));
   }
   
   return result;
}

bool ossimElevManager::closeCell(const ossimFilename& cell)
{
   if (cell == ossimFilename::NIL) return false;
   
   ossimElevSourceListIterator s = theElevSourceList.begin();
   while (s != theElevSourceList.end())
   {
      if (cell == (*s)->getFilename())
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "Closed cell:  " << (*s)->getFilename().c_str()
               << std::endl;
         }
         theElevSourceList.erase(s);
         return true;
      }
      ++s;
   }
   
   return false;
}

void ossimElevManager::closeAllCells()
{
   ossimElevSourceListIterator s = theElevSourceList.begin();
   while (s != theElevSourceList.end())
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "Closed cell:  " << (*s)->getFilename().c_str() << std::endl;
      }
      ++s;
   }

   theElevSourceList.clear();
}

void ossimElevManager::getOpenCellList(std::vector<ossimFilename>& list) const
{
   list.clear();
   
   ossimElevSourceListConstIterator s = theElevSourceList.begin();

   while (s != theElevSourceList.end())
   {
      list.push_back((*s)->getFilename());
      ++s;
   }
}

ossimFilename ossimElevManager::getDefaultElevationPath() const
{
   return theDefaultElevationPath;
}

void ossimElevManager::setDefaultElevationPath(const ossimFilename& path)
{
   theDefaultElevationPath = path;
}

bool ossimElevManager::moveCellUpOne(const ossimFilename& cell)
{
   if (cell == ossimFilename::NIL) return false;

   ossimElevSourceListIterator a = theElevSourceList.begin();
   ossimElevSourceListIterator b = a;
   ++b;
   while (b != theElevSourceList.end())
   {
      if ( cell == (*b)->getFilename() )
      {
         iter_swap(a, b);
         return true;
      }
      ++a;
      ++b;
   }
   
   return false;
}

bool ossimElevManager::moveCellDownOne(const ossimFilename& cell)
{
   if (cell == ossimFilename::NIL) return false;

   ossimElevSourceListIterator a = theElevSourceList.begin();
   ossimElevSourceListIterator b = a;
   ++b;
   while (b != theElevSourceList.end())
   {
      if ( cell == (*a)->getFilename() )
      {
         iter_swap(a, b);
         return true;
      }
      ++a;
      ++b;
   }
   
   return false;
}

bool ossimElevManager::moveCellToTop(const ossimFilename& cell)
{
   if (cell == ossimFilename::NIL) return false;

   ossimElevSourceListIterator a = theElevSourceList.begin();
   ossimElevSourceListIterator b = a;
   ++b;
   while (b != theElevSourceList.end())
   {
      if ( cell == (*b)->getFilename() )
      {
         ossimFilename f = (*b)->getFilename();
         while (moveCellUpOne(f) == true){}
         return true;
      }
      ++b;
   }
   
   return false;
}

bool ossimElevManager::moveCellToBottom(const ossimFilename& cell)
{
   if (cell == ossimFilename::NIL) return false;

   ossimElevSourceListIterator a = theElevSourceList.begin();
   ossimElevSourceListIterator b = theElevSourceList.end();
   --b;
   while (a != b)
   {
      if ( cell == (*a)->getFilename() )
      {
         ossimFilename f = (*a)->getFilename();
         while (moveCellDownOne(f) == true){}
         return true;
      }
      ++a;
   }
   
   return false;
}

void ossimElevManager::addDtedFactories(const ossimKeywordlist& kwl,
                                        const char* prefix)
{
   //---
   // Find the dted direcories to search in.
   // We'll capture these regardless of being enabled in case someone wants to
   // turn on the autoload midstream in a program.
   //---
   const char DTED_DIRECTORY_KW[] = "dted_directory";
   
   int number_of_directories = kwl.numberOf(DTED_DIRECTORY_KW);
   
   if (number_of_directories)
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG ossimElevManager::addDtedFactories:"
            << "\nNumber of dirs = " << number_of_directories
            << std::endl;
      }
      
      int count = 0;
      
      //---
      // Look for just the keyword "dted_directory" with no number appended
      // to it.
      //
      const char* lookup = kwl.find(prefix, DTED_DIRECTORY_KW);
      if (lookup)
      {
         ++count;
         ossimFilename f = lookup;
         addDtedFactory(f);
      }
      
      // Look for numbered keyword like "dted_directory1".
      int dir_number = 0;
      while ((count < number_of_directories) &&
             (dir_number < 1000) )
      {
         ossimString key = DTED_DIRECTORY_KW;
         key += ossimString::toString(dir_number);
         
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "DEBUG ossimElevManager::addDtedFactories:"
               << "\nLooking for key:  " << key
               << std::endl;
         }
         
         lookup = kwl.find(prefix, key);
         if (lookup)
         {
            ++count;
            ossimFilename f = lookup;
            addDtedFactory(f);
         }
         
         ++dir_number;
         
      } // End of "while (count < number_of_directories)"
      
   } // End of "if (number_of_directories)
   else
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG ossimElevManager::addDtedFactories::"
            << "\nNo dted_directories found in the keyword list."
            << std::endl;
      }
   }
   
}

void ossimElevManager::addSrtmFactories(const ossimKeywordlist& kwl,
                                        const char* prefix)
{
   //---
   // Find the srtm direcories to search in.
   // We'll capture these regardless of being enabled in case someone wants to
   // turn on the autoload midstream in a program.
   //---
   const char SRTM_DIRECTORY_KW[] = "srtm_directory";
   
   int number_of_directories = kwl.numberOf(SRTM_DIRECTORY_KW);
   
   if (number_of_directories)
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG ossimElevManager::addSrtmFactories:"
            << "\nNumber of dirs = " << number_of_directories
            << std::endl;
      }
      
      int count = 0;
      
      //---
      // Look for just the keyword "srtm_directory" with no number appended
      // to it.
      //---
      const char* lookup = kwl.find(prefix, SRTM_DIRECTORY_KW);
       if (lookup)
       {
          ++count;
          ossimFilename f = lookup;
          
          if (f.isDir())
          {
             // Add a srtm factory for this directory.
             ossimRefPtr<ossimElevSourceFactory> factory = new ossimSrtmFactory(f);
             addElevSourceFactory(factory);
             if (traceDebug())
             {
                ossimNotify(ossimNotifyLevel_DEBUG)
                   << "Added elevation factory for:  " << f.c_str()
                   << std::endl;
             }
          }
       }
       
       // Look for numbered keyword like "srtm_directory1".
       int dir_number = 0;
       while ((count < number_of_directories) &&
              (dir_number < 1000) )
       {
          ossimString key = SRTM_DIRECTORY_KW;
          key += ossimString::toString(dir_number);
          
          if (traceDebug())
          {
             ossimNotify(ossimNotifyLevel_DEBUG)
                << "DEBUG ossimElevManager::addSrtmFactories:"
                << "\nLooking for key:  " << key
                << std::endl;
          }
          
          lookup = kwl.find(prefix, key);
          if (lookup)
          {
             ++count;
             
             ossimFilename f = lookup;
             if (f.isDir())
             {
                // Add a srtm factory for this directory.
                ossimRefPtr<ossimElevSourceFactory> factory = new ossimSrtmFactory(f);
                addElevSourceFactory(factory);
                
                if (traceDebug())
                {
                   ossimNotify(ossimNotifyLevel_DEBUG)
                      << "Added srtm factory for:  " << f.c_str() << std::endl;
                }
             }
          }
          
          ++dir_number;
          
       } // End of "while (count < number_of_directories)"
       
   } // End of "if (number_of_directories)
   else
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG ossimElevManager::addSrtmFactories::"
            << "\nNo srtm_directories found in the keyword list."
            << std::endl;
      }
   }
}

bool ossimElevManager::isDtedFilenameFormat(const ossimFilename& file) const
{
   // Format example: "n32.dt2"
   
   // Strip any path away.
   ossimFilename f = file.file();

   // Downcase it.
   f.downcase();

   // Must be seven characters long.
   if (f.size() != 7)
   {
      return false;
   }

   // Must start with 'n' or 's'.
   if ( (f[0] != 'n') && (f[0] != 's') )
   {
      return false;
   }

   // Fourth character must be a dot '.'.
   if (f[3] != '.')
   {
      return false;
   }

   // Fith character must be a 'd'.
   if (f[4] != 'd')
   {
      return false;
   }

   // Sixth character must be a 't'.
   if (f[5] != 't')
   {
      return false;
   }

   return true;
}

bool ossimElevManager::isSrtmFilenameFormat(const ossimFilename& file) const
{
   // Format example:  "N37W120.hgt"
   // or N37W120.hgt.gz
   
   // Strip any path away.
   ossimFilename f = file.file();

   // Downcase it.
   f.downcase();

   // Must be eleven characters long. or if it's gzipped then
   // it can be 14 characters lond with the .gz added extension
   //
   if ((f.size() != 11)&&
       (f.size() != 14))
   {
      return false;
   }

   // Must start with 'n' or 's'.
   if ( (f[0] != 'n') && (f[0] != 's') )
   {
      return false;
   }

   // Fourth character must be 'e' or 'w'.
   if ( (f[3] != 'e') && (f[3] != 'w') )
   {
      return false;
   }

   // Eight character must be a dot '.'.
   if (f[7] != '.')
   {
      return false;
   }

   if(!f.contains(".hgt")&&
      !f.contains(".hgt.gz"))
   {
      return false;
   }
   // Extension must be "hgt".
//    if (f.ext() != "hgt")
//    {
//       return false;
//    }

   return true;
}


bool ossimElevManager::isDtedDirectory(const ossimFilename& dir) const
{
   //---
   // Sample format
   //                          dir
   //                    |            |
   //                 e040            e041
   //              |        |        |       |
   //            n31.dt2 n32.dt2  n31.dt2 n32.dt2
   //---
   if (!dir.isDir())
   {
      return false;
   }

   ossimDirectory od(dir);
   if (od.isOpened() == false)
   {
      return false;
   }

   ossimFilename f;
   // Get the first directory.
   od.getFirst(f, ossimDirectory::OSSIM_DIR_DEFAULT);

   do
   {
      // Must be a directory.
      if (f.isDir())
      {
         
         // Discard any full path.
         f = f.file();
         
         // Downcase it.
         f.downcase();
         
         // Must start with 'e' or 'w'.
         if ( ((f.c_str()[0] == 'e') || ( f.c_str()[0] == 'w')) &&
              (f.size() == 4))
         {
            return true;
         }
      }
   }while(od.getNext(f));
   
   return false;
}

bool ossimElevManager::isSrtmDirectory(const ossimFilename& dir) const
{
   //---
   // Sample format:
   //            dir
   //         |          |
   //    N35W121.hgt N35W121.hgt
   //---
   if (!dir.isDir())
   {
      return false;
   }

   ossimDirectory od(dir);
   if (od.isOpened() == false)
   {
      return false;
   }

   ossimFilename f;

   if(od.getFirst(f, ossimDirectory::OSSIM_DIR_FILES))
   {
      do
      {
         if(isSrtmFilenameFormat(f))
         {
            return true;
         }
      }while(od.getNext(f));
   }

   
   return isSrtmFilenameFormat(f);
}

bool ossimElevManager::openDtedCell(const ossimFilename& file)
{
   if (isCellOpen(file))
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG ossimElevManager::openDtedCell:\n"
            << file << " has already been opened by the manager."
            << std::endl;
      }

      return false;
   }
   
   ossimElevSource* source = new ossimDtedHandler(file);
   if (source->getErrorStatus() == ossimErrorCodes::OSSIM_OK)
   {
      // Add the source to the list.
      addElevSource(source);
      
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG ossimElevManager::openDtedCell:"
            << "\nAdded dted cell:  " << file << std::endl;
      }
   }
   else
   {
      delete source;
      return false;
   }
   return true;
}

bool ossimElevManager::openSrtmCell(const ossimFilename& file)
{
   if (isCellOpen(file))
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG ossimElevManager::openSrtmCell:\n"
            << file << " has already been opened by the manager."
            << std::endl;
      }
      return false;
   }
   
   ossimElevSource* source = new ossimSrtmHandler(file);
   if (source->getErrorStatus() == ossimErrorCodes::OSSIM_OK)
   {
      // Add the source to the list.
      addElevSource(source);
      
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG ossimElevManager::openSrtmCell:"
            << "\nAdded srtm cell:  " << file << std::endl;
      }
   }
   else
   {
      delete source;
      return false;
   }
   return true;
}

void ossimElevManager::addDtedFactory(const ossimFilename& file)
{
   // Add a dted factory for this directory.
   ossimRefPtr<ossimElevSourceFactory> factory = new ossimDtedFactory(file);
   addElevSourceFactory(factory);
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimElevManager::addDtedFactory:"
         << "\nAdded elevation factory for:  " << file.c_str()
         << std::endl;
   }
}

void ossimElevManager::addSrtmFactory(const ossimFilename& file)
{
   ossimRefPtr<ossimElevSourceFactory> factory = new ossimSrtmFactory(file);
   addElevSourceFactory(factory);
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "Added elevation factory for:  " << file.c_str()
         << std::endl;
   }
}

