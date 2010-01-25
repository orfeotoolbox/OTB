//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks)
// Description: A brief description of the contents of the file.
//
//*************************************************************************
// $Id: ossimDirectoryTree.cpp 9966 2006-11-29 02:01:07Z gpotts $

#include <ossim/base/ossimDirectoryTree.h>
#include <ossim/base/ossimDirectory.h>
#include <ossim/base/ossimRegExp.h>
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
