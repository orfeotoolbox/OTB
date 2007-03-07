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
// $Id: ossimDirectoryTree.h,v 1.5 2004/05/04 12:17:25 gpotts Exp $
#ifndef ossimDirectoryTree_HEADER
#define ossimDirectoryTree_HEADER
#include <queue>
#include <vector>
using namespace std;

#include "ossimDirectory.h"

class OSSIMDLLEXPORT ossimDirectoryTree
{
public:
   ossimDirectoryTree();
   ~ossimDirectoryTree();

   bool open(const ossimFilename& dir);

   bool isOpened() const;
   
   bool getFirst(ossimFilename &filename,
                 int flags = ossimDirectory::OSSIM_DIR_DEFAULT);
   
   // get next file in the enumeration started with either GetFirst() or
   // GetFirstNormal()
   bool getNext(ossimFilename &filename) ;

  void findAllFilesThatMatch(std::vector<ossimFilename>& result,
			     const ossimString& regularExpressionPattern,
			     int flags = ossimDirectory::OSSIM_DIR_DEFAULT);
private:
   class OSSIMDLLEXPORT ossimDirData
   {
   public:
      ~ossimDirData()
         {
            if(theDirectory)
            {
               delete theDirectory;
               theFilename = "";
            }
         }
      ossimDirData(ossimDirectory *aDir,
                         const ossimFilename &filename)
                         :theDirectory(aDir),
                         theFilename(filename)
         {
            
         }
      ossimDirectory* theDirectory;
      ossimFilename   theFilename;
   };
   
   queue<ossimFilename> theDirectoryQueue;
   ossimDirData*        theCurrentDirectoryData;
   int                  theFlags;

   void deleteAll();

   void checkToPushDirectory(const ossimFilename &filename);
   // for now we will hide copy
   ossimDirectoryTree(const ossimDirectoryTree &rhs);
   const ossimDirectoryTree& operator =(const ossimDirectoryTree& rhs);
};

#endif
