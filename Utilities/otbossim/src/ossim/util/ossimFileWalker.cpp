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
// Description:
//
// Utility class to walk through directories and get a list of files to
// process.
// 
//----------------------------------------------------------------------------
// $Id$

#include <ossim/util/ossimFileWalker.h>
#include <ossim/base/ossimDirectory.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimTrace.h>

static ossimTrace traceDebug(ossimString("ossimFileWalker:debug"));

ossimFileWalker::ossimFileWalker()
   : m_processFileCallBackPtr(0),
     m_filteredExtensions(0)
{
}

void ossimFileWalker::walk(const ossimFilename& root) const
{
   static const char M[] = "ossimFileWalker::walk";
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << M << " entered root=" << root << "\n";
   }
   ossimFilename rootFile = root.expand();
   if ( rootFile.size() && rootFile.exists() )
   {
      if (rootFile.isDir())
      {
         walkDir(rootFile);
      }
      else
      {
         if ( isFiltered(rootFile) == false )
         {
            bool recurseFlag; // Not used here but needed for call.
            if ( m_processFileCallBackPtr )
            {
               m_processFileCallBackPtr->operator()(rootFile, recurseFlag);
            }
         }
      }  
   }
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << M << " exiting...\n";
   }  
}

bool ossimFileWalker::walkDir(const ossimFilename& dir) const
{
   static const char M[] = "ossimFileWalker::walkDir";
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << M << " entered...\n" << "processing dir: " << dir << "\n";
   }

   bool continueWalkFlag = true;
   
   // List of directories in this directory...
   std::vector<ossimFilename> dirs;
   
   // List of files in this directory...
   std::vector<ossimFilename> files;
   
   ossimDirectory d;
   if ( d.open(dir) )
   {
      // Loop to get the list of files and directories in this directory.
      ossimFilename f;
      if ( d.getFirst(f) )
      {
         while ( f != "" )
         {
            if ( isFiltered(f) == false )
            {
               if (f.isDir())
               {
                  dirs.push_back(f);
               }
               else
               {
                  files.push_back(f);
               }
            }
            d.getNext(f);
         }
      }
         
      // Process files first before recursing.

      //---
      // recurse is set by processFileCallback indicates recursion should stop if false.
      // This typically is set false for a directory based image like a RPF a.toc.
      //---
      bool recurseFlag = true;
      
      std::vector<ossimFilename>::const_iterator i = files.begin();
      while (i != files.end())
      {
         if ( m_processFileCallBackPtr )
         {
            continueWalkFlag = m_processFileCallBackPtr->operator()(*i, recurseFlag);
         }
         
         if (continueWalkFlag == false) break; // Callee is finished...

         ++i;
      }
      
      if ( continueWalkFlag && recurseFlag )
      {
         // Process sub directories...
         i = dirs.begin();
         while (i != dirs.end())
         {
            continueWalkFlag = walkDir( (*i) );
            
            if ( continueWalkFlag == false ) break; // Callee is finished...
            
            ++i;
         }
      }
   }
   
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << M << " continueWalkFlag=" << continueWalkFlag<< " exited...\n";
   }
   return continueWalkFlag;
}

bool ossimFileWalker::isFiltered(const ossimFilename& file) const
{
   bool result = false;
   if ( file.size() )
   {
      if ( isDotFile(file) )
      {
         result = true;
      }
      else 
      {
         std::string ext = file.ext().downcase().c_str();
         if ( ext.size() )
         {
            std::vector<std::string>::const_iterator i = m_filteredExtensions.begin();
            while ( i != m_filteredExtensions.end() )
            {
               if ( ext == (*i) )
               {
                  result = true;
                  break;
               }
               ++i;
            }
         }
      }
   }
#if 0 /* Please leave for debug. (drb) */
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimFileWalker::isFiltered file " << (result?"filtered: ":"not filtered: ")
         << file << "\n";
   }
#endif
   
   return result;
}

bool ossimFileWalker::isDotFile(const ossimFilename& f) const
{
   bool result = false;
   
   // Get the size in bytes.
   if ( f.size() )
   {
      std::string::size_type firstDotPos = f.find('.');
      if ( firstDotPos == 0 )
      {
         result = true;
      }
      else if ( firstDotPos != std::string::npos ) // Dot in file.
      {
         // Get the position of first dot from the end.
         std::string::size_type lastDotPos = f.find_last_of('.');
         if ( lastDotPos != std::string::npos )
         {
            // Make copy.
            ossimFilename f2 = f;
            
            // Convert an '\'s to '/'s. 
            f2.convertBackToForwardSlashes();
            
            // Find the first slash from end.
            std::string::size_type lastSlashPos = f2.find_last_of('/');
            
            if (lastSlashPos != std::string::npos) // Found a slash.
            {
               if ( (lastSlashPos+1) == lastDotPos )
               {
                  // dot in front of slash like /home/foo/.xemacs
                  result = true;
               }
            }
         }
      }
   }

   return result;
}

const std::vector<std::string>& ossimFileWalker::getFilteredExtensions() const
{
   return m_filteredExtensions;
}

std::vector<std::string>& ossimFileWalker::getFilteredExtensions()
{
   return m_filteredExtensions;
}

void ossimFileWalker::initializeDefaultFilterList()
{
   // Common extensions to filter out, most common first.
   m_filteredExtensions.push_back(std::string("ovr"));
   m_filteredExtensions.push_back(std::string("omd"));
   m_filteredExtensions.push_back(std::string("his"));
   m_filteredExtensions.push_back(std::string("aux"));
   m_filteredExtensions.push_back(std::string("geom"));
   m_filteredExtensions.push_back(std::string("jpw"));
   m_filteredExtensions.push_back(std::string("sdw"));
   m_filteredExtensions.push_back(std::string("tfw"));
   m_filteredExtensions.push_back(std::string("spec"));
   m_filteredExtensions.push_back(std::string("statistics"));
   m_filteredExtensions.push_back(std::string("txt"));

   m_filteredExtensions.push_back(std::string("dbf"));
   m_filteredExtensions.push_back(std::string("shx"));

   
}

void ossimFileWalker::registerProcessFileCallback(
   ossimCallback2wRet<const ossimFilename&, bool&, bool>* cb)
{
   m_processFileCallBackPtr = cb;
}
