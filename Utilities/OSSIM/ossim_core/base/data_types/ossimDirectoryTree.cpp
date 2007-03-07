//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License 
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author: Garrett Potts (gpotts@imagelinks)
// Description: A brief description of the contents of the file.
//
//*************************************************************************
// $Id: ossimDirectoryTree.cpp,v 1.4 2004/05/04 12:17:06 gpotts Exp $

#include "ossimDirectoryTree.h"
#include "ossimDirectory.h"
#include "ossimRegExp.h"
ossimDirectoryTree::ossimDirectoryTree()
   :
      theCurrentDirectoryData(NULL),
      theFlags(0)
{
}

ossimDirectoryTree::~ossimDirectoryTree()
{
   deleteAll();
}

void ossimDirectoryTree::findAllFilesThatMatch(std::vector<ossimFilename>& result,
					       const ossimString& regularExpressionPattern,
					       int flags)
{
  ossimFilename filename;
  ossimRegExp   regExpr;
  regExpr.compile(regularExpressionPattern.c_str());
  if(getFirst(filename, flags))
  {
     do
     {
        if(regExpr.find(filename.c_str()))
        {
           result.push_back(filename);
        }
     }while(getNext(filename));
  }
}

bool ossimDirectoryTree::open(const ossimFilename& dir)
{
   if(theCurrentDirectoryData)
   {
      deleteAll();
   }

   theCurrentDirectoryData = new ossimDirData(new ossimDirectory, dir);
   theCurrentDirectoryData->theDirectory->open(dir);
   if(theCurrentDirectoryData->theDirectory->isOpened())
   {
      return true;
   }
   else
   {
      delete theCurrentDirectoryData;
      theCurrentDirectoryData = NULL;
   }

   return isOpened();
}

bool ossimDirectoryTree::isOpened() const
{
   if(theCurrentDirectoryData)
   {
      return theCurrentDirectoryData->theDirectory->isOpened();
   }
   
   return false;
}

bool ossimDirectoryTree::getFirst(ossimFilename &filename, int flags)
{
   bool result = false;
   theFlags    = flags | ossimDirectory::OSSIM_DIR_DIRS;
   if(theCurrentDirectoryData && isOpened())
   {
      result = theCurrentDirectoryData->theDirectory->getFirst(filename,
                                                               flags);
      while(result&&filename.isDir())
      {
         checkToPushDirectory(filename);
         result = theCurrentDirectoryData->theDirectory->getNext(filename);
      }

      if(!result)
      {
         if(!theDirectoryQueue.empty())
         {
            ossimFilename newDir = theDirectoryQueue.front();
            theDirectoryQueue.pop();
            theCurrentDirectoryData->theDirectory->open(newDir);
            
            return getFirst(filename, flags);
         }
      }
   }
   
   return result;
}

// get next file in the enumeration started with either GetFirst() or
// GetFirstNormal()
bool ossimDirectoryTree::getNext(ossimFilename &filename)
{
   bool result = false;
   
   if(theCurrentDirectoryData)
   {
      result = theCurrentDirectoryData->theDirectory->getNext(filename);
      if(result)
      {
         checkToPushDirectory(filename);
      }
      else
      {
         if(!theDirectoryQueue.empty())
         {
            ossimFilename newDir = theDirectoryQueue.front();
            theDirectoryQueue.pop();
            theCurrentDirectoryData->theDirectory->open(newDir);
            return getFirst(filename);
         }
      }
   }
   
   return result;
}

void ossimDirectoryTree::deleteAll()
{
   if(theCurrentDirectoryData)
   {
      delete theCurrentDirectoryData;
      theCurrentDirectoryData = NULL;
   }
}

void ossimDirectoryTree::checkToPushDirectory(const ossimFilename &filename)
{
   if((filename.file().trim() == ".") ||
      (filename.file().trim() == ".."))
   {
      return;
   }
   else if(filename.isDir())
   {
      theDirectoryQueue.push(filename);
   }
}
